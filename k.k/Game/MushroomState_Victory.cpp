#include "stdafx.h"
#include "MushroomState_Victory.h"
#include "Mushroom.h"

MushroomState_Victory::~MushroomState_Victory()
{
}

void MushroomState_Victory::PlayAnimation()
{
	m_mushroom->GetModelRender().PlayAnimation(
		enMushroomState_Victory
	);
}

void MushroomState_Victory::Entry()
{
	m_mushroom->ActionActive();
}

void MushroomState_Victory::Ubdate()
{
}

void MushroomState_Victory::Exit()
{
}
