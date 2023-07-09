#include "stdafx.h"
#include "Hero.h"
#include "HeroStateIdle.h"
#include "HeroStateWalk.h"
#include "HeroStateRun.h"
#include "HeroStateAttack_1.h"
#include "HeroStateAttack_2.h"
#include "HeroStateAttack_3.h"
#include "HeroStateAttack_4.h"


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

	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

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
	m_animationClip[enAnimClip_Attack_4].Load("Assets/animData/character/Player/Attack4.tka");
	m_animationClip[enAnimClip_Attack_4].SetLoopFlag(false);


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

	SetTransFormModel(m_modelRender);
	m_modelRender.Update();
}

void Hero::Move()
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
	

	

	m_moveSpeed = m_player->GetMoveSpeed();
	m_position = m_player->GetPosition();
	//m_position = m_charaCon.Execute(m_moveSpeed = calcVelocity(m_status), 1.0f / 60.0f);
	Rotation();
}

void Hero::Attack()
{
	//アタックパターンがなしの時
	//1コンボ
	if (g_pad[0]->IsTrigger(enButtonY)&& m_enAttackPatternState==enAttackPattern_None)
	{
		//攻撃パターン１
		m_enAttackPatternState = enAttackPattern_1;

		SetNextAnimationState(enAnimationState_Attack_1);
		return;
	}
	//2コンボ
	if (m_enAttackPatternState == enAttackPattern_1)
	{
		if (g_pad[0]->IsTrigger(enButtonY))
		{
			m_createAttackCollisionFlag = true;

			m_enAttackPatternState = enAttackPattern_2;
			//アニメーションイベントあるなら要らない
			SetNextAnimationState(enAnimationState_Attack_2);
			return;
		}
	}
	//3コンボ
	if (m_enAttackPatternState == enAttackPattern_2)
	{
		if (g_pad[0]->IsTrigger(enButtonY))
		{
			
			m_enAttackPatternState = enAttackPattern_3;
			//アニメーションイベントあるなら要らない
			SetNextAnimationState(enAnimationState_Attack_3);
			return;
		}
	}
	//4コンボ
	if (m_enAttackPatternState == enAttackPattern_3)
	{
		m_createAttackCollisionFlag = false;
		if (g_pad[0]->IsTrigger(enButtonY))
		{
			m_enAttackPatternState = enAttackPattern_4;
			//アニメーションイベントあるなら要らない
			SetNextAnimationState(enAnimationState_Attack_4);
			return;
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
		Vector3(10.0f, 5.0f, 50.0f)
	);

	//Vector3 swordPos = { 0.0f, 0.0f, -50.0f };

	//剣のボーンのワールド座標を取得
	Matrix SwordBoonMatrix = m_modelRender.GetBone(m_swordBoonId)->GetWorldMatrix();
	//SwordBoonMatrix.Apply(swordPos);
	//AtkCollision->SetPosition(swordPos);
	AtkCollision->SetWorldMatrix(SwordBoonMatrix);
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
	case Hero::enAnimationState_Attack_4:
		//アタック４ステートを作成する。
		m_state = new HeroStateAttack_4(this);
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
	if (m_dashFlag == true)
	{
		SetNextAnimationState( enAninationState_Run );
		return;
	}

	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		SetNextAnimationState(enAninationState_Walk);
	}
	else
	{
		SetNextAnimationState(enAninationState_Idle);
	}

	
}
//アタック１の状態遷移処理
void Hero::OnProcessAttack_1StateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		/*if (m_enAttackPatternState == enAttackPattern_2)
		{
			SetNextAnimationState(enAnimationState_Attack_2);
			return;
		}*/

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

void Hero::OnProcessAttack_4StateTransition()
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

void Hero::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//アタック1のコンボ受付タイムが終わったら
	if (wcscmp(eventName, L"Attack1_ComboEnd") == 0)
	{
		int a = 0;

		if (m_enAttackPatternState == enAttackPattern_2)
		{
			SetNextAnimationState(enAnimationState_Attack_2);
		}
	}

	//アタック２のコンボ受付タイムが始まったら
	if (wcscmp(eventName, L"Attack2_ComboStart") == 0)
	{
		int a = 1;
	}
	//アタック２のコンボ受付タイムが終わったら
	if (wcscmp(eventName, L"Attack2_ComboEnd") == 0)
	{
		int a = 1;
	}
}

void Hero::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}




