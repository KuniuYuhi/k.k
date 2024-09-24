#include "stdafx.h"
#include "BeholderEyeState_Victory.h"
#include "BeholderEye.h"


BeholderEyeState_Victory::~BeholderEyeState_Victory()
{
}

void BeholderEyeState_Victory::PlayAnimation()
{
	m_beholderEye->GetModelRender().PlayAnimation(
		enBeholderEyeState_Victory
	);
}

void BeholderEyeState_Victory::Entry()
{
	m_beholderEye->ActionActive();
}

void BeholderEyeState_Victory::Ubdate()
{
}

void BeholderEyeState_Victory::Exit()
{
}
