#include "stdafx.h"
#include "Summoner.h"

#include "InitEffect.h"
//マネージャー
#include "CharactersInfoManager.h"
#include "GameManager.h"
//ステートマシン
#include "SummonerSM_Attack.h"
#include "SummonerSM_Vigilance.h"
//各種ステート
#include "SummonerState_Idle.h"
#include "SummonerState_DarkBall.h"
#include "SummonerState_DarkWall.h"
#include "SummonerState_KnockBack.h"
#include "SummonerState_NAttack_1.h"
#include "SummonerState_NAttack_2.h"
#include "SummonerState_NAttack_3.h"
#include "SummonerState_DMeteo_Start.h"
#include "SummonerState_DMeteo_Main.h"
#include "SummonerState_Angry.h"
#include "SummonerState_Command.h"
#include "SummonerState_Hit.h"
#include "SummonerState_Die.h"
#include "SummonerState_Summon.h"
#include "SummonerState_Walk.h"
#include "SummonerState_Warp.h"
#include "SummonerState_Victory.h"
#include "SummonerState_DarkSpearMain.h"
#include "SummonerState_DarkSpearEnd.h"
#include "SummonerState_DarkSpearStart.h"
//技でだすオブジェクト
#include "DarkBall.h"
#include "DarkWall.h"
#include "ComboFinishBomb.h"

#include "GameSceneManager.h"
#include "AllGameSceneState.h"

namespace {
	const float SCALE_UP = 4.0f;		//キャラクターのサイズ

	const float MAX_SUPERARMOR_POINT = 180.0f;

	const float KNOCKBACK_RANGE = 300.0f;
	const float KNOCKBACK_POWER = 400.0f;
	const float KNOCKBACK_LIMMIT = 2.0f;

	const float ROT_SPEED = 5.0f;
	const float ROT_ONLY_SPEED = 5.0f;

	const Vector3 NORMAL_ATTACK_COLLISION_SIZE = { 20.0f, 430.0f,100.0f };
	const float NORMAL_ATTACK_1_SIZE = 17.0f;
	const float NORMAL_ATTACK_2_SIZE = 30.0f;
	const Vector3 KNOCKBACK_EFFECT_SIZE = { 30.0f,17.0f,30.0f };

	const float RECOVERY_SUPERARMOR_SPEED = 9.0f;
}

Summoner::Summoner()
{
}

Summoner::~Summoner()
{
	//死んだときにモブモンスターを消しているので消す必要なし
	if (GameManager::GetInstance()->GetBossDeleteOkFlag() == true)
	{
		return;
	}

	//モブモンスターを削除
	DeleteMobMonsters(false);
}

bool Summoner::Start()
{
	//　乱数を初期化。
	srand((unsigned)time(NULL));

	//プレイヤーのインスタンスを探す
	m_player = FindGO<Player>("player");

	//ステータスの初期化
	m_status.InitCharacterStatus(GetName());

	//モデルの初期化
	InitModel();
	//ステートマシンの生成
	SetStartStateMachine(enStateMachineState_Vigilance);

	//最初のアニメーション設定
	SetNextAnimationState(enAnimationState_Idle);

	//プレイヤーのインスタンスの代入
	m_player = CharactersInfoManager::GetInstance()->GetPlayerInstance();

	return true;
}

void Summoner::Update()
{
	//ポーズ画面なら処理をしない
	if (GameManager::GetInstance()->GetGameSeenState() ==
		GameManager::enGameSeenState_Pause)
	{
		return;
	}

	if (IsStopProcessing() != true)
	{
		//回転処理
		ProcessRotation();
		//当たり判定の処理
		DamageCollision(m_charaCon);
	}
	
	//スーパーアーマーの回復
	RecoverySuperArmor();

	//ステートマシンの毎フレームの処理
	m_stateMachine->Execute();
	//状態管理
	ManageState();

	//アニメーション
	PlayAnimation();

	//モデルのTRSと設定と更新
	SetTransFormModel(m_modelRender);
	m_modelRender.Update();
}

void Summoner::PlayAnimation()
{
	m_nowBossState->PlayAnimation();
}

void Summoner::ManageState()
{
	m_nowBossState->ManageState();
}

