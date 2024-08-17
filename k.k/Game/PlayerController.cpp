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
	//左スティック
	Vector3 LStick = Vector3::Zero;
	//X軸の入力量を取得
	LStick.x = g_pad[0]->GetLStickXF();
	//Y軸の入力量を取得
	LStick.y = g_pad[0]->GetLStickYF();

	return LStick;
}

Vector3 PlayerController::GetRStickInput()
{
	//右スティック
	Vector3 RStick = Vector3::Zero;
	//X軸の入力量を取得
	RStick.x = g_pad[0]->GetRStickXF();
	//Y軸の入力量を取得
	RStick.y = g_pad[0]->GetRStickYF();

	return RStick;
}
