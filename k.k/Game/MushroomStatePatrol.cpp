#include "stdafx.h"
#include "MushroomStatePatrol.h"
#include "Mushroom.h"

void MushroomStatePatrol::ManageState()
{
	m_mushroom->MovePatrol(m_mushroom->GetCharacterController());
}

void MushroomStatePatrol::PlayAnimation()
{
	if (fabsf(m_mushroom->GetMoveSpeed().x) >= 0.001f || fabsf(m_mushroom->GetMoveSpeed().z) >= 0.001f)
	{
		m_mushroom->GetModelRender().PlayAnimation(enAninationClip_Patrol, 0.2f);
	}
	else
	{
		m_mushroom->GetModelRender().PlayAnimation(enAninationClip_Idle, 0.3f);
	}
}