bool Summoner::IsStopProcessing()
{
	if (GameSceneManager::GetInstance()->GetCurrentGameSceneState() != enGameSceneState_Game)
	{
		return true;
	}

	switch (GameSceneManager::GetInstance()->GetBattleOutCome())
	{
	case GameSceneManager::enBattleOutCome_PlayerWin:
		//やられアニメーションステート
		SetNextAnimationState(enAnimationState_Die);
		m_modelRender.SetAnimationSpeed(0.7f);
		return true;
	case GameSceneManager::enBattleOutCome_PlayerLose:
		//勝利アニメーション再生
		SetNextAnimationState(enAnimationState_Victory);
		return true;

	default:
		break;
	}

	return false;
}

void Summoner::Damage(int attack)
{
	//ダメージを受ける処理
	ProcessHit(attack);
	//やられたとき
	if (m_status.GetHp() <= 0)
	{
		//ゲームマネージャーのプレイヤーの勝ちフラグを設定
		GameManager::GetInstance()->SetPlayerWinFlag(true);

		//ゲームシーンマネージャーにプレイヤーが勝利したことを伝える
		GameSceneManager::GetInstance()->
			SetBattleOutCome(GameSceneManager::enBattleOutCome_PlayerWin);

		//やられた時の処理
		ProcessDead();
		return;
	}
}

void Summoner::HitNormalAttack()
{
	if (GetDamageHitEnableFlag() == true)
	{
		Damage(m_player->GetAtk());
		CreateDamageFont(m_player->GetAtk());
		SetDamageHitEnableFlag(false);
		PlayHitSound();
	}
	return;
}

void Summoner::HitSkillAttack()
{
	//スキル攻撃を受けられないならダメージ処理をしない(多段ヒットのため)
	if (m_player->GetHittableFlag() != true)
	{
		return;
	}
	Damage(m_player->GetSkillAtk());
	CreateDamageFont(m_player->GetSkillAtk());
	//多段ヒットしたのでフラグをリセット。多段ヒットでなくとも
	m_player->SetHittableFlag(false);
}

void Summoner::ProcessDead(bool seFlag)
{
	//やられるところをゆっくりにする
	//フレームレートを落とす
	g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Variable, 30);
	m_status.SetHp(0);
	//効果音の再生
	if (seFlag == true)
	{

	}
}

void Summoner::ProcessRotation()
{
	//プレイヤーに向かって移動
	//プレイヤーの座標を取得
	m_targetPosition = m_player->GetPosition();
	//移動方向を設定
	m_moveSpeed = CalcVelocity(m_status, m_targetPosition);

	//前方向の設定
	m_forward = m_moveSpeed;
	m_forward.Normalize();

	//回転可能でないなら
	if (isRotationEnable() == false)
	{
		//回転しない
		return;
	}
	//前方向を使って回転の設定
	Rotation(ROT_SPEED, ROT_ONLY_SPEED);
}

void Summoner::ProcessHit(int hitDamage)
{
	//HPを減らす
	m_status.CalcHp(hitDamage, false);
	//被ダメージカウント加算
	m_damageCount++;
	//スーパーアーマーがブレイクしていないなら
	if (GetBreakSuperArmorFlag() == false)
	{
		//スーパーアーマーを減らす
		CalcSuperArmor(false, hitDamage);
	}
	//スーパーアーマーがブレイクしているなら
	if (GetBreakSuperArmorFlag() == true)
	{
		//一定確率で怯む
		//ブレイクした瞬間なら確定で怯む
		if (IsFlinch() == true || (m_oldBreakSuperArmorFlag != GetBreakSuperArmorFlag()))
		{
			//技の途中かもしれないのでダークウォールを削除
			if (m_darkWall != nullptr)
			{
				DeleteGO(m_darkWall);
			}
			//被ダメージアニメーション
			SetNextAnimationState(enAnimationState_CriticalHit);
		}
	}
	//前フレームのスーパーアーマーブレイクフラグを取得
	m_oldBreakSuperArmorFlag = GetBreakSuperArmorFlag();
}

void Summoner::RecoverySuperArmor()
{
	//スーパーアーマーがブレイクしていないなら処理しない
	if (GetBreakSuperArmorFlag() != true)
	{
		return;
	}
	//スーパーアーマーの回復
	CalcSuperArmor(true, g_gameTime->GetFrameDeltaTime() * RECOVERY_SUPERARMOR_SPEED);
}

