#include "stdafx.h"
#include "CactusStateAppear.h"
#include "Cactus.h"

void CactusStateAppear::ManageState()
{
	m_cactus->OnProcessAppearStateTransition();
}

void CactusStateAppear::PlayAnimation()
{
	m_cactus->GetModelRender().PlayAnimation(enAnimationClip_Appear);
}
