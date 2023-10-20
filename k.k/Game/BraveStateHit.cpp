#include "stdafx.h"
#include "BraveStateHit.h"
#include "Brave.h"

void BraveStateHit::ManageState()
{
	m_brave->ProcessHitStateTransition();
}

void BraveStateHit::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(Brave::enAnimClip_Hit, 0.1f);
}