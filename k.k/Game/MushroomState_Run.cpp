#include "stdafx.h"
#include "MushroomState_Run.h"
#include "Mushroom.h"

MushroomState_Run::~MushroomState_Run()
{
}

void MushroomState_Run::PlayAnimation()
{
	m_mushroom->GetModelRender().PlayAnimation(enMushroomState_Run, 0.2f);
}

void MushroomState_Run::Entry()
{
}

void MushroomState_Run::Ubdate()
{
	if (fabsf(m_mushroom->GetMoveSpeed().x) <= 0.001f &&
		fabsf(m_mushroom->GetMoveSpeed().z) <= 0.001f)
	{
		m_stateCotext->ChangeMushroomState(m_mushroom, enMushroomState_Idle);
		return;
	}
}

void MushroomState_Run::Exit()
{
}
