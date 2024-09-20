#include "stdafx.h"
#include "PlayerController.h"


namespace {
	EnButton NORMAL_ATTACK_BUTTON = enButtonA;		//通常攻撃
	EnButton SKILL_BUTTON = enButtonX;		//スキル攻撃
	EnButton DEFENCIVEACTTION_BUTTON = enButtonB;	//回避

	EnButton RIGHT_CHANGE_WEAPON_BUTTON = enButtonRB2;		//右武器切り替えボタン
	EnButton LEFT_CHANGE_WEAPON_BUTTON = enButtonLB2;		//左武器切り替えボタン

	EnButton DEFENCIVEACTTION_BUTTON_RIGHT = enButtonRB1;	//回避
	EnButton DEFENCIVEACTTION_BUTTON_LEFT = enButtonLB1;	//回避
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
	return g_pad[0]->IsTrigger(NORMAL_ATTACK_BUTTON);
}

bool PlayerController::IsTriggerSkillAttackButton()
{
	return g_pad[0]->IsTrigger(SKILL_BUTTON);
}

bool PlayerController::IsPressSkillAttackButton()
{
	return g_pad[0]->IsPress(SKILL_BUTTON);
}

bool PlayerController::IsTriggerDefensiveActionButton()
{
	return g_pad[0]->IsTrigger(DEFENCIVEACTTION_BUTTON_RIGHT)|| g_pad[0]->IsTrigger(DEFENCIVEACTTION_BUTTON_LEFT);


	return g_pad[0]->IsTrigger(DEFENCIVEACTTION_BUTTON);
}

bool PlayerController::IsPressDefensiveActionButton()
{
	return g_pad[0]->IsPress(DEFENCIVEACTTION_BUTTON_RIGHT) || g_pad[0]->IsPress(DEFENCIVEACTTION_BUTTON_LEFT);


	return g_pad[0]->IsPress(DEFENCIVEACTTION_BUTTON);
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
