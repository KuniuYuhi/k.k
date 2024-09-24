#include "stdafx.h"
#include "SwordShieldUI.h"

#include "WeaponManager.h"

#include "SwordShield.h"



namespace {
	const Vector2 SHEIELD_ENDURANCE_POS = { 650.0f,-450.0f };
	const Vector2 ENDURANCE_FONT_POS = { 676.0f,-450.0f };

	const float ENDRANCE_FONT_SCALE = 1.5f;

	const float DEFAULT_Y_OFFSET = 18.0f;
	const float DEFAULT_X_OFFSET = -25.0f;



	const int SHAKE_STEWNGTH = 30;
	const float SHAKE_VIBRATO = 15.0f;
	const float SHAKE_TIME_LIMIT = 0.7f;

}

bool SwordShieldUI::Start()
{


	m_swordShield = FindGO<SwordShield>("SwordShield");

	m_oldEndranceValue = m_swordShield->GetShieldEndrance();

	Init();

	//揺れ情報を設定
	SetShakeInfo(SHAKE_STEWNGTH, SHAKE_VIBRATO, SHEIELD_ENDURANCE_POS);

	return true;
}

void SwordShieldUI::UIUpdate()
{
}

void SwordShieldUI::MainWeaponUpdate()
{
	m_isViewEndrance = true;

	//耐久値を計算
	UpdateShieldEndrance();


	m_spriteList[enSprite_WeaponIcon].SetPosition(MAIN_WEAPON_CENTER_POS);
	m_spriteList[enSprite_WeaponIcon].SetScale(MAIN_WEAPON_SCALE);


	m_spriteList[enSprite_WeaponIcon].Update();
}

void SwordShieldUI::SubWeaponUpdate()
{
	m_isViewEndrance = false;

	m_spriteList[enSprite_WeaponIcon].SetPosition(SUB_WEAPON_CENTER_POS);
	m_spriteList[enSprite_WeaponIcon].SetScale(SUB_WEAPON_SCALE);


	m_spriteList[enSprite_WeaponIcon].Update();
}

void SwordShieldUI::SubWeapon2Update()
{
	m_isViewEndrance = false;

	m_spriteList[enSprite_WeaponIcon].SetPosition(SUB_WEAPON2_COENTER_POS);
	m_spriteList[enSprite_WeaponIcon].SetScale(SUB_WEAPON_SCALE);



	m_spriteList[enSprite_WeaponIcon].Update();
}








void SwordShieldUI::Draw(RenderContext& rc)
{
	m_spriteList[enSprite_WeaponIcon].Draw(rc);


	if (m_isViewEndrance)
	{
		m_spriteList[enSpriteName_Shield].Draw(rc);
		m_fontList[enFontName_Endurance].Draw(rc);
	}


}

void SwordShieldUI::DeleteSelf()
{
	DeleteGO(this);
}

void SwordShieldUI::Init()
{
	InitSpriteRenders();
	InitFontRenders();
}

void SwordShieldUI::InitSpriteRenders()
{
	//武器アイコンの初期化
	InitSpriteRender(
		m_spriteList[enSprite_WeaponIcon],
		"Assets/sprite/InGame/Character/SwordShield.DDS", 256, 256, g_vec2Zero);

	m_endranceCurrentPosition = SHEIELD_ENDURANCE_POS;
	//シールドアイコンの初期化
	InitSpriteRender(
		m_spriteList[enSpriteName_Shield],
		"Assets/sprite/InGame/Character/Shield_Endurance.DDS", 164, 162, m_endranceCurrentPosition);




}

void SwordShieldUI::InitFontRenders()
{
	//
	InitFontRender(
		m_fontList[enFontName_Endurance],
		ENDURANCE_FONT_POS,
		ENDRANCE_FONT_SCALE
	);

}

void SwordShieldUI::UpdateShieldEndrance()
{
	int endrance = m_swordShield->GetShieldEndrance();

	//耐久値が変動したら
	if (endrance != m_oldEndranceValue)
	{
		//タイマーリセット
		m_shakeTimer = 0.0f;
		m_isVariableEndrance = true;
	}

	//ダメージの桁数を取得する
	int digitCount = static_cast<int>(log10(endrance) + 1);
	Vector2 finalOffset = g_vec2Zero;
	//Yオフセット
	finalOffset.y = DEFAULT_X_OFFSET * ENDRANCE_FONT_SCALE;
	//Xオフセット
	finalOffset.x = digitCount * DEFAULT_Y_OFFSET * ENDRANCE_FONT_SCALE;


	wchar_t text[255];
	swprintf_s(text, 255, L"%d", endrance);


	m_fontList[enFontName_Endurance].SetOffset(finalOffset);
	m_fontList[enFontName_Endurance].SetText(text);

	//耐久値が変動したら
	if (m_isVariableEndrance)
	{
		//縦を揺らす
		ShakeShield();
	}


	//前フレームの耐久値を保存
	m_oldEndranceValue = endrance;
}

void SwordShieldUI::ShakeShield()
{
	//揺らす座標を計算
	m_endranceCurrentPosition = GetUpdateShakePosition(m_shakeTimer, SHAKE_TIME_LIMIT, m_endranceCurrentPosition);

	//タイマーの計算
	if (m_shakeTimer > SHAKE_TIME_LIMIT)
	{
		//タイムリミットに達したら
		m_isVariableEndrance = false;
		//位置を初期位置に戻す
		m_endranceCurrentPosition = SHEIELD_ENDURANCE_POS;
		//タイマーリセット
		m_shakeTimer = 0.0f;
	}
	else
	{
		m_shakeTimer += g_gameTime->GetFrameDeltaTime();
	}
	

	m_spriteList[enSpriteName_Shield].SetPosition(m_endranceCurrentPosition);
	m_spriteList[enSpriteName_Shield].Update();

}


