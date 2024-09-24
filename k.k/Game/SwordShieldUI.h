#pragma once
#include "WeaponUIBase.h"
#include "WeaponInfo.h"

class SwordShield;



using namespace WeaponType;
using namespace WeaponUIPoint;

class SwordShieldUI : public WeaponUIBase
{
private:

	enum EnSpriteName
	{
		enSprite_WeaponIcon,
		enSpriteName_Shield,

		enSprite_Num
	};

	enum EnFontName
	{
		enFontName_Endurance,
		enFontName_Num
	};

public:
	SwordShieldUI() {}
	~SwordShieldUI() {}

	bool Start() override;

	void UIUpdate() override;

	void MainWeaponUpdate() override;

	void SubWeaponUpdate() override;

	void SubWeapon2Update() override;

	void Draw(RenderContext& rc) override;

	void DeleteSelf() override;

private:

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// スプライトを初期化
	/// </summary>
	void InitSpriteRenders();

	/// <summary>
	/// フォントを初期化
	/// </summary>
	void InitFontRenders();

	/// <summary>
	/// 盾の耐久値の更新
	/// </summary>
	void UpdateShieldEndrance();


	/// <summary>
	/// 
	/// </summary>
	void ShakeShield();


	

private:

	SpriteRender m_spriteList[enSprite_Num];


	FontRender m_fontList[enFontName_Num];


	const EnWeaponType m_weaponType = enSwordShield;


	SwordShield* m_swordShield = nullptr;

	Vector2 m_endranceCurrentPosition = g_vec2Zero;



	bool m_isViewEndrance = false;

	int m_oldEndranceValue = 0;
	bool m_isVariableEndrance = false;

	float m_shakeTimer = 0.0f;

};

