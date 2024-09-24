#include "stdafx.h"
#include "MushroomState_Appear.h"
#include "Mushroom.h"

MushroomState_Appear::~MushroomState_Appear()
{
}

void MushroomState_Appear::PlayAnimation()
{
	m_mushroom->GetModelRender().PlayAnimation(
		enMushroomAnimClip_Appear, 0.2f
	);
}

void MushroomState_Appear::Entry()
{
	//アクション中にする
	m_mushroom->ActionActive();

	//プレイヤーの方を向くようにする
	m_mushroom->TurnToPlayer();
}

void MushroomState_Appear::Ubdate()
{
	//アニメーションが終わったら
	if (m_mushroom->GetModelRender().IsPlayingAnimation() == false)
	{
		m_mushroom->ProcessCommonTranstion();
	}
}

void MushroomState_Appear::Exit()
{
	//アクションを終わる
	m_mushroom->ActionDeactive();
}
