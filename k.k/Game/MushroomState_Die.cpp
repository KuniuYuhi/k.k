#include "stdafx.h"
#include "MushroomState_Die.h"
#include "Mushroom.h"

MushroomState_Die::~MushroomState_Die()
{
}

void MushroomState_Die::PlayAnimation()
{
	m_mushroom->GetModelRender().PlayAnimation(enMushroomAnimClip_Die, 0.2f);
}

void MushroomState_Die::Entry()
{
	m_mushroom->ActionActive();
}

void MushroomState_Die::Ubdate()
{
	//アニメーションが終わったら
	if (m_mushroom->GetModelRender().IsPlayingAnimation() == false)
	{
		m_mushroom->DieProcess();
	}

}

void MushroomState_Die::Exit()
{
	m_mushroom->ActionDeactive();
}
