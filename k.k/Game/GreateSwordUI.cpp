#include "stdafx.h"
#include "GreateSwordUI.h"



bool GreateSwordUI::Start()
{
	Init();

	return true;
}

void GreateSwordUI::UIUpdate()
{
}

void GreateSwordUI::MainWeaponUpdate()
{
	m_spriteList[enSprite_WeaponIcon].SetPosition(MAIN_WEAPON_CENTER_POS);
	m_spriteList[enSprite_WeaponIcon].SetScale(MAIN_WEAPON_SCALE);


	m_spriteList[enSprite_WeaponIcon].Update();
}

void GreateSwordUI::SubWeaponUpdate()
{
	m_spriteList[enSprite_WeaponIcon].SetPosition(SUB_WEAPON_CENTER_POS);
	m_spriteList[enSprite_WeaponIcon].SetScale(SUB_WEAPON_SCALE);


	m_spriteList[enSprite_WeaponIcon].Update();
}

void GreateSwordUI::SubWeapon2Update()
{
	m_spriteList[enSprite_WeaponIcon].SetPosition(SUB_WEAPON2_COENTER_POS);
	m_spriteList[enSprite_WeaponIcon].SetScale(SUB_WEAPON_SCALE);



	m_spriteList[enSprite_WeaponIcon].Update();
}







void GreateSwordUI::Draw(RenderContext& rc)
{
	m_spriteList[enSprite_WeaponIcon].Draw(rc);
}

void GreateSwordUI::DeleteSelf()
{
	DeleteGO(this);
}

void GreateSwordUI::Init()
{
	InitSpriteRenders();
	InitFontRenders();
}

void GreateSwordUI::InitSpriteRenders()
{
	//ïêäÌÉAÉCÉRÉìÇÃèâä˙âª
	InitSpriteRender(
		m_spriteList[enSprite_WeaponIcon],
		"Assets/sprite/InGame/Character/GreatSword.DDS", 300, 300, g_vec2Zero);


}

void GreateSwordUI::InitFontRenders()
{
}
