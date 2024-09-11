#pragma once
#include "GameUIBase.h"


class Summoner;

/// <summary>
/// 
/// </summary>
class SummonerUI : public GameUIBase
{
private:

	enum EnSpriteName
	{
		enSpriteName_HPBar_Front,		//HP�o�[�\
		enSpriteName_HPBar_Back,		//HP�o�[��
		enSpriteName_HPBar_White,		//HP�����������ɏo�Ă��锒��HP�o�[
		enSpriteName_HPBar_Frame,		//HP�o�[�̃t���[��



		enSprite_Num
	};


	enum EnFontName
	{
		enFontName_HP,
		enFontName_HP_Value,

		enFontName_StaminaInsufficient,

		enFontName_Num
	};


public:

	SummonerUI() {}
	~SummonerUI() {}

	bool Start() override;

	void UIUpdate() override;

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


	/// <summary>
	/// �v���C���[�̃X�e�[�^�X��UI���X�V
	/// </summary>
	void UpdateStatusUI();

	/// <summary>
	/// HP�Ɋւ���X�V����
	/// </summary>
	void UpdateSummonerHP();


	void UpdateMainHPBar(int currentHp,int maxHp);

	void UpdateWhiteHPBar(int currentHp, int maxHp);

	/// <summary>
	/// 
	/// </summary>
	void ShakeHPBar();


	



private:
	Summoner* m_summmoner = nullptr;

	SpriteRender m_spriteList[enSprite_Num];


	FontRender m_fontList[enFontName_Num];

	Vector2 m_currentShakeCenterPosition = g_vec2Zero;
	Vector2 m_currentShakeGaugePosition = g_vec2Zero;

	int m_oldHpValue = 0;
	bool m_isVariableHp = false;
	float m_shakeTimer = 0.0f;

	int m_whiteHP = 0;

};

