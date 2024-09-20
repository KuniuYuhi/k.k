#include "stdafx.h"
#include "Summoner.h"
#include "Brave.h"
#include "EnemyManager.h"

#include "SummonerAIController.h"

#include "KnockBackInfoManager.h"

#include "GameSceneManager.h"

#include "UseEffect.h"

namespace {
	float MUL_SCALE = 4.0f;

	const Vector3 START_POSITION = { 0.0f,0.0f,400.0f };
	const Vector3 START_FORWARD = g_vec3Back;


	const float FIRST_COMBO_EFFECT_SCALE = 20.0f;

	const float SECOND_AND_THIRD_COMBO_EFFECT_SCALE = 15.0f;

}

Summoner::~Summoner()
{
}

bool Summoner::Start()
{
	//ボスは死んでいない！
	GameSceneManager::GetInstance()->ResetBossDelete();

	m_player = FindGO<Brave>("Brave");

	//モデルの読み込み
	InitModel();

	//キャラコン初期化
	if (m_charaCon == nullptr)
	{
		CreateCharacterController();
		m_charaCon.get()->Init(90.0f, 160.0f, m_position);
	}
	else
	{
		m_charaCon.get()->SetPosition(m_position);
	}


	//ステータスを初期化
	m_status.InitCommonEnemyStatus("Summoner");
	//技に関するステータスを初期化
	m_skillStatus.InitSkillStatus();

	//ボスエネミーのインスタンスを設定
	EnemyManager::GetInstance()->SetBossEnemyInstance(this);

	//コンポーネントを初期化していないなら
	if (!m_isSettingComponents)
	{
		InitComponents();
	}


	if (m_stateContext == nullptr)
	{
		m_stateContext = std::make_unique<SummonerStateContext>();
		m_stateContext.get()->Init(this,enSummonerState_Appear);
	}



	//インスタンスがないなら初期化
	if (m_StateMachineCotext == nullptr)
	{
		m_StateMachineCotext = std::make_unique<SummonerSMContext>();
		m_StateMachineCotext.get()->Init(this, enSubStateMachine_General);
	}





	return true;
}

void Summoner::ChangeState(EnSummonerState changeState)
{
	m_stateContext.get()->ChangeState(changeState);
}

bool Summoner::CheckAndTransitionStateMachine()
{
	return m_StateMachineCotext.get()->CheckAndTransitionStateMachine();
}

void Summoner::ProcessCommonTranstion()
{
	if (fabsf(GetMoveSpeed().x) >= 0.001f ||
		fabsf(GetMoveSpeed().z) >= 0.001f)
	{
		m_stateContext.get()->ChangeState(enSummonerState_Run);
	}
	else
	{
		m_stateContext.get()->ChangeState(enSummonerState_Idle);
	}
}

float Summoner::GetDistanceToPlayerPositionValue()
{
	return CalcDistanceToTargetPosition(m_player->GetPosition());
}

void Summoner::UpdateComboAttackProcess()
{
	//当たり判定を生成するなら
	if (m_isCreateCollision)
	{
		m_aiController->CreateComboAttackCollision();
	}


	if (m_isAttackMove)
	{
		AttackChaseMovement(m_player->GetPosition(),300.0f);
	}
}

void Summoner::UpdateAttackTurnToTargetProcess()
{
	//フラグが立っている間は回転だけする
	if (m_isAttackMove)
	{
		TurnToTarget();
	}
}

void Summoner::UpdateShockWaveProcess()
{
	if (m_isAttackMove)
	{
		AttackChaseMovement(m_player->GetPosition(), 200.0f);
	}
}

void Summoner::AfterDieProcess()
{
	//
	if (!m_isViewModel)
	{
		m_dieEffect = FindGO<UseEffect>("SummonerDieEffect");

		if (m_dieEffect == nullptr)
		{
			//ボスがやられた
			//カメラをプレイヤーに移す
			GameSceneManager::GetInstance()->BossDelete();

			//自身を削除
			DeleteGO(this);
			return;
		}

		return;
	}


	//ボスがやられると同時にフィールド上のモブエネミーも死亡させる
	//モブエネミーの死亡エフェクトも再生させる
	EnemyManager::GetInstance()->DeleteAllEnemy(true);

	//モデルは表示させない
	m_isViewModel = false;

	//死亡エフェクト生成
	m_dieEffect = NewGO<UseEffect>(0, "SummonerDieEffect");
	m_dieEffect->PlayEffect(enEffect_Mob_Dead,
		m_position, g_vec3One * 10.0f, Quaternion::Identity, false);

	
}

