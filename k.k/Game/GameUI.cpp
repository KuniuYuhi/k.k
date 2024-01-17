#include "stdafx.h"

#include <codecvt>

#include "GameUI.h"
#include "Game.h"
#include "Player.h"
#include "Boss.h"
#include "GameManager.h"

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

	const Vector2 HP_PIBOT = { 0.0f,0.5f };							//HP��MP�̃s�{�b�g

	//�X�e�[�^�X�o�[
	const Vector3 MAIN_STATUS_BAR = { -650.0f, -470.0f, 0.0f };
	//�s�{�b�g��ݒ肵��HP�̃o�[
	const Vector3 MAIN_HP_BAR = { -940.0f, -470.0f, 0.0f };
	
	const Vector2 HP_FONT_POS = { -930.0f,-428.0f };

	//+10+10
	const Vector3 SKILL_CENTER_POS = { 755.0f,-396.0f,0.0f };
	const Vector3 SKILL_1__POS = { 592.0f,-376.0f,0.0f };

	const Vector3 SKILL_1_X_POS = { 674.0f,-376.0f,0.0f };

	const Vector3 ENDURANCE_SPRITE_POS = { 835.0f,-450.0f,0.0f };
	const Vector2 ENDURANCE_FONT_POS = { 800.0f,-405.0f };


	const Vector3 TIME_FLAME_POS = { 0.0f,509.0f,0.0f };

	const Vector3 TIME_FLAME_SIZE = { 0.6f, 0.5f, 0.6f };


	const Vector3 HP_SCALE_END_POS = { 0.0f,1.0f,1.0f };


	const Vector3 PHASE_FLAME_POS = { -800.0f,240.0f,0.0f };
	const Vector2 PHASE_FONT_POS = { -960.0f,288.0f };

	const float WHITE_HP_LERP_START = 0.1f;
	const float WHITE_HP_LERP_END = 4.5f;
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

	//�t�F�[�Y�̃t���[��
	InitSpriteRender(
		m_PhaseFlameRender, "Assets/sprite/InGame/Character/Wave_Flame.DDS", 629, 97,
		PHASE_FLAME_POS, g_vec3One
	);
	//�t�F�[�Y�̕���
	InitFontRender(m_PhadeFont, PHASE_FONT_POS, 1.5f);

	m_oldMainCharaHP = m_player->GetNowActorStatus().GetHp();

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
	//��������
	TimerUIUpdate();
	//�t�F�[�Y�̏���
	ProcessPhase();
	//�X�e�[�^�X
	UpdateMainStatus();
	//�E�F�|��
	UpdateWeapon();
}

void GameUI::UpdateMainStatus()
{
	//HP�̏���
	ProcessPlayerHp();
	
}

