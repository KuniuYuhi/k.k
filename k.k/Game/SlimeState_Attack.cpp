#include "stdafx.h"
#include "SlimeState_Attack.h"
#include "Slime.h"

SlimeState_Attack::~SlimeState_Attack()
{
}

void SlimeState_Attack::PlayAnimation()
{
	m_slime->GetModelRender().PlayAnimation(enSlimeState_Attack, 0.2f);
}

void SlimeState_Attack::Entry()
{
}

void SlimeState_Attack::Ubdate()
{
}

void SlimeState_Attack::Exit()
{
}
