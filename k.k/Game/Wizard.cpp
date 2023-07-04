#include "stdafx.h"
#include "Wizard.h"

namespace {
	int MAXHP = 150;
	int ATK = 70;
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

void Wizard::InitModel()
{
	//アニメーションクリップをロードする。
	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/Wizard/Idle.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Walk].Load("Assets/animData/character/Wizard/Walk_Normal.tka");
	m_animationClip[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClip[enAnimClip_Run].Load("Assets/animData/character/Wizard/Run.tka");
	m_animationClip[enAnimClip_Run].SetLoopFlag(true);

	m_modelRender.Init(
		"Assets/modelData/character/Wizard/Wizard.tkm",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);

	m_modelRender.Update();

	m_charaCon.Init(12.0f, 33.0f, g_vec3Zero);
}

void Wizard::Update()
{
	Move();

	ManageState();
	PlayAnimation();

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

	m_position = m_charaCon.Execute(m_moveSpeed = calcVelocity(m_status), 1.0f / 60.0f);
	Rotation();
}

void Wizard::PlayAnimation()
{
	switch (m_enAnimationState)
	{
	case Wizard::enIdle:
		m_modelRender.PlayAnimation(enAnimClip_Idle, 0.3f);
		break;
	case Wizard::enWalk:
		m_modelRender.PlayAnimation(enAnimClip_Walk, 0.2f);
		break;
	case Wizard::enRun:
		m_modelRender.PlayAnimation(enAnimClip_Run, 0.2f);
		break;

	default:
		break;
	}
}

void Wizard::ManageState()
{
	switch (m_enAnimationState)
	{
	case Wizard::enIdle:
		OnProcessCommonStateTransition();
		break;
	case Wizard::enWalk:
		OnProcessCommonStateTransition();
		break;
	case Wizard::enRun:
		OnProcessCommonStateTransition();
		break;

	default:
		break;
	}
}

void Wizard::OnProcessCommonStateTransition()
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

void Wizard::OnProcessRunStateTransition()
{
}

void Wizard::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
