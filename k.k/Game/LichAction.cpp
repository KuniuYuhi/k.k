#include "stdafx.h"
#include "LichAction.h"
#include "Lich.h"
#include "Player.h"

//�s���̗D��x����
void LichAction::SettingPriority()
{
	m_action[enAttack_None].m_priority = 0;
	m_action[enAttack_1].m_priority = 1;
	m_action[enAttack_2].m_priority = 2;
	m_action[enAttack_DarkMeteorite].m_priority = 4;
	m_action[enAttack_Summon].m_priority = 3;
	m_action[enAttack_AngryMode].m_priority = 0;
}

int LichAction::NextAction()
{
	//�]���l������
	/*for (int i=0;i< m_actionAmount;i++)
	{
		m_action[i].m_eval = 0;
	}*/
	CalcEvalIdle(enAttack_None);

	CalcEvalAttack1(enAttack_1);

	CalcEvalAttack2(enAttack_2);

	CalcEvalSummon(enAttack_Summon);

	CalcEvalDarkMeteorite(enAttack_DarkMeteorite);

	CalcEvalAngryMode(enAttack_AngryMode);

	//�]���l����Ԃ̔z��̔ԍ����擾
	int NextActionNomber = CalcMaxEvalNumber();


	switch (NextActionNomber)
	{
	case enAttack_None:
		//�������Ȃ�(�ړ�)
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
		//���ʂȍs���X�e�[�g�����ɖ߂�
		//m_lich->SetSpecialActionState(Lich::enSpecialActionState_Normal);
		//�X�e�[�g�Z�b�g
		m_lich->SetNextAnimationState(Lich::enAnimationState_Attack_DarkMeteorite_start);
		//�ǂ�ȍs�����������ۑ�����
		m_oldActionNumber = enAttack_DarkMeteorite;
		//�]���l���Z�b�g
		m_action[NextActionNomber].m_eval = 0;

		return 0;
		break;
	case enAttack_Summon:
		//���̋Z�͖��G��Ԃőł�
		m_lich->SetInvincibleFlag(true);
		//���[�v
		//m_lich->Warp(Lich::enSpecialActionState_CenterWarp);
		//�X�e�[�g�Z�b�g
		m_lich->SetNextAnimationState(Lich::enAninationState_Summon);
		//�~�ς����l��0�ɂ���
		m_lich->SetHitCountAndDamage(0, 0);
		
		//�ǂ�ȍs�����������ۑ�����
		m_oldActionNumber = enAttack_Summon;
		return 0;

	case enAttack_AngryMode:
		//���̋Z�͖��G��Ԃőł�
		m_lich->SetInvincibleFlag(true);
		//�X�e�[�g�Z�b�g
		m_lich->SetNextAnimationState(Lich::enAnimationState_Angry);

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
	//todo �l�X�g�C�ɂȂ�
	for (int i = 0; i < m_actionAmount; i++)
	{
		if (MaxEval < m_action[i].m_eval)
		{
			//�]���l���������̓��m�Ȃ�
			//�D��x�Ō��߂�
			/*if (MaxEval == m_action[i].m_eval)
			{
				if (m_action[NextActionNomber].m_priority > m_action[i].m_priority)
				{
					continue;
				}
			}*/

			//�ő�l�X�V
			MaxEval = m_action[i].m_eval;
			NextActionNomber = i;
		}
	}
	return NextActionNomber;
}

void LichAction::CalcEvalAttack1(EnActionNumber m_enActionNumber)
{
	//�U���͈͓��Ƀv���C���[��������
	if(m_lich->IsFindPlayer(m_lich->GetInfoAboutAttack().m_Attack_1Distance)==true)
	{
		m_action[m_enActionNumber].m_eval = 100;
	}
	else
	{
		m_eval[m_enActionNumber] += INT_MIN;
	}
}

void LichAction::CalcEvalAttack2(EnActionNumber m_enActionNumber)
{
	//�U���͈͓��Ƀv���C���[��������
	if (m_lich->IsFindPlayer(m_lich->GetInfoAboutAttack().m_Attack_2Distance) == true)
	{
		m_action[m_enActionNumber].m_eval = 120;
	}
	else
	{
		m_action[m_enActionNumber].m_eval = INT_MIN;
	}
}

void LichAction::CalcEvalSummon(EnActionNumber m_enActionNumber)
{
	//������
	m_action[m_enActionNumber].m_eval = 0;

	//�ŏ��̃A�N�V�����Ȃ�
	if (m_firstActionNumber == m_enActionNumber)
	{
		m_action[m_enActionNumber].m_eval = INT_MAX;
		m_firstActionNumber = enAttack_None;
		return;
	}


	//����6�̈ȏヂ���X�^�[�������炱�̍s���������Ȃ�
	if (m_lich->GetAIActors() >= 6)
	{
		m_action[m_enActionNumber].m_eval = INT_MIN;
		return;
	}
	//���ɓ{�胂�[�h�Ȃ炱�̍s���ɂ����Ȃ�
	if (m_lich->GetSpecialActionState() == Lich::enSpecialActionState_AngryMode)
	{
		m_action[m_enActionNumber].m_eval = INT_MIN;
		return;
	}

	//���g�̏󋵂��悭�Ȃ��Ƃ��ɏ���
	//���W
	//5��U�����ꂽ��
	//�J�E���g��10�����₷
	//��_���[�W�����񐔕]���l������
	for (int i = 0; i < m_lich->GetHitCount(); i++)
	{
		m_action[m_enActionNumber].m_eval += 10;
	}

	//�ő�HP��1/5�̃_���[�W���~�ς�����]���l�𑝂₷
	if (m_lich->GetAccumulationDamage()>m_lich->GetStatus().maxHp/5)
	{
		m_action[m_enActionNumber].m_eval += 50;
	}
	//���̏���

}

void LichAction::CalcEvalDarkMeteorite(EnActionNumber m_enActionNumber)
{
	//���A�������s���ɂ��Ȃ�
	if (m_oldActionNumber == enAttack_DarkMeteorite)
	{
		m_action[m_enActionNumber].m_eval = INT_MIN;
		return;
	}
	//���Ԃ�30�b��������

	//todo ����
	m_action[m_enActionNumber].m_eval += 5;
	
	if (m_lich->IsFindPlayer(m_lich->GetInfoAboutAttack().m_Attack_2Distance) == false)
	{
		m_action[m_enActionNumber].m_eval += 5;
	}

	
}

void LichAction::CalcEvalAngryMode(EnActionNumber m_enActionNumber)
{
	//�{�胂�[�h���Ȃ珈�����Ȃ�
	//���ɓ{�胂�[�h�Ȃ炱�̍s���ɂ����Ȃ�
	if (m_lich->GetSpecialActionState() == Lich::enSpecialActionState_AngryMode)
	{
		m_action[m_enActionNumber].m_eval = INT_MIN;
		return;
	}

	//�_���[�W�����܂�����{�胂�[�h
	if (m_lich->GetAngryModeCount() >= 3)
	{
		m_action[m_enActionNumber].m_eval = INT_MAX;
	}

}



void LichAction::CalcEvalIdle(EnActionNumber m_enActionNumber)
{
	//�������Ȃ�(�ړ�)

}