void Summoner::WinProcess()
{
	//勝利アニメーションステートに遷移
	m_stateContext.get()->ChangeState(enSummonerState_Victory);
}

void Summoner::DieFlomOutside()
{
	//ボスがやられると同時にフィールド上のモブエネミーも死亡させる
	EnemyManager::GetInstance()->DeleteAllEnemy();

	//自身を削除
	DeleteGO(this);
}

bool Summoner::IsStopRequested()
{
	//勝敗が着いたら
	if (GameSceneManager::GetInstance()->IsGameOutcome())return true;

	//死亡したら
	if (IsDie()) return true;

	//ここまで来たら処理は止まらない
	return false;
}

void Summoner::ProcessHit(DamageInfo damageInfo)
{
	//ダメージを受ける
	TakeDamage(damageInfo.attackPower);

	//ダメージを受けて死亡したなら
	if (IsDie())
	{
		//死亡アニメーションステートに遷移
		m_stateContext.get()->ChangeState(enSummonerState_Die);
	}
	else
	{
		//ヒット音再生
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_Monster_Hit,
			g_soundManager->GetSEVolume()
		);
	}

}

bool Summoner::IgnoreCollision()
{
	//無敵中なら処理しない
	if (IsInvincible()) return true;

	return false;
}

void Summoner::ReleaseThis()
{
}

void Summoner::AddMoreComponent()
{
}

void Summoner::InitComponents()
{
	//基本的なコンポーネントを設定
	SettingDefaultComponent();


	AddComponent<SummonerAIController>();
	m_aiController = GetComponent<SummonerAIController>();

}

void Summoner::Rotation()
{
	//回転方向がないなら処理しない
	if (fabsf(m_rotateDirection.x) < 0.001f && fabsf(m_rotateDirection.z) < 0.001f) {
		return;
	}

	//線形補間で回転する方向を計算。徐々に回転させる
	m_currentRotDirection = Math::Lerp(
		g_gameTime->GetFrameDeltaTime() * 3.0f, m_currentRotDirection, m_rotateDirection
	);
	
	//回転
	m_rotation.SetRotationYFromDirectionXZ(m_currentRotDirection);

	//前方向を回転方向に合わせる
	m_forward = m_currentRotDirection;
}


void Summoner::Update()
{
	//処理を止める要求がない限り処理をする
	if (!IsStopRequested())
	{
		//移動処理
		ChaseMovement(m_player->GetPosition());

		//回転処理
		Rotation();

		//当たり判定
		CheckSelfCollision();


		//サブステートマシンの更新処理
		m_StateMachineCotext.get()->CurrentSubStateMachineUpdate();
	}
	
	//ステートの更新処理
	m_stateContext.get()->UpdateCurrentState();
	//アニメーション
	m_stateContext.get()->PlayAnimationCurrentState();
	



	//モデルの更新
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void Summoner::Render(RenderContext& rc)
{
	//モデルを表示させないなら
	if (!m_isViewModel) return;

	m_modelRender.Draw(rc);
}

void Summoner::InitModel()
{
	//モデルを初期化していないなら
	if (!m_modelRender.IsInit())
	{
		//アニメーションの読み込み
		LoadAnimationClip();

		//モデルを初期化
		m_modelRender.Init(
			"Assets/modelData/character/Lich/Lich_real_Selllook.tkm",
			L"Assets/shader/ToonTextrue/lamp_Lich.DDS",
			m_animationClip,
			enSummonerAnimClip_Num,
			enModelUpAxisZ
		);

		//アニメーションイベント用の関数を設定する。
		m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
			OnAnimationEvent(clipName, eventName);
		});

	}

	//拡大率を変更
	m_scale *= MUL_SCALE;

	m_position = START_POSITION;

	m_forward = START_FORWARD;

	m_rotateDirection = m_forward;

	//トランスフォームの設定
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);

	//杖のボーンIDを取得
	m_staffBoonId = m_modelRender.FindBoneID(L"Staff");
	//右手のボーンIDを取得
	m_rightHandBoonId = m_modelRender.FindBoneID(L"Index_Proximal_R");
	//左手のボーンIDを取得
	m_leftHandBoonId = m_modelRender.FindBoneID(L"Hand_L");

	
}

