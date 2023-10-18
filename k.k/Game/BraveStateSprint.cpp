#include "stdafx.h"
#include "BraveStateSprint.h"
#include "Brave.h"

void BraveStateSprint::ManageState()
{
	if (fabsf(m_brave->GetMoveSpeed().x) >= 0.001f ||
		fabsf(m_brave->GetMoveSpeed().z) >= 0.001f
		)
	{
		//‘–‚é
		m_brave->SetNextAnimationState(Brave::enAninationState_Sprint);
	}
	else
	{
		//•à‚­
		m_brave->SetNextAnimationState(Brave::enAninationState_Idle);
	}
}

void BraveStateSprint::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(Brave::enAnimClip_Sprint, 0.3f);
}
