#include "stdafx.h"
#include "SlimeState_Attack.h"
#include "Slime.h"

SlimeState_Attack::~SlimeState_Attack()
{
}

void SlimeState_Attack::PlayAnimation()
{
	m_slime->GetModelRender().PlayAnimation(enSlimeState_Attack, 0.2f);
}

void SlimeState_Attack::Entry()
{
	//攻撃アクションに入った時の処理
	m_slime->EntryAttackActionProcess();
	
}

void SlimeState_Attack::Ubdate()
{


	m_slime->UpdateAttackActionProcess();

	//アニメーションが終わったら
	if (m_slime->GetModelRender().IsPlayingAnimation() == false)
	{
		m_slime->ProcessCommonTranstion();
	}


}

void SlimeState_Attack::Exit()
{
	//アクションを終わる時の処理
	m_slime->ExitAttackActionProcess();
}
