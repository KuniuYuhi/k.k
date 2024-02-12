#include "stdafx.h"

#include <codecvt>

#include "GameUI.h"
#include "Game.h"
#include "Player.h"
#include "Boss.h"
#include "GameManager.h"
#include "CharactersInfoManager.h"

namespace {

	const Vector2 TIMER_POS = { -34.0f,522.0f };
	const Vector2 TIMER_OFFSET = { 50.0f,-20.0f };

	/// <summary>
	/// 978
	/// </summary>
	const Vector3 BOSS_HP_FLAME_POS = { 0.0f,450.0f,0.0f };
	const Vector3 BOSS_HP_FRONT_POS = { -489.0f,450.0f,0.0f };
	const Vector3 BOSS_HP_BACK_POS = { 0.0f,450.0f,0.0f };

	const Vector2 BOSS_HP_FONT_POS = { -200.0f, 486.0f };

	const Vector3 BOSS_SUPERARMOR_FLONT_BAR_POS = { -333.0f,400.0f,0.0f };
	const Vector3 BOSS_SUPERARMOR_POS = { 0.0f,400.0f,0.0f };

	/// <summary>
	/// �v���C���[��
	/// </summary>

	const Vector2 GAUGE_PIBOT = { 0.0f,0.5f };							//HP��MP�̃s�{�b�g

	//�X�e�[�^�X�o�[
	const Vector3 MAIN_STATUS_BAR = { -650.0f, -470.0f, 0.0f };
	//�s�{�b�g��ݒ肵��HP�̃o�[
	const Vector3 MAIN_HP_BAR = { -940.0f, -470.0f, 0.0f };
	
	const Vector2 HP_FONT_POS = { -930.0f,-428.0f };

	const Vector3 ENDURANCE_SPRITE_POS = { 835.0f,-450.0f,0.0f };
	const Vector2 ENDURANCE_FONT_POS = { 800.0f,-405.0f };

	const Vector3 COMMAND_LB = { 740.0f,38.0f,0.0f };
	const Vector3 COMMAND_RB = { 740.0f,-110.0f,0.0f };

	const Vector3 TIME_FLAME_POS = { 0.0f,509.0f,0.0f };

	const Vector3 TIME_FLAME_SIZE = { 0.6f, 0.5f, 0.6f };


	const Vector3 HP_SCALE_END_POS = { 0.0f,1.0f,1.0f };


	const Vector3 PHASE_FLAME_POS = { -740.0f,370.0f,0.0f };
	const Vector2 NOW_PHASE_FONT_POS = { -735.0f,515.0f };
	const float NOW_PHASE_SIZE = 2.0f;

	const Vector2 NOW_PHASE_MONSTERS_POS= { -765.0f,355.0f };
	const float NOW_PHASE_MONSTERS_SIZE = 1.8f;
	const float NOW_PHASE_MONSTERS_DEFAULT_X_OFFSET = 17.0f;
	const float NOW_PHASE_MONSTERS_DEFAULT_Y_OFFSET = 10.0f;

	const Vector3 PHASE_TIME_FLAME_POS = { -740.0f,345.0f,0.0f };
	const Vector3 PHASE_TIME_BAR_POS = { -908.0f,345.0f,0.0f };

	const Vector4 SUPERSARMOR_GRAY_COLOR = { 0.9f,0.9f,0.9f,0.5f };

	const float WHITE_HP_SPEED = 25.0f;
	const float BOSS_WHITE_HP_SPEED = 50.0f;
}

GameUI::GameUI()
{
}

GameUI::~GameUI()
{
}

bool GameUI::Start()
{
	//�v���C���[��UI�̏�����
	InitPlayerUI();
	//�����X�^�[��UI�̏�����
	InitMonsterUI();
	//�t�F�[�YUI�̏�����
	InitPhaseUI();

	//��������
	InitFontRender(m_timerFont, TIMER_POS, 1.1f);
	m_timerFont.SetOffset(TIMER_OFFSET);

	//�������Ԃ̘g
	InitSpriteRender(
		m_timeFlameRender, "Assets/sprite/InGame/Character/TimeFlame2.DDS", 500, 124, TIME_FLAME_POS, TIME_FLAME_SIZE
	);


	



	m_nowPlayerWhiteHp = m_player->GetNowActorStatus().GetMaxHp();

	return true;
}

