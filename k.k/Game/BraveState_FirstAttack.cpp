#include "stdafx.h"
#include "BraveState_FirstAttack.h"
#include "Brave.h"
#include "PlayerController.h"

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

	//�A�N�V�������ɂ���
	m_brave->ActionActive();
	//�g���K�[�`�F�b�N�t���O�����Z�b�g
	m_isTrigger = false;
	//�x���t���O���Z�b�g
	m_isDelay = false;
}

void BraveState_FirstAttack::Ubdate()
{
	//�U�����ɒʏ�U���{�^�����������Ȃ�
	if (m_playerController->IsTriggerNromalAttackButton()&& !m_isTrigger&& m_isDelay)
	{
		m_isTrigger = true;
	}

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

void BraveState_FirstAttack::Exit()
{
	//�{�^���������Ă��Ȃ�������
	if (!m_isTrigger)
	{
		//�A�N�V�������I���
		m_brave->ExitAttackAction();
	}
	
	//�g���K�[�`�F�b�N�t���O�����Z�b�g
	m_isTrigger = false;
	//�x���t���O���Z�b�g
	m_isDelay = false;
}
