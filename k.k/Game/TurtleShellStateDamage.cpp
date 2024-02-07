#include "stdafx.h"
#include "TurtleShellStateDamage.h"
#include "TurtleShell.h"

void TurtleShellStateDamage::ManageState()
{
	m_turtleShell->OnProcessDamageStateTransition();
}

void TurtleShellStateDamage::PlayAnimation()
{
	//最初のアニメーションの再生はアニメーションをリセット
	m_turtleShell->GetModelRender().PlayAnimation(enAnimationClip_Hit, 0.0f, m_animationResetFlag);
	//アニメーションをリセットしたのでフラグを下げる
	m_animationResetFlag = false;
}
