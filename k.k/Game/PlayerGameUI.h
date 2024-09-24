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
		enSpriteName_HPBar_Front,		//HP�o�[�\
		enSpriteName_HPBar_Back,		//HP�o�[��
		enSpriteName_HPBar_White,		//HP�����������ɏo�Ă��锒��HP�o�[
		enSpriteName_HPBar_Frame,		//HP�o�[�̃t���[��

		enSpriteName_StaminaBar_Front,	//�X�^�~�i�o�[�\
		enSpriteName_StaminaBar_Back,	//�X�^�~�i�o�[��
		enSpriteName_StaminaBar_Frame,	//�X�^�~�i�o�[�̃t���[��

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
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// ����UI�̏�����
	/// </summary>
	void WeaponUIInit();

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
	void UpdatePlayerStatusUI();

	/// <summary>
	/// HP�Ɋւ���X�V����
	/// </summary>
	void UpdatePlayerHP();
	/// <summary>
	/// �X�^�~�i�Ɋւ���X�V����
	/// </summary>
	void UpdatePlayerStamina();


	/// <summary>
	/// ����Ɋւ���UI�̍X�V
	/// </summary>
	void UpdateWeaponUI();

	/// <summary>
	/// �o�t�̍X�V
	/// </summary>
	void UpdateHaveBuffs();


private:

	Brave* m_player = nullptr;


	SpriteRender m_spriteList[enSprite_Num];


	FontRender m_fontList[enFontName_Num];

	
	std::map<WeaponType::EnWeaponType, WeaponUIBase*> m_weaponUIMap;		//����UI�}�b�v

	int m_whiteHP = 0;

	bool m_isVeiwStaminaGauge = false;



	const float m_staminaInsufficientViewTimeLimit = 2.0f;
	float m_staminaInsufficientViewTimer = 0.0f;
	bool m_isStaminaInsufficient = false;


	bool m_isViewBuffSpritsAndFonts = false;

};

