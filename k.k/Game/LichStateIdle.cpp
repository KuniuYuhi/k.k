#include "stdafx.h"
#include "LichStateIdle.h"
#include "Lich.h"

void LichStateIdle::ManageState()
{
	m_lich->ProcessCommonStateTransition();
}

void LichStateIdle::PlayAnimation()
{
	m_lich->GetModelRender().PlayAnimation(Lich::enAnimClip_Idle,0.2f);
}