void GameUI::UpdateWeapon()
{
	//����ւ��m�肵�Ă��珈��
	if (m_player->GetChangeWeaponCompleteFlag() == true)
	{
		if (m_player->GetChangeTargetUseWeapon() == enWeapon_Sub)
		{
			//�؂�ւ��Ώۂ̕���ƕ���̃X�v���C�g�����ւ���
			ChangeWeapon(m_weaponSprits[enWeapon_Sub]);
		}
		else
		{
			//�؂�ւ��Ώۂ̕���ƕ���̃X�v���C�g�����ւ���
			ChangeWeapon(m_weaponSprits[enWeapon_Sub2]);
		}
		
		//�؂�ւ����̂ŁA�t���O�����Z�b�g
		m_player->SetChangeWeaponCompleteFlag(false);
	}

	//�ϋv�l�̃t�H���g�̏���
	ProcessWeaponEndranceFont();
	
	//�摜�̍X�V
	for (int num = 0; num < enWeapon_num; num++)
	{
		m_weaponSprits[num].m_weaponRender->SetPosition(m_weaponIconPos[num]);
		m_weaponSprits[num].m_weaponRender->SetScale(m_weaponIconScale[num]);
		m_weaponSprits[num].m_weaponRender->Update();
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
	if (m_boss == nullptr)
	{
		return;
	}

	//�{�X��HP�̏���
	ProcessBossHP();

	
}

void GameUI::TimerUIUpdate()
{
	//���̎擾
	int minute = GameManager::GetInstance()->GetMinute();
	//�b�̎擾
	int second = GameManager::GetInstance()->GetSecond();

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
	
	//���C��
	m_playerUI.m_hpFlameRender.Draw(rc);
	
	//HP�̃t�H���g
	m_playerUI.m_hpFont.Draw(rc);


	//���C������̃t���[��
	m_playerUI.m_mainWeaponFlameRender.Draw(rc);
	////�T�u����P�̃t���[��
	m_playerUI.m_subWeaponFlameRender.Draw(rc);
	////�T�u����P�̃R�}���h
	m_playerUI.m_subWeaponCommandRender.Draw(rc);
	////�T�u����Q�̃t���[��
	m_playerUI.m_sub2WeaponFlameRender.Draw(rc);
	////�T�u����Q�̃R�}���h
	m_playerUI.m_sub2WeaponCommandRender.Draw(rc);

	//����̃A�C�R��
	//�ϋv�l�̔w�i
	for (int num = 0; num < enWeapon_num; num++)
	{
		m_weaponSprits[num].m_weaponRender->Draw(rc);
	}

	if (m_weaponSprits[enWeapon_Main].m_weaponEndranceRender != nullptr)
	{
		m_weaponSprits[enWeapon_Main].m_weaponEndranceRender->Draw(rc);
	}

	//����̑ϋv�l��-�P���傫��������`��
	if (m_player->GetNowWeaponEndrance() > -1)
	{
		m_playerUI.m_weaponEndranceFont.Draw(rc);
	}
}

void GameUI::DrawMonsterUI(RenderContext& rc)
{
	if (m_boss == nullptr)
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

	m_PhaseFlameRender.Draw(rc);
	m_PhadeFont.Draw(rc);
}

void GameUI::InitPlayerUI()
{

	//HP�̒l
	InitFontRender(m_playerUI.m_hpFont, HP_FONT_POS, 1.3f);
	
	//�X�e�[�^�X�o�[
	InitSpriteRender(
		m_playerUI.m_hpFlameRender, "Assets/sprite/InGame/Character/Player_HP_Flame.DDS", 608, 88, MAIN_STATUS_BAR);

	//HP�o�[
	InitSpriteRender(
		m_playerUI.m_hpFrontRender, "Assets/sprite/InGame/Character/Player_HP_Front.DDS", 588, 78, MAIN_HP_BAR);
	//�s�{�b�g�̐ݒ�
	m_playerUI.m_hpFrontRender.SetPivot(HP_PIBOT);

	//����HP�o�[
	InitSpriteRender(
		m_playerUI.m_hpWhiteRender, "Assets/sprite/InGame/Character/Player_HP_White.DDS", 588, 78, MAIN_HP_BAR);
	//�s�{�b�g�̐ݒ�
	m_playerUI.m_hpWhiteRender.SetPivot(HP_PIBOT);

	//HP�o�[�̗���
	InitSpriteRender(
		m_playerUI.m_hpBackRender, "Assets/sprite/InGame/Character/Player_HP_Back.DDS", 600, 80, MAIN_STATUS_BAR);

	
	//�X�L���P�̃t���[��
	InitSpriteRender(
		m_playerUI.m_Skill_1FlameRender, "Assets/sprite/InGame/Character/SkillFlame.DDS", 181, 181, SKILL_1__POS, g_vec3One);


	//���C������̃A�C�R��
	InitSpriteRender(
		m_playerUI.m_weaponRender[enWeapon_Main],
		"Assets/sprite/InGame/Character/SwordShield.DDS", 256, 256,
		m_weaponIconPos[enWeapon_Main], m_weaponIconScale[enWeapon_Main]
	);
	m_weaponSprits[enWeapon_Main].m_weaponRender = &m_playerUI.m_weaponRender[enWeapon_Main];

	//���C������̃t���[��
	InitSpriteRender(
		m_playerUI.m_mainWeaponFlameRender, "Assets/sprite/InGame/Character/MainWeaponFlame.DDS", 238, 237,
		m_weaponIconPos[enWeapon_Main], g_vec3One*1.1f
	);

	//�T�u����P�̃A�C�R��
	InitSpriteRender(
		m_playerUI.m_weaponRender[enWeapon_Sub],
		"Assets/sprite/InGame/Character/GreatSword.DDS", 300, 300,
		m_weaponIconPos[enWeapon_Sub], m_weaponIconScale[enWeapon_Sub]
	);
	m_weaponSprits[enWeapon_Sub].m_weaponRender = &m_playerUI.m_weaponRender[enWeapon_Sub];

	//�T�u����P�̃t���[��
	InitSpriteRender(
		m_playerUI.m_subWeaponFlameRender, "Assets/sprite/InGame/Character/SubWeaponFlame.DDS", 156, 155,
		m_weaponIconPos[enWeapon_Sub], g_vec3One * 0.9f
	);
	//�T�u����P�̃R�}���h
	InitSpriteRender(
		m_playerUI.m_subWeaponCommandRender, "Assets/sprite/InGame/Character/SkillKye.DDS", 60, 56,
		{ 750.0f,38.0f,0.0f }, g_vec3One
	);

	//�T�u����Q�̃A�C�R��
	InitSpriteRender(
		m_playerUI.m_weaponRender[enWeapon_Sub2],
		"Assets/sprite/InGame/Character/BowArrow.DDS", 290, 290,
		m_weaponIconPos[enWeapon_Sub2], m_weaponIconScale[enWeapon_Sub2]
	);
	m_weaponSprits[enWeapon_Sub2].m_weaponRender = &m_playerUI.m_weaponRender[enWeapon_Sub2];

	//�T�u����Q�̃t���[��
	InitSpriteRender(
		m_playerUI.m_sub2WeaponFlameRender, "Assets/sprite/InGame/Character/SubWeaponFlame.DDS", 156, 155,
		m_weaponIconPos[enWeapon_Sub2], g_vec3One * 0.9f
	);
	//�T�u����Q�̃R�}���h
	Quaternion rot;
	rot.SetRotationDegZ(180.0f);
	InitSpriteRender(
		m_playerUI.m_sub2WeaponCommandRender, "Assets/sprite/InGame/Character/SkillKye.DDS", 60, 56,
		{ 750.0f,-110.0f,0.0f }, g_vec3One, rot
	);	

	//���C������(�\�[�h���V�[���h)�̑ϋv�l�̔w�i
	InitSpriteRender(m_playerUI.m_weaponEndranceRender[enWeapon_Main],
		"Assets/sprite/InGame/Character/Shield_Endurance.DDS", 134, 132,
		ENDURANCE_SPRITE_POS, g_vec3One);
	m_weaponSprits[enWeapon_Main].m_weaponEndranceRender = &m_playerUI.m_weaponEndranceRender[enWeapon_Main];
	//�T�u����Q(�{�E���A���[)�̑ϋv�l�̔w�i
	InitSpriteRender(m_playerUI.m_weaponEndranceRender[enWeapon_Sub2],
		"Assets/sprite/InGame/Character/ArrowStock.DDS", 177, 182,
		ENDURANCE_SPRITE_POS, g_vec3One);
	m_weaponSprits[enWeapon_Sub2].m_weaponEndranceRender = &m_playerUI.m_weaponEndranceRender[enWeapon_Sub2];

	//�ϋv�͂̃t�H���g
	InitFontRender(
		m_playerUI.m_weaponEndranceFont, ENDURANCE_FONT_POS,1.5f
	);

	//wchar_t srr = m_playerUI.m_weaponEndranceFont.GetText();

	m_playerUI.m_weaponEndranceFont.SetOffset({ 20.0f,0.0f });

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
	m_monsterUI.m_HpFrontRender.SetPivot(HP_PIBOT);
	
	//�{�X�̔���HP�o�[
	InitSpriteRender(
		m_monsterUI.m_HpWhiteRender, "Assets/sprite/InGame/Character/HP_White_Boss.DDS", 978, 47, BOSS_HP_FRONT_POS);
	//�s�{�b�g�̐ݒ�
	m_monsterUI.m_HpWhiteRender.SetPivot(HP_PIBOT);

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
	int maxHP = m_player->GetNowActorStatus().GetMaxHp();
	int nowHP = m_player->GetNowActorStatus().GetHp();

	//HP�o�[�̌����Ă��������B
	Vector3 HpScale = Vector3::One;
	HpScale = CalcGaugeScale(maxHP, nowHP);
	m_playerUI.m_hpFrontRender.SetScale(HpScale);
	//���݂̃t���[����HP�ƑO�t���[����HP�̃T�C�Y���Ⴄ�Ȃ�
	if (HpScale.x < m_oldPlayerHpScale.x)
	{
		m_playerHpWhiteScale = m_oldPlayerHpScale;
	}

	//����HP�o�[�����炷���߂̕�ԗ����v�Z�B
	//HP�������قǒx�����Ȃ��Ȃ�قǑ�������
	float num = nowHP / maxHP;
	m_playerLerpSpeed = Math::Lerp(num, WHITE_HP_LERP_START, WHITE_HP_LERP_END);

	//����HP�o�[�̌����Ă�������
	m_playerHpWhiteScale = Math::Lerp(
		g_gameTime->GetFrameDeltaTime() * m_playerLerpSpeed,
		m_playerHpWhiteScale, HP_SCALE_END_POS);
	m_playerUI.m_hpWhiteRender.SetScale(m_playerHpWhiteScale);

	//HP�t�H���g
	wchar_t HP[255];
	swprintf_s(HP, 255, L"HP      %3d", nowHP);
	m_playerUI.m_hpFont.SetText(HP);

	//�O�t���[����Hp�X�P�[����ݒ�
	m_oldPlayerHpScale = HpScale;

	//�X�V
	m_playerUI.m_hpFrontRender.Update();
	m_playerUI.m_hpWhiteRender.Update();
}

void GameUI::ProcessBossHP()
{
	int maxHP = m_boss->GetStatus().GetMaxHp();
	int nowHP = m_boss->GetStatus().GetHp();

	//HP�o�[�̌����Ă��������B
	Vector3 HpScale = Vector3::One;
	HpScale = CalcGaugeScale(maxHP, nowHP);
	m_monsterUI.m_HpFrontRender.SetScale(HpScale);
	//���݂̃t���[����HP�ƑO�t���[����HP�̃T�C�Y���Ⴄ�Ȃ�
	if (HpScale.x < m_oldBossHpScale.x)
	{
		m_BossHpWhiteScale = m_oldBossHpScale;
	}

	//����HP�o�[�����炷���߂̕�ԗ����v�Z�B
	//HP�������قǒx�����Ȃ��Ȃ�قǑ�������
	float num = (float)nowHP / (float)maxHP;
	m_bossLerpSpeed = Math::Lerp(num, WHITE_HP_LERP_START, WHITE_HP_LERP_END);

	//����HP�o�[�̌����Ă�������
	m_BossHpWhiteScale = Math::Lerp(
		g_gameTime->GetFrameDeltaTime() * m_bossLerpSpeed,
		m_BossHpWhiteScale, HP_SCALE_END_POS);
	m_monsterUI.m_HpWhiteRender.SetScale(m_BossHpWhiteScale);

	//�{�X��HP�̕\��
	wchar_t MP[255];
	swprintf_s(MP, 255, L"HP %3d/%d", nowHP, maxHP);
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

void GameUI::ProcessPhase()
{
	int PhaseNumber = GameManager::GetInstance()->GetNowPhaseState();
	wchar_t NowPhase[255];
	swprintf_s(NowPhase, 255, L"�t�F�[�Y%d", PhaseNumber+1);

	m_PhadeFont.SetText(NowPhase);
}

void GameUI::ChangeWeapon(
	WeaponSprits& changeWeaponSprite
)
{
	WeaponSprits temporary;
	temporary = m_weaponSprits[enWeapon_Main];
	m_weaponSprits[enWeapon_Main] = changeWeaponSprite;
	changeWeaponSprite = temporary;
}

void GameUI::ProcessWeaponEndranceFont()
{
	int num = m_player->GetNowWeaponEndrance();
	wchar_t endrance[255];
	swprintf_s(endrance, 255, L"%d", num);

	//wchar_t�^��string�^�ɕϊ�
	//std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	//std::string str = converter.to_bytes(endrance);
	////������̒������擾
	//int length= str.length();
	//m_playerUI.m_weaponEndranceFont.SetOffset({ length/2.0f ,0.0f });

	m_playerUI.m_weaponEndranceFont.SetText(endrance);

	int harfEndrance = m_player->GetNowWeaponMaxEndrance() / 2;

	//��������Ȃ甒�F�ɂ���
	if (m_player->GetNowWeaponEndrance() > harfEndrance)
	{
		m_playerUI.m_weaponEndranceFont.SetColor(g_vec4White);
	}
	//�ϋv�l�̔����̔���(�l���̈�)��������ԐF�ɂ���
	else if (m_player->GetNowWeaponEndrance() <= harfEndrance / 2)
	{
		m_playerUI.m_weaponEndranceFont.SetColor(g_vec4Red);
	}
	//�����ȉ��Ȃ物�F�ɂ���
	else
	{
		m_playerUI.m_weaponEndranceFont.SetColor(g_vec4Yellow);
	}
}

