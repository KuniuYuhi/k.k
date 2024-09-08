#include "stdafx.h"
#include "BeholderEyeState_Idle.h"
#include "BeholderEye.h"

BeholderEyeState_Idle::~BeholderEyeState_Idle()
{
}

void BeholderEyeState_Idle::PlayAnimation()
{
	m_beholderEye->GetModelRender().PlayAnimation(enBeholderEyeAnimClip_Idle, 0.2f);
}

void BeholderEyeState_Idle::Entry()
{
}

void BeholderEyeState_Idle::Ubdate()
{
	//ˆÚ“®—Ê‚ª‚ ‚Á‚½‚ç•à‚«ó‘Ô‚É‘JˆÚ
	if (fabsf(m_beholderEye->GetMoveSpeed().x) >= 0.001f ||
		fabsf(m_beholderEye->GetMoveSpeed().z) >= 0.001f)
	{
		m_stateCotext->ChangeBeholderEyeState(m_beholderEye, enBeholderEyeState_Run);
		return;
	}
}

void BeholderEyeState_Idle::Exit()
{
}
