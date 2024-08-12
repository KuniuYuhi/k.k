#include "stdafx.h"
#include "BraveState_ChangeWeapon.h"
#include "Brave.h"

void BraveState_ChangeWeapon::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		enBraveAnimClip_ChangeWeapon + m_brave->GetCurrentAnimationStartIndexNo()
	);
}

void BraveState_ChangeWeapon::Entry()
{
	//�A�N�V�������ɂ���
	m_brave->ActionActive();
}

void BraveState_ChangeWeapon::Ubdate()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_brave->GetModelRender().IsPlayingAnimation() == false)
	{
		m_brave->ProcessCommonStateTransition();
	}
}

void BraveState_ChangeWeapon::Exit()
{
	//�A�N�V�������I���
	m_brave->ActionDeactive();
	//�A�j���[�V�����̍ŏ��̔ԍ���؂�ւ���̕���̂��̂ɕύX����
	m_brave->SetCurrentAnimationStartIndexNoForMainWeaponType();


}
