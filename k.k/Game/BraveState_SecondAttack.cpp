#include "stdafx.h"
#include "BraveState_SecondAttack.h"
#include "Brave.h"
#include "PlayerController.h"
#include "WeaponBase.h"


void BraveState_SecondAttack::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		enBraveAnimClip_SecondAttack + m_brave->GetCurrentAnimationStartIndexNo(), 0.1f
	);
}

void BraveState_SecondAttack::Entry()
{
	if (m_playerController == nullptr)
	{
		//�v���C���[�R���g���[���[�R���|�[�l���g���擾
		m_playerController = m_brave->GetComponent<PlayerController>();
	}

	//����̃G���g���[�̏������s��
	m_brave->GetArmedWeapon()->EntryNormalAttackProcess(WeaponBase::enCombo_Second);

	//�A�N�V�������ɂ���
	m_brave->ActionActive();
	//�g���K�[�`�F�b�N�t���O�����Z�b�g
	m_isTrigger = false;
	//�x���t���O���Z�b�g
	m_isDelay = false;
}

void BraveState_SecondAttack::Ubdate()
{
	//�U�����ɒʏ�U���{�^�����������Ȃ�
	if (m_playerController->IsTriggerNromalAttackButton() && !m_isTrigger && m_isDelay)
	{
		m_isTrigger = true;
	}

	//����̍X�V����
	m_brave->GetArmedWeapon()->UpdateNormalAttackProcess(WeaponBase::enCombo_Second);

	//�A�j���[�V�������I�������
	if (m_brave->GetModelRender().IsPlayingAnimation() == false)
	{
		//�{�^���������Ă�����
		if (m_isTrigger)
		{
			//���̃R���{����������
			m_brave->NormalAttackProcess();
		}
		else
		{
			//�X�e�[�g�̋��ʏ���
			m_brave->ProcessCommonStateTransition();
		}

	}

	//�x���t���O�𗧂Ă�
	m_isDelay = true;
}

void BraveState_SecondAttack::Exit()
{
	//�{�^���������Ă��Ȃ�������
	if (!m_isTrigger)
	{
		//�A�N�V�������I���
		m_brave->ExitAttackAction();
	}

	//����̔����o������
	m_brave->GetArmedWeapon()->ExitNormalAttackProcess(WeaponBase::enCombo_Second);


	//�g���K�[�`�F�b�N�t���O�����Z�b�g
	m_isTrigger = false;
	//�x���t���O���Z�b�g
	m_isDelay = false;
}
