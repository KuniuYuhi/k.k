#include "stdafx.h"
#include "MushroomStateAppear.h"
#include "Mushroom.h"

void MashroomStateAppear::ManageState()
{
	m_mushroom->OnProcessAppearStateTransition();
}

void MashroomStateAppear::PlayAnimation()
{
	m_mushroom->GetModelRender().PlayAnimation(enAnimationClip_Appear);
}
