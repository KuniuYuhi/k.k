#include "stdafx.h"
#include "BraveState_Die.h"
#include "Brave.h"
#include "GameSceneManager.h"

void BraveState_Die::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		enBraveAnimClip_Die + m_brave->GetCurrentAnimationStartIndexNo(), 0.2f
	);
}

void BraveState_Die::Entry()
{
	m_brave->ActionActive();
	//�A�j���[�V�������x�𗎂Ƃ�
	m_brave->GetModelRender().SetAnimationSpeed(0.5f);
	//�v���C���[�̔s�k��m�点��
	GameSceneManager::GetInstance()->SetOutComPlayerLose();
}

void BraveState_Die::Ubdate()
{

}

void BraveState_Die::Exit()
{
}
