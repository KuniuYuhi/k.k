#include "stdafx.h"
#include "SlimeStateAppear.h"
#include "Slime.h"

void SlimeStateAppear::ManageState()
{
	m_slime->OnProcessAppearStateTransition();
}

void SlimeStateAppear::PlayAnimation()
{
	m_slime->GetModelRender().PlayAnimation(enAnimationClip_Appear);
}
