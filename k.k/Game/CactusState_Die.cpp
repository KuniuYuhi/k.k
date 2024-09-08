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
	m_cactus->ActionActive();
}

void CactusState_Die::Ubdate()
{
	//アニメーションが終わったら
	if (m_cactus->GetModelRender().IsPlayingAnimation() == false)
	{
		m_cactus->DieProcess();
	}

}

void CactusState_Die::Exit()
{
	m_cactus->ActionDeactive();
}
