#include "stdafx.h"
#include "Hero.h"
#include "HeroStateIdle.h"
#include "HeroStateWalk.h"
#include "HeroStateRun.h"

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


	//m_charaCon.Init(12.0f, 33.0f, g_vec3Zero);
}

void Hero::Update()
{
	Move();
	ManageState();
	PlayAnimation();

	SetTransFormModel(m_modelRender);
	m_modelRender.Update();
}

void Hero::Move()
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
	//m_position = m_charaCon.Execute(m_moveSpeed = calcVelocity(m_status), 1.0f / 60.0f);
	Rotation();
}

void Hero::PlayAnimation()
{
	m_state->PlayAnimation();
}

void Hero::SetNextAnimationState(EnAnimationState nextState)
{
	if (m_state != nullptr) {
		// 古いステートを削除する。
		delete m_state;
		m_state = nullptr;
	}
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
	default:
		// ここに来たらステートのインスタンス作成処理の追加忘れ。
		std::abort();
		break;
	}
	m_enAnimationState = nextState;
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

void Hero::OnProcessRunStateTransition()
{

}

void Hero::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}




