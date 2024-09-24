#include "stdafx.h"
#include "BeholderEyeStateContext.h"
#include "IMobEnemyState.h"

#include "BeholderEyeState_Idle.h"
#include "BeholderEyeState_Run.h"
#include "BeholderEyeState_Attack.h"
#include "BeholderEyeState_Hit.h"
#include "BeholderEyeState_Die.h"
#include "BeholderEyeState_Victory.h"
#include "BeholderEyeState_Appear.h"



BeholderEyeStateContext::BeholderEyeStateContext()
{
}

BeholderEyeStateContext::~BeholderEyeStateContext()
{
	m_currentState = nullptr;
}

void BeholderEyeStateContext::Init(BeholderEye* beholderEye, EnBeholderEyeState startState)
{
	m_enCurrentSlimeState = startState;
	//�X�e�[�g��؂�ւ���
	ChangeBeholderEyeState(beholderEye, startState);
}

void BeholderEyeStateContext::ChangeBeholderEyeState(BeholderEye* beholderEye, EnBeholderEyeState changeState)
{
	//�O�̃X�e�[�g�������������ۑ����Ă���
	m_enPreviousSlimeState = m_enCurrentSlimeState;

	if (m_currentState != nullptr)
	{
		//���݂̃X�e�[�g�𔲂��o�����̏���
		m_currentState->Exit();
	}

	m_currentState = nullptr;


	//�V�����X�e�[�g�N���X����
	switch (changeState)
	{
	case BeholderEyeStates::enBeholderEyeState_Idle:
		m_currentState = new BeholderEyeState_Idle(beholderEye, this);
		break;
	case BeholderEyeStates::enBeholderEyeState_Run:
		m_currentState = new BeholderEyeState_Run(beholderEye, this);
		break;
	case BeholderEyeStates::enBeholderEyeState_Attack:
		m_currentState = new BeholderEyeState_Attack(beholderEye, this);
		break;
	case BeholderEyeStates::enBeholderEyeState_Hit:
		m_currentState = new BeholderEyeState_Hit(beholderEye, this);
		break;
	case BeholderEyeStates::enBeholderEyeState_Die:
		m_currentState = new BeholderEyeState_Die(beholderEye, this);
		break;
	case BeholderEyeStates::enBeholderEyeState_Victory:
		m_currentState = new BeholderEyeState_Victory(beholderEye, this);
		break;
	case BeholderEyeStates::enBeholderEyeState_Appear:
		m_currentState = new BeholderEyeState_Appear(beholderEye, this);
		break;
	default:
		break;
	}

	//�ŏ��̏��������s
	m_currentState->Entry();

	//���݂̃X�e�[�g�̒l��ۑ�
	m_enCurrentSlimeState = changeState;
}

void BeholderEyeStateContext::UpdateCurrentState()
{
	if (m_currentState == nullptr) return;
	//�X�V����
	m_currentState->Ubdate();
}

void BeholderEyeStateContext::PlayAnimationCurrentState()
{
	if (m_currentState == nullptr) return;
	//�A�j���[�V�������Đ�
	m_currentState->PlayAnimation();
}
