#include "stdafx.h"
#include "MushroomStateWalk.h"
#include "Mushroom.h"

void MushroomStateWalk::ManageState()
{
	m_mushroom->ProcessCommonStateTransition();
}

void MushroomStateWalk::PlayAnimation()
{
	m_mushroom->GetModelRender().PlayAnimation(Mushroom::enAnimClip_Walk);
}
