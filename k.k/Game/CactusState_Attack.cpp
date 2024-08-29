#include "stdafx.h"
#include "CactusState_Attack.h"
#include "Cactus.h"

CactusState_Attack::~CactusState_Attack()
{
}

void CactusState_Attack::PlayAnimation()
{
	m_cactus->GetModelRender().PlayAnimation(
		enCactusAnimClip_Attack
	);
}

void CactusState_Attack::Entry()
{
	//攻撃アクションに入った時の処理
	m_cactus->EntryAttackActionProcess();
}

void CactusState_Attack::Ubdate()
{
	m_cactus->UpdateAttackActionProcess();

	//アニメーションが終わったら
	if (m_cactus->GetModelRender().IsPlayingAnimation() == false)
	{
		m_cactus->ProcessCommonTranstion();
	}

}

void CactusState_Attack::Exit()
{
	//アクションを終わる時の処理
	m_cactus->ExitAttackActionProcess();
}
