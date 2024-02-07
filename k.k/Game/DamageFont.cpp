#include "stdafx.h"
#include "DamageFont.h"

namespace {
	const float SCALE_PLAYER = 1.2f;
	const float SCALE_MONSTER = 1.2f;
	const float SCALE_BOSS = 1.5f;
	const float Y_UP_PLAYER =50.0f;
	const float Y_UP_MONSTER = 50.0f;
	const float Y_UP_BOSS = 170.0f;

	const float BACK_SPRITE_SCALE_PLAYER = 1.2f;
	const float BACK_SPRITE_SCALE_MOBMONSTER = 1.2f;
	const float BACK_SPRITE_SCALE_BOSS = 1.5f;

	const float DEFAULT_Y_OFFSET = 18.0f;
	const float DEFAULT_X_OFFSET = -25.0f;

	const int RAMDOM_RANGE = 50.0f;
}

DamageFont::DamageFont()
{
}

DamageFont::~DamageFont()
{
}

bool DamageFont::Start()
{
	InitFont();
	//todo �^����_���[�W�̕����T�C�Y�ɂ���ăI�t�Z�b�g
		

	return true;
}

void DamageFont::Update()
{
	//�����ɂȂ������������
	if (m_color.w <= 0.0f)
	{
		DeleteGO(this);
	}

	//�����ɂ��Ă���
	TransparencyFont();
	//�ړ�����
	Move();
}

void DamageFont::InitFont()
{
	//�w�i�̉摜�̏�����
	m_damageBackRender.Init(
		"Assets/sprite/InGame/DamageEffect/Damage.DDS", 150, 150
	);

	//�_���[�W�t�H���g�̃I�t�Z�b�g���v�Z
	Vector2 fontOffset = g_vec2Zero;


	//�_���[�W���󂯂�A�N�^�[�ɂ���Đݒ�
	switch (m_damageActor)
	{
	case DamageFont::enDamageActor_Player:
		//�_���[�W�t�H���g�̃I�t�Z�b�g���v�Z
		fontOffset = CalcDamageFontOffset(SCALE_PLAYER);
		//�_���[�W�t�H���g�Ɣw�i�̐ݒ�
		SetDamageSpriteAndFont(
			g_vec4Red, SCALE_PLAYER, fontOffset,
			Y_UP_PLAYER, BACK_SPRITE_SCALE_PLAYER
		);
		break;
	case DamageFont::enDamageActor_Monster:
		//�_���[�W�t�H���g�̃I�t�Z�b�g���v�Z
		fontOffset = CalcDamageFontOffset(SCALE_MONSTER);
		//�_���[�W�t�H���g�Ɣw�i�̐ݒ�
		SetDamageSpriteAndFont(
			g_vec4White, SCALE_MONSTER, fontOffset,
			Y_UP_MONSTER, BACK_SPRITE_SCALE_MOBMONSTER
		);
		break;
	case DamageFont::enDamageActor_Boss:
		//�_���[�W�t�H���g�̃I�t�Z�b�g���v�Z
		fontOffset = CalcDamageFontOffset(SCALE_BOSS);
		//�_���[�W�t�H���g�Ɣw�i�̐ݒ�
		SetDamageSpriteAndFont(
			g_vec4White, SCALE_BOSS, fontOffset,
			Y_UP_MONSTER, BACK_SPRITE_SCALE_BOSS
		);
		//���W�������_���ɂ���
		m_largeRandomPositionFlag = true;
		break;
	default:
		std::abort();
		break;
	}

	//�_���[�W�t�H���g�𐶐�������W���v�Z
	m_damagePosition = CalcDamagePosition();
	
	wchar_t damage[255];
	swprintf_s(damage, 255, L"%d", m_damage);
	//�_���[�W�ʂ�ݒ�
	m_damageFont.SetText(damage);
	//�e������
	m_damageFont.SetShadowParam(true, 2.0f, g_vec4Black);
	//���W��ݒ�
	m_damageFont.SetPosition(m_damagePosition);

	//�_���[�W�t�H���g�̔w�i�̍��W��ݒ�
	m_damageBackPosition.x = m_damagePosition.x;
	m_damageBackPosition.y = m_damagePosition.y;
	m_damageBackRender.SetPosition(m_damageBackPosition);
	//�X�V
	m_damageBackRender.Update();
}

