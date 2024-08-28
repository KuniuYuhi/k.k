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
	//
	m_isNextNormalAttackProgress = false;
}

void BraveState_SecondAttack::Ubdate()
{
	//���̒ʏ�U���ҋ@��ԓ��Œʏ�U���{�^�����������Ȃ�
	if (m_playerController->IsTriggerNromalAttackButton() && !m_isTrigger && 
		m_brave->GetArmedWeapon()->IsStandbyPeriod())
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
			m_isNextNormalAttackProgress = true;
			//���̃R���{����������
			m_brave->NormalAttackProcess();
		}
		else
		{
			//�X�e�[�g�̋��ʏ���
			m_brave->ProcessCommonStateTransition();
		}

	}

}

void BraveState_SecondAttack::Exit()
{
	//���̍U���Ɉڂ�Ȃ��Ȃ�
	if (!m_isNextNormalAttackProgress)
	{
		//�A�N�V�������I���
		m_brave->ExitAttackAction();
	}

	//����̔����o������
	m_brave->GetArmedWeapon()->ExitNormalAttackProcess(WeaponBase::enCombo_Second);


	//�g���K�[�`�F�b�N�t���O�����Z�b�g
	m_isTrigger = false;
}
