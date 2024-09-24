#include "stdafx.h"
#include "SlimeStateContext.h"
#include "IMobEnemyState.h"

#include "SlimeState_Idle.h"
#include "SlimeState_Run.h"
#include "SlimeState_Attack.h"
#include "SlimeState_Hit.h"
#include "SlimeState_Die.h"
#include "SlimeState_Victory.h"
#include "SlimeState_Appear.h"



SlimeStateContext::SlimeStateContext()
{
}

SlimeStateContext::~SlimeStateContext()
{
	m_currentSlimeState = nullptr;
}

void SlimeStateContext::Init(Slime* slime, EnSlimeState startState)
{
	m_enCurrentSlimeState = startState;
	//�X�e�[�g��؂�ւ���
	ChangeSlimeState(slime, startState);
}

void SlimeStateContext::ChangeSlimeState(Slime* slime, EnSlimeState changeState)
{
	//�O�̃X�e�[�g�������������ۑ����Ă���
	m_enPreviousSlimeState = m_enCurrentSlimeState;

	if (m_currentSlimeState != nullptr)
	{
		//���݂̃X�e�[�g�𔲂��o�����̏���
		m_currentSlimeState->Exit();
	}

	m_currentSlimeState = nullptr;

	//�V�����X�e�[�g�N���X����
	switch (changeState)
	{
	case SlimeStates::enSlimeState_Idle:
		m_currentSlimeState = new SlimeState_Idle(slime, this);
		break;
	case SlimeStates::enSlimeState_Run:
		m_currentSlimeState = new SlimeState_Run(slime, this);
		break;
	case SlimeStates::enSlimeState_Attack:
		m_currentSlimeState = new SlimeState_Attack(slime, this);
		break;
	case SlimeStates::enSlimeState_Hit:
		m_currentSlimeState = new SlimeState_Hit(slime, this);
		break;
	case SlimeStates::enSlimeState_Die:
		m_currentSlimeState = new SlimeState_Die(slime, this);
		break;
	case SlimeStates::enSlimeState_Victory:
		m_currentSlimeState = new SlimeState_Victory(slime, this);
		break;
	case SlimeStates::enSlimeState_Appear:
		m_currentSlimeState = new SlimeState_Appear(slime, this);
		break;
	default:
		break;
	}

	//�ŏ��̏��������s
	m_currentSlimeState->Entry();

	//���݂̃X�e�[�g�̒l��ۑ�
	m_enCurrentSlimeState = changeState;

}

void SlimeStateContext::UpdateCurrentState()
{
	if (m_currentSlimeState == nullptr) return;
	//�X�V����
	m_currentSlimeState->Ubdate();
}

void SlimeStateContext::PlayAnimationCurrentState()
{
	if (m_currentSlimeState == nullptr) return;
	//�A�j���[�V�������Đ�
	m_currentSlimeState->PlayAnimation();
}