void Summoner::SetNextAnimationState(EnAnimationState nextState)
{
	//古いステートを削除する
	if (m_nowBossState != nullptr)
	{
		delete m_nowBossState;
		m_nowBossState = nullptr;
	}
	//新しいステートを作成
	m_enAnimationState = nextState;

	switch (nextState)
	{
	case Summoner::enAnimationState_Idle://待機状態
		m_nowBossState = new SummonerState_Idle(this);
		break;
	case Summoner::enAnimationState_Walk://歩き状態
		m_nowBossState = new SummonerState_Walk(this);
		break;
	case Summoner::enAnimationState_DarkBall://ダークボール
		m_nowBossState = new SummonerState_DarkBall(this);
		break;
	case Summoner::enAnimationState_DarkWall://ダークウォール
		m_nowBossState = new SummonerState_DarkWall(this);
		break;
	case Summoner::enAnimationState_DarkSpear_Start://ダークスピアメイン
		m_nowBossState = new SummonerState_DarkSpearStart(this);
		break;
	case Summoner::enAnimationState_DarkSpear_Main://ダークスピアメイン
		m_nowBossState = new SummonerState_DarkSpearMain(this);
		break;
	case Summoner::enAnimationState_DarkSpear_End://ダークスピアエンド
		m_nowBossState = new SummonerState_DarkSpearEnd(this);
		break;
	case Summoner::enAnimationState_KnockBack://ノックバック
		m_nowBossState = new SummonerState_KnockBack(this);
		break;
	case Summoner::enAnimationState_NormalAttack_1://通常攻撃１
		m_nowBossState = new SummonerState_NAttack_1(this);
		break;
	case Summoner::enAnimationState_NormalAttack_2://通常攻撃２
		m_nowBossState = new SummonerState_NAttack_2(this);
		break;
	case Summoner::enAnimationState_NormalAttack_3://通常攻撃３
		m_nowBossState = new SummonerState_NAttack_3(this);
		break;
	case Summoner::enAnimationState_Attack_DarkMeteorite_start://ダークメテオスタート
		m_nowBossState = new SummonerState_DMeteo_Start(this);//ダークメテオメイン
		break;
	case Summoner::enAnimationState_Attack_DarkMeteorite_main:
		m_nowBossState = new SummonerState_DMeteo_Main(this);
		break;
	case Summoner::enAnimationState_Angry://怒りモード
		m_nowBossState = new SummonerState_Angry(this);
		break;
	case Summoner::enAninationState_Summon://召喚
		m_nowBossState = new SummonerState_Summon(this);
		break;
	case Summoner::enAnimationState_Command://モブに号令
		m_nowBossState = new SummonerState_Command(this);
		break;
	case Summoner::enAnimationState_CriticalHit://スーパーアーマー割られた時
		m_nowBossState = new SummonerState_Hit(this);
		break;
	case Summoner::enAnimationState_Die://やられた
		m_nowBossState = new SummonerState_Die(this);
		break;
	case Summoner::enAnimationState_Victory://勝利
		m_nowBossState = new SummonerState_Victory(this);
		break;

	default:
		// ここに来たらステートのインスタンス作成処理の追加忘れ。
		std::abort();
		break;
	}
	
}

void Summoner::ProcessCommonStateTransition()
{
	SetNextAnimationState(enAnimationState_Idle);
}

