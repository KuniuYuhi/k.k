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
	m_brave->ActionActive();
}

void BraveState_DefensiveAction::Ubdate()
{


	//����A�h��A�N�V�������I���Ȃ�
	if (m_brave->GetArmedWeapon()->IsEndDefensiveAction())
	{
		//�X�e�[�g�̋��ʏ���
		m_brave->ProcessCommonStateTransition();
	}


}

void BraveState_DefensiveAction::Exit()
{
	m_brave->ActionDeactive();
}
