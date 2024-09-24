#include "stdafx.h"
#include "BraveState_WinStart.h"
#include "Brave.h"
#include "BraveStateContext.h"

void BraveState_WinStart::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		enBraveAnimClip_Win_Start + m_brave->GetCurrentAnimationStartIndexNo(), 0.1f
	);
}

void BraveState_WinStart::Entry()
{
	m_brave->ActionActive();
}

void BraveState_WinStart::Ubdate()
{
	if (m_brave->GetModelRender().IsPlayingAnimation() == false)
	{
		//アニメーションが終わったらメインに進む
		m_stateCotext->ChangeBraveState(enBraveState_WinMain);
	}

}

void BraveState_WinStart::Exit()
{
}
