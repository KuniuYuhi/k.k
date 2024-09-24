#pragma once
#include "GameUIBase.h"
#include "WeaponInfo.h"


class WeaponUIBase;
class Brave;


using namespace WeaponType;
using namespace WeaponUIPoint;

class PlayerGameUI:public GameUIBase
{
private:

	enum EnSpriteName
	{
		enSpriteName_HPBar_Front,		//HPバー表
		enSpriteName_HPBar_Back,		//HPバー裏
		enSpriteName_HPBar_White,		//HPが減った時に出てくる白いHPバー
		enSpriteName_HPBar_Frame,		//HPバーのフレーム

		enSpriteName_StaminaBar_Front,	//スタミナバー表
		enSpriteName_StaminaBar_Back,	//スタミナバー裏
		enSpriteName_StaminaBar_Frame,	//スタミナバーのフレーム

		enSpriteName_MainWeaponFlame,
		enSpriteName_SubWeaponFlame,
		enSpriteName_SubWeapon2Flame,

		enSpriteName_AttackBuffIcon,

		enSprite_Num
	};


	enum EnFontName
	{
		enFontName_HP,
		enFontName_HP_Value,

		enFontName_StaminaInsufficient,

		enFontName_AttackIconAmount,

		enFontName_Num
	};



public:

	PlayerGameUI() {}

	~PlayerGameUI();

	bool Start() override;

	void UIUpdate() override;

	void Draw(RenderContext& rc) override;

	void DeleteSelf() override;

private:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 武器UIの初期化
	/// </summary>
	void WeaponUIInit();

	/// <summary>
	/// スプライトを初期化
	/// </summary>
	void InitSpriteRenders();

	/// <summary>
	/// フォントを初期化
	/// </summary>
	void InitFontRenders();


	/// <summary>
	/// プレイヤーのステータスのUIを更新
	/// </summary>
	void UpdatePlayerStatusUI();

	/// <summary>
	/// HPに関する更新処理
	/// </summary>
	void UpdatePlayerHP();
	/// <summary>
	/// スタミナに関する更新処理
	/// </summary>
	void UpdatePlayerStamina();


	/// <summary>
	/// 武器に関するUIの更新
	/// </summary>
	void UpdateWeaponUI();

	/// <summary>
	/// バフの更新
	/// </summary>
	void UpdateHaveBuffs();


private:

	Brave* m_player = nullptr;


	SpriteRender m_spriteList[enSprite_Num];


	FontRender m_fontList[enFontName_Num];

	
	std::map<WeaponType::EnWeaponType, WeaponUIBase*> m_weaponUIMap;		//武器UIマップ

	int m_whiteHP = 0;

	bool m_isVeiwStaminaGauge = false;



	const float m_staminaInsufficientViewTimeLimit = 2.0f;
	float m_staminaInsufficientViewTimer = 0.0f;
	bool m_isStaminaInsufficient = false;


	bool m_isViewBuffSpritsAndFonts = false;

};

