#include "stdafx.h"
#include "LichStateAngry.h"
#include "Lich.h"

void LichStateAngry::ManageState()
{
	m_lich->OnProcessAngryStateTransition();
}

void LichStateAngry::PlayAnimation()
{
	m_lich->GetModelRender().PlayAnimation(Lich::enAnimationState_Angry);
}