void GameUI::Update()
{
	//��������
	TimerUIUpdate();
	//�t�F�[�Y�̏���
	ProcessPhase();
	//�v���C���[��UI�̏���
	PlayerUIUpdate();
	//�{�X��UI�̏���
	MonsterUIUpdate();
}

void GameUI::PlayerUIUpdate()
{
	//�X�e�[�^�X
	UpdateMainStatus();
	//�E�F�|��
	UpdateWeapon();
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
	//�{�X�̃X�[�p�[�A�[�}�[�̏���
	ProcessBossSuperArmor();
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
	//�����Ă����������v�Z
	scale.x = value / Maxvalue;
	return scale;
}

void GameUI::TimerUIUpdate()
{
	//���̎擾
	int minute = GameManager::GetInstance()->GetMinute();
	//�b�̎擾
	int second = GameManager::GetInstance()->GetSecond();

	wchar_t time[256];
	swprintf_s(time, 256, L"%d:%02d", minute, second);
	//�e�L�X�g��ݒ�
	m_timerFont.SetText(time);
}

void GameUI::ProcessPlayerHp()
{
	//�ő�HP�ƌ��݂�HP���擾
	int maxHP = m_player->GetNowActorStatus().GetMaxHp();
	int nowHP = m_player->GetNowActorStatus().GetHp();

	//HP�o�[�̌����Ă��������B
	Vector3 HpScale = Vector3::One;
	//HP�̃X�P�[�����v�Z
	HpScale = CalcGaugeScale(maxHP, nowHP);
	//�X�P�[����ݒ�
	m_playerUI.m_hpFrontRender.SetScale(HpScale);
	//HP�t�H���g�̐ݒ�
	wchar_t HP[255];
	//���݂�HP��\��
	swprintf_s(HP, 255, L"HP      %3d", nowHP);
	//�e�L�X�g��ݒ�
	m_playerUI.m_hpFont.SetText(HP);

	//����HP�����݂�HP��菬�����Ȃ�
	if (m_nowPlayerWhiteHp < nowHP)
	{
		//����HP�Ɍ��݂�HP��������
		m_nowPlayerWhiteHp = nowHP;
	}
	else
	{
		//����HP�����炷
		m_nowPlayerWhiteHp -= g_gameTime->GetFrameDeltaTime() * WHITE_HP_SPEED;
	}
	//����HP�̃X�P�[�����v�Z
	m_playerWhiteHpScale = CalcGaugeScale(maxHP, m_nowPlayerWhiteHp);
	//�X�P�[����ݒ�
	m_playerUI.m_hpWhiteRender.SetScale(m_playerWhiteHpScale);

	//�X�V
	m_playerUI.m_hpFrontRender.Update();
	m_playerUI.m_hpWhiteRender.Update();
}

void GameUI::ProcessBossHP()
{
	//�ő�HP�ƌ��݂�HP���擾
	int maxHP = m_boss->GetStatus().GetMaxHp();
	int nowHP = m_boss->GetStatus().GetHp();

	//HP�o�[�̌����Ă��������B
	Vector3 HpScale = Vector3::One;
	//HP�̃X�P�[�����v�Z
	HpScale = CalcGaugeScale(maxHP, nowHP);
	//�X�P�[����ݒ�
	m_monsterUI.m_hpFrontRender.SetScale(HpScale);
	//�{�X��HP�̕\��
	wchar_t MP[255];
	//���݂�HP��\��
	swprintf_s(MP, 255, L"HP %3d/%d", nowHP, maxHP);
	//�e�L�X�g��ݒ�
	m_monsterUI.m_hpFont.SetText(MP);

	/*int a = m_lich->GetAccumulationDamage();
	int b = m_lich->GetHitCount();
	wchar_t A[255];
	swprintf_s(A, 255, L"%3d%3d��", a,b);
	m_monsterUI.m_AccumulationDamageFont.SetText(A);*/


	//����HP�����݂�HP��菬�����Ȃ�
	if (m_nowBossWhiteHp < nowHP)
	{
		//����HP�Ɍ��݂�HP��������
		m_nowBossWhiteHp = nowHP;
	}
	else
	{
		//����HP�����炷
		m_nowBossWhiteHp -= g_gameTime->GetFrameDeltaTime() * BOSS_WHITE_HP_SPEED;
	}
	//����HP�̃X�P�[�����v�Z
	m_bossHpWhiteScale = CalcGaugeScale(maxHP, m_nowBossWhiteHp);
	//�X�P�[����ݒ�
	m_monsterUI.m_hpWhiteRender.SetScale(m_bossHpWhiteScale);


	
	//�X�V
	m_monsterUI.m_hpFrontRender.Update();
	m_monsterUI.m_hpWhiteRender.Update();
}

