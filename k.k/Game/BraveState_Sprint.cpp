#include "stdafx.h"
#include "BraveState_Sprint.h"
#include "Brave.h"
#include "BraveStateContext.h"

using namespace BraveAnimationClip;

void BraveState_Sprint::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		enBraveAnimClip_Sprint + m_brave->GetCurrentAnimationStartIndexNo()
	);
}

void BraveState_Sprint::Entry()
{
}

void BraveState_Sprint::Ubdate()
{
	if (fabsf(m_brave->GetMoveSpeed().x) <= 0.001f &&
		fabsf(m_brave->GetMoveSpeed().z) <= 0.001f)
	{
		m_stateCotext->ChangeBraveState(enBraveState_Idle);
		return;
	}

}

void BraveState_Sprint::Exit()
{
}