void Summoner::LoadAnimationClip()
{
	//各種アニメーションクリップのロード
	m_animationClip[enSummonerAnimClip_Appear].Load("Assets/animData/character/Lich/FogRemoval.tka");
	m_animationClip[enSummonerAnimClip_Appear].SetLoopFlag(false);
	m_animationClip[enSummonerAnimClip_Idle].Load("Assets/animData/character/Lich/Idle.tka");
	m_animationClip[enSummonerAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enSummonerAnimClip_Run].Load("Assets/animData/character/Lich/Walk.tka");
	m_animationClip[enSummonerAnimClip_Run].SetLoopFlag(true);
	m_animationClip[enSummonerAnimClip_DarkBall].Load("Assets/animData/character/Lich/Attack1.tka");
	m_animationClip[enSummonerAnimClip_DarkBall].SetLoopFlag(false);
	m_animationClip[enSummonerAnimClip_DarkWall].Load("Assets/animData/character/Lich/Attack2.tka");
	m_animationClip[enSummonerAnimClip_DarkWall].SetLoopFlag(false);
	m_animationClip[enSummonerAnimClip_ShockWave].Load("Assets/animData/character/Lich/ShockWave.tka");
	m_animationClip[enSummonerAnimClip_ShockWave].SetLoopFlag(false);
	m_animationClip[enSummonerAnimClip_KnockBackAttack].Load("Assets/animData/character/Lich/KnockBack.tka");
	m_animationClip[enSummonerAnimClip_KnockBackAttack].SetLoopFlag(false);
	m_animationClip[enSummonerAnimClip_FirstAttack].Load("Assets/animData/character/Lich/NormalAttack_1.tka");
	m_animationClip[enSummonerAnimClip_FirstAttack].SetLoopFlag(false);
	m_animationClip[enSummonerAnimClip_SecondAttack].Load("Assets/animData/character/Lich/NormalAttack_2.tka");
	m_animationClip[enSummonerAnimClip_SecondAttack].SetLoopFlag(false);
	m_animationClip[enSummonerAnimClip_ThirdAttack].Load("Assets/animData/character/Lich/NormalAttack_3.tka");
	m_animationClip[enSummonerAnimClip_ThirdAttack].SetLoopFlag(false);
	m_animationClip[enSummonerState_Attack_DarkMeteorite].Load("Assets/animData/character/Lich/DarkMeteorite.tka");
	m_animationClip[enSummonerState_Attack_DarkMeteorite].SetLoopFlag(false);
	m_animationClip[enSummonerAnimClip_Summon].Load("Assets/animData/character/Lich/Summon.tka");
	m_animationClip[enSummonerAnimClip_Summon].SetLoopFlag(false);
	m_animationClip[enSummonerAnimClip_Victory].Load("Assets/animData/character/Lich/Victory.tka");
	m_animationClip[enSummonerAnimClip_Victory].SetLoopFlag(true);
	m_animationClip[enSummonerAnimClip_Angry].Load("Assets/animData/character/Lich/Angry.tka");
	m_animationClip[enSummonerAnimClip_Angry].SetLoopFlag(false);
	m_animationClip[enSummonerAnimClip_Die].Load("Assets/animData/character/Lich/Die.tka");
	m_animationClip[enSummonerAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enSummonerAnimClip_Hit].Load("Assets/animData/character/Lich/Damage.tka");
	m_animationClip[enSummonerAnimClip_Hit].SetLoopFlag(false);
	m_animationClip[enSummonerAnimClip_Warp].Load("Assets/animData/character/Lich/Warp.tka");
	m_animationClip[enSummonerAnimClip_Warp].SetLoopFlag(false);

}


