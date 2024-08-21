#include "stdafx.h"
#include "SlimeState_Idle.h"
#include "Slime.h"

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
}

void SlimeState_Idle::Exit()
{
}
