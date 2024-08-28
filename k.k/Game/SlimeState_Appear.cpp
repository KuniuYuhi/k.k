#include "stdafx.h"
#include "SlimeState_Appear.h"
#include "Slime.h"

SlimeState_Appear::~SlimeState_Appear()
{

}

void SlimeState_Appear::PlayAnimation()
{
	m_slime->GetModelRender().PlayAnimation(enSlimeState_Appear, 0.2f);
}

void SlimeState_Appear::Entry()
{
	//アクション中にする
	m_slime->ActionActive();

	//プレイヤーの方を向くようにする
	m_slime->TrunToTarget();

}

void SlimeState_Appear::Ubdate()
{
	//アニメーションが終わったら
	if (m_slime->GetModelRender().IsPlayingAnimation() == false)
	{
		m_slime->ProcessCommonTranstion();
	}

}

void SlimeState_Appear::Exit()
{
	//アクションを終わる
	m_slime->ActionDeactive();
}
