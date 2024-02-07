#include "stdafx.h"
#include "MushroomStateDamage.h"
#include "Mushroom.h"

void MushroomStateDamage::ManageState()
{ 
	m_mushroom->OnProcessDamageStateTransition();
}

void MushroomStateDamage::PlayAnimation()
{
	//�ŏ��̃A�j���[�V�����̍Đ��̓A�j���[�V���������Z�b�g
	m_mushroom->GetModelRender().PlayAnimation(enAnimationClip_Hit, 0.0f, m_animationResetFlag);
	//�A�j���[�V���������Z�b�g�����̂Ńt���O��������
	m_animationResetFlag = false;
}
