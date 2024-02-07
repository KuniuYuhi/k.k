#include "stdafx.h"
#include "CactusStateDamage.h"
#include "Cactus.h"

void CactusStateDamage::ManageState()
{
	m_cactus->OnProcessDamageStateTransition();
}

void CactusStateDamage::PlayAnimation()
{
	//最初のアニメーションの再生はアニメーションをリセット
	m_cactus->GetModelRender().PlayAnimation(enAnimationClip_Hit, 0.0f, m_animationResetFlag);
	//アニメーションをリセットしたのでフラグを下げる
	m_animationResetFlag = false;
}
