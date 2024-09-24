#include "stdafx.h"
#include "BraveState_Idle.h"
#include "Brave.h"
#include "BraveStateContext.h"

void BraveState_Idle::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		enBraveAnimClip_Idle + m_brave->GetCurrentAnimationStartIndexNo(),0.2f
	);
}

void BraveState_Idle::Entry()
{
}

void BraveState_Idle::Ubdate()
{

	


	if (fabsf(m_brave->GetMoveSpeed().x) >= 0.001f ||
		fabsf(m_brave->GetMoveSpeed().z) >= 0.001f)
	{
		m_stateCotext->ChangeBraveState(enBraveState_Sprint);
		return;
	}
}

void BraveState_Idle::Exit()
{
}
