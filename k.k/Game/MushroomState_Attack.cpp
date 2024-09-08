#include "stdafx.h"
#include "MushroomState_Attack.h"
#include "Mushroom.h"

MushroomState_Attack::~MushroomState_Attack()
{
}

void MushroomState_Attack::PlayAnimation()
{
	m_mushroom->GetModelRender().PlayAnimation(enMushroomState_Attack, 0.2f);
}

void MushroomState_Attack::Entry()
{
	//攻撃アクションに入った時の処理
	m_mushroom->EntryAttackActionProcess();
}

void MushroomState_Attack::Ubdate()
{
	m_mushroom->UpdateAttackActionProcess();

	//アニメーションが終わったら
	if (m_mushroom->GetModelRender().IsPlayingAnimation() == false)
	{
		m_mushroom->ProcessCommonTranstion();
	}
}

void MushroomState_Attack::Exit()
{
	//アクションを終わる時の処理
	m_mushroom->ExitAttackActionProcess();
}
