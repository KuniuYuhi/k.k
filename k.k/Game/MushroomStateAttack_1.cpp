#include "stdafx.h"
#include "MushroomStateAttack_1.h"
#include "Mushroom.h"

void MushroomStateAttack_1::ManageState()
{
	m_mushroom->OnProcessAttack_1StateTransition();
}

void MushroomStateAttack_1::PlayAnimation()
{
	m_mushroom->GetModelRender().PlayAnimation(Mushroom::enAnimClip_Attack_1);
}