void GameUI::ProcessBossSuperArmor()
{
	//�X�[�p�[�A�[�}�[�̃o�[�̌����Ă��������B
	Vector3 spuerArmorScale = Vector3::One;
	spuerArmorScale = CalcGaugeScale(
		m_boss->GetStatus().GetMaxSuperArmorPoint(), m_boss->GetStatus().GetSuperArmorPoint());
	m_monsterUI.m_superArmor_FrontBarRender.SetScale(spuerArmorScale);

	//�X�[�p�[�A�[�}�[���u���C�N���Ă��鎞�́A�Q�[�W�̐F���Â�����
	if (m_boss->GetSuperArmorBreakFlag() == true)
	{
		m_monsterUI.m_superArmor_FrontBarRender.SetMulColor(SUPERSARMOR_GRAY_COLOR);
	}
	else
	{
		m_monsterUI.m_superArmor_FrontBarRender.SetMulColor(g_vec4White);
	}

	//�X�V
	m_monsterUI.m_superArmor_FrontBarRender.Update();
}

void GameUI::ProcessPhase()
{
	//���݂̃t�F�[�Y���擾
	int PhaseNumber = GameManager::GetInstance()->GetNowPhaseState();
	wchar_t NowPhase[255];
	swprintf_s(NowPhase, 255, L"%d", PhaseNumber+1);
	//�e�L�X�g��ݒ�
	m_phaseUI.m_nowPhaseFont.SetText(NowPhase);


	//���݂̓G�̐����擾
	int monsterNum = CharactersInfoManager::GetInstance()->GetMobMonsters().size();
	wchar_t NowMonsterNum[255];
	swprintf_s(NowMonsterNum, 255, L"%d", monsterNum);
	//�e�L�X�g��ݒ�
	m_phaseUI.m_nowPhaseMonstersFont.SetText(NowMonsterNum);
	//�e�L�X�g�̃I�t�Z�b�g�ʂ��v�Z
	CalcOffsetForNowPhaseMonsters(monsterNum);


	//�^�C�}�[�̌��݂̒l�ƍő�l���擾
	float nowTimer = GameManager::GetInstance()->GetPhaseTimer();
	float timerLimmit = GameManager::GetInstance()->GetNowPhaseTimerLimmit();

	//�^�C���o�[�̌����Ă��������B
	Vector3 phaseTimerScale = Vector3::One;
	//�^�C���o�[�̃X�P�[�����v�Z
	phaseTimerScale = CalcGaugeScale(timerLimmit, nowTimer);
	//�X�P�[����ݒ�
	m_phaseUI.m_phaseTimeBarRender.SetScale(phaseTimerScale);
	m_phaseUI.m_phaseTimeBarRender.Update();
}

