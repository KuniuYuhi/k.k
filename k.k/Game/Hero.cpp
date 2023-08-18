#include "stdafx.h"
#include "Hero.h"
#include "Player.h"
#include "HeroStateIdle.h"
#include "HeroStateWalk.h"
#include "HeroStateRun.h"
#include "HeroStateAttack_1.h"
#include "HeroStateAttack_2.h"
#include "HeroStateAttack_3.h"
#include "HeroStateAttack_4.h"
#include "HeroStateAttack_Skill_Main.h"
#include "HeroStateDie.h"
#include "HeroStateDamage.h"
#include "HeroStatePowerUp.h"
#include "HeroStateVictory.h"

namespace {
	int MAXHP = 200;
	int MAXMP = 100;
	int ATK = 50;
	float SPEED = 100.0f;
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
	m_animationClip[enAnimClip_Run].Load("Assets/animData/character/Player/SprintFWD_Battle.tka");
	m_animationClip[enAnimClip_Run].SetLoopFlag(true);
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

	//無敵時間でないなら当たり判定の処理を行う
	if (CalcInvincibleTime() == false&& CalcInvicibleDash()==false)
	{
		DamageCollision(m_player->GetCharacterController());
	}
	
	SetTransFormModel(m_modelRender);
	m_modelRender.Update();
}

void Hero::Move()
{
	//特定のアニメーションが再生中のとき
	//説明変える
	if (isAnimationSwappable() != true)
	{
		if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
		{
			if (g_pad[0]->IsPress(enButtonA))
			{
				//ダッシュした瞬間だけ無敵時間にする
				if (GetInvicibleDashState() == enDashInvicibleState_None)
				{
					SetInvicibleDashState(enDashInvicibleState_On);
					m_invincbledDashTimer = 0.0f;
				}
				
				m_dashFlag = true;
			}
			else
			{
				SetInvicibleDashState(enDashInvicibleState_None);
				m_dashFlag = false;
			}
		}
	}

	m_moveSpeed = m_player->GetMoveSpeed();
	m_position = m_player->GetPosition();
	
	Rotation();
}

bool Hero::RotationOnly()
{
	//スキルのチャージ時間の間
	if (isRotationEntable()!=true)
	{
		//xかzの移動速度があったら(スティックの入力があったら)。
		if (fabsf(m_SaveMoveSpeed.x) >= 0.001f || fabsf(m_SaveMoveSpeed.z) >= 0.001f)
		{
			m_rotation.SetRotationYFromDirectionXZ(m_SaveMoveSpeed);
		}
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
		Vector3(10.0f, 5.0f, 100.0f)
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
	//スキル使用時にダメージを受けたかもしれない
	m_createSkillCollisionFlag = false;

	if (m_status.hp > 0)
	{
		m_status.hp -= attack;
		SetInvicibleTimeFlag(true);
		SetNextAnimationState(enAnimationState_Damage);
	}

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
	case Hero::enAninationState_Run:
		// 走るステートを作成する。
		m_state = new HeroStateRun(this);
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
		if (m_dashFlag == true)
		{
			SetNextAnimationState(enAninationState_Run);
			return;
		}

		SetNextAnimationState(enAninationState_Walk);
	}
	else
	{
		m_dashFlag = false;
		SetNextAnimationState(enAninationState_Idle);
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
		m_ChargeTimer += g_gameTime->GetFrameDeltaTime()*2.0f;
		//
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

		m_enAttackPatternState = enAttackPattern_Skill_Main;
		SetNextAnimationState(enAnimationState_Attack_Skill_Main);
	}

}

void Hero::OnProcessAttack_Skill_MainStateTransition()
{
	//チャージした時間が切れたら
	if (m_ChargeTimer < 0.0f)
	{
		//ダッシュフラグをfalseにする
		m_dashFlag = false;

		m_createSkillCollisionFlag = false;

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
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		
	}
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
	//無敵時間の間は
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
	




