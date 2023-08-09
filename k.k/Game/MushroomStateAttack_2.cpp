#include "stdafx.h"
#include "MushroomStateAttack_2.h"
#include "Mushroom.h"

void MushroomStateAttack_2::ManageState()
{
	m_mushroom->OnProcessAttack_2StateTransition();
}

void MushroomStateAttack_2::PlayAnimation()
{
	m_mushroom->GetModelRender().PlayAnimation(Mushroom::enAnimationState_Attack_2);
}
