#include "stdafx.h"
#include "Hero.h"
#include "Player.h"
#include "HeroStateIdle.h"
#include "HeroStateWalk.h"
#include "HeroStateDash.h"
#include "HeroStateAttack_1.h"
#include "HeroStateAttack_2.h"
#include "HeroStateAttack_3.h"
#include "HeroStateAttack_4.h"
#include "HeroStateAttack_Skill_Main.h"
#include "HeroStateDie.h"
#include "HeroStateDamage.h"
#include "HeroStatePowerUp.h"
#include "HeroStateVictory.h"

#include "InitEffect.h"

//todo ダッシュ時攻撃アニメーション変更

namespace {
	const float ROT_SPEED = 2.0f;
	const float ROT_ONLY_SPEED = 2.2f;

	const float SWORD_EFFECT_SIZE = 10.0f;
	const Vector3 DASH_EFFECT_SIZE = { 10.0f,20.0f,8.0f };

	int MAXHP = 200;
	int MAXMP = 100;
	int ATK = 50;
	float SPEED = 200.0f;
	const char* NAME = "Hero";

	const int SKILL_ATTACK_POWER = 30;
}

Hero::Hero()
{
	m_skillAttackPower = SKILL_ATTACK_POWER;
}

Hero::~Hero()
{
}

bool Hero::Start()
{
	m_player = FindGO<Player>("player");

	// 初期のアニメーションステートを待機状態にする。
	SetNextAnimationState(enAninationState_Idle);

	//ステータスの初期化
	m_status.InitStatus(
	MAXHP,
	MAXMP,
	ATK,
	SPEED,
	NAME
	);

	InitModel();




	return true;
}

void Hero::InitModel()
{
	//アニメーションクリップをロードする。
	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/Player/Idle_Normal.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Walk].Load("Assets/animData/character/Player/MoveFWD_Battle.tka");
	m_animationClip[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClip[enAnimClip_Dash].Load("Assets/animData/character/Player/Dash.tka");
	m_animationClip[enAnimClip_Dash].SetLoopFlag(true);
	m_animationClip[enAnimClip_Attack_1].Load("Assets/animData/character/Player/Attack1_1.tka");
	m_animationClip[enAnimClip_Attack_1].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_2].Load("Assets/animData/character/Player/Attack2.tka");
	m_animationClip[enAnimClip_Attack_2].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_3].Load("Assets/animData/character/Player/Attack3.tka");
	m_animationClip[enAnimClip_Attack_3].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_Skill_Charge].Load("Assets/animData/character/Player/Attack_Skill_Charge.tka");
	m_animationClip[enAnimClip_Attack_Skill_Charge].SetLoopFlag(true);
	m_animationClip[enAnimClip_Attack_Skill_Main].Load("Assets/animData/character/Player/Attack_Skill_Main.tka");
	m_animationClip[enAnimClip_Attack_Skill_Main].SetLoopFlag(true);
	m_animationClip[enAnimClip_Die].Load("Assets/animData/character/Player/Dead.tka");
	m_animationClip[enAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enAnimClip_Damage].Load("Assets/animData/character/Player/Damage.tka");
	m_animationClip[enAnimClip_Damage].SetLoopFlag(false);
	m_animationClip[enAnimClip_PowerUp].Load("Assets/animData/character/Player/PowerUp.tka");
	m_animationClip[enAnimClip_PowerUp].SetLoopFlag(false);
	m_animationClip[enAnimClip_Victory].Load("Assets/animData/character/Player/Victory.tka");
	m_animationClip[enAnimClip_Victory].SetLoopFlag(true);



	m_modelRender.Init(
		"Assets/modelData/character/Player/Hero.tkm",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);

	//m_modelRender.SetPosition(50.0f, 0.0f, 50.0f);
	m_scale = { 1.2f,1.2f,1.2f };
	m_modelRender.SetScale(m_scale);
	m_modelRender.Update();

	//ボーンIDの取得
	m_swordBoonId = m_modelRender.FindBoneID(L"hand_r");
	m_skillBoonId = m_modelRender.FindBoneID(L"spine_03");

	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});


	

}

