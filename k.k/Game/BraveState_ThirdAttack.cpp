#include "stdafx.h"
#include "BraveState_ThirdAttack.h"
#include "Brave.h"


void BraveState_ThirdAttack::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		enBraveAnimClip_ThirdAttack + m_brave->GetCurrentAnimationStartIndexNo(), 0.1f
	);
}

void BraveState_ThirdAttack::Entry()
{
	//アクション中にする
	m_brave->ActionActive();
}

void BraveState_ThirdAttack::Ubdate()
{
	//アニメーションが終わったら
	if (m_brave->GetModelRender().IsPlayingAnimation() == false)
	{
		//ステートの共通処理
		m_brave->ProcessCommonStateTransition();
	}
}

void BraveState_ThirdAttack::Exit()
{
	//アクションを終わる
	m_brave->ExitAttackAction();
}
