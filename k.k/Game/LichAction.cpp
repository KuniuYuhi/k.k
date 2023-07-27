#include "stdafx.h"
#include "LichAction.h"
#include "Lich.h"
#include "Player.h"

namespace {
	const int ACTION_AMOUNT = 3;
}

int LichAction::NextAction()
{
	//�]���l������
	for (int i=0;i< m_actionAmount;i++)
	{
		m_action[i].m_eval = 0;
	}

	CalcEvalAttack1(m_action[enAttack_1]);

	CalcEvalAttack2(m_action[enAttack_2]);


	//�]���l����Ԃ̔z��̔ԍ����擾
	int NextActionNomber = CalcMaxEvalNumber();


	switch (NextActionNomber)
	{
	case enAttack_None:
		//�������Ȃ�
		return 0;
		break;
	case enAttack_1:
		//�X�e�[�g�Z�b�g
		m_lich->SetNextAnimationState(Lich::enAnimationState_Attack_1);
		return 0;
		break;
	case enAttack_2:
		//�X�e�[�g�Z�b�g
		m_lich->SetNextAnimationState(Lich::enAnimationState_Attack_2);
		return 0;
		break;

	default:
		return 0;
		break;
	}



	return 0;
}

int LichAction::CalcMaxEvalNumber()
{
	int MaxEval = 0;
	int NextActionNomber = 0;

	//��ԍ����]���l��T��
	for (int i = 0; i < m_actionAmount; i++)
	{
		if (MaxEval < m_action[i].m_eval)
		{
			MaxEval = m_action[i].m_eval;
			NextActionNomber = i;
		}
	}
	return NextActionNomber;
}

void LichAction::CalcEvalAttack1(Action& action)
{
	//�U���͈͓��Ƀv���C���[��������
	if(m_lich->IsFindPlayer(m_lich->GetInfoAboutAttack().m_Attack_1Distance)==true)
	{
		action.m_eval += 100;
	}
	else
	{
		action.m_eval = INT_MIN;
	}
}

void LichAction::CalcEvalAttack2(Action& action)
{
	//�U���͈͓��Ƀv���C���[��������
	if (m_lich->IsFindPlayer(m_lich->GetInfoAboutAttack().m_Attack_2Distance) == true)
	{
		action.m_eval += 120;
	}
	else
	{
		action.m_eval = INT_MIN;
	}
}



void LichAction::CalcEvalIdle(Action& action)
{
	//���񂩂Ɉ�񉽂����Ȃ�

}