void Hero::Update()
{
	//勝ったときに処理しない
	//やられたなら他の処理を実行しない
	if (GetDieFlag() == true|| m_player->GetGameEndFlag() == true)
	{
		m_invincibleTimeFlag = false;
		ManageState();
		PlayAnimation();
		m_modelRender.Update();
		return;
	}



	RecoveryMP();
	Move();
	Attack();
	ManageState();
	PlayAnimation();

	PowerUpTimer();

	if (m_createAttackCollisionFlag == true)
	{
		CreateCollision();
	}

	if (m_createSkillCollisionFlag == true)
	{
		CreateSkillCollision();
	}

	CalcInvincibleTime();

	//無敵時間でないなら当たり判定の処理を行う
	if (CalcInvicibleDash() == false)
	{
		DamageCollision(m_player->GetCharacterController());
	}
	
	SetTransFormModel(m_modelRender);
	m_modelRender.Update();
}

void Hero::Move()
{
	//特定のアニメーションが再生中のとき
	//入れ替え可能アニメーションのときだけ
	if (isAnimationSwappable() != true)
	{
		if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
		{
			//リジットボディでダッシュ
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				//ダッシュ用の前方向を取得
				m_dashForward = m_forward;
				//ダッシュした瞬間だけ無敵状態にする
				SetInvicibleDashState(enDashInvicibleState_On);
				m_invincbledDashTimer = 0.0f;
				//ダッシュの途中で攻撃したりダメージ受けたかもしれないのでタイマーリセット
				m_dashTimer = 1.0f;
				SetNextAnimationState(enAninationState_Dash);
				//ダッシュエフェクト再生
				//ダッシュエフェクトの読み込み
				m_dashEffect = NewGO<EffectEmitter>(0);
				m_dashEffect->Init(InitEffect::enEffect_Dash);
				m_dashEffect->Play();
				m_dashEffect->SetScale(DASH_EFFECT_SIZE);
				m_dashEffect->SetPosition(m_position);
				m_dashEffect->SetRotation(m_rotation);
				m_dashEffect->Update();
				m_dashEffectFlag = true;
			}	
		}
	}

	m_moveSpeed = m_player->GetMoveSpeed();
	m_position = m_player->GetPosition();
	
	Rotation(ROT_SPEED, ROT_ONLY_SPEED);

	if (m_dashEffectFlag ==true)
	{
		m_dashEffect->SetPosition(m_position);
		m_dashEffect->SetRotation(m_rotation);
		m_dashEffect->Update();
	}
}

bool Hero::RotationOnly()
{
	//スキルのチャージ時間の間
	if (isRotationEntable()!=true)
	{
		return true;
	}

	return false;
}

void Hero::Attack()
{
	//被ダメージ時、デス時に攻撃しない
	if (isCollisionEntable() != true)
	{
		return;
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	// 通常攻撃
	///////////////////////////////////////////////////////////////////////////////////////////
	//1コンボ
	if (g_pad[0]->IsTrigger(enButtonB)&& m_enAttackPatternState==enAttackPattern_None)
	{
		
		
		m_enAttackPatternState = enAttackPattern_1;
		SetNowComboState(enNowCombo_1);
		SetNextAnimationState(enAnimationState_Attack_1);
		return;
	}
	//2コンボ目受付
	if (m_enAttackPatternState == enAttackPattern_1)
	{
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			m_enAttackPatternState = enAttackPattern_1to2;
			return;
		}
	}
	//3コンボ目受付
	if (m_enAttackPatternState == enAttackPattern_2)
	{
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			m_enAttackPatternState = enAttackPattern_2to3;
			return;
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	// スキル
	/////////////////////////////////////////////////////////////////////////////////////////


	//まずはチャージ
	if (m_enAttackPatternState == enAttackPattern_None ||
		m_enAttackPatternState == enAnimationState_Attack_Skill_Charge)
	{
		//MPがスキルのMPより多かったら
		if (g_pad[0]->IsPress(enButtonX)&&m_status.mp>=m_skillMp)
		{
			
			m_enAttackPatternState = enAttackPattern_Skill_Charge;
			SetNextAnimationState(enAnimationState_Attack_Skill_Charge);
			//MP回復状態を止める
			SetRecoveryMpFlag(false);
			return;
		}
		
	}

	//パワーアップ
	if (m_enAttackPatternState == enAttackPattern_None)
	{
		//MPがパワーアップのMPより多かったら
		if (g_pad[0]->IsPress(enButtonY) && m_status.mp >= m_skillPowerUpMp)
		{
			m_enAttackPatternState = enAttackPattern_Skill_PowerUp;
			SetNextAnimationState(enAnimationState_PowerUp);
			//MP回復状態を止める
			SetRecoveryMpFlag(false);
		}	
	}
}

void Hero::PlayAnimation()
{
	m_state->PlayAnimation();
}

void Hero::CreateCollision()
{
	auto AtkCollision = NewGO<CollisionObject>(0,"Attack");
	
	//ボックスの当たり判定作成
	AtkCollision->CreateBox(
		m_position,
		Quaternion(90.0f,0.0f,0.0f,1.0f),
		Vector3(11.0f, 5.0f, 140.0f)
	);

	//剣のボーンのワールド座標を取得
	Matrix SwordBoonMatrix = m_modelRender.GetBone(m_swordBoonId)->GetWorldMatrix();
	
	AtkCollision->SetWorldMatrix(SwordBoonMatrix);
}

void Hero::CreateSkillCollision()
{
	auto SkillCollision = NewGO<CollisionObject>(0, "SkillAttack");

	//カプセルの当たり判定作成
	SkillCollision->CreateSphere(
		m_position,
		Quaternion::Identity,
		70.0f
	);
	//剣のボーンのワールド座標を取得
	Matrix SkillBoonMatrix = m_modelRender.GetBone(m_skillBoonId)->GetWorldMatrix();

	SkillCollision->SetWorldMatrix(SkillBoonMatrix);
}

void Hero::Damage(int attack)
{
	//スキル使用中なら
	{
		m_dashFlag = false;
		//スキルを打ち終わったのでMP回復フラグをtrueにする
		SetRecoveryMpFlag(true);
		//スキル使用時にダメージを受けたかもしれない
		m_createSkillCollisionFlag = false;
		//エフェクトの停止
		if (m_swordStormEffect != nullptr)
		{
			m_swordStormEffect->Stop();
		}
	}
	//HPが0より大きいなら
	if (m_status.hp > 0)
	{
		m_status.hp -= attack;
		SetInvicibleTimeFlag(true);
		SetNextAnimationState(enAnimationState_Damage);
	}
	//0か0以下なら
	if (m_status.hp <= 0)
	{
		//やられたのでdieFlagをtrueにする
		SetDieFlag(true);
		m_status.hp = 0;

		//フレームレートを落とす
		g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Variable, 30);

		SetNextAnimationState(enAnimationState_Die);
	}
}

