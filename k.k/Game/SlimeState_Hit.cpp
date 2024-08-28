#include "stdafx.h"
#include "SlimeState_Hit.h"
#include "Slime.h"

SlimeState_Hit::~SlimeState_Hit()
{
}

void SlimeState_Hit::PlayAnimation()
{
	m_slime->GetModelRender().PlayAnimation(enSlimeState_Hit, 0.2f, m_playAnimationResetFlag);

	m_playAnimationResetFlag = false;
}

void SlimeState_Hit::Entry()
{
	//アクション中にする
	m_slime->ActionActive();

	m_slime->EntryHitActionProcess();

	m_playAnimationResetFlag = true;
}

void SlimeState_Hit::Ubdate()
{
	m_slime->UpdateHitActionProcess();

	////アニメーションが終わったら
	//if (m_slime->GetModelRender().IsPlayingAnimation() == false)
	//{
	//	//少し硬直して共通ステート処理に移行

	//	m_slime->ProcessCommonTranstion();
	//}

}

void SlimeState_Hit::Exit()
{
	//アクションを終わる
	m_slime->ActionDeactive();
}
