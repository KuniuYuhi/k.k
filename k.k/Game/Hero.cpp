#include "stdafx.h"
#include "Hero.h"

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
	switch (m_enAnimationState)
	{
	case Hero::enIdle:
		m_modelRender.PlayAnimation(enAnimClip_Idle, 0.3f);
		break;
	case Hero::enWalk:
		m_modelRender.PlayAnimation(enAnimClip_Walk,0.2f);
		break;
	case Hero::enRun:
		m_modelRender.PlayAnimation(enAnimClip_Run, 0.2f);
		break;

	default:
		break;
	}
}

void Hero::ManageState()
{
	switch (m_enAnimationState)
	{
	case Hero::enIdle:
		OnProcessCommonStateTransition();
		break;
	case Hero::enWalk:
		OnProcessCommonStateTransition();
		break;
	case Hero::enRun:
		OnProcessCommonStateTransition();
		break;
		
	default:
		break;
	}
}

void Hero::OnProcessCommonStateTransition()
{
	if (m_dashFlag == true)
	{
		m_enAnimationState = enRun;
		return;
	}

	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		m_enAnimationState = enWalk;
	}
	else
	{
		m_enAnimationState = enIdle;
	}
}

void Hero::OnProcessRunStateTransition()
{

}

void Hero::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}




