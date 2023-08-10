#include "stdafx.h"
#include "CactusStatePlant.h"
#include "Cactus.h"

void CactusStatePlant::ManageState()
{
	m_cactus->OnProcessPlantStateTransition();
}

void CactusStatePlant::PlayAnimation()
{
	m_cactus->GetModelRender().PlayAnimation(Cactus::enAnimClip_Plant);
}
