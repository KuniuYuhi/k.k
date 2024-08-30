#include "stdafx.h"
#include "BeholderEyeState_Attack.h"
#include "BeholderEye.h"

BeholderEyeState_Attack::~BeholderEyeState_Attack()
{
}

void BeholderEyeState_Attack::PlayAnimation()
{
	m_beholderEye->GetModelRender().PlayAnimation(enBeholderEyeState_Attack, 0.2f);
}

void BeholderEyeState_Attack::Entry()
{
	//攻撃アクションに入った時の処理
	m_beholderEye->EntryAttackActionProcess();
}

void BeholderEyeState_Attack::Ubdate()
{

	//アニメーションが終わったら
	if (m_beholderEye->GetModelRender().IsPlayingAnimation() == false)
	{
		m_beholderEye->ProcessCommonTranstion();
	}

}

void BeholderEyeState_Attack::Exit()
{
	//アクションを終わる時の処理
	m_beholderEye->ExitAttackActionProcess();
}
