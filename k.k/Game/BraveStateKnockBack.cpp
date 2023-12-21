#include "stdafx.h"
#include "BraveStateKnockBack.h"
#include "Brave.h"

namespace {
	const float STIFFNESS_LIMMIT = 0.4f;
}

void BraveStateKnockBack::ManageState()
{
	if (m_knockBackEndFlag == true)
	{
		//���������d������
		//�d�����Ȃ��Ȃ����珈���I���
		if (IsStiffness() == false)
		{
			//�X�e�[�g���ʂ̏�ԑJ�ڏ����ɑJ��
			m_brave->ProcessCommonStateTransition();
		}
		return;
	}

	if (m_brave->IsKnockBack() != true)
	{
		m_knockBackEndFlag = true;
	}
}

void BraveStateKnockBack::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		Brave::enAnimClip_KnockBack + m_brave->GetCurrentMainWeaponAnimationStartIndexNo()
	);
}

bool BraveStateKnockBack::IsStiffness()
{
	if (m_stiffnessTimer > STIFFNESS_LIMMIT)
	{
		return false;
	}
	else
	{
		m_stiffnessTimer += g_gameTime->GetFrameDeltaTime();
	}
	
	return true;
}
