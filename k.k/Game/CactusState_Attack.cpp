#include "stdafx.h"
#include "CactusState_Attack.h"
#include "Cactus.h"

CactusState_Attack::~CactusState_Attack()
{
}

void CactusState_Attack::PlayAnimation()
{
	m_cactus->GetModelRender().PlayAnimation(
		enCactusAnimClip_Attack
	);
}

void CactusState_Attack::Entry()
{
	//�U���A�N�V�����ɓ��������̏���
	m_cactus->EntryAttackActionProcess();
}

void CactusState_Attack::Ubdate()
{
	m_cactus->UpdateAttackActionProcess();

	//�A�j���[�V�������I�������
	if (m_cactus->GetModelRender().IsPlayingAnimation() == false)
	{
		m_cactus->ProcessCommonTranstion();
	}

}

void CactusState_Attack::Exit()
{
	//�A�N�V�������I��鎞�̏���
	m_cactus->ExitAttackActionProcess();
}
