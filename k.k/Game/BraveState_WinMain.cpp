#include "stdafx.h"
#include "BraveState_WinMain.h"
#include "Brave.h"

void BraveState_WinMain::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		enBraveAnimClip_Win_Main + m_brave->GetCurrentAnimationStartIndexNo(), 0.1f
	);
}

void BraveState_WinMain::Entry()
{
}

void BraveState_WinMain::Ubdate()
{
}

void BraveState_WinMain::Exit()
{
	m_brave->ActionDeactive();
}
