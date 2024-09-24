#include "stdafx.h"
#include "BeholderEyeState_Run.h"
#include "BeholderEye.h"

BeholderEyeState_Run::~BeholderEyeState_Run()
{
}

void BeholderEyeState_Run::PlayAnimation()
{
	m_beholderEye->GetModelRender().PlayAnimation(enBeholderEyeState_Run, 0.2f);
}

void BeholderEyeState_Run::Entry()
{
}

void BeholderEyeState_Run::Ubdate()
{
	if (fabsf(m_beholderEye->GetMoveSpeed().x) <= 0.001f &&
		fabsf(m_beholderEye->GetMoveSpeed().z) <= 0.001f)
	{
		m_stateCotext->ChangeBeholderEyeState(m_beholderEye, enBeholderEyeState_Idle);
		return;
	}

}

void BeholderEyeState_Run::Exit()
{
}
