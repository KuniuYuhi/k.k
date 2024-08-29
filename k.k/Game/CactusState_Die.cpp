#include "stdafx.h"
#include "CactusState_Die.h"
#include "Cactus.h"

CactusState_Die::~CactusState_Die()
{
}

void CactusState_Die::PlayAnimation()
{
	m_cactus->GetModelRender().PlayAnimation(
		enCactusAnimClip_Die,0.2f
	);
}

void CactusState_Die::Entry()
{
}

void CactusState_Die::Ubdate()
{
}

void CactusState_Die::Exit()
{
}
