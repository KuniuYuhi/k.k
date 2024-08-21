#include "stdafx.h"
#include "SlimeState_Run.h"
#include "Slime.h"

SlimeState_Run::~SlimeState_Run()
{
	
}

void SlimeState_Run::PlayAnimation()
{
	m_slime->GetModelRender().PlayAnimation(enSlimeState_Run, 0.2f);
}

void SlimeState_Run::Entry()
{
}

void SlimeState_Run::Ubdate()
{
}

void SlimeState_Run::Exit()
{
}
