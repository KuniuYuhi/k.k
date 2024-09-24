#include "stdafx.h"
#include "SlimeState_Victory.h"
#include "Slime.h"

SlimeState_Victory::~SlimeState_Victory()
{
}

void SlimeState_Victory::PlayAnimation()
{
	m_slime->GetModelRender().PlayAnimation(enSlimeState_Victory, 0.2f);
}

void SlimeState_Victory::Entry()
{
}

void SlimeState_Victory::Ubdate()
{
}

void SlimeState_Victory::Exit()
{
}
