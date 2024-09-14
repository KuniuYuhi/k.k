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

	//�h�����ݒ�
	SetShakeInfo(SHAKE_STEWNGTH, SHAKE_VIBRATO, SHEIELD_ENDURANCE_POS);

	return true;
}

void SwordShieldUI::UIUpdate()
{
}

void SwordShieldUI::MainWeaponUpdate()
{
	m_isViewEndrance = true;

	//�ϋv�l���v�Z
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
	//����A�C�R���̏�����
	InitSpriteRender(
		m_spriteList[enSprite_WeaponIcon],
		"Assets/sprite/InGame/Character/SwordShield.DDS", 256, 256, g_vec2Zero);

	m_endranceCurrentPosition = SHEIELD_ENDURANCE_POS;
	//�V�[���h�A�C�R���̏�����
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

	//�ϋv�l���ϓ�������
	if (endrance != m_oldEndranceValue)
	{
		//�^�C�}�[���Z�b�g
		m_shakeTimer = 0.0f;
		m_isVariableEndrance = true;
	}

	//�_���[�W�̌������擾����
	int digitCount = static_cast<int>(log10(endrance) + 1);
	Vector2 finalOffset = g_vec2Zero;
	//Y�I�t�Z�b�g
	finalOffset.y = DEFAULT_X_OFFSET * ENDRANCE_FONT_SCALE;
	//X�I�t�Z�b�g
	finalOffset.x = digitCount * DEFAULT_Y_OFFSET * ENDRANCE_FONT_SCALE;


	wchar_t text[255];
	swprintf_s(text, 255, L"%d", endrance);


	m_fontList[enFontName_Endurance].SetOffset(finalOffset);
	m_fontList[enFontName_Endurance].SetText(text);

	//�ϋv�l���ϓ�������
	if (m_isVariableEndrance)
	{
		//�c��h�炷
		ShakeShield();
	}


	//�O�t���[���̑ϋv�l��ۑ�
	m_oldEndranceValue = endrance;
}

void SwordShieldUI::ShakeShield()
{
	//�h�炷���W���v�Z
	m_endranceCurrentPosition = GetUpdateShakePosition(m_shakeTimer, SHAKE_TIME_LIMIT, m_endranceCurrentPosition);

	//�^�C�}�[�̌v�Z
	if (m_shakeTimer > SHAKE_TIME_LIMIT)
	{
		//�^�C�����~�b�g�ɒB������
		m_isVariableEndrance = false;
		//�ʒu�������ʒu�ɖ߂�
		m_endranceCurrentPosition = SHEIELD_ENDURANCE_POS;
		//�^�C�}�[���Z�b�g
		m_shakeTimer = 0.0f;
	}
	else
	{
		m_shakeTimer += g_gameTime->GetFrameDeltaTime();
	}
	

	m_spriteList[enSpriteName_Shield].SetPosition(m_endranceCurrentPosition);
	m_spriteList[enSpriteName_Shield].Update();

}


