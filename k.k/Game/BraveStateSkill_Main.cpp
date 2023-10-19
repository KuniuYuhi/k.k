#include "stdafx.h"
#include "BraveStateSkill_Main.h"
#include "Brave.h"

void BraveStateSkill_Main::ManageState()
{
	m_brave->ProcessSkillMainStateTransition();
}

void BraveStateSkill_Main::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(Brave::enAnimClip_Skill_Main, 0.2f);
}
