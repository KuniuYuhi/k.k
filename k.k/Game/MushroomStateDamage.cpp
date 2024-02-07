#include "stdafx.h"
#include "MushroomStateDamage.h"
#include "Mushroom.h"

void MushroomStateDamage::ManageState()
{ 
	m_mushroom->OnProcessDamageStateTransition();
}

void MushroomStateDamage::PlayAnimation()
{
	//最初のアニメーションの再生はアニメーションをリセット
	m_mushroom->GetModelRender().PlayAnimation(enAnimationClip_Hit, 0.0f, m_animationResetFlag);
	//アニメーションをリセットしたのでフラグを下げる
	m_animationResetFlag = false;
}
