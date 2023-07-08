#include "stdafx.h"
#include "Wizard.h"
#include "WizardStateIdle.h"
#include "WizardStateWalk.h"
#include "WizardStateRun.h"

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
	// �����̃A�j���[�V�����X�e�[�g��ҋ@��Ԃɂ���B
	SetNextAnimationState(enAninationState_Idle);

	//�X�e�[�^�X�̏�����
	m_status.InitStatus(
		MAXHP,
		ATK,
		SPEED,
		NAME
	);

	InitModel();

	//��A�N�e�B�u��
	Deactivate();

	return true;
}

void Wizard::InitModel()
{
	//�A�j���[�V�����N���b�v�����[�h����B
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
	//m_position = { 70.0f, 0.0f, 0.0f };
	//m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	//m_charaCon.Init(12.0f, 33.0f, m_position);
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
	
	m_moveSpeed = m_player->GetMoveSpeed();
	m_position = m_player->GetPosition();
	//m_position = m_charaCon.Execute(m_moveSpeed = calcVelocity(m_status), 1.0f / 60.0f);
	Rotation();
}

void Wizard::PlayAnimation()
{
	m_animationState->PlayAnimation();
}

void Wizard::ManageState()
{
	m_animationState->ManageState();
}

void Wizard::SetNextAnimationState(EnAnimationState nextState)
{
	if (m_animationState != nullptr) {
		// �Â��X�e�[�g���폜����B
		delete m_animationState;
		m_animationState = nullptr;
	}

	switch (m_enAnimationState)
	{
	case Wizard::enAninationState_Idle:
		//�ҋ@�X�e�[�g���쐬����B
		m_animationState = new WizardStateIdle(this);
		break;
	case Wizard::enAninationState_Walk:
		//�����X�e�[�g���쐬����B
		m_animationState = new WizardStateWalk(this);
		break;
	case Wizard::enAninationState_Run:
		//����X�e�[�g���쐬����B
		m_animationState = new WizardStateRun(this);
		break;

	default:
		// �����ɗ�����X�e�[�g�̃C���X�^���X�쐬�����̒ǉ��Y��B
		std::abort();
		break;
	}

	m_enAnimationState = nextState;
}

void Wizard::ProcessCommonStateTransition()
{
	if (m_dashFlag == true)
	{
		SetNextAnimationState(enAninationState_Run);
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

void Wizard::OnProcessRunStateTransition()
{
}

void Wizard::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