void GameUI::CalcOffsetForNowPhaseMonsters(int monsters)
{
	//�����ς��Ȃ��Ȃ�I�t�Z�b�g���������Ȃ�
	if (monsters == m_oldPhaseMonstersNum)
	{
		return;
	}
	//�G�̐����񌅂ƈꌅ������̂ŃI�t�Z�b�g���v�Z
	Vector2 offset = CalcNumberCount(monsters, NOW_PHASE_MONSTERS_DEFAULT_X_OFFSET, NOW_PHASE_MONSTERS_DEFAULT_Y_OFFSET);
	//�I�t�Z�b�g��ݒ�
	m_phaseUI.m_nowPhaseMonstersFont.SetOffset(offset);

	m_oldPhaseMonstersNum = monsters;
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

Vector2 GameUI::CalcNumberCount(float num, float xOffset, float yOffset)
{
	int digitCount = 1;

	if (num > 0)
	{
		//�����̌������擾
		digitCount = static_cast<int>(log10(num) + 1);
	}

	//�I�t�Z�b�g���v�Z
	Vector2 offset = g_vec2Zero;
	//X�I�t�Z�b�g���v�Z
	offset.x = xOffset * digitCount;
	//Y�I�t�Z�b�g���v�Z
	offset.y = yOffset;

	return offset;
}

void GameUI::Render(RenderContext& rc)
{

	DrawPlayerUI(rc);
	DrawMonsterUI(rc);

	m_timeFlameRender.Draw(rc);
	m_timerFont.Draw(rc);

	m_phaseUI.m_phaseFlameRender.Draw(rc);
	m_phaseUI.m_nowPhaseFont.Draw(rc);
	m_phaseUI.m_nowPhaseMonstersFont.Draw(rc);

	m_phaseUI.m_phaseTimeBarRender.Draw(rc);
	m_phaseUI.m_phaseTimeFlameRender.Draw(rc);

}

void GameUI::DrawPlayerUI(RenderContext& rc)
{
	//���C��HP
	m_playerUI.m_hpBackRender.Draw(rc);

	if (m_playerWhiteHpScale.x > 0.0f)
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
	//HP
	m_monsterUI.m_hpBackRender.Draw(rc);
	if (m_bossHpWhiteScale.x > 0.0f)
	{
		m_monsterUI.m_hpWhiteRender.Draw(rc);
	}
	m_monsterUI.m_hpFrontRender.Draw(rc);

	//HP�̃t���[��
	m_monsterUI.m_hpFlameRender.Draw(rc);

	//HP�̃t�H���g
	m_monsterUI.m_hpFont.Draw(rc);
	m_monsterUI.m_AccumulationDamageFont.Draw(rc);

	//�X�[�p�[�A�[�}�[
	m_monsterUI.m_superArmor_FlameRender.Draw(rc);
	m_monsterUI.m_superArmor_BackBarRender.Draw(rc);
	m_monsterUI.m_superArmor_FrontBarRender.Draw(rc);
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
	m_playerUI.m_hpFrontRender.SetPivot(GAUGE_PIBOT);

	//����HP�o�[
	InitSpriteRender(
		m_playerUI.m_hpWhiteRender, "Assets/sprite/InGame/Character/Player_HP_White.DDS", 588, 78, MAIN_HP_BAR);
	//�s�{�b�g�̐ݒ�
	m_playerUI.m_hpWhiteRender.SetPivot(GAUGE_PIBOT);

	//HP�o�[�̗���
	InitSpriteRender(
		m_playerUI.m_hpBackRender, "Assets/sprite/InGame/Character/Player_HP_Back.DDS", 600, 80, MAIN_STATUS_BAR);

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
		m_weaponIconPos[enWeapon_Main], g_vec3One * 1.1f
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
		m_playerUI.m_subWeaponCommandRender, "Assets/sprite/InGame/Character/Button_LB.DDS", 80, 60,
		COMMAND_LB, g_vec3One
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
	InitSpriteRender(
		m_playerUI.m_sub2WeaponCommandRender, "Assets/sprite/InGame/Character/Button_RB.DDS", 80, 60,
		COMMAND_RB, g_vec3One
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
		m_playerUI.m_weaponEndranceFont, ENDURANCE_FONT_POS, 1.5f
	);
	//�ϋv�l�̃I�t�Z�b�g
	m_playerUI.m_weaponEndranceFont.SetOffset({ 20.0f,0.0f });

}

void GameUI::InitMonsterUI()
{
	//HP�̒l
	InitFontRender(m_monsterUI.m_hpFont, BOSS_HP_FONT_POS, 1.1f);

	//�m�F�p
	InitFontRender(m_monsterUI.m_AccumulationDamageFont, { 0.0f, 500.0f });

	//�{�X��HP�̃t���[��
	InitSpriteRender(
		m_monsterUI.m_hpFlameRender,
		"Assets/sprite/InGame/Character/HP_Flame_Boss.DDS", 1000, 60, BOSS_HP_FLAME_POS
	);

	//�{�X��HP�o�[
	InitSpriteRender(
		m_monsterUI.m_hpFrontRender,
		"Assets/sprite/InGame/Character/HP_Front_Boss.DDS", 978, 47, BOSS_HP_FRONT_POS
	);
	//�s�{�b�g�̐ݒ�
	m_monsterUI.m_hpFrontRender.SetPivot(GAUGE_PIBOT);

	//�{�X�̔���HP�o�[
	InitSpriteRender(
		m_monsterUI.m_hpWhiteRender,
		"Assets/sprite/InGame/Character/HP_White_Boss.DDS", 978, 47, BOSS_HP_FRONT_POS
	);
	//�s�{�b�g�̐ݒ�
	m_monsterUI.m_hpWhiteRender.SetPivot(GAUGE_PIBOT);

	//HP�o�[�̗���
	InitSpriteRender(
		m_monsterUI.m_hpBackRender,
		"Assets/sprite/InGame/Character/HP_Back_Boss.DDS", 978, 47, BOSS_HP_BACK_POS
	);

	//�X�[�p�[�A�[�}�[�̃t���[��
	InitSpriteRender(
		m_monsterUI.m_superArmor_FlameRender,
		"Assets/sprite/InGame/Character/SuperArmor_Flame.DDS", 670, 26, BOSS_SUPERARMOR_POS
	);

	//�X�[�p�[�A�[�}�[�̕ϓ�����o�[
	InitSpriteRender(
		m_monsterUI.m_superArmor_FrontBarRender,
		"Assets/sprite/InGame/Character/SuperArmor_Front.DDS", 666, 24, BOSS_SUPERARMOR_FLONT_BAR_POS
	);
	//�s�{�b�g�̐ݒ�
	m_monsterUI.m_superArmor_FrontBarRender.SetPivot(GAUGE_PIBOT);

	//�X�[�p�[�A�[�}�[�̗��̃o�[
	InitSpriteRender(
		m_monsterUI.m_superArmor_BackBarRender,
		"Assets/sprite/InGame/Character/SuperArmor_Back.DDS", 664, 22, BOSS_SUPERARMOR_POS
	);

}

void GameUI::InitPhaseUI()
{
	//�t�F�[�Y�̃t���[��
	InitSpriteRender(
		m_phaseUI.m_phaseFlameRender,
		"Assets/sprite/InGame/Wave/Wave_Flame.DDS", 500, 300,
		PHASE_FLAME_POS, g_vec3One
	);
	//���݂̃t�F�[�Y�̐��̕���
	InitFontRender(
		m_phaseUI.m_nowPhaseFont, NOW_PHASE_FONT_POS, NOW_PHASE_SIZE);
	//���݂̃t�F�[�Y�̃����X�^�[�̐�
	InitFontRender(
		m_phaseUI.m_nowPhaseMonstersFont, NOW_PHASE_MONSTERS_POS, NOW_PHASE_MONSTERS_SIZE);
	//�I�t�Z�b�g���v�Z
	Vector2 offset = CalcNumberCount(1, NOW_PHASE_MONSTERS_DEFAULT_X_OFFSET, NOW_PHASE_MONSTERS_DEFAULT_Y_OFFSET);
	//�I�t�Z�b�g��ݒ�
	m_phaseUI.m_nowPhaseMonstersFont.SetOffset(offset);

	//�t�F�[�Y�̎c�莞�Ԃ̃t���[��
	InitSpriteRender(
		m_phaseUI.m_phaseTimeFlameRender,
		"Assets/sprite/InGame/Wave/Wave_time_Flame.DDS", 350, 46,
		PHASE_TIME_FLAME_POS, g_vec3One
	);
	//�t�F�[�Y�̎c�莞�Ԃ̃o�[
	InitSpriteRender(
		m_phaseUI.m_phaseTimeBarRender,
		"Assets/sprite/InGame/Wave/Wave_time_Bar.DDS", 336, 36,
		PHASE_TIME_BAR_POS, g_vec3One
	);
	m_phaseUI.m_phaseTimeBarRender.SetPivot(GAUGE_PIBOT);

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