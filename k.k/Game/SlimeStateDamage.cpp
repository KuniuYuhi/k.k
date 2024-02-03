#include "stdafx.h"
#include "SlimeStateDamage.h"
#include "Slime.h"

void SlimeStateDamage::ManageState()
{
	m_slime->OnProcessDamageStateTransition();
}

void SlimeStateDamage::PlayAnimation()
{
	//最初のアニメーションの再生はアニメーションをリセット
	m_slime->GetModelRender().PlayAnimation(enAnimationClip_Hit,0.0f, m_animationResetFlag);
	//アニメーションをリセットしたのでフラグを下げる
	m_animationResetFlag = false;
}