void Summoner::InitModel()
{
	//各種アニメーションクリップのロード
	m_animationClip[enAnimClip_Idle].Load(
		"Assets/animData/character/Lich/Idle.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Walk].Load(
		"Assets/animData/character/Lich/Walk.tka");
	m_animationClip[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClip[enAnimClip_DarkBall].Load(
		"Assets/animData/character/Lich/Attack1.tka");
	m_animationClip[enAnimClip_DarkBall].SetLoopFlag(false);
	m_animationClip[enAnimClip_DarkWall].Load(
		"Assets/animData/character/Lich/Attack2.tka");
	m_animationClip[enAnimClip_DarkWall].SetLoopFlag(false);
	m_animationClip[enAnimClip_DarkSpear_Start].Load(
		"Assets/animData/character/Lich/DarkSpear_Start.tka");
	m_animationClip[enAnimClip_DarkSpear_Start].SetLoopFlag(false);
	m_animationClip[enAnimClip_DarkSpear_Main].Load(
		"Assets/animData/character/Lich/DarkSpear_Main.tka");
	m_animationClip[enAnimClip_DarkSpear_Main].SetLoopFlag(false);
	m_animationClip[enAnimClip_DarkSpear_End].Load(
		"Assets/animData/character/Lich/DarkSpear_End.tka");
	m_animationClip[enAnimClip_DarkSpear_End].SetLoopFlag(false);
	m_animationClip[enAnimClip_KnockBack].Load(
		"Assets/animData/character/Lich/KnockBack.tka");
	m_animationClip[enAnimClip_KnockBack].SetLoopFlag(false);
	m_animationClip[enAnimClip_NormalAttack_1].Load(
		"Assets/animData/character/Lich/NormalAttack_1.tka");
	m_animationClip[enAnimClip_NormalAttack_1].SetLoopFlag(false);
	m_animationClip[enAnimClip_NormalAttack_2].Load(
		"Assets/animData/character/Lich/NormalAttack_2.tka");
	m_animationClip[enAnimClip_NormalAttack_2].SetLoopFlag(false);
	m_animationClip[enAnimClip_NormalAttack_3].Load(
		"Assets/animData/character/Lich/NormalAttack_3.tka");
	m_animationClip[enAnimClip_NormalAttack_3].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_DarkMeteorite_start].Load(
		"Assets/animData/character/Lich/DarkMeteorite_Start.tka");
	m_animationClip[enAnimClip_Attack_DarkMeteorite_start].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_DarkMeteorite_main].Load(
		"Assets/animData/character/Lich/DarkMeteorite_Main.tka");
	m_animationClip[enAnimClip_Attack_DarkMeteorite_main].SetLoopFlag(false);
	m_animationClip[enAnimClip_Command].Load(
		"Assets/animData/character/Lich/Command.tka");
	m_animationClip[enAnimClip_Command].SetLoopFlag(false);
	m_animationClip[enAnimClip_Summon].Load(
		"Assets/animData/character/Lich/Summon.tka");
	m_animationClip[enAnimClip_Summon].SetLoopFlag(false);
	m_animationClip[enAnimClip_Victory].Load(
		"Assets/animData/character/Lich/Victory.tka");
	m_animationClip[enAnimClip_Victory].SetLoopFlag(true);
	m_animationClip[enAnimClip_Angry].Load(
		"Assets/animData/character/Lich/Angry.tka");
	m_animationClip[enAnimClip_Angry].SetLoopFlag(false);
	m_animationClip[enAnimClip_Die].Load(
		"Assets/animData/character/Lich/Die.tka");
	m_animationClip[enAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enAnimClip_CriticalHit].Load(
		"Assets/animData/character/Lich/Damage.tka");
	m_animationClip[enAnimClip_CriticalHit].SetLoopFlag(false);
	
	m_modelRender.Init("Assets/modelData/character/Lich/Lich_real_Selllook.tkm",
		L"Assets/shader/ToonTextrue/lamp_Lich.DDS",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);

	m_targetPosition = m_player->GetPosition();
	m_moveSpeed= CalcVelocity(m_status, m_targetPosition);
	m_forward = m_moveSpeed;
	m_forward.Normalize();
	//プレイヤーの方向に回転
	Rotation(ROT_SPEED, ROT_ONLY_SPEED);

	//モデルのTRSの設定
	m_modelRender.SetTransform(
		m_position,
		m_rotation,
		m_scale *= SCALE_UP
	);
	m_modelRender.Update();

	//キャラコンの設定
	m_charaCon.Init(
		70.0f,
		50.0f,
		m_position
	);
	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
	});

	//ダークウォールに使うボーンID取得
	m_darkWallBoonId = m_modelRender.FindBoneID(L"Staff");
	//通常攻撃３で使うボーンのID取得
	m_comboFinishBoonId = m_modelRender.FindBoneID(L"Index_Proximal_R");
	//ハンドLのボーンのID取得
	m_handLBoonId = m_modelRender.FindBoneID(L"Hand_L");

}

