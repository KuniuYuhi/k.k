#include "stdafx.h"
#include "CactusStateIdle.h"
#include "Cactus.h"

void CactusStateIdle::ManageState()
{
	//m_cactus->ProcessCommonStateTransition();
}

void CactusStateIdle::PlayAnimation()
{
	m_cactus->GetModelRender().PlayAnimation(enAnimationClip_Idle,0.4f);
}
