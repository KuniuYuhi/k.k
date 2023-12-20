#include "stdafx.h"
#include "MushroomStateIdle.h"
#include "Mushroom.h"

void MushroomStateIdle::ManageState()
{
	m_mushroom->ProcessCommonStateTransition();
}

void MushroomStateIdle::PlayAnimation()
{
	m_mushroom->GetModelRender().PlayAnimation(enAnimationClip_Idle, 0.3f);
}
