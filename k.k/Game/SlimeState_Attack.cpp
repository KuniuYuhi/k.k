#include "stdafx.h"
#include "SlimeState_Attack.h"
#include "Slime.h"

SlimeState_Attack::~SlimeState_Attack()
{
}

void SlimeState_Attack::PlayAnimation()
{
	m_slime->GetModelRender().PlayAnimation(enSlimeState_Attack, 0.2f);
}

void SlimeState_Attack::Entry()
{
	//�U���A�N�V�����ɓ��������̏���
	m_slime->EntryAttackActionProcess();
	
}

void SlimeState_Attack::Ubdate()
{


	m_slime->UpdateAttackActionProcess();

	//�A�j���[�V�������I�������
	if (m_slime->GetModelRender().IsPlayingAnimation() == false)
	{
		m_slime->ProcessCommonTranstion();
	}


}

void SlimeState_Attack::Exit()
{
	//�A�N�V�������I��鎞�̏���
	m_slime->ExitAttackActionProcess();
}