void Hero::PowerUpTimer()
{
	//パワーアップフラグが立っていないと処理しない
	if (m_powerUpTimeFlag != true)
	{
		return;
	}
	//制限時間に達したら
	if (m_powerUpTime < m_powerUpTimer)
	{
		//パワーアップ終了
		m_powerUpTimer = 0.0f;
		m_powerUpTimeFlag = false;
		//攻撃力を戻す
		m_status.atk = m_status.defaultAtk;
		m_skillAttackPower = SKILL_ATTACK_POWER;
	}
	else
	{
		m_powerUpTimer += g_gameTime->GetFrameDeltaTime();
	}
}

bool Hero::CalcDash()
{
	if (m_dashTimer < 0.2f)
	{
		//ダッシュ終わり
		m_dashTimer = 1.0f;

		return true;
	}
	//線形補間
	float speed;
	speed = Math::Lerp(m_dashTimer, 0.0f, 8.0f);

	m_dashTimer -= g_gameTime->GetFrameDeltaTime()*(1.0f);

	Vector3 forward = m_dashForward;
	forward *= speed;

	//新しい座標にする
	m_player->CalcPosition(forward, 1.0f);
	m_position = m_player->GetPosition();

	forward.Normalize();
	m_forward = forward;

	//更新
	SetTransFormModel(m_modelRender);
	m_modelRender.Update();

	return false;
}

