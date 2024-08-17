#include "stdafx.h"
#include "BraveState_ThirdAttack.h"
#include "Brave.h"
#include "WeaponBase.h"


void BraveState_ThirdAttack::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		enBraveAnimClip_ThirdAttack + m_brave->GetCurrentAnimationStartIndexNo(), 0.1f
	);
}

void BraveState_ThirdAttack::Entry()
{
	//����̃G���g���[�̏������s��
	m_brave->GetArmedWeapon()->EntryNormalAttackProcess(WeaponBase::enCombo_Third);

	//�A�N�V�������ɂ���
	m_brave->ActionActive();
}

void BraveState_ThirdAttack::Ubdate()
{
	//����̍X�V����
	m_brave->GetArmedWeapon()->UpdateNormalAttackProcess(WeaponBase::enCombo_Third);

	//�A�j���[�V�������I�������
	if (m_brave->GetModelRender().IsPlayingAnimation() == false)
	{
		//�X�e�[�g�̋��ʏ���
		m_brave->ProcessCommonStateTransition();
	}
}

void BraveState_ThirdAttack::Exit()
{
	//����̔����o������
	m_brave->GetArmedWeapon()->ExitNormalAttackProcess(WeaponBase::enCombo_Third);

	//�A�N�V�������I���
	m_brave->ExitAttackAction();
}
