#include "stdafx.h"
#include "BeholderEyeState_Hit.h"
#include "BeholderEye.h"

BeholderEyeState_Hit::~BeholderEyeState_Hit()
{
}

void BeholderEyeState_Hit::PlayAnimation()
{
	m_beholderEye->GetModelRender().PlayAnimation(
		enBeholderEyeAnimClip_Hit, 0.2f, 
		m_playAnimationResetFlag
	);

	m_playAnimationResetFlag = false;
}

void BeholderEyeState_Hit::Entry()
{
	//ƒAƒNƒVƒ‡ƒ“’†‚É‚·‚é
	m_beholderEye->ActionActive();

	m_beholderEye->EntryHitActionProcess();

	m_playAnimationResetFlag = true;
}

void BeholderEyeState_Hit::Ubdate()
{
	m_beholderEye->UpdateHitActionProcess();
}

void BeholderEyeState_Hit::Exit()
{
	m_beholderEye->ExitHitActionProcess();
}
