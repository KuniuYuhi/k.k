#include "stdafx.h"
#include "CactusStateIdle.h"
#include "Cactus.h"

void CactusStateIdle::ManageState()
{
	m_cactus->ProcessCommonStateTransition();
}

void CactusStateIdle::PlayAnimation()
{
	m_cactus->GetModelRender().PlayAnimation(Cactus::enAnimClip_Idle);
}
