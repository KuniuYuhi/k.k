#include "stdafx.h"
#include "Wizard.h"
#include "WizardStateIdle.h"
#include "WizardStateWalk.h"
#include "WizardStateRun.h"
#include "WizardStateAttack_1.h"
#include "WizardStateAttack_2_Start.h"
#include "WizardStateAttack_2_main.h"
#include "WizardStateAttack_3_Start.h"
#include "WizardStateAttack_3_main.h"

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
	//Attack3_maintain
	//�A�j���[�V�����N���b�v�����[�h����B
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
	m_animationClip[enAnimClip_Attack_2_main].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_3_start].Load("Assets/animData/character/Wizard/Attack3_start.tka");
	m_animationClip[enAnimClip_Attack_3_start].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_3_main].Load("Assets/animData/character/Wizard/Attack3_maintain.tka");
	m_animationClip[enAnimClip_Attack_3_main].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_4].Load("Assets/animData/character/Wizard/Attack4.tka");
	m_animationClip[enAnimClip_Attack_4].SetLoopFlag(false);


	m_modelRender.Init(
		"Assets/modelData/character/Wizard/Wizard.tkm",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);
	//m_position = { 70.0f, 0.0f, 0.0f };
	//m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//m_charaCon.Init(12.0f, 33.0f, m_position);
}

void Wizard::Update()
{
	Move();
	Attack();
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

void Wizard::Attack()
{
	if (g_pad[0]->IsTrigger(enButtonY))
	{
		m_enAttackPatternState = enAttackPattern_1;
		SetNextAnimationState(enAnimationState_Attack_1);
	}

	if (g_pad[0]->IsTrigger(enButtonX))
	{
		m_enAttackPatternState = enAttackPattern_3_start;
		SetNextAnimationState(enAnimationState_Attack_3_start);
	}

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

	m_enAnimationState = nextState;

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
	case Wizard::enAnimationState_Attack_1:
		//�A�^�b�N�P�X�e�[�g���쐬����B
		m_animationState = new WizardStateAttack_1(this);
		break;
	case Wizard::enAnimationState_Attack_2_start:
		//�A�^�b�N�Q�X�^�[�g�X�e�[�g���쐬����B
		m_animationState = new WizardStateAttack_2_Start(this);
		break;
	case Wizard::enAnimationState_Attack_2_main:
		//�A�^�b�N�Q���C���X�e�[�g���쐬����B
		m_animationState = new WizardStateAttack_2_main(this);
		break;
	case Wizard::enAnimationState_Attack_3_start:
		//�A�^�b�N�R�X�^�[�g�X�e�[�g���쐬����B
		m_animationState = new WizardStateAttack_3_Start(this);
		break;
	case Wizard::enAnimationState_Attack_3_main:
		//�A�^�b�N�R���C���X�e�[�g���쐬����B
		m_animationState = new WizardStateAttack_3_Main(this);
		break;

	default:
		// �����ɗ�����X�e�[�g�̃C���X�^���X�쐬�����̒ǉ��Y��B
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
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�U���p�^�[�����Ȃ���Ԃɂ���
		m_enAttackPatternState = enAttackPattern_None;
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}
void Wizard::OnProcessAttack_2StateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�^�[�g��������
		if (m_enAttackPatternState == enAttackPattern_2_start)
		{
			m_enAttackPatternState = enAttackPattern_2_main;
			//���C���A�j���[�V�������Đ�
			SetNextAnimationState(enAnimationState_Attack_2_main);
			return;
		}

		//�U���p�^�[�����Ȃ���Ԃɂ���
		m_enAttackPatternState = enAttackPattern_None;
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}
void Wizard::OnProcessAttack_3StateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�^�[�g��������
		if (m_enAttackPatternState == enAttackPattern_3_start)
		{
			m_enAttackPatternState = enAttackPattern_3_main;
			//���C���A�j���[�V�������Đ�
			SetNextAnimationState(enAnimationState_Attack_3_main);
			return;
		}

		//�U���p�^�[�����Ȃ���Ԃɂ���
		m_enAttackPatternState = enAttackPattern_None;
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}
void Wizard::OnProcessAttack_4StateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�U���p�^�[�����Ȃ���Ԃɂ���
		m_enAttackPatternState = enAttackPattern_None;
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Wizard::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//��i�ڂ̃A�^�b�N�̃A�j���[�V�������n�܂�����
	if (wcscmp(eventName, L"Attack1_Collision_Start") == 0)
	{
		int a = 0;
	}
}

void Wizard::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}


