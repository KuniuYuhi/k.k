#include "stdafx.h"
#include "BraveState_Hit.h"
#include "Brave.h"
#include "BraveStateContext.h"

void BraveState_Hit::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		enBraveAnimClip_Hit + m_brave->GetCurrentAnimationStartIndexNo(), 0.2f
	);
}

void BraveState_Hit::Entry()
{

	m_brave->EntryHitActionProcess();
}

void BraveState_Hit::Ubdate()
{
	//����A�h��ł���悤�ɂ���

	//�A�j���[�V�������I�������
	m_brave->UpdateHitActionProcess();
}

void BraveState_Hit::Exit()
{

	m_brave->ExitHitActionProcess();
}
