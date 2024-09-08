#include "stdafx.h"
#include "MushroomState_Hit.h"
#include "Mushroom.h"

MushroomState_Hit::~MushroomState_Hit()
{
}

void MushroomState_Hit::PlayAnimation()
{
	m_mushroom->GetModelRender().PlayAnimation(
		enMushroomAnimClip_Hit, 0.2f, m_playAnimationResetFlag
	);
	m_playAnimationResetFlag = false;
}

void MushroomState_Hit::Entry()
{
	//アクション中にする
	m_mushroom->ActionActive();

	m_mushroom->EntryHitActionProcess();

	m_playAnimationResetFlag = true;
}

void MushroomState_Hit::Ubdate()
{
	m_mushroom->UpdateHitActionProcess();
}

void MushroomState_Hit::Exit()
{
	//アクションを終わる
	m_mushroom->ActionDeactive();
}
