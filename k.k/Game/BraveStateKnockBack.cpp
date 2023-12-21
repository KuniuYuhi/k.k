#include "stdafx.h"
#include "BraveStateKnockBack.h"
#include "Brave.h"

namespace {
	const float STIFFNESS_LIMMIT = 0.4f;
}

void BraveStateKnockBack::ManageState()
{
	if (m_knockBackEndFlag == true)
	{
		//­‚µ‚¾‚¯d’¼‚·‚é
		//d’¼‚µ‚È‚­‚È‚Á‚½‚çˆ—I‚í‚è
		if (IsStiffness() == false)
		{
			//ƒXƒe[ƒg‹¤’Ê‚Ìó‘Ô‘JˆÚˆ—‚É‘JˆÚ
			m_brave->ProcessCommonStateTransition();
		}
		return;
	}

	if (m_brave->IsKnockBack() != true)
	{
		m_knockBackEndFlag = true;
	}
}

void BraveStateKnockBack::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		Brave::enAnimClip_KnockBack + m_brave->GetCurrentMainWeaponAnimationStartIndexNo()
	);
}

bool BraveStateKnockBack::IsStiffness()
{
	if (m_stiffnessTimer > STIFFNESS_LIMMIT)
	{
		return false;
	}
	else
	{
		m_stiffnessTimer += g_gameTime->GetFrameDeltaTime();
	}
	
	return true;
}
