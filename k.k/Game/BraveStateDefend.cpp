#include "stdafx.h"
#include "BraveStateDefend.h"
#include "Brave.h"

namespace {
	const int DEFENCE = 0;
	const int AVOID = 1;
}

void BraveStateDefend::ManageState()
{
	//����ɂ���ĈႤ����
	switch (m_brave->GetMainWeaponDefendTipe())
	{
	case DEFENCE:
		ProcessDefence();
		break;
	case AVOID:
		ProcessAvoid();
		break;
	default:
		break;
	}
}

void BraveStateDefend::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		Brave::enAnimClip_Defend + m_brave->GetCurrentMainWeaponAnimationStartIndexNo()
	);

}

void BraveStateDefend::ProcessDefence()
{
	//Y�{�^���𗣂�����h���ԉ���
	if (g_pad[0]->IsPress(enButtonY) != true)
	{
		//�h���Ԃ��������ꂽ�̂ŃA�N�V�����t���O��false�ɂ���
		m_brave->SetAllInfoAboutActionFlag(false);
		m_brave->ProcessCommonStateTransition();
	}
}

void BraveStateDefend::ProcessAvoid()
{
	//�O�i����t���O�������Ă���Ԃ͏���
	if (m_brave->GetMoveForwardFlag() == true)
	{
		//�����Ă�������Ɉړ�
		m_brave->MoveForward(m_brave->GetAvoidSpeed());
	}

	m_brave->ProcessDefendStateTransition();
}
