#include "stdafx.h"
#include "MushroomStateChase.h"
#include "Mushroom.h"

void MushroomStateChase::ManageState()
{
	//ƒvƒŒƒCƒ„[‚ð’Ç‚¢‚©‚¯‚é
	m_mushroom->MoveChasePlayer(m_mushroom->GetCharacterController());
}

void MushroomStateChase::PlayAnimation()
{
	m_mushroom->GetModelRender().PlayAnimation(enAninationClip_Chase, 0.1f);
}
