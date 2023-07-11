#include "stdafx.h"
#include "Hero.h"
#include "HeroStateIdle.h"
#include "HeroStateWalk.h"
#include "HeroStateRun.h"
#include "HeroStateAttack_1.h"
#include "HeroStateAttack_2.h"
#include "HeroStateAttack_3.h"
#include "HeroStateAttack_4.h"
#include "HeroStateAttack_Skill_Main.h"


namespace {
	int MAXHP = 200;
	int ATK = 50;
	float SPEED = 100.0f;
	const char* NAME = "Hero";
}

Hero::Hero()
{
}

Hero::~Hero()
{
}

bool Hero::Start()
{
	// 初期のアニメーションステートを待機状態にする。
	SetNextAnimationState(enAninationState_Idle);

	//ステータスの初期化
	m_status.InitStatus(
	MAXHP,
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
	m_animationClip[enAnimClip_Attack_1].Load("Assets/animData/character/Player/Attack1.tka");
	m_animationClip[enAnimClip_Attack_1].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_2].Load("Assets/animData/character/Player/Attack2.tka");
	m_animationClip[enAnimClip_Attack_2].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_3].Load("Assets/animData/character/Player/Attack3.tka");
	m_animationClip[enAnimClip_Attack_3].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_Skill_Charge].Load("Assets/animData/character/Player/Attack_Skill_Charge.tka");
	m_animationClip[enAnimClip_Attack_Skill_Charge].SetLoopFlag(true);
	m_animationClip[enAnimClip_Attack_Skill_Main].Load("Assets/animData/character/Player/Attack_Skill_Main.tka");
	m_animationClip[enAnimClip_Attack_Skill_Main].SetLoopFlag(true);


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

/// <summary>
/// 毎フレームSetNextAnimationState()を実行
/// </summary>

void Hero::Update()
{
	
	Move();
	Attack();
	ManageState();
	PlayAnimation();

	if (m_createAttackCollisionFlag == true)
	{
		CreateCollision();
	}

	if (m_createSkillCollisionFlag == true)
	{
		CreateSkillCollision();
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
				m_dashFlag = true;
			}
			else
			{
				m_dashFlag = false;
			}
		}
	}

	
	
	m_moveSpeed = m_player->GetMoveSpeed();
	m_position = m_player->GetPosition();
	
	Rotation();
}

void Hero::Attack()
{
	

	///////////////////////////////////////////////////////////////////////////////////////////
	// 通常攻撃
	///////////////////////////////////////////////////////////////////////////////////////////
	//アタックパターンがなしの時
	//1コンボ
	if (g_pad[0]->IsTrigger(enButtonY)&& m_enAttackPatternState==enAttackPattern_None)
	{
		m_createAttackCollisionFlag = true;
		//攻撃パターン１
		m_enAttackPatternState = enAttackPattern_1;

		SetNextAnimationState(enAnimationState_Attack_1);
		return;
	}
	//2コンボ目受付
	if (m_enAttackPatternState == enAttackPattern_1)
	{
		if (g_pad[0]->IsTrigger(enButtonY))
		{
			m_enAttackPatternState = enAttackPattern_1to2;
			return;
		}
	}
	//3コンボ目受付
	if (m_enAttackPatternState == enAttackPattern_2)
	{
		if (g_pad[0]->IsTrigger(enButtonY))
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
		if (g_pad[0]->IsPress(enButtonX))
		{
			m_enAttackPatternState = enAttackPattern_Skill_Charge;
			SetNextAnimationState(enAnimationState_Attack_Skill_Charge);
		}
		return;
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

	//カプセルの当たり判定作成
	/*SkillCollision->CreateBox(
		m_position,
		Quaternion::Identity,
		Vector3(60.0f,40.0f,30.0f)
	);*/

	//剣のボーンのワールド座標を取得
	Matrix SkillBoonMatrix = m_modelRender.GetBone(m_skillBoonId)->GetWorldMatrix();

	SkillCollision->SetWorldMatrix(SkillBoonMatrix);
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

	default:
		// ここに来たらステートのインスタンス作成処理の追加忘れ。
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
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Hero::OnProcessAttack_3StateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
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
		m_ChargeTimer += g_gameTime->GetFrameDeltaTime();
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

void Hero::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//アタック1のコンボ受付タイムが終わったら
	if (wcscmp(eventName, L"Attack1_ComboEnd") == 0)
	{
		m_createAttackCollisionFlag = false;

		//2コンボ
		if (m_enAttackPatternState == enAttackPattern_1to2)
		{
			m_enAttackPatternState = enAttackPattern_2;
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


}

void Hero::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}




