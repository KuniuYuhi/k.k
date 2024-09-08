#include "stdafx.h"
#include "MushroomState_Idle.h"
#include "Mushroom.h"

MushroomState_Idle::~MushroomState_Idle()
{
}

void MushroomState_Idle::PlayAnimation()
{
	m_mushroom->GetModelRender().PlayAnimation(enMushroomState_Idle, 0.2f);
}

void MushroomState_Idle::Entry()
{
}

void MushroomState_Idle::Ubdate()
{
	//ˆÚ“®—Ê‚ª‚ ‚Á‚½‚ç•à‚«ó‘Ô‚É‘JˆÚ
	if (fabsf(m_mushroom->GetMoveSpeed().x) >= 0.001f ||
		fabsf(m_mushroom->GetMoveSpeed().z) >= 0.001f)
	{
		m_stateCotext->ChangeMushroomState(m_mushroom, enMushroomState_Run);
		return;
	}
}

void MushroomState_Idle::Exit()
{
}
