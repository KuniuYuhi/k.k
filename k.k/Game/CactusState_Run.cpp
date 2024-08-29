#include "stdafx.h"
#include "CactusState_Run.h"
#include "Cactus.h"

CactusState_Run::~CactusState_Run()
{
}

void CactusState_Run::PlayAnimation()
{
	m_cactus->GetModelRender().PlayAnimation(
		enCactusAnimClip_Run, 0.2f
	);
}

void CactusState_Run::Entry()
{
}

void CactusState_Run::Ubdate()
{
	if (fabsf(m_cactus->GetMoveSpeed().x) <= 0.001f &&
		fabsf(m_cactus->GetMoveSpeed().z) <= 0.001f)
	{
		m_stateCotext->ChangeCactusState(m_cactus, enCactusState_Idle);
		return;
	}

}

void CactusState_Run::Exit()
{
}
