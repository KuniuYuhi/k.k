#include "stdafx.h"
#include "CactusStateRun.h"
#include "Cactus.h"

void CactusStateRun::ManageState()
{
	m_cactus->ProcessCommonStateTransition();
}

void CactusStateRun::PlayAnimation()
{
	m_cactus->GetModelRender().PlayAnimation(Cactus::enAnimClip_Run);
}
