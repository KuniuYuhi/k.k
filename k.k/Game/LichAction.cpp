#include "stdafx.h"
#include "LichAction.h"
#include "Lich.h"
#include "Player.h"

int LichAction::NextAction()
{
	//�]���l������
	/*for (int i=0;i< m_actionAmount;i++)
	{
		m_action[i].m_eval = 0;
	}*/

	//m_eval[m_actionAmount];

	for (int i = 0; i < m_actionAmount; i++)
	{
		m_eval[i] = 0;
	}


	CalcEvalAttack1(enAttack_1/*m_action[enAttack_1]*/);

	CalcEvalAttack2(enAttack_2/*m_action[enAttack_2]*/);

	CalcEvalSummon(enAttack_Summon);

	CalcEvalDarkMeteorite(enAttack_DarkMeteorite/*m_action[enAttack_3]*/);


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
		//�ǂ�ȍs�����������ۑ�����
		m_oldActionNumber = enAttack_1;
		return 0;
		break;
	case enAttack_2:
		//�X�e�[�g�Z�b�g
		m_lich->SetNextAnimationState(Lich::enAnimationState_Attack_2);
		//�ǂ�ȍs�����������ۑ�����
		m_oldActionNumber = enAttack_2;
		return 0;
		break;
	case enAttack_DarkMeteorite:
		//���̋Z�͖��G��Ԃőł�
		m_lich->SetInvincibleFlag(true);
		//���[�v
		m_lich->Warp(Lich::enSpecialActionState_Warp);
		//���ʂȍs���X�e�[�g�����ɖ߂�
		m_lich->SetSpecialActionState(Lich::enSpecialActionState_Normal);
		//�X�e�[�g�Z�b�g
		m_lich->SetNextAnimationState(Lich::enAnimationState_Attack_DarkMeteorite_start);
		//�ǂ�ȍs�����������ۑ�����
		m_oldActionNumber = enAttack_DarkMeteorite;
		return 0;
		break;
	case enAttack_Summon:
		//���[�v
		m_lich->Warp(Lich::enSpecialActionState_CenterWarp);
		//�X�e�[�g�Z�b�g
		m_lich->SetNextAnimationState(Lich::enAninationState_Summon);
		
		
		//�ǂ�ȍs�����������ۑ�����
		m_oldActionNumber = enAttack_Summon;
		return 0;


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
		if (MaxEval < m_eval[i])
		{
			MaxEval = m_eval[i];
			NextActionNomber = i;
		}
	}
	return NextActionNomber;
}

void LichAction::CalcEvalAttack1(EnActionNumber m_enActionNumber/*Action& action*/)
{
	//�U���͈͓��Ƀv���C���[��������
	if(m_lich->IsFindPlayer(m_lich->GetInfoAboutAttack().m_Attack_1Distance)==true)
	{
		//action.m_eval += 100;
		m_eval[m_enActionNumber] += 100;
	}
	else
	{
		//action.m_eval = INT_MIN;
		m_eval[m_enActionNumber] += INT_MIN;
	}
}

void LichAction::CalcEvalAttack2(EnActionNumber m_enActionNumber/*Action& action*/)
{
	//�U���͈͓��Ƀv���C���[��������
	if (m_lich->IsFindPlayer(m_lich->GetInfoAboutAttack().m_Attack_2Distance) == true)
	{
		//action.m_eval += 120;
		m_eval[m_enActionNumber] += 120;
	}
	else
	{
		//action.m_eval = INT_MIN;
		m_eval[m_enActionNumber] += INT_MIN;
	}
}

void LichAction::CalcEvalSummon(EnActionNumber m_enActionNumber/*Action& action*/)
{
	//���g�̏󋵂��悭�Ȃ��Ƃ��ɏ���
	//���W
	//5��U�����ꂽ��
	//�J�E���g��10�����₷
	//��_���[�W�����񐔕]���l������
	for (int i = 0; i < m_lich->GetHitCount(); i++)
	{
		m_eval[m_enActionNumber] += 10;
	}

	//�ő�HP��1/5�̃_���[�W���~�ς�����]���l�𑝂₷
	if (m_lich->GetAccumulationDamage()>m_lich->GetStatus().maxHp/5)
	{
		m_eval[m_enActionNumber] += 50;
	}
	//���̏���

}

void LichAction::CalcEvalDarkMeteorite(EnActionNumber m_enActionNumber/*Action& action*/)
{
	//���[�v�����Ȃ�
	if (m_lich->GetSpecialActionState() == Lich::enSpecialActionState_Warp)
	{
		//action.m_eval = INT_MAX;
		m_eval[m_enActionNumber] += INT_MAX;
	}
	else
	{
		//action.m_eval = INT_MIN;
		m_eval[m_enActionNumber] += INT_MIN;
	}
}



void LichAction::CalcEvalIdle(EnActionNumber m_enActionNumber/*Action& action*/)
{
	//���񂩂Ɉ�񉽂����Ȃ�

}
