#include "stdafx.h"
#include "GameUI.h"
#include "Game.h"
#include "Player.h"
#include "Lich.h"

//todo MP�������o�[�����H

namespace {

	const Vector2 TIMER_POS = { -34.0f,522.0f };
	const Vector2 TIMER_OFFSET = { 50.0f,-20.0f };

	/// <summary>
	/// 978
	/// </summary>
	const Vector3 BOSS_ICON_POS = { -582.0f,450.0f,0.0f };

	const Vector3 BOSS_HP_FLAME_POS = { 0.0f,450.0f,0.0f };
	const Vector3 BOSS_HP_FRONT_POS = { -489.0f,450.0f,0.0f };
	const Vector3 BOSS_HP_BACK_POS = { 0.0f,450.0f,0.0f };

	const Vector2 BOSS_HP_FONT_POS = { -200.0f, 486.0f };

	/// <summary>
	/// �v���C���[��
	/// </summary>

	const Vector2 HP_OR_MP_PIBOT = { 0.0f,0.5f };							//HP��MP�̃s�{�b�g

	const Vector3 MAIN_ICON_POS = { -301.0f, -411.0f, 0.0f };
	const Vector3 MAIN_ICON_BASE_POS = { -301.0f, -411.0f, 0.0f };

	const Vector3 CHARA_CHANGE_ICON_POS = { -490.0f,-467.0f,0.0f };
	const Vector3 CHARA_CHANGE_ICON_FONT_POS = { -546.0f,-433.0f,0.0f };

	const Vector3 MAIN_STATUS_BAR = { 36.0f, -432.0f, 0.0f };

	const Vector3 MAIN_HP_FRONT_BAR = { -166.0f, -469.0f, 0.0f };
	const Vector3 MAIN_HP_BACK_BAR = { 101.0f, -468.0f, 0.0f };
	const Vector3 MAIN_MP_FRONT_BAR = { -168.5f, -394.0f, 0.0f };
	const Vector3 MAIN_MP_BACK_BAR = { 73.0f, -394.0f, 0.0f };

	const Vector2 HP_FONT_POS = { -164.0f,-425.0f };
	const Vector2 MP_FONT_POS = { -164.0f, -354.0f };

	//+10+10
	const Vector3 SKILL_CENTER_POS = { 755.0f,-396.0f,0.0f };
	const Vector3 SKILL_1__POS = { 592.0f,-376.0f,0.0f };

	const Vector3 SKILL_1_X_POS = { 674.0f,-376.0f,0.0f };





	const Vector3 TIME_FLAME_POS = { 0.0f,509.0f,0.0f };

	const Vector3 TIME_FLAME_SIZE = { 0.6f, 0.5f, 0.6f };


	const Vector3 HP_SCALE_END_POS = { 0.0f,1.0f,1.0f };

	const float WHITE_HP_LERP_START = 0.6f;
	const float WHITE_HP_LERP_END = 5.0f;
}

GameUI::GameUI()
{
}

GameUI::~GameUI()
{
}

bool GameUI::Start()
{
	//�v���C���[��UI
	InitPlayerUI();
	//�����X�^�[��UI
	InitMonsterUI();

	//��������
	InitFontRender(m_TimerFont, TIMER_POS, 1.1f);
	m_TimerFont.SetOffset(TIMER_OFFSET);

	//�������Ԃ̘g
	InitSpriteRender(
		m_TimeFlameRender, "Assets/sprite/InGame/Character/TimeFlame2.DDS", 500, 124, TIME_FLAME_POS, TIME_FLAME_SIZE
	);

	m_oldMainCharaHP = m_player->GetNowActorStatus().hp;

	return true;
}

void GameUI::Update()
{
	//�v���C���[��UI�̏���
	PlayerUIUpdate();
	//�{�X��UI�̏���
	MonsterUIUpdate();


}

void GameUI::PlayerUIUpdate()
{
	TimerUIUpdate();

	UpdateMainStatus();

	UpdateCharaIcon();
}

void GameUI::UpdateMainStatus()
{
	//HP�̏���
	ProcessPlayerHp();
	//MP�̏���
	ProcessPlayerMp();

	
}

void GameUI::UpdateCharaIcon()
{
	//���ꂽ�L�����̃A�C�R�����O���[�X�P�[���ɂ���
	if (m_player->GetNowActorDieFlag()==true)
	{
		m_playerUI.m_characterIconRender.SetGrayScale(true);
	}
}

Vector3 GameUI::CalcGaugeScale(float Maxvalue, float value)
{
	Vector3 scale = g_vec3One;
	scale.x = value / Maxvalue;
	return scale;
}

