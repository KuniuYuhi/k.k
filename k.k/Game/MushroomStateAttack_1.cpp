#include "stdafx.h"
#include "MushroomStateAttack_1.h"
#include "Mushroom.h"

void MushroomStateAttack_1::ManageState()
{
	//当たり判定生成フラグがセットされていたら当たり判定生成
	if (m_mushroom->GetCreateAttackCollisionFlag() == true)
	{
		m_mushroom->CreateCollision();
	}

	m_mushroom->OnProcessAttack_1StateTransition();
}

void MushroomStateAttack_1::PlayAnimation()
{
	m_mushroom->GetModelRender().PlayAnimation(enAnimationClip_Attack);
}
