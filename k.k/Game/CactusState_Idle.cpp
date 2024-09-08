#include "stdafx.h"
#include "CactusState_Idle.h"
#include "Cactus.h"

CactusState_Idle::~CactusState_Idle()
{
}

void CactusState_Idle::PlayAnimation()
{
	m_cactus->GetModelRender().PlayAnimation(
		enCactusAnimClip_Idle, 0.2f
	);
}

void CactusState_Idle::Entry()
{
}

void CactusState_Idle::Ubdate()
{
	//ˆÚ“®—Ê‚ª‚ ‚Á‚½‚ç•à‚«ó‘Ô‚É‘JˆÚ
	if (fabsf(m_cactus->GetMoveSpeed().x) >= 0.001f ||
		fabsf(m_cactus->GetMoveSpeed().z) >= 0.001f)
	{
		m_stateCotext->ChangeCactusState(m_cactus, enCactusState_Run);
		return;
	}
}

void CactusState_Idle::Exit()
{
}