void GameUI::MonsterUIUpdate()
{
	//�{�X�����Ȃ��Ȃ珈�����Ȃ�
	if (m_lich == nullptr)
	{
		return;
	}

	//�{�X��HP�̏���
	ProcessBossHP();

	
}

void GameUI::TimerUIUpdate()
{
	//���̎擾
	int minute = m_game->GetMinute();
	//�b�̎擾
	int second = m_game->GetSecond();

	wchar_t time[256];
	swprintf_s(time, 256, L"%d:%02d", minute, second);
	
	m_TimerFont.SetText(time);
}

void GameUI::DrawPlayerUI(RenderContext& rc)
{
	//���C��HP
	m_playerUI.m_hpBackRender.Draw(rc);

	if (m_playerHpWhiteScale.x > 0.0f)
	{
		m_playerUI.m_hpWhiteRender.Draw(rc);
	}

	
	m_playerUI.m_hpFrontRender.Draw(rc);
	//���C��MP
	m_playerUI.m_mpBackRender.Draw(rc);
	m_playerUI.m_mpFrontRender.Draw(rc);
	
	//���C���X�e�[�^�X�o�[
	m_playerUI.m_statusBarRender.Draw(rc);
	//���C���A�C�R���x�[�X
	m_playerUI.m_iconBaseRender.Draw(rc);
	//�X�L���̐^�񒆂́�
	m_playerUI.m_SkillCenterRender.Draw(rc);
	//�X�L���P�̃t���[��
	m_playerUI.m_Skill_1FlameRender.Draw(rc);
	//�X�L���P�̓����̃t���[��
	m_playerUI.m_Skill_1FlameInsideRender.Draw(rc);

	//���C���A�C�R��
	m_playerUI.m_characterIconRender.Draw(rc);
	//X�{�^��
	m_playerUI.m_SkillButtonXRender.Draw(rc);

	//HP��MP�̃t�H���g
	m_playerUI.m_hpFont.Draw(rc);
	m_playerUI.m_mpFont.Draw(rc);
}

void GameUI::DrawMonsterUI(RenderContext& rc)
{
	if (m_lich == nullptr)
	{
		return;
	}
	//�A�C�R��
	m_monsterUI.m_IconRender.Draw(rc);

	//HP
	

	m_monsterUI.m_HpBackRender.Draw(rc);
	if (m_BossHpWhiteScale.x > 0.0f)
	{
		m_monsterUI.m_HpWhiteRender.Draw(rc);
	}
	m_monsterUI.m_HpFrontRender.Draw(rc);

	//HP�̃t���[��
	m_monsterUI.m_HpFlameRender.Draw(rc);

	//HP�̃t�H���g
	m_monsterUI.m_hpFont.Draw(rc);
	m_monsterUI.m_AccumulationDamageFont.Draw(rc);
} 



void GameUI::Render(RenderContext& rc)
{
	DrawPlayerUI(rc);
	DrawMonsterUI(rc);

	m_TimeFlameRender.Draw(rc);
	m_TimerFont.Draw(rc);
}

