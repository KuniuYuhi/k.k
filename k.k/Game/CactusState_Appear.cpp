#include "stdafx.h"
#include "CactusState_Appear.h"
#include "Cactus.h"

CactusState_Appear::~CactusState_Appear()
{
}

void CactusState_Appear::PlayAnimation()
{
	m_cactus->GetModelRender().PlayAnimation(
		enCactusAnimClip_Appear, 0.2f
	);
}

void CactusState_Appear::Entry()
{
	//アクション中にする
	m_cactus->ActionActive();

	//プレイヤーの方を向くようにする
	m_cactus->TurnToPlayer();
}

void CactusState_Appear::Ubdate()
{

	//アニメーションが終わったら
	if (m_cactus->GetModelRender().IsPlayingAnimation() == false)
	{
		m_cactus->ProcessCommonTranstion();
	}
}

void CactusState_Appear::Exit()
{
	//アクションを終わる
	m_cactus->ActionDeactive();
}
