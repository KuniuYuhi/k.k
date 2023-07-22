#include "stdafx.h"
#include "Wizard.h"
#include "Player.h"
#include "WizardStateIdle.h"
#include "WizardStateWalk.h"
#include "WizardStateRun.h"
#include "WizardStateAttack_1.h"
//スキル入れ替えて使える
//連続攻撃ファイヤーボール
#include "WizardStateAttack_2_Start.h"
#include "WizardStateAttack_2_main.h"
#include "FireBall.h"
//フレイムーピラー
#include "WizardStateAttack_3_Start.h"
#include "WizardStateAttack_3_main.h"
#include "WizardState_Attack_4.h"
#include "FlamePillar.h"
#include "WizardStateDie.h"

namespace {
	int MAXHP = 150;
	int MAXMP = 200;
	int ATK = 30;
	float SPEED = 80.0f;
	const char* NAME = "Wizard";

	
}

Wizard::Wizard()
{
}

Wizard::~Wizard()
{
}

bool Wizard::Start()
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

	//非アクティブ化
	Deactivate();


	m_fireBallTimer = m_createFireBallTime;

	m_wandRotation.AddRotationDegX(50.0f);


	return true;
}

void Wizard::InitModel()
{
	//Attack3_maintain
	//アニメーションクリップをロードする。
	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/Wizard/Idle.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Walk].Load("Assets/animData/character/Wizard/Walk_Normal.tka");
	m_animationClip[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClip[enAnimClip_Run].Load("Assets/animData/character/Wizard/Run.tka");
	m_animationClip[enAnimClip_Run].SetLoopFlag(true);
	m_animationClip[enAnimClip_Attack_1].Load("Assets/animData/character/Wizard/Attack1.tka");
	m_animationClip[enAnimClip_Attack_1].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_2_start].Load("Assets/animData/character/Wizard/Attack2_start.tka");
	m_animationClip[enAnimClip_Attack_2_start].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_2_main].Load("Assets/animData/character/Wizard/Attack2_maintain.tka");
	m_animationClip[enAnimClip_Attack_2_main].SetLoopFlag(true);
	m_animationClip[enAnimClip_Attack_3_start].Load("Assets/animData/character/Wizard/Attack3_start.tka");
	m_animationClip[enAnimClip_Attack_3_start].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_3_main].Load("Assets/animData/character/Wizard/Attack3_maintain.tka");
	m_animationClip[enAnimClip_Attack_3_main].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_4].Load("Assets/animData/character/Wizard/Attack4.tka");
	m_animationClip[enAnimClip_Attack_4].SetLoopFlag(false);
	m_animationClip[enAnimClip_Die].Load("Assets/animData/character/Wizard/Die.tka");
	m_animationClip[enAnimClip_Die].SetLoopFlag(false);


	m_modelRender.Init(
		"Assets/modelData/character/Wizard/Wizard.tkm",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);
	//m_position = { 70.0f, 0.0f, 0.0f };
	//m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	//ボーンIDの取得
	m_magicWandBoonId = m_modelRender.FindBoneID(L"Weapon");


	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//m_charaCon.Init(12.0f, 33.0f, m_position);
}