void DamageFont::Move()
{
	//�㏸������
	m_damagePosition.y += g_gameTime->GetFrameDeltaTime() * 60.0f;
	//�w�i��Y���W���t�H���g�ƍ��킹��
	m_damageBackPosition.y = m_damagePosition.y;
	//�t�H���g�̍��W��ݒ�
	m_damageFont.SetPosition(m_damagePosition);
	//�w�i�̍��W��ݒ�ƍX�V
	m_damageBackRender.SetPosition(m_damageBackPosition);
	m_damageBackRender.Update();
}

void DamageFont::TransparencyFont()
{
	//�F�𓧖��ɂ��Ă���
	m_color.w -= g_gameTime->GetFrameDeltaTime() * 2.2f;
	m_backColor.w = m_color.w;
	//�t�H���g�̐F�̐ݒ�
	m_damageFont.SetColor(m_color);
	//�w�i�̐F�̐ݒ�ƍX�V
	m_damageBackRender.SetMulColor(m_backColor);
	m_damageBackRender.Update();
}

void DamageFont::SetDamageSpriteAndFont(
	Vector4 fontColor, 
	float fontScale, Vector2 fontOffset, float yUp, 
	float backSpriteScale)
{
	//�����̐F��ݒ�
	m_color = fontColor;
	SetDamageFontInfo(m_color, fontScale, fontOffset);
	//Y���W�������グ��
	m_yUp = yUp;
	//�摜���g��
	m_damageBackRender.SetScale(g_vec3One * backSpriteScale);
}

void DamageFont::SetDamageFontInfo(Vector4 color, float scale, Vector2 offset)
{
	m_damageFont.SetColor(color);
	m_damageFont.SetScale(scale);
	m_damageFont.SetOffset(offset);
}

Vector2 DamageFont::CalcDamagePosition()
{
	//�_���[�W���󂯂�L�����N�^�[�̍��W��Y���W�������グ��
	Vector3 DamagePosition = m_position;
	DamagePosition.y += m_yUp;

	Vector2 finalPos = g_vec2Zero;

	//���W�����[���h���W����X�N���[�����W�ɕϊ�
	g_camera3D->CalcScreenPositionFromWorldPosition(finalPos, DamagePosition);
	//�L���͈͂ō��W�������_���ȏꏊ�ɂ������Ȃ�
	if (m_largeRandomPositionFlag == true)
	{
		finalPos.x += (rand() % (RAMDOM_RANGE*2 - (-RAMDOM_RANGE*2) + 1)) + (-RAMDOM_RANGE*2);
		finalPos.y += (rand() % (RAMDOM_RANGE*2 - (-RAMDOM_RANGE*2) + 1)) + (-RAMDOM_RANGE*2);
	}
	else
	{
		finalPos.x += (rand() % (RAMDOM_RANGE - (-RAMDOM_RANGE) + 1)) + (-RAMDOM_RANGE);
		finalPos.y += (rand() % (RAMDOM_RANGE - (-RAMDOM_RANGE) + 1)) + (-RAMDOM_RANGE);
	}

	return finalPos;
}

Vector2 DamageFont::CalcDamageFontOffset(float fontScale)
{
	//�_���[�W�̌������擾����
	int digitCount = static_cast<int>(log10(m_damage) + 1);
	Vector2 finalOffset = g_vec2Zero;
	//Y�I�t�Z�b�g
	finalOffset.y = DEFAULT_X_OFFSET * fontScale;
	//X�I�t�Z�b�g
	finalOffset.x = digitCount * DEFAULT_Y_OFFSET * fontScale;

	return finalOffset;
}

void DamageFont::Render(RenderContext& rc)
{
	m_damageBackRender.Draw(rc);

	m_damageFont.Draw(rc);
}
