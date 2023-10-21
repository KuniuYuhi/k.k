#include "stdafx.h"
#include "BraveStateDefend.h"
#include "Brave.h"

void BraveStateDefend::ManageState()
{
	//Y�{�^���𗣂�����h���ԉ���
	if (g_pad[0]->IsPress(enButtonY) != true)
	{
		//�h���Ԃ��������ꂽ�̂ŃA�N�V�����t���O��false�ɂ���
		m_brave->SetIsActionFlag(false);
		m_brave->ProcessCommonStateTransition();
	}
}

void BraveStateDefend::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		Brave::enAnimClip_Defend + m_brave->GetCurrentMainWeaponAnimationStartIndexNo(),
		0.2f
	);

}
