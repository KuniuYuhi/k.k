#include "stdafx.h"
#include "BraveStateSkill_Start.h"
#include "Brave.h"

void BraveStateSkill_Start::ManageState()
{
	m_brave->ProcessSkillStartStateTransition();
}

void BraveStateSkill_Start::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		Brave::enAnimClip_Skill_Start + m_brave->GetCurrentMainWeaponAnimationStartIndexNo(), 
		0.2f
	);
}