void Hero::SetNextAnimationState(EnAnimationState nextState)
{
	if (m_state != nullptr) {
		// 古いステートを削除する。
		delete m_state;
		m_state = nullptr;
	}

	//アニメーションステートを次のステートに変える
	m_enAnimationState = nextState;

	switch (m_enAnimationState)
	{
	case Hero::enAninationState_Idle:
		// 待機ステートを作成する。
		m_state = new HeroStateIdle(this);
		break;
	case Hero::enAninationState_Walk:
		// 歩きステートを作成する。
		m_state = new HeroStateWalk(this);
		break;
	case Hero::enAninationState_Dash:
		// ダッシュステートを作成する。
		m_state = new HeroStateDash(this);
		break;
	case Hero::enAnimationState_Attack_1:
		//アタック１ステートを作成する。
		m_state = new HeroStateAttack_1(this);
		break;
	case Hero::enAnimationState_Attack_2:
		//アタック２ステートを作成する。
		m_state = new HeroStateAttack_2(this);
		break;
	case Hero::enAnimationState_Attack_3:
		//アタック３ステートを作成する。
		m_state = new HeroStateAttack_3(this);
		break;
	case Hero::enAnimationState_Attack_Skill_Charge:
		//アタックスキルチャージステートを作成する。
		m_state = new HeroStateAttack_4(this);
		break;
	case Hero::enAnimationState_Attack_Skill_Main:
		//アタックスキルメインステートを作成する。
		m_state = new HeroStateAttack_Skill_Main(this);
		break;
	case Hero::enAnimationState_Die:
		//Dieメインステートを作成する。
		m_state = new HeroStateDie(this);
		break;
	case Hero::enAnimationState_Damage:
		//被ダメージメインステートを作成する。
		m_state = new HeroStateDamage(this);
		break;
	case Hero::enAnimationState_PowerUp:
		//パワーアップメインステートを作成する。
		m_state = new HeroStatePowerUp(this);
		break;
	case Hero::enAnimationState_Victory:
		//勝利メインステートを作成する。
		m_state = new HeroStateVictory(this);
		break;
	default:
		// ここに来たらステートのインスタンス作成処理の追加忘れ。
		// クラッシュする
		std::abort();
		break;
	}
}

//状態遷移管理
void Hero::ManageState()
{
	m_state->ManageState();
}

//共通の状態遷移処理
void Hero::ProcessCommonStateTransition()
{
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		SetNextAnimationState(enAninationState_Walk);
	}
	else
	{
		m_dashEffectFlag = false;
		SetNextAnimationState(enAninationState_Idle);
	}
}

void Hero::OnProcessDashStateTransition()
{
	if (CalcDash() == true)
	{
		m_dashEffect->Stop();
		
		m_dashEffectFlag = false;

		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

//アタック１の状態遷移処理
void Hero::OnProcessAttack_1StateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//攻撃パターンをなし状態にする
		m_enAttackPatternState = enAttackPattern_None;
		//コンボが終わったら
		SetNowComboState(enNowCombo_None);
		SetDamagedComboState(enDamageCombo_None);
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}

}

void Hero::OnProcessAttack_2StateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//攻撃パターンをなし状態にする
		m_enAttackPatternState = enAttackPattern_None;
		//コンボが終わったら
		SetNowComboState(enNowCombo_None);
		SetDamagedComboState(enDamageCombo_None);
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Hero::OnProcessAttack_3StateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//コンボが終わりなのでコンボ状態をなしに戻す
		SetNowComboState(enNowCombo_None);
		SetDamagedComboState(enDamageCombo_None);
		//攻撃パターンをなし状態にする
		m_enAttackPatternState = enAttackPattern_None;
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Hero::OnProcessAttack_Skill_ChargeStateTransition()
{
	//チャージ時間を計算
	if (m_MaxChargeTime > m_ChargeTimer)
	{
		//チャージ
		m_ChargeTimer += g_gameTime->GetFrameDeltaTime()*2.0f;
		//最大まで溜まったら最大値にする
		if (m_ChargeTimer > m_MaxChargeTime)
		{
			m_ChargeTimer = m_MaxChargeTime;
		}
	}
	
	//ボタンを離したらスキルメインに移る
	if (g_pad[0]->IsPress(enButtonX) == false)
	{
		//最低でも1秒はスキルが発動するようにする
		if (m_ChargeTimer < 1.0f) {
			m_ChargeTimer = 1.0f;
		}

		//MPを減らす
		m_status.mp -= m_skillMp;

		//スキルだがダッシュしたことにする
		m_dashFlag = true;
		//当たり判定作成
		m_createSkillCollisionFlag = true;
		//ステートをメインに遷移
		m_enAttackPatternState = enAttackPattern_Skill_Main;
		SetNextAnimationState(enAnimationState_Attack_Skill_Main);
		//エフェクト再生
		m_swordStormEffect = NewGO<EffectEmitter>(0);
		m_swordStormEffect->Init(InitEffect::enEffect_SwordStorm);
		m_swordStormEffect->Play();
		m_swordStormEffect->SetPosition(m_position);
		m_swordStormEffect->SetScale(g_vec3One * SWORD_EFFECT_SIZE);
		m_swordStormEffect->Update();

	}

}

