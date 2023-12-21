#include "stdafx.h"
#include "CactusStateDie.h"
#include "Cactus.h"

void CactusStateDie::ManageState()
{
	m_cactus->OnProcessDieStateTransition();
}

void CactusStateDie::PlayAnimation()
{
	m_cactus->GetModelRender().PlayAnimation(enAnimationClip_Die);
}
