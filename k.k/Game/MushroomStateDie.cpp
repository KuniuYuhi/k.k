#include "stdafx.h"
#include "MushroomStateDie.h"
#include "Mushroom.h"

void MushroomStateDie::ManageState()
{
	m_mushroom->OnProcessDieStateTransition();
}

void MushroomStateDie::PlayAnimation()
{
	m_mushroom->GetModelRender().PlayAnimation(enAnimationClip_Die);
}
