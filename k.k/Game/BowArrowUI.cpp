#include "stdafx.h"
#include "BowArrowUI.h"



bool BowArrowUI::Start()
{
	Init();

	return true;
}

void BowArrowUI::UIUpdate()
{
}

void BowArrowUI::MainWeaponUpdate()
{
	m_spriteList[enSprite_WeaponIcon].SetPosition(MAIN_WEAPON_CENTER_POS);
	m_spriteList[enSprite_WeaponIcon].SetScale(MAIN_WEAPON_SCALE);


	m_spriteList[enSprite_WeaponIcon].Update();
}

void BowArrowUI::SubWeaponUpdate()
{
	m_spriteList[enSprite_WeaponIcon].SetPosition(SUB_WEAPON_CENTER_POS);
	m_spriteList[enSprite_WeaponIcon].SetScale(SUB_WEAPON_SCALE);


	m_spriteList[enSprite_WeaponIcon].Update();
}

void BowArrowUI::SubWeapon2Update()
{
	m_spriteList[enSprite_WeaponIcon].SetPosition(SUB_WEAPON2_COENTER_POS);
	m_spriteList[enSprite_WeaponIcon].SetScale(SUB_WEAPON_SCALE);



	m_spriteList[enSprite_WeaponIcon].Update();
}







void BowArrowUI::Draw(RenderContext& rc)
{
	m_spriteList[enSprite_WeaponIcon].Draw(rc);
}

void BowArrowUI::DeleteSelf()
{
	DeleteGO(this);
}

void BowArrowUI::Init()
{
	InitSpriteRenders();
	InitFontRenders();

}

void BowArrowUI::InitSpriteRenders()
{
	//ïêäÌÉAÉCÉRÉìÇÃèâä˙âª
	InitSpriteRender(
		m_spriteList[enSprite_WeaponIcon],
		"Assets/sprite/InGame/Character/BowArrow.DDS", 290, 290, g_vec2Zero);



}

void BowArrowUI::InitFontRenders()
{
}
