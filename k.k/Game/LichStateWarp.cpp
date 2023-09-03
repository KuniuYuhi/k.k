#include "stdafx.h"
#include "LichStateWarp.h"
#include "Lich.h"

void LichStateWarp::ManageState()
{
	m_lich->OnProcessWarpStateTransition();
}

void LichStateWarp::PlayAnimation()
{
	m_lich->GetModelRender().PlayAnimation(Lich::enAnimClip_Idle);
}