void GameUI::InitPlayerUI()
{
	//HP�̒l
	InitFontRender(m_playerUI.m_hpFont, HP_FONT_POS, 1.3f);
	//MP�̒l
	InitFontRender(m_playerUI.m_mpFont, MP_FONT_POS, 1.2f);

	//�L�����A�C�R��
	InitSpriteRender(
		m_playerUI.m_characterIconRender, "Assets/sprite/InGame/Character/Icon_Hero.DDS", 219, 219, MAIN_ICON_POS);

	//�A�C�R���x�[�X
	InitSpriteRender(
		m_playerUI.m_iconBaseRender, "Assets/sprite/InGame/Character/Icon_Base_Main.DDS", 250, 250, MAIN_ICON_BASE_POS);

	//�X�e�[�^�X�o�[
	InitSpriteRender(
		m_playerUI.m_statusBarRender, "Assets/sprite/InGame/Character/StatusBar_Main.DDS", 720, 206, MAIN_STATUS_BAR);
	
	//HP�o�[
	InitSpriteRender(
		m_playerUI.m_hpFrontRender, "Assets/sprite/InGame/Character/HP_Front_Main.DDS", 550, 72, MAIN_HP_FRONT_BAR);
	//�s�{�b�g�̐ݒ�
	m_playerUI.m_hpFrontRender.SetPivot(HP_OR_MP_PIBOT);

	//����HP�o�[
	InitSpriteRender(
		m_playerUI.m_hpWhiteRender, "Assets/sprite/InGame/Character/HP_White.DDS", 550, 72, MAIN_HP_FRONT_BAR);
	//�s�{�b�g�̐ݒ�
	m_playerUI.m_hpWhiteRender.SetPivot(HP_OR_MP_PIBOT);

	//HP�o�[�̗���
	InitSpriteRender(
		m_playerUI.m_hpBackRender, "Assets/sprite/InGame/Character/HP_Back_Main.DDS", 585, 63, MAIN_HP_BACK_BAR);

	//MP�o�[
	InitSpriteRender(
		m_playerUI.m_mpFrontRender, "Assets/sprite/InGame/Character/MP_Front_Main.DDS", 483, 53, MAIN_MP_FRONT_BAR);
	//�s�{�b�g�̐ݒ�
	m_playerUI.m_mpFrontRender.SetPivot(HP_OR_MP_PIBOT);

	//MP�o�[�̗���
	InitSpriteRender(
		m_playerUI.m_mpBackRender, "Assets/sprite/InGame/Character/MP_Back_Main.DDS", 483, 53, MAIN_MP_BACK_BAR);
	
	//�X�L���̐^�񒆂́�
	InitSpriteRender(
		m_playerUI.m_SkillCenterRender, "Assets/sprite/InGame/Character/Skill_Center.DDS", 400, 400, SKILL_CENTER_POS,g_vec3One*0.7f);

	//�X�L���P�̃t���[��
	InitSpriteRender(
		m_playerUI.m_Skill_1FlameRender, "Assets/sprite/InGame/Character/Skill_Flame.DDS", 285, 285, SKILL_1__POS, g_vec3One * 0.7f);

	//�X�L���P�̓����̃t���[��
	InitSpriteRender(
		m_playerUI.m_Skill_1FlameInsideRender, "Assets/sprite/InGame/Character/Skill_Flame_Inside.DDS", 262, 262, SKILL_1__POS, g_vec3One * 0.7f);

	//�X�L���P�̃{�^��X
	InitSpriteRender(
		m_playerUI.m_SkillButtonXRender, "Assets/sprite/InGame/Character/SkillButtonX.DDS", 100, 90, SKILL_1_X_POS, g_vec3One * 0.7f);

	
}

void GameUI::InitMonsterUI()
{
	//HP�̒l
	InitFontRender(m_monsterUI.m_hpFont, BOSS_HP_FONT_POS, 1.1f);

	//�m�F�p
	InitFontRender(m_monsterUI.m_AccumulationDamageFont, { 0.0f, 500.0f });
	
	//�{�X�̃A�C�R��
	InitSpriteRender(
		m_monsterUI.m_IconRender, "Assets/sprite/InGame/Character/Icon_Lich.DDS", 180, 180, BOSS_ICON_POS, g_vec3One * 0.8f);

	//�{�X��HP�̃t���[��
	InitSpriteRender(
		m_monsterUI.m_HpFlameRender, "Assets/sprite/InGame/Character/HP_Flame_Boss.DDS", 1000, 60, BOSS_HP_FLAME_POS);

	//�{�X��HP�o�[
	InitSpriteRender(
		m_monsterUI.m_HpFrontRender, "Assets/sprite/InGame/Character/HP_Front_Boss.DDS", 978, 47, BOSS_HP_FRONT_POS);
	//�s�{�b�g�̐ݒ�
	m_monsterUI.m_HpFrontRender.SetPivot(HP_OR_MP_PIBOT);
	
	//�{�X�̔���HP�o�[
	InitSpriteRender(
		m_monsterUI.m_HpWhiteRender, "Assets/sprite/InGame/Character/HP_White_Boss.DDS", 978, 47, BOSS_HP_FRONT_POS);
	//�s�{�b�g�̐ݒ�
	m_monsterUI.m_HpWhiteRender.SetPivot(HP_OR_MP_PIBOT);

	//HP�o�[�̗���
	InitSpriteRender(
		m_monsterUI.m_HpBackRender, "Assets/sprite/InGame/Character/HP_Back_Boss.DDS", 978, 47, BOSS_HP_BACK_POS);
}


void GameUI::InitSpriteRender(
	SpriteRender& spriterender, 
	const char* filePath, 
	const float width, const float height,
	Vector3 position, 
	Vector3 scale,
	Quaternion rotation
)
{
	//������
	spriterender.Init(filePath, width, height);
	//���W�A�T�C�Y�A��]�̐ݒ�ƍX�V
	spriterender.SetPosition(position);
	spriterender.SetScale(scale);
	spriterender.SetRotation(rotation);
	spriterender.Update();
}

void GameUI::InitFontRender(
	FontRender& fontRender, 
	Vector2 position, 
	float scale,
	Vector4 color, 
	bool isShadowParam, float shadowOffset, Vector4 shadowColor)
{
	fontRender.SetPosition(position);
	fontRender.SetScale(scale);
	fontRender.SetColor(color);
	if (isShadowParam)
	{
		fontRender.SetShadowParam(
			isShadowParam, shadowOffset, shadowColor
		);
	}
}