void Summoner::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//ノックバックタイミングイベントキーフレーム
	if (wcscmp(eventName, L"KnockBack") == 0)
	{
		//ノックバック
		ProcessKnockBack();
	}
	//ダークウォール生成イベントキーフレーム
	if (wcscmp(eventName, L"Create_DarkWall") == 0)
	{
		CreateDarkWall();
	}
	//ダークウォール生成終了イベントキーフレーム
	if (wcscmp(eventName, L"CreateEnd_DarkWall") == 0)
	{
		if (m_darkWall != nullptr)
		{
			DeleteGO(m_darkWall);
		}
	}
	//ダークボール生成イベントキーフレーム
	if (wcscmp(eventName, L"Create_Ball") == 0)
	{
		//ボール生成
		CreateDarkBall();
	}
	//通常攻撃当たり判定生成
	if (wcscmp(eventName, L"Create_NormalAttackCollision") == 0)
	{
		//このアニメーションキーフレームの間当たり判定生成
		CreateNormalAttackCollision();
	}
	//通常攻撃１のイベントキーフレーム
	if (wcscmp(eventName, L"PlayNormalAttack1Effect") == 0)
	{
		Vector3 pos = g_vec3Zero;
		Quaternion rot = g_quatIdentity;
		SettingEffectInfo(
			pos, rot, m_modelRender.GetBone(m_handLBoonId)->GetWorldMatrix()
		);
		//攻撃エフェクト再生
		PlayNormalAttack1Effect(pos,rot);
	}
	//通常攻撃２のイベントキーフレーム
	if (wcscmp(eventName, L"PlayNormalAttack2Effect") == 0)
	{
		Vector3 pos = g_vec3Zero;
		Quaternion rot = g_quatIdentity;
		SettingEffectInfo(
			pos, rot, m_modelRender.GetBone(m_handLBoonId)->GetWorldMatrix()
		);
		//攻撃エフェクト再生
		PlayNormalAttack2Effect(pos, rot);
	}
	//通常攻撃の最後の爆発イベントキーフレーム
	if (wcscmp(eventName, L"Combofinnish") == 0)
	{
		//このアニメーションキーフレームの間当たり判定生成
		NormalComboFinnish();
	}
	//ノックバックエフェクトイベントキーフレーム
	if (wcscmp(eventName, L"PlayKnockBackEffect") == 0)
	{
		//ノックバックエフェクト再生
		PlayKnockBackEffect(m_position);
		//音再生
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_Boss_KnockBackWind,
			g_soundManager->GetSEVolume()
		);
	}

}

void Summoner::CreateDarkBall()
{
	DarkBall* darkBall = NewGO<DarkBall>(0, "darkball");
	darkBall->SetAttack(GetStatus().GetAtk());
	darkBall->Setting(GetPosition(),GetRotation());
}

void Summoner::CreateDarkWall()
{
	m_darkWall = NewGO<DarkWall>(0, "darkwall");
}

void Summoner::ProcessKnockBack()
{
	//既にプレイヤーをノックバックさせたなら
	if (GetPlayerKnockedBackFlag() == true)
	{
		return;
	}

	//プレイヤーが範囲内にいたらノックバックするようにする
	Vector3 diff = m_position -
		CharactersInfoManager::GetInstance()->GetPlayerInstance()->GetPosition();
	//ノックバック範囲内なら
	if (diff.Length() < KNOCKBACK_RANGE)
	{
		//プレイヤーをノックバックしたのでフラグをセット
		SetPlayerKnockedBackFlag(true);
		//プレイヤーがノックバックするための情報を設定
		CharactersInfoManager::GetInstance()->
			GetPlayerInstance()->SetKnockBackInfo(
				true, m_position, KNOCKBACK_POWER, KNOCKBACK_LIMMIT);
	}
}

void Summoner::CreateNormalAttackCollision()
{
	CollisionObject* collision = NewGO<CollisionObject>(0, "monsterattack");
	collision->CreateBox(
		m_position,
		g_quatIdentity,
		NORMAL_ATTACK_COLLISION_SIZE
	);
	collision->SetCreatorName(GetName());
	collision->SetWorldMatrix(m_modelRender.GetBone(m_darkWallBoonId)->GetWorldMatrix());
	collision->Update();
}

void Summoner::NormalComboFinnish()
{
	//爆発の中心座標のワールド座標を取得
	Matrix matrix = m_modelRender.GetBone(m_comboFinishBoonId)->GetWorldMatrix();

	//爆発エフェクトと当たり判定生成
	ComboFinishBomb* comboFinishBomb = 
		NewGO<ComboFinishBomb>(0, "combofinishbomb");
	
	//ワールド座標をローカル座標に適応
	comboFinishBomb->ApplyPositionToMatrix(matrix);

}

void Summoner::SettingEffectInfo(
	Vector3& effectPos, Quaternion& rot, Matrix matrix)
{
	effectPos = g_vec3Zero;
	Vector3 forwardPos = GetForward();
	matrix.Apply(effectPos);
	rot = g_quatIdentity;
	rot.SetRotationYFromDirectionXZ(forwardPos);
}

