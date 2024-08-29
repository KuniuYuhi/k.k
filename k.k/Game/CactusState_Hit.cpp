#include "stdafx.h"
#include "CactusState_Hit.h"
#include "Cactus.h"

CactusState_Hit::~CactusState_Hit()
{
}

void CactusState_Hit::PlayAnimation()
{
	m_cactus->GetModelRender().PlayAnimation(
		enCactusAnimClip_Hit, 0.1f,
		m_playAnimationResetFlag
	);

	m_playAnimationResetFlag = false;
}

void CactusState_Hit::Entry()
{
	//アクション中にする
	m_cactus->ActionActive();

	m_cactus->EntryHitActionProcess();

	m_playAnimationResetFlag = true;
}

void CactusState_Hit::Ubdate()
{
	m_cactus->UpdateHitActionProcess();
}

void CactusState_Hit::Exit()
{
	//アクションを終わる
	m_cactus->ActionDeactive();
}
