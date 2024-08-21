#include "stdafx.h"
#include "SlimeState_Hit.h"
#include "Slime.h"

SlimeState_Hit::~SlimeState_Hit()
{
}

void SlimeState_Hit::PlayAnimation()
{
	m_slime->GetModelRender().PlayAnimation(enSlimeState_Hit, 0.2f);
}

void SlimeState_Hit::Entry()
{
}

void SlimeState_Hit::Ubdate()
{
}

void SlimeState_Hit::Exit()
{
}
