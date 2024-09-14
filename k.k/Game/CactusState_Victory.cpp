#include "stdafx.h"
#include "CactusState_Victory.h"
#include "Cactus.h"

CactusState_Victory::~CactusState_Victory()
{
}

void CactusState_Victory::PlayAnimation()
{
	m_cactus->GetModelRender().PlayAnimation(
		enCactusAnimClip_Victory
	);
}

void CactusState_Victory::Entry()
{
	m_cactus->ActionActive();
}

void CactusState_Victory::Ubdate()
{
}

void CactusState_Victory::Exit()
{
}
