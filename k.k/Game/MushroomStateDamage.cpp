#include "stdafx.h"
#include "MushroomStateDamage.h"
#include "Mushroom.h"

void MushroomStateDamage::ManageState()
{ 
	m_mushroom->OnProcessDamageStateTransition();
}

void MushroomStateDamage::PlayAnimation()
{
	m_mushroom->GetModelRender().PlayAnimation(Mushroom::enAnimationState_Damage);
}
