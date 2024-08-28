#include "stdafx.h"
#include "SlimeState_Run.h"
#include "Slime.h"
#include "SlimeInfo.h"

using namespace SlimeStates;


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
	if (fabsf(m_slime->GetMoveSpeed().x) <= 0.001f &&
		fabsf(m_slime->GetMoveSpeed().z) <= 0.001f)
	{
		m_stateCotext->ChangeSlimeState(m_slime,enSlimeState_Idle);
		return;
	}

}

void SlimeState_Run::Exit()
{
}
