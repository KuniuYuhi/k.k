#include "stdafx.h"
#include "LichStateWalk.h"
#include "Lich.h"

void LichStateWalk::ManageState()
{
	m_lich->ProcessCommonStateTransition();
}

void LichStateWalk::PlayAnimation()
{
	m_lich->GetModelRender().PlayAnimation(Lich::enAnimClip_Walk,0.3f);
}
