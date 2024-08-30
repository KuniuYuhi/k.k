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
	//�U���A�N�V�����ɓ��������̏���
	m_beholderEye->EntryAttackActionProcess();
}

void BeholderEyeState_Attack::Ubdate()
{

	//�A�j���[�V�������I�������
	if (m_beholderEye->GetModelRender().IsPlayingAnimation() == false)
	{
		m_beholderEye->ProcessCommonTranstion();
	}

}

void BeholderEyeState_Attack::Exit()
{
	//�A�N�V�������I��鎞�̏���
	m_beholderEye->ExitAttackActionProcess();
}
