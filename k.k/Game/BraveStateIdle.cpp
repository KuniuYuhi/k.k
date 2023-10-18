#include "stdafx.h"
#include "BraveStateIdle.h"
#include "Brave.h"

void BraveStateIdle::ManageState()
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

void BraveStateIdle::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(Brave::enAnimClip_Idle, 0.5f);
}
