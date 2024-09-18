#include "stdafx.h"
#include "SlimeState_Idle.h"
#include "Slime.h"
#include "SlimeInfo.h"

using namespace SlimeStates;

SlimeState_Idle::~SlimeState_Idle()
{
}

void SlimeState_Idle::PlayAnimation()
{
	m_slime->GetModelRender().PlayAnimation(enSlimeState_Idle, 0.2f);
}

void SlimeState_Idle::Entry()
{
}

void SlimeState_Idle::Ubdate()
{
	m_slime->TurnToPlayer();

	//ˆÚ“®—Ê‚ª‚ ‚Á‚½‚ç•à‚«ó‘Ô‚É‘JˆÚ
	if (fabsf(m_slime->GetMoveSpeed().x) >= 0.001f ||
		fabsf(m_slime->GetMoveSpeed().z) >= 0.001f)
	{
		m_stateCotext->ChangeSlimeState(m_slime, enSlimeState_Run);
		return;
	}
}

void SlimeState_Idle::Exit()
{
}