void Summoner::PlayNormalAttack1Effect(Vector3 position, Quaternion rotation)
{
	EffectEmitter* attack1Effect = NewGO<EffectEmitter>(0);
	attack1Effect->Init(InitEffect::enEffect_Boss_Combo_1);
	attack1Effect->Play();
	attack1Effect->SetScale(g_vec3One * NORMAL_ATTACK_1_SIZE);
	attack1Effect->SetPosition(position);
	attack1Effect->SetRotation(rotation);
	attack1Effect->Update();
}

void Summoner::PlayNormalAttack2Effect(Vector3 position, Quaternion rotation)
{
	EffectEmitter* attack1Effect = NewGO<EffectEmitter>(0);
	attack1Effect->Init(InitEffect::enEffect_Boss_Combo_2);
	attack1Effect->Play();
	attack1Effect->SetScale(g_vec3One * NORMAL_ATTACK_2_SIZE);
	attack1Effect->SetPosition(position);
	attack1Effect->SetRotation(rotation);
	attack1Effect->Update();
}

void Summoner::PlayKnockBackEffect(Vector3 position)
{
	EffectEmitter* KnockBackEffect = NewGO<EffectEmitter>(0);
	KnockBackEffect->Init(InitEffect::enEffect_Boss_KnockBack);
	KnockBackEffect->Play();
	KnockBackEffect->SetScale(KNOCKBACK_EFFECT_SIZE);
	KnockBackEffect->SetPosition(position);
	KnockBackEffect->Update();
}

void Summoner::ProcessVigilance()
{
	//移動処理
	m_position = m_charaCon.Execute(
		m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	return;
}

void Summoner::SetNextStateMachine(EnStateMachineState nextStateMachine)
{
	//現在のステートマシンの中身を削除する
	if (m_stateMachine != nullptr)
	{
		delete m_stateMachine;
		m_stateMachine = nullptr;
	}

	m_stateMachineState = nextStateMachine;

	switch (m_stateMachineState)
	{
	case Summoner::enStateMachineState_Vigilance:
		//警戒ステートマシンの生成
		m_stateMachine = new SummonerSM_Vigilance(this);
		break;
	case Summoner::enStateMachineState_Attack:
		//攻撃ステートマシンの生成
		m_stateMachine = new SummonerSM_Attack(this);
		break;
	default:
		std::abort();
		break;
	}

}

void Summoner::SetStartStateMachine(EnStateMachineState nextStateMachine)
{
	if (m_stateMachine != nullptr)
	{
		m_stateMachine = nullptr;
	}

	m_stateMachineState = nextStateMachine;

	switch (m_stateMachineState)
	{
	case Summoner::enStateMachineState_Vigilance:
		//最初の生成なので、タイマーをリセットする
		m_stateMachine = new SummonerSM_Vigilance(this, true);
		break;
	case Summoner::enStateMachineState_Attack:
		//最初の生成なので、前のアクションをリセットする
		m_stateMachine = new SummonerSM_Attack(this,true);
		break;
	default:
		std::abort();
		break;
	}
}

void Summoner::ProcessEndAttackState()
{
	//警戒ステートマシンに変更
	SetNextStateMachine(Summoner::enStateMachineState_Vigilance);
	//攻撃がおわったので連続攻撃回数を0にリセットする
	GetNowStateMachine()->SetContinuousAttackCount(0);
	//共通ステート処理
	ProcessCommonStateTransition();
}

void Summoner::DeleteMobMonsters(bool processDeadFlag)
{
	//モブモンスターが0体でないならリスト内のモブモンスターを死亡
	int mobMonsterNum = CharactersInfoManager::GetInstance()->GetMobMonsters().size();
	if (mobMonsterNum == 0)
		return;
	//存在しているモンスターを削除
	for (auto monster : CharactersInfoManager::GetInstance()->GetMobMonsters())
	{
		//死亡時処理フラグがセットされているなら
		if (processDeadFlag == true)
		{
			//エフェクトの生成(効果音なし)
			monster->ProcessDead(false);
		}
		//モブモンスターの削除
		DeleteGO(monster);
	}
	//リストをクリア
	CharactersInfoManager::GetInstance()->GetMobMonsters().clear();
}

void Summoner::Render(RenderContext& rc)
{
	if (GetIsDrawModelFlag() != true)
	{
		return;
	}

	m_modelRender.Draw(rc);

}