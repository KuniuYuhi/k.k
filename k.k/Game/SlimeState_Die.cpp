#include "stdafx.h"
#include "SlimeState_Die.h"
#include "Slime.h"

SlimeState_Die::~SlimeState_Die()
{
}

void SlimeState_Die::PlayAnimation()
{
	m_slime->GetModelRender().PlayAnimation(enSlimeState_Die, 0.2f);
}

void SlimeState_Die::Entry()
{
}

void SlimeState_Die::Ubdate()
{
}

void SlimeState_Die::Exit()
{
}