void GameUI::ProcessPlayerHp()
{
	//HP�o�[�̌����Ă��������B
	Vector3 HpScale = Vector3::One;
	HpScale = CalcGaugeScale(m_player->GetNowActorStatus().maxHp, m_player->GetNowActorStatus().hp);
	m_playerUI.m_hpFrontRender.SetScale(HpScale);
	//���݂̃t���[����HP�ƑO�t���[����HP�̃T�C�Y���Ⴄ�Ȃ�
	if (HpScale.x < m_oldPlayerHpScale.x)
	{
		m_playerHpWhiteScale = m_oldPlayerHpScale;
	}

	//����HP�o�[�����炷���߂̕�ԗ����v�Z�B
	//HP�������قǒx�����Ȃ��Ȃ�قǑ�������
	float num = m_player->GetNowActorStatus().hp / m_player->GetNowActorStatus().maxHp;
	m_playerLerpSpeed = Math::Lerp(num, WHITE_HP_LERP_START, WHITE_HP_LERP_END);

	//����HP�o�[�̌����Ă�������
	m_playerHpWhiteScale = Math::Lerp(
		g_gameTime->GetFrameDeltaTime() * m_playerLerpSpeed,
		m_playerHpWhiteScale, HP_SCALE_END_POS);
	m_playerUI.m_hpWhiteRender.SetScale(m_playerHpWhiteScale);

	//HP�t�H���g
	int HpFont = m_player->GetNowActorStatus().hp;
	wchar_t HP[255];
	swprintf_s(HP, 255, L"HP      %3d", HpFont);
	m_playerUI.m_hpFont.SetText(HP);

	//�O�t���[����Hp�X�P�[����ݒ�
	m_oldPlayerHpScale = HpScale;

	//�X�V
	m_playerUI.m_hpFrontRender.Update();
	m_playerUI.m_hpWhiteRender.Update();
}

void GameUI::ProcessPlayerMp()
{
	//MP�o�[�̌����Ă��������B
	Vector3 MpScale = Vector3::One;
	MpScale = CalcGaugeScale(m_player->GetNowActorStatus().maxMp, m_player->GetNowActorStatus().mp);
	m_playerUI.m_mpFrontRender.SetScale(MpScale);




	//MP�t�H���g
	int MpFont = m_player->GetNowActorStatus().mp;
	wchar_t MP[255];
	swprintf_s(MP, 255, L"MP     %3d", MpFont);
	m_playerUI.m_mpFont.SetText(MP);

	//�X�V
	m_playerUI.m_mpFrontRender.Update();
}

void GameUI::ProcessBossHP()
{
	//HP�o�[�̌����Ă��������B
	Vector3 HpScale = Vector3::One;
	HpScale = CalcGaugeScale(m_lich->GetStatus().maxHp, m_lich->GetStatus().hp);
	m_monsterUI.m_HpFrontRender.SetScale(HpScale);
	//���݂̃t���[����HP�ƑO�t���[����HP�̃T�C�Y���Ⴄ�Ȃ�
	if (HpScale.x < m_oldBossHpScale.x)
	{
		m_BossHpWhiteScale = m_oldBossHpScale;
	}

	//����HP�o�[�����炷���߂̕�ԗ����v�Z�B
	//HP�������قǒx�����Ȃ��Ȃ�قǑ�������
	float num = m_lich->GetStatus().hp / m_lich->GetStatus().maxHp;
	m_bossLerpSpeed = Math::Lerp(num, WHITE_HP_LERP_START, WHITE_HP_LERP_END);

	//����HP�o�[�̌����Ă�������
	m_BossHpWhiteScale = Math::Lerp(
		g_gameTime->GetFrameDeltaTime() * m_bossLerpSpeed,
		m_BossHpWhiteScale, HP_SCALE_END_POS);
	m_monsterUI.m_HpWhiteRender.SetScale(m_BossHpWhiteScale);

	//�{�X��HP�̕\��
	int NowActorMP = m_lich->GetStatus().hp;
	int NowActorMaxMP = m_lich->GetStatus().maxHp;
	wchar_t MP[255];
	swprintf_s(MP, 255, L"HP %3d/%d", NowActorMP, NowActorMaxMP);
	m_monsterUI.m_hpFont.SetText(MP);

	/*int a = m_lich->GetAccumulationDamage();
	int b = m_lich->GetHitCount();
	wchar_t A[255];
	swprintf_s(A, 255, L"%3d%3d��", a,b);
	m_monsterUI.m_AccumulationDamageFont.SetText(A);*/

	m_oldBossHpScale = HpScale;

	m_monsterUI.m_HpFrontRender.Update();
	m_monsterUI.m_HpWhiteRender.Update();
}