void Wizard::Update()
{
	//やられたなら他の処理を実行しない
	if (GetDieFlag() == true)
	{
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

	if (m_createAttackCollisionFlag == true)
	{
		CreateCollision();
	}
	
	Damage(10);

	SetTransFormModel(m_modelRender);
	m_modelRender.Update();
}

void Wizard::Move()
{
	if (g_pad[0]->IsPress(enButtonA))
	{
		m_dashFlag = true;
	}
	else
	{
		m_dashFlag = false;
	}
	
	m_moveSpeed = m_player->GetMoveSpeed();
	m_position = m_player->GetPosition();
	
	Rotation();
}

bool Wizard::RotationOnly()
{
	//ファイヤーボールを打っているとき
	if (m_enAttackPatternState == enAttackPattern_2_main)
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

void Wizard::Attack()
{
	//スキルの切り替え
	if (g_pad[0]->IsTrigger(enButtonRB3))
	{
		switch (m_enSkillPatternState)
		{
			//フレイムピラーからファイヤーボールに切り替え
		case Wizard::enSkillPattern_FlamePillar:
			m_enSkillPatternState = enSkillPattern_FireBall;
			break;
			//ファイヤーボールからフレイムピラーに切り替え
		case Wizard::enSkillPattern_FireBall:
			m_enSkillPatternState = enSkillPattern_FlamePillar;
			break;
		default:
			break;
		}
	}


	////////////////////////////////////////////////////////////////////////////////////////
	// 通常攻撃
	////////////////////////////////////////////////////////////////////////////////////////
	//1コンボ
	if (g_pad[0]->IsTrigger(enButtonY)&& m_enAttackPatternState==enAttackPattern_None)
	{
		m_enAttackPatternState = enAttackPattern_1;
		SetNowComboState(enNowCombo_1);
		SetNextAnimationState(enAnimationState_Attack_1);
		return;
	}
	//2コンボ受付タイム
	if (g_pad[0]->IsTrigger(enButtonY) && m_enAttackPatternState == enAttackPattern_1)
	{
		m_enAttackPatternState = enAttackPattern_1to4;
		return;
	}



	////////////////////////////////////////////////////////////////////////////////////////
	// スキル
	////////////////////////////////////////////////////////////////////////////////////////

	

	
	
	if (g_pad[0]->IsTrigger(enButtonX) && m_enAttackPatternState == enAttackPattern_None)
	{
		//フレイムピラー
		if (m_enSkillPatternState == enSkillPattern_FlamePillar)
		{
			//スキルのMPが足りないなら抜け出す
			if (m_status.mp < m_flamePillar_skillMp) {
				return;
			}
			m_enAttackPatternState = enAttackPattern_3_start;
			SetNextAnimationState(enAnimationState_Attack_3_start);
			//MP回復状態を止める
			SetRecoveryMpFlag(false);
			return;
		}
		//ファイヤーボール
		else
		{
			//スキルのMPが足りないなら抜け出す
			if (m_status.mp < m_fireBall_SkillMp) {
				return;
			}
			m_enAttackPatternState = enAttackPattern_2_start;
			SetNextAnimationState(enAnimationState_Attack_2_start);
			//MP回復状態を止める
			SetRecoveryMpFlag(false);
			return;
		}
	}
	

}

void Wizard::CreateCollision()
{
	auto AtkCollision = NewGO<CollisionObject>(0, "Attack");

	//ボックスの当たり判定作成
	AtkCollision->CreateBox(
		m_position,
		m_wandRotation,
		Vector3(10.0f, 100.0f, 10.0f)
	);

	//杖のボーンのワールド座標を取得
	Matrix WandBoonMatrix = m_modelRender.GetBone(m_magicWandBoonId)->GetWorldMatrix();
	/*Matrix WandRotMatrix;
	WandRotMatrix.MakeRotationFromQuaternion(m_wandRotation);

	Matrix finalWandBoonMatrix = WandBoonMatrix * WandRotMatrix;*/

	AtkCollision->SetWorldMatrix(WandBoonMatrix);
}

void Wizard::Damage(int attack)
{
	if (m_status.hp > 0)
	{
		m_status.hp -= attack;
	}

	if (m_status.hp <= 0)
	{
		//Dieフラグをtrueにする
		SetDieFlag(true);
		m_status.hp = 0;
		//フレームレートを落とす
		g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Variable, 30);

		SetNextAnimationState(enAnimationState_Die);
	}
}

void Wizard::CreateFlamePillar()
{
	//フレイムピラー生成
	FlamePillar* flamePillar = NewGO<FlamePillar>(0, "flamepillar");
	flamePillar->SetWizard(this);
	//MPを減らす
	m_status.mp -= m_flamePillar_skillMp;
}

bool Wizard::CreateFireBall()
{
	//スキルのMPが足りないなら抜け出す
	if (m_status.mp < m_fireBall_SkillMp) {
		return false;
	}


	if (m_createFireBallTime < m_fireBallTimer)
	{
		//ファイヤーボール生成
		FireBall* fireBall = NewGO<FireBall>(0, "fireball");
		fireBall->SetWizard(this);
		//MPを減らす
		m_status.mp -= m_fireBall_SkillMp;

		m_fireBallTimer = 0.0f;
	}
	else
	{
		m_fireBallTimer += g_gameTime->GetFrameDeltaTime();
	}

	return true;
}

void Wizard::PlayAnimation()
{
	m_animationState->PlayAnimation();
}

void Wizard::ManageState()
{
	m_animationState->ManageState();
}

bool Wizard::IsComboStateSame()
{
	return false;
}

void Wizard::SetNextAnimationState(EnAnimationState nextState)
{
	if (m_animationState != nullptr) {
		// 古いステートを削除する。
		delete m_animationState;
		m_animationState = nullptr;
	}

	m_enAnimationState = nextState;

	switch (m_enAnimationState)
	{
	case Wizard::enAninationState_Idle:
		//待機ステートを作成する。
		m_animationState = new WizardStateIdle(this);
		break;
	case Wizard::enAninationState_Walk:
		//歩きステートを作成する。
		m_animationState = new WizardStateWalk(this);
		break;
	case Wizard::enAninationState_Run:
		//走るステートを作成する。
		m_animationState = new WizardStateRun(this);
		break;
	case Wizard::enAnimationState_Attack_1:
		//アタック１ステートを作成する。
		m_animationState = new WizardStateAttack_1(this);
		break;
	case Wizard::enAnimationState_Attack_2_start:
		//アタック２スタートステートを作成する。
		m_animationState = new WizardStateAttack_2_Start(this);
		break;
	case Wizard::enAnimationState_Attack_2_main:
		//アタック２メインステートを作成する。
		m_animationState = new WizardStateAttack_2_main(this);
		break;
	case Wizard::enAnimationState_Attack_3_start:
		//アタック３スタートステートを作成する。
		m_animationState = new WizardStateAttack_3_Start(this);
		break;
	case Wizard::enAnimationState_Attack_3_main:
		//アタック３メインステートを作成する。
		m_animationState = new WizardStateAttack_3_Main(this);
		break;
	case Wizard::enAnimationState_Attack_4:
		//アタック４ステートを作成する。
		m_animationState = new WizardState_Attack_4(this);
		break;
	case Wizard::enAnimationState_Die:
		//アタック４ステートを作成する。
		m_animationState = new WizardStateDie(this);
		break;

	default:
		// ここに来たらステートのインスタンス作成処理の追加忘れ。
		std::abort();
		break;
	}
	
	
}

void Wizard::ProcessCommonStateTransition()
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

void Wizard::OnProcessAttack_1StateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//コンボが終わったら
		SetNowComboState(enNowCombo_None);
		SetDamagedComboState(enDamageCombo_None);
		//攻撃パターンをなし状態にする
		m_enAttackPatternState = enAttackPattern_None;
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}
void Wizard::OnProcessAttack_2StateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_enAttackPatternState = enAttackPattern_2_main;



		//メインアニメーションを再生
		SetNextAnimationState(enAnimationState_Attack_2_main);
		//ファイヤーボールタイマーをセット
		m_fireBallTimer = m_createFireBallTime;
	}
}

