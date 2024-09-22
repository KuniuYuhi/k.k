#include "stdafx.h"
#include "PlayerController.h"


namespace {
	EnButton NORMAL_ATTACK_BUTTON = enButtonA;		//�ʏ�U��
	EnButton SKILL_BUTTON = enButtonX;		//�X�L���U��
	EnButton NORMAL_ATTACK_BUTTON_2 = enButtonB;		//�ʏ�U��
	EnButton SKILL_BUTTON_2 = enButtonY;		//�X�L���U��



	EnButton RIGHT_CHANGE_WEAPON_BUTTON = enButtonRB2;		//�E����؂�ւ��{�^��
	EnButton LEFT_CHANGE_WEAPON_BUTTON = enButtonLB2;		//������؂�ւ��{�^��

	EnButton DEFENCIVEACTTION_BUTTON_RIGHT = enButtonRB1;	//���
	EnButton DEFENCIVEACTTION_BUTTON_LEFT = enButtonLB1;	//���
}

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
	return g_pad[0]->IsTrigger(NORMAL_ATTACK_BUTTON)|| g_pad[0]->IsTrigger(NORMAL_ATTACK_BUTTON_2);
}

bool PlayerController::IsTriggerSkillAttackButton()
{
	return g_pad[0]->IsTrigger(SKILL_BUTTON)|| g_pad[0]->IsTrigger(SKILL_BUTTON_2);
}

bool PlayerController::IsPressSkillAttackButton()
{
	return g_pad[0]->IsPress(SKILL_BUTTON);
}

bool PlayerController::IsTriggerDefensiveActionButton()
{
	return g_pad[0]->IsTrigger(DEFENCIVEACTTION_BUTTON_RIGHT)|| g_pad[0]->IsTrigger(DEFENCIVEACTTION_BUTTON_LEFT);
}

bool PlayerController::IsPressDefensiveActionButton()
{
	return g_pad[0]->IsPress(DEFENCIVEACTTION_BUTTON_RIGHT) || g_pad[0]->IsPress(DEFENCIVEACTTION_BUTTON_LEFT);
}

bool PlayerController::IsTriggerRightChangeWeaponButton()
{
	return g_pad[0]->IsTrigger(RIGHT_CHANGE_WEAPON_BUTTON);
}

bool PlayerController::IsTriggerLeftChangeWeaponButton()
{
	return g_pad[0]->IsTrigger(LEFT_CHANGE_WEAPON_BUTTON);
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
