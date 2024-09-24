#pragma once
#include "WeaponUIBase.h"
#include "WeaponInfo.h"

using namespace WeaponUIPoint;

class BowArrowUI : public WeaponUIBase
{
private:

	enum EnSpriteName
	{
		enSprite_WeaponIcon,


		enSprite_Num
	};

	enum EnFontName
	{
		enFontName_a,
		enFontName_Num
	};

public:
	BowArrowUI() {}
	~BowArrowUI() {}

	bool Start() override;

	void UIUpdate() override;

	void MainWeaponUpdate() override;

	void SubWeaponUpdate() override;

	void SubWeapon2Update() override;

	void Draw(RenderContext& rc) override;

	void DeleteSelf() override;

private:

	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �X�v���C�g��������
	/// </summary>
	void InitSpriteRenders();

	/// <summary>
	/// �t�H���g��������
	/// </summary>
	void InitFontRenders();





private:

	SpriteRender m_spriteList[enSprite_Num];


	FontRender m_fontList[enFontName_Num];


};

