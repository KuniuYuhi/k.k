#include "stdafx.h"
#include "SummonerSSM_Offensive.h"
#include "Summoner.h"
#include "SummonerSMContext.h"

#include "EnemyManager.h"

#include "Brave.h"


namespace {
	const float DARKMETEORITE_COOLTIME_LIMIT = 3.0f;

	const float MELEE_ATTACK_DISTANCE = 400.0f;
}

void SummonerSSM_Offensive::Entry()
{
	if (m_player == nullptr)
	{
		m_player = FindGO<Brave>("Brave");
	}

}

void SummonerSSM_Offensive::Update()
{
	//�A�N�V�������͏������Ȃ�
	if (m_summoner->IsAction())
	{
		return;
	}

		
	SelectAttackBasedOnDistance();
	


}

bool SummonerSSM_Offensive::IsEnableChangeStateMachine()
{
	//�\��X�e�[�g���Ȃ��ɂ���
	m_stateMachineContext->SetReservationState(enSummonerState_None);

	//�������������낦�Ε��i�̃X�e�[�g�}�V���ɑJ��

	//HP���ւ������痣�ꂽ��
	//���[�v�̗\��X�e�[�g���ݒ肵�Ă���

	//�R���{�U���Ȃ玟�̃R���{�U���ɍs��
	//���̃A�N�V���������܂����̂ŃX�e�[�g�}�V���͐؂�ւ��Ȃ�
	if (ChangeNextComboAttackState())
	{
		return false;
	}

	if (CheckRangedAttack())
	{
		return false;
	}
	


	//���̍U���ɍs���O�Ƀ^�C�}�[������

	m_stateMachineContext->ChangeSubStateMachine(enSubStateMachine_General);
	return true;
}

void SummonerSSM_Offensive::SelectAttackBasedOnDistance()
{
	//�\��X�e�[�g������Ȃ�
	if (m_stateMachineContext->IsReservationState())
	{
		//�\�񂳂�Ă���X�e�[�g�ɑJ��
		m_summoner->ChangeState(m_stateMachineContext->GetReservationState());
		m_stateMachineContext->SetPreviousState(m_stateMachineContext->GetReservationState());
		return;
	}

	//���g����v���C���[�Ɍ������x�N�g�����狗�����擾
	float distance = m_summoner->GetDistanceToPlayerPositionValue();


	if (distance <= MELEE_ATTACK_DISTANCE)
	{
		SelectMeleeAttack();
	}
	else
	{
		SelectRangedAttack();
	}
}

void SummonerSSM_Offensive::SelectMeleeAttack()
{
	//�m���łǂ���ɂ��邩�I��

	int r = rand() % 10;
	//

	if (r > 4)
	{
		//�R���{�U���̈�R���{��
		m_summoner->ChangeState(enSummonerState_FirstAttack);
		//�O��̃X�e�[�g��ݒ�
		m_stateMachineContext->SetPreviousState(enSummonerState_FirstAttack);
	}
	else
	{

		//�_�[�N���e�I�ɐ؂�ւ��邩�`�F�b�N
		if (CheckChangeDarkMeteoriteState())
		{
			m_summoner->ChangeState(enSummonerState_Attack_DarkMeteorite);
			//�e��v�f���v�Z
			m_darkMeteoriteRemainingCount--;
			m_darkMeteoriteCoolTimer = 0.0f;
			//�O��̃X�e�[�g��ݒ�
			m_stateMachineContext->SetPreviousState(enSummonerState_Attack_DarkMeteorite);
			return;
		}

		//�͈͍U��
		m_summoner->ChangeState(enSummonerState_ShockWave);
		//�O��̃X�e�[�g��ݒ�
		m_stateMachineContext->SetPreviousState(enSummonerState_ShockWave);
	}
	


	

}

void SummonerSSM_Offensive::SelectRangedAttack()
{
	//�؂�ւ��X�e�[�g
	EnSummonerState changeState = enSummonerState_None;

	//�����ɂ���̂Ƀ_���[�W���󂯂Ă��遁�������U��
	//�����I�ɋ߂��ɔ��ł���


	//�_�[�N���e�I�ɐ؂�ւ��邩�`�F�b�N
	if (CheckChangeDarkMeteoriteState())
	{
		changeState = enSummonerState_Attack_DarkMeteorite;
		//�e��v�f���v�Z
		m_darkMeteoriteRemainingCount--;
		m_darkMeteoriteCoolTimer = 0.0f;
	}
	//�_�[�N�{�[���ɐ؂�ւ��邩�`�F�b�N
	else if (CheckChangeDarkBallState())
	{
		changeState = enSummonerState_DarkBall;
	}

	//changeState = enSummonerState_Attack_DarkMeteorite;
	////�e��v�f���v�Z
	//m_darkMeteoriteRemainingCount--;
	//m_darkMeteoriteCoolTimer = 0.0f;

	//�X�e�[�g���؂�ւ��Ȃ��Ȃ珈�����Ȃ�
	if (changeState == enSummonerState_None) return;

	//�X�e�[�g�؂�ւ�
	m_summoner->ChangeState(changeState);
	//�O��̃X�e�[�g��ݒ�
	m_stateMachineContext->SetPreviousState(changeState);
}

