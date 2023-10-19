#include "stdafx.h"
#include "BraveStateDefendHit.h"
#include "Brave.h"

void BraveStateDefendHit::ManageState()
{
	m_brave->ProcessHitStateTransition();
}

void BraveStateDefendHit::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(Brave::enAnimClip_DefendHit, 0.1f);
}
