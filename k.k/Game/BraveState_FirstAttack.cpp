#include "stdafx.h"
#include "BraveState_FirstAttack.h"
#include "Brave.h"
#include "PlayerController.h"
#include "WeaponBase.h"

void BraveState_FirstAttack::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		enBraveAnimClip_FirstAttack + m_brave->GetCurrentAnimationStartIndexNo(),0.1f
	);
}

void BraveState_FirstAttack::Entry()
{
	if (m_playerController == nullptr)
	{
		//�v���C���[�R���g���[���[�R���|�[�l���g���擾
		m_playerController = m_brave->GetComponent<PlayerController>();
	}

	//����̃G���g���[�̏������s��
	m_brave->GetArmedWeapon()->EntryNormalAttackProcess(WeaponBase::enCombo_First);

	//�A�N�V�������ɂ���
	m_brave->ActionActive();
	//�g���K�[�`�F�b�N�t���O�����Z�b�g
	m_isTrigger = false;
	//
	m_isNextNormalAttackProgress = false;
}

void BraveState_FirstAttack::Ubdate()
{
	//���̒ʏ�U���ҋ@��ԓ��Œʏ�U���{�^�����������Ȃ�
	if (m_playerController->IsTriggerNromalAttackButton()&& !m_isTrigger&&
		m_brave->GetArmedWeapon()->IsStandbyPeriod())
	{
		m_isTrigger = true;
	}

	//����̍X�V����
	m_brave->GetArmedWeapon()->UpdateNormalAttackProcess(WeaponBase::enCombo_First);

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

void BraveState_FirstAttack::Exit()
{
	//���̍U���Ɉڂ�Ȃ��Ȃ�
	//todo ��_���[�W���󂯂Ă��Ă����
	if (!m_isNextNormalAttackProgress)
	{
		//�A�N�V�������I���
		m_brave->ExitAttackAction();
	}
	
	//����̔����o������
	m_brave->GetArmedWeapon()->ExitNormalAttackProcess(WeaponBase::enCombo_First);

	//�g���K�[�`�F�b�N�t���O�����Z�b�g
	m_isTrigger = false;
	
}
