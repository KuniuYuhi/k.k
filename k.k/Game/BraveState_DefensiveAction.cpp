#include "stdafx.h"
#include "BraveState_DefensiveAction.h"
#include "Brave.h"
#include "WeaponBase.h"

void BraveState_DefensiveAction::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		enBraveAnimClip_DefensiveActions + m_brave->GetCurrentAnimationStartIndexNo(), 0.1f
	);
}

void BraveState_DefensiveAction::Entry()
{
	//�A�N�V�������ɂ���
	m_brave->ActionActive();

	//�������Ă��镐��̉���A�h��A�N�V�����ɓ��������̍ŏ��̏��������s
	m_brave->GetArmedWeapon()->EntryDefensiveActionProcess();
}

void BraveState_DefensiveAction::Ubdate()
{
	//�������Ă��镐��̍X�V����
	m_brave->GetArmedWeapon()->UpdateDefensiveActionProcess();

	//����A�h��A�N�V�������I���Ȃ�
	//if (m_brave->GetArmedWeapon()->IsEndDefensiveAction())
	//{
	//	//�X�e�[�g�̋��ʏ���
	//	m_brave->ProcessCommonStateTransition();
	//}


}

void BraveState_DefensiveAction::Exit()
{
	m_brave->GetArmedWeapon()->ExitDefensiveActionProcess();

	m_brave->ActionDeactive();
}