void Wizard::OnProcessAttack_2MainStateTransition()
{
	//Xボタンを押している間
	if (g_pad[0]->IsPress(enButtonX))
	{
		//MPが足りなくなったら強制的にスキルを終わる
		if (!CreateFireBall())
		{
			//攻撃パターンをなし状態にする
			m_enAttackPatternState = enAttackPattern_None;
			//共通の状態遷移処理に移行
			ProcessCommonStateTransition();
			//スキルを打ち終わったのでMP回復フラグをtrueにする
			SetRecoveryMpFlag(true);
		}
	}
	else
	{
		//攻撃パターンをなし状態にする
		m_enAttackPatternState = enAttackPattern_None;
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
		//スキルを打ち終わったのでMP回復フラグをtrueにする
		SetRecoveryMpFlag(true);
	}
}

void Wizard::OnProcessAttack_3StateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//スタートだったら
		if (m_enAttackPatternState == enAttackPattern_3_start)
		{
			m_enAttackPatternState = enAttackPattern_3_main;
			//メインアニメーションを再生
			SetNextAnimationState(enAnimationState_Attack_3_main);

			//フレイムピラー生成
			CreateFlamePillar();
			
			return;
		}

		//攻撃パターンをなし状態にする
		m_enAttackPatternState = enAttackPattern_None;
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
		//スキルを打ち終わったのでMP回復フラグをtrueにする
		SetRecoveryMpFlag(true);
	}
}
void Wizard::OnProcessAttack_4StateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//コンボが終わったら
		SetNowComboState(enNowCombo_None);
		SetDamagedComboState(enDamageCombo_None);
		//攻撃パターンをなし状態にする
		m_enAttackPatternState = enAttackPattern_None;
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Wizard::OnProcessDieStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//フレームレートを戻す
		g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Fix, 60);
		//アニメーションが終わったのでキャラクターを切り替えるフラグをtrueにする
		SetDieToChangeFlag(true);
	}
}

void Wizard::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (wcscmp(eventName, L"Attack1_Collision_Start") == 0)
	{
		m_createAttackCollisionFlag = true;
	}
	if (wcscmp(eventName, L"Attack1_Collision_End") == 0)
	{
		m_createAttackCollisionFlag = false;
	}
	//アタック１のコンボ受付タイムが終わったら
	if (wcscmp(eventName, L"Attack1_ComboEnd") == 0)
	{
		if (m_enAttackPatternState == enAttackPattern_1to4)
		{
			m_enAttackPatternState = enAttackPattern_4;
			SetNowComboState(enNowCombo_2);
			SetNextAnimationState(enAnimationState_Attack_4);
		}
	}

	//これより後はダメージを受けない
	if (wcscmp(eventName, L"Damageless") == 0)
	{

	}
	if (wcscmp(eventName, L"Attack4_CollisionStart") == 0)
	{
		m_createAttackCollisionFlag = true;
	}
	if (wcscmp(eventName, L"Attack4_CollisionEnd") == 0)
	{
		m_createAttackCollisionFlag = false;
	}
}

void Wizard::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}


