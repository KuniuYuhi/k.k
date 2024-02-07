#include "stdafx.h"
#include "CactusStateDamage.h"
#include "Cactus.h"

void CactusStateDamage::ManageState()
{
	m_cactus->OnProcessDamageStateTransition();
}

void CactusStateDamage::PlayAnimation()
{
	//�ŏ��̃A�j���[�V�����̍Đ��̓A�j���[�V���������Z�b�g
	m_cactus->GetModelRender().PlayAnimation(enAnimationClip_Hit, 0.0f, m_animationResetFlag);
	//�A�j���[�V���������Z�b�g�����̂Ńt���O��������
	m_animationResetFlag = false;
}
