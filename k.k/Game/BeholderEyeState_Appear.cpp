#include "stdafx.h"
#include "BeholderEyeState_Appear.h"
#include "BeholderEye.h"

BeholderEyeState_Appear::~BeholderEyeState_Appear()
{
}

void BeholderEyeState_Appear::PlayAnimation()
{
	m_beholderEye->GetModelRender().PlayAnimation(
		enBeholderEyeAnimClip_Appear, 0.2f
	);
}

void BeholderEyeState_Appear::Entry()
{
	//アクション中にする
	m_beholderEye->ActionActive();

	//プレイヤーの方を向くようにする
	m_beholderEye->TurnToPlayer();
}

void BeholderEyeState_Appear::Ubdate()
{
	//アニメーションが終わったら
	if (m_beholderEye->GetModelRender().IsPlayingAnimation() == false)
	{
		m_beholderEye->ProcessCommonTranstion();
	}
}

void BeholderEyeState_Appear::Exit()
{
	//アクションを終わる
	m_beholderEye->ActionDeactive();
}
