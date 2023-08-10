#include "stdafx.h"
#include "CactusStatePlantToBattle.h"
#include "Cactus.h"

void CactusStatePlantToBattle::ManageState()
{
	m_cactus->OnProcessPlantToBattleStateTransition();
}

void CactusStatePlantToBattle::PlayAnimation()
{
	m_cactus->GetModelRender().PlayAnimation(Cactus::enAnimClip_PlantToBattle);
}