bool SummonerSSM_Offensive::ChangeNextComboAttackState()
{
	//�O��̃X�e�[�g���R���{�U���X�e�[�g�Ȃ�
	if (m_stateMachineContext->GetPreviousState() == enSummonerState_FirstAttack ||
		m_stateMachineContext->GetPreviousState() == enSummonerState_SecondAttack)
	{
		//���̃R���{�U���X�e�[�g�ɂ���
		EnSummonerState state = 
			static_cast<EnSummonerState>(m_stateMachineContext->GetPreviousState() + 1);
		//�\��X�e�[�g�ݒ�
		m_stateMachineContext->SetReservationState(state);

		return true;
	}

	return false;
}

bool SummonerSSM_Offensive::CheckRangedAttack()
{
	if (CheckNextDarkBallAction())
	{
		//���̃A�N�V�������_�[�N�{�[���Ɍ���
		//�\��X�e�[�g�ݒ�
		m_stateMachineContext->SetReservationState(enSummonerState_DarkBall);


		return true;
	}

	return false;
}

bool SummonerSSM_Offensive::CheckNextDarkBallAction()
{
	//�X�R�A
	int score = 0;
	//�O��̃X�e�[�g���_�[�N�{�[���X�e�[�g�Ȃ�m����������B
	//�A���Ŏg�p���邽�ъm����������
	if (m_stateMachineContext->GetPreviousState() == enSummonerState_DarkBall)
	{
		score -= 15 * m_darkBallActionCount;
	}

	//�̗͂������Ă����ƘA���Ŏg���p�x���オ��
	//HP�̊��������߂�
	float ratio = 
		(float)m_summoner->GetCommonStatus().GetCurrentHp() / (float)m_summoner->GetCommonStatus().GetMaxHp();
	//������7���ȉ��Ȃ�X�R�A�����Z����
	if (ratio <= 0.7f)
	{
		//�c���Ă���HP�̊�����������HP�̊����ɂ���
		ratio = 1 - ratio;

		ratio = ratio * 10;

		int addProbability = rand() % 10 + 1;

		//�X�R�A�����Z����
		score += ratio * addProbability;

	}



	//�X�R�A�����ȏ�Ȃ炱�̍s���ɂ���
	if (score > 30) {
		//�_�[�N�{�[���J�E���g���Z
		m_darkBallActionCount++;
		return true;
	}

	//���̒��̍s�������Ȃ������̂ŃJ�E���g�����Z�b�g
	m_darkBallActionCount = 0;
	return false;
}

bool SummonerSSM_Offensive::CheckChangeDarkBallState()
{
	//��������true
	return true;
}

bool SummonerSSM_Offensive::CheckChangeDarkMeteoriteState()
{
	//�v���C���[�̃X�^�~�i������
	//�v���C���[�̎���ɓG�����邩
	//HP�̊����ɕ����Ă��̉񐔂����g����悤�ɂ���


	//�N�[���^�C�����I����Ă��Ȃ��Ȃ珈�����Ȃ�
	if (m_darkMeteoriteCoolTimer < DARKMETEORITE_COOLTIME_LIMIT)
	{
		//�^�C�}�[�����Z���ď������I���
		m_darkMeteoriteCoolTimer ++;
		return false;
	}

	//�X�R�A
	int score = 0;

	//���݂�HP���ő�HP�����������Ŋ����Ď����̎c��񐔂����߂�
	int result = m_summoner->GetCommonStatus().GetCurrentHp() / m_stateMachineContext->GetMaxHpQuotient();
	//���ʂƎc��񐔂������Ȃ�g�p�ł�����
	if (result <= m_darkMeteoriteRemainingCount)
	{
		score += 25;
	}

	//���݂̃X�^�~�i���ő�l�X�^�~�i�Ŋ����Ċ������v�Z
	float staminaRatio = m_player->GetStatus().GetCurrentStamina() / m_player->GetStatus().GetMaxStamina();
	//�t�ɂ��ăX�^�~�i�����Ȃ��قǃ��[�g���オ��悤�ɂ���
	staminaRatio = 1 - staminaRatio;
	//�������g���ăX�R�A���Z�B�X�^�~�i�����Ȃ��قǃX�R�A���傫���Ȃ�
	score += 100 * (staminaRatio * 1.2f);

	result = EnemyManager::GetInstance()->GetNearbyEnemyCount(m_player->GetPosition(), 350.0f);
	//5�̈ȏア����5�ɐ�������
	if (result > 6) result = 6;

	score += result * 14;

	//�ŏI�I�ȃX�R�A��120�ȉ��Ȃ炱�̍s���͂��Ȃ�
	if (score < 88) return false;

	return true;
}
