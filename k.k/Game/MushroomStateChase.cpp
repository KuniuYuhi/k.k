#include "stdafx.h"
#include "MushroomStateChase.h"
#include "Mushroom.h"

void MushroomStateChase::ManageState()
{
	//プレイヤーを追いかける
	m_mushroom->MoveChasePlayer(m_mushroom->GetCharacterController());
}

void MushroomStateChase::PlayAnimation()
{
	m_mushroom->GetModelRender().PlayAnimation(enAnimationClip_Chase, 0.1f);
}
