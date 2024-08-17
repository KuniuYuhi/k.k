#include "stdafx.h"
#include "PlayerController.h"

PlayerController::PlayerController()
{
	Start();
}

PlayerController::~PlayerController()
{
}

void PlayerController::Start()
{
}

void PlayerController::UpdateComponent()
{
}

bool PlayerController::IsTriggerNromalAttackButton()
{
	return g_pad[0]->IsTrigger(enButtonA);
}

bool PlayerController::IsTriggerSkillAttackButton()
{
	return g_pad[0]->IsTrigger(enButtonB);
}

bool PlayerController::IsPressSkillAttackButton()
{
	return g_pad[0]->IsPress(enButtonB);
}

bool PlayerController::IsTriggerDefensiveActionButton()
{
	return g_pad[0]->IsTrigger(enButtonY);
}

bool PlayerController::IsPressDefensiveActionButton()
{
	return g_pad[0]->IsPress(enButtonY);
}

bool PlayerController::IsButtonTrigger(EnButton button)
{
	return g_pad[0]->IsTrigger(button);
}

bool PlayerController::IsButtonPress(EnButton button)
{
	return g_pad[0]->IsPress(button);
}

Vector3 PlayerController::GetLStickInput()
{
	//���X�e�B�b�N
	Vector3 LStick = Vector3::Zero;
	//X���̓��͗ʂ��擾
	LStick.x = g_pad[0]->GetLStickXF();
	//Y���̓��͗ʂ��擾
	LStick.y = g_pad[0]->GetLStickYF();

	return LStick;
}

Vector3 PlayerController::GetRStickInput()
{
	//�E�X�e�B�b�N
	Vector3 RStick = Vector3::Zero;
	//X���̓��͗ʂ��擾
	RStick.x = g_pad[0]->GetRStickXF();
	//Y���̓��͗ʂ��擾
	RStick.y = g_pad[0]->GetRStickYF();

	return RStick;
}