void Hero::OnProcessAttack_Skill_MainStateTransition()
{
	//チャージした時間が切れたら
	if (m_ChargeTimer < 0.0f)
	{
		//ダッシュフラグをfalseにする
		m_dashFlag = false;
		//当たり判定の生成をやめる
		m_createSkillCollisionFlag = false;
		//エフェクトの停止
		m_swordStormEffect->Stop();
		//チャージ時間をリセット
		m_ChargeTimer = 0.0f;
		//スキルを打ち終わったのでMP回復フラグをtrueにする
		SetRecoveryMpFlag(true);
		//攻撃パターンをなし状態にする
		m_enAttackPatternState = enAttackPattern_None;
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
	else
	{
		//エフェクトの座標を更新
		m_swordStormEffect->SetPosition(m_position);
		m_swordStormEffect->Update();
		//チャージした時間を減らす
		m_ChargeTimer -= g_gameTime->GetFrameDeltaTime();
	}
}

void Hero::OnProcessDieStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//フレームレートを戻す
		g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Variable, 60);
		//アニメーションが終わったのでキャラクターを切り替えるフラグをtrueにする
		SetDieToChangeFlag(true);
	}
}

void Hero::OnProcessDamageStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//行動の途中かもしれないのでステートやフラグを初期に戻す
		SetInvicibleDashState(enDashInvicibleState_None);
		//攻撃パターンをなし状態にする
		m_enAttackPatternState = enAttackPattern_None;
		m_createAttackCollisionFlag = false;
		//コンボが終わりなのでコンボ状態をなしに戻す
		SetNowComboState(enNowCombo_None);
		SetDamagedComboState(enDamageCombo_None);

		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Hero::OnProcessPowerUpStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//一定時間パワーアップを開始
		m_powerUpTimeFlag = true;
		//攻撃力を上げる
		m_status.atk += m_powerUpPower;
		m_skillAttackPower += m_powerUpPower;
		//スキルを打ち終わったのでMP回復フラグをtrueにする
		SetRecoveryMpFlag(true);
		//攻撃パターンをなし状態にする
		m_enAttackPatternState = enAttackPattern_None;
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Hero::OnProcessVictoryStateTransition()
{
}

void Hero::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//アタック1のコンボ受付タイムが始まったら
	if (wcscmp(eventName, L"Attack1_ComboStart") == 0)
	{
		m_createAttackCollisionFlag = true;
	}
	
	if (wcscmp(eventName, L"Attack1_ClollisionEnd") == 0)
	{
		m_createAttackCollisionFlag = false;
	}

	//アタック1のコンボ受付タイムが終わったら
	if (wcscmp(eventName, L"Attack1_ComboEnd") == 0)
	{
		//2コンボ
		if (m_enAttackPatternState == enAttackPattern_1to2)
		{
			m_enAttackPatternState = enAttackPattern_2;
			SetNowComboState(enNowCombo_2);
			SetNextAnimationState(enAnimationState_Attack_2);
		}
	}

	//アタック２のコンボ受付タイムが始まったら
	if (wcscmp(eventName, L"Attack2_ComboStart") == 0)
	{
		m_createAttackCollisionFlag = true;
		
	}
	//アタック２のが終わったら
	if (wcscmp(eventName, L"Attack2_CollisionDelete") == 0)
	{
		m_createAttackCollisionFlag = false;
	}
	//アタック２のコンボ受付タイムが終わったら
	if (wcscmp(eventName, L"Attack2_ComboEnd") == 0)
	{
		//3コンボ
		if (m_enAttackPatternState == enAttackPattern_2to3)
		{
			m_enAttackPatternState = enAttackPattern_3;
			SetNowComboState(enNowCombo_3);
			SetNextAnimationState(enAnimationState_Attack_3);
		}
		
	}

	//アタック３のコンボ受付タイムが始まったら
	if (wcscmp(eventName, L"Attack3_ComboStart") == 0)
	{
		m_createAttackCollisionFlag = true;

	}
	//アタック３のコンボ受付タイムが終わったら
	if (wcscmp(eventName, L"Attack3_ComboEnd") == 0)
	{
		m_createAttackCollisionFlag = false;
	}

	//パワーアップ！！
	if (wcscmp(eventName, L"PowerUp") == 0)
	{
		//MPを減らす
		m_status.mp -= m_skillPowerUpMp;
	}

}

void Hero::Render(RenderContext& rc)
{
	//無敵時間の間は点滅
	if (m_invincibleTimeFlag == true)
	{
		if (m_modelDrawFlag == true)
		{
			m_modelRender.Draw(rc);
			m_modelDrawFlag = false;
		}
		else
		{
			m_modelDrawFlag =! m_modelDrawFlag;
		}
	}
	else
	m_modelRender.Draw(rc);

	
}
	




