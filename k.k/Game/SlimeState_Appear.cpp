#include "stdafx.h"
#include "SlimeState_Appear.h"
#include "Slime.h"

SlimeState_Appear::~SlimeState_Appear()
{

}

void SlimeState_Appear::PlayAnimation()
{
	m_slime->GetModelRender().PlayAnimation(enSlimeState_Appear, 0.2f);
}

void SlimeState_Appear::Entry()
{
}

void SlimeState_Appear::Ubdate()
{
}

void SlimeState_Appear::Exit()
{
}