void Summoner::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//当たり判定生成開始アニメーションキーフレーム
	if (wcscmp(eventName, L"StartCreateCollision") == 0)
	{
		m_isCreateCollision = true;
	}
	//当たり判定生成終了アニメーションキーフレーム
	if (wcscmp(eventName, L"EndCreateCollision") == 0)
	{
		m_isCreateCollision = false;
	}

	//移動開始アニメーションキーフレーム
	if (wcscmp(eventName, L"StartAttackMove") == 0)
	{
		m_isAttackMove = true;
	}
	//移動終了アニメーションキーフレーム
	if (wcscmp(eventName, L"EndAttackMove") == 0)
	{
		m_isAttackMove = false;
	}
	

	
	//衝撃波攻撃アニメーションキーフレーム
	if (wcscmp(eventName, L"ShockWave") == 0)
	{
		m_aiController->CreateShockWaveCollision();

		//エフェクト生成
		UseEffect* effect = NewGO<UseEffect>(0, "ShockWaveEffect");
		effect->PlayEffect(enEffect_Boss_KnockBack,
			m_position, g_vec3One * SECOND_AND_THIRD_COMBO_EFFECT_SCALE, Quaternion::Identity, false);

		//音再生
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_Boss_ShockWave,
			g_soundManager->GetSEVolume()
		);

		//カメラを揺らす
		g_camera3D->StartCameraShake(
			60.0f, 60.0f, 0.25f
		);
	}

	//ダークボール生成アニメーションキーフレーム
	if (wcscmp(eventName, L"CreateDarkBall") == 0)
	{
		//音再生
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_Boss_DarkBall,
			g_soundManager->GetSEVolume()
		);

		m_aiController->CreateDarkBall();
	}
	

	//ダークメテオ生成位置決定アニメーションキーフレーム
	if (wcscmp(eventName, L"DecisionShotPoint") == 0)
	{
		m_aiController->DecisionDarkMeteoriteFallPoint();
	}
	//ダークメテオ生成アニメーションキーフレーム
	if (wcscmp(eventName, L"ShotDarkMeteo") == 0)
	{
		m_aiController->CreateDarkMeteorite();
	}


	//ワープ位置決定アニメーションキーフレーム
	if (wcscmp(eventName, L"DecisionWarpPoint") == 0)
	{
		m_aiController->DecisionWarpPoint();

		Vector3 position = m_aiController->GetWarpPosition();
		position.y = 3.0f;

		//ワープ先にエフェクト生成
		UseEffect* warpEffect = NewGO<UseEffect>(0, "WarpEffect");
		warpEffect->PlayEffect(enEffect_Boss_WarpSircle,
			position, g_vec3One * 20.0f, Quaternion::Identity, false);
	}
	//ワープスタートアニメーションキーフレーム
	if (wcscmp(eventName, L"WarpStart") == 0)
	{
		//音再生
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_Boss_Warp,
			g_soundManager->GetSEVolume()
		);

		//エフェクト生成
		UseEffect* effect = NewGO<UseEffect>(0, "WarpWindEffect");
		effect->PlayEffect(enEffect_Boss_WarpWind,
			m_position, g_vec3One * 30.0f, Quaternion::Identity, false);

		//座標をワープ先に設定
		m_aiController->WarpProcess();

		

	}

	//通常攻撃1のエフェクト発生イベントキーフレーム
	if (wcscmp(eventName, L"PlayNormalAttack1Effect") == 0)
	{
		Vector3 position = g_vec3Zero;
		Matrix m = m_modelRender.GetBone(m_staffBoonId)->GetWorldMatrix();
		m.Apply(position);
		Quaternion rot;
		rot.SetRotationYFromDirectionXZ(m_forward);

		UseEffect* effect = NewGO<UseEffect>(0, "SrashEffect");
		effect->PlayEffect(enEffect_Boss_Combo_1,
			position, g_vec3One * FIRST_COMBO_EFFECT_SCALE, rot, false);

		//音再生
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_Boss_Slash,
			g_soundManager->GetSEVolume()
		);
	}

	//通常攻撃2のエフェクト発生イベントキーフレーム
	if (wcscmp(eventName, L"PlayNormalAttack2Effect") == 0)
	{
		Vector3 position = g_vec3Zero;
		Matrix m = m_modelRender.GetBone(m_leftHandBoonId)->GetWorldMatrix();

		Quaternion rot;
		rot.SetRotationYFromDirectionXZ(m_forward);

		m.Apply(position);
		UseEffect* effect = NewGO<UseEffect>(0, "PokeEffect");
		effect->PlayEffect(enEffect_Boss_Combo_2,
			position, g_vec3One * SECOND_AND_THIRD_COMBO_EFFECT_SCALE, rot, false);

		//音再生
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_Boss_Poke,
			g_soundManager->GetSEVolume()
		);
	}

	//通常攻撃3のエフェクト発生イベントキーフレーム
	if (wcscmp(eventName, L"PlayNormalAttack3Effect") == 0)
	{
		//当たり判定発生
		m_aiController->CreateThirdComboCollision();

		Vector3 position = g_vec3Zero;
		Matrix m = m_modelRender.GetBone(m_rightHandBoonId)->GetWorldMatrix();

		m.Apply(position);
		UseEffect* effect = NewGO<UseEffect>(0, "ComboFinishEffect");
		effect->PlayEffect(enEffect_ComboFinishExplosion,
			position, g_vec3One * SECOND_AND_THIRD_COMBO_EFFECT_SCALE, Quaternion::Identity, false);

		//音再生
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_Boss_DarkBall_Explosion,
			g_soundManager->GetSEVolume()
		);
	}


}



void Summoner::SettingDamageInfo(EnSkillSType skillType)
{
	SetDamageInfo(
		KnockBackInfoManager::GetInstance()->GetAddAttackId(),
		m_skillStatus.GetSkillPower(skillType),
		m_skillStatus.GetSkillKnockBackTimeScale(skillType),
		m_skillStatus.GetSkillKnockBackPattern(skillType)
	);
}


