#include "stdafx.h"
#include "MushroomStateVictory.h"
#include "Mushroom.h"

void MushroomStateVictory::ManageState()
{
	m_mushroom->OnProcessVictoryStateTransition();
}

void MushroomStateVictory::PlayAnimation()
{
	m_mushroom->GetModelRender().PlayAnimation(enAnimationClip_Victory);
}
