#include "stdafx.h"
#include "SlimeState_Die.h"
#include "Slime.h"

SlimeState_Die::~SlimeState_Die()
{
}

void SlimeState_Die::PlayAnimation()
{
	m_slime->GetModelRender().PlayAnimation(enSlimeState_Die, 0.2f);
}

void SlimeState_Die::Entry()
{
	m_slime->ActionActive();
}

void SlimeState_Die::Ubdate()
{
	//アニメーションが終わったら
	if (m_slime->GetModelRender().IsPlayingAnimation() == false)
	{
		m_slime->DieProcess();
	}
}

void SlimeState_Die::Exit()
{
	m_slime->ActionDeactive();
}
