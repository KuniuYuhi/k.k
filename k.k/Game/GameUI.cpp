#include "stdafx.h"

#include <codecvt>

#include "GameUI.h"
#include "Game.h"


#include "PlayerGameUI.h"


#include "GameSceneManager.h"
#include "AllGameSceneState.h"

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
	/// プレイヤー側
	/// </summary>

	const Vector2 GAUGE_PIBOT = { 0.0f,0.5f };							//HPかMPのピボット

	//ステータスバー
	const Vector3 MAIN_STATUS_BAR = { -650.0f, -470.0f, 0.0f };
	//ピボットを設定したHPのバー
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
	DeleteGO(m_playerGameUI);
}

bool GameUI::Start()
{

	m_playerGameUI = NewGO<PlayerGameUI>(0, "PlayerGameUI");



	return true;
}

void GameUI::Update()
{
	//勝敗が着いたら削除する
	if (GameSceneManager::GetInstance()->GetBattleOutCome() != 
		GameSceneManager::enBattleOutCome_None)
	{
		DeleteGO(this);
		return;
	}


	m_playerGameUI->UIUpdate();

	
}

void GameUI::Render(RenderContext& rc)
{
	m_playerGameUI->Draw(rc);
}


//void GameUI::InitPlayerUI()
//{
//	//HPの値
//	InitFontRender(m_playerUI.m_hpFont, HP_FONT_POS, 1.3f);
//
//	//ステータスバー
//	InitSpriteRender(
//		m_playerUI.m_hpFlameRender, "Assets/sprite/InGame/Character/Player_HP_Flame.DDS", 608, 88, MAIN_STATUS_BAR);
//
//	//HPバー
//	InitSpriteRender(
//		m_playerUI.m_hpFrontRender, "Assets/sprite/InGame/Character/Player_HP_Front.DDS", 588, 78, MAIN_HP_BAR);
//	//ピボットの設定
//	m_playerUI.m_hpFrontRender.SetPivot(GAUGE_PIBOT);
//
//	//白いHPバー
//	InitSpriteRender(
//		m_playerUI.m_hpWhiteRender, "Assets/sprite/InGame/Character/Player_HP_White.DDS", 588, 78, MAIN_HP_BAR);
//	//ピボットの設定
//	m_playerUI.m_hpWhiteRender.SetPivot(GAUGE_PIBOT);
//
//	//HPバーの裏側
//	InitSpriteRender(
//		m_playerUI.m_hpBackRender, "Assets/sprite/InGame/Character/Player_HP_Back.DDS", 600, 80, MAIN_STATUS_BAR);
//
//	//メイン武器のアイコン
//	InitSpriteRender(
//		m_playerUI.m_weaponRender[enMaxWeapons_Main],
//		"Assets/sprite/InGame/Character/SwordShield.DDS", 256, 256,
//		m_weaponIconPos[enMaxWeapons_Main], m_weaponIconScale[enMaxWeapons_Main]
//	);
//	m_weaponSprits[enMaxWeapons_Main].m_weaponRender = &m_playerUI.m_weaponRender[enMaxWeapons_Main];
//
//	//メイン武器のフレーム
//	InitSpriteRender(
//		m_playerUI.m_mainWeaponFlameRender, "Assets/sprite/InGame/Character/MainWeaponFlame.DDS", 238, 237,
//		m_weaponIconPos[enMaxWeapons_Main], g_vec3One * 1.1f
//	);
//
//	//サブ武器１のアイコン
//	InitSpriteRender(
//		m_playerUI.m_weaponRender[enMaxWeapons_Sub],
//		"Assets/sprite/InGame/Character/GreatSword.DDS", 300, 300,
//		m_weaponIconPos[enMaxWeapons_Sub], m_weaponIconScale[enMaxWeapons_Sub]
//	);
//	m_weaponSprits[enMaxWeapons_Sub].m_weaponRender = &m_playerUI.m_weaponRender[enMaxWeapons_Sub];
//
//	//サブ武器１のフレーム
//	InitSpriteRender(
//		m_playerUI.m_subWeaponFlameRender, "Assets/sprite/InGame/Character/SubWeaponFlame.DDS", 156, 155,
//		m_weaponIconPos[enMaxWeapons_Sub], g_vec3One * 0.9f
//	);
//	//サブ武器１のコマンド
//	InitSpriteRender(
//		m_playerUI.m_subWeaponCommandRender, "Assets/sprite/InGame/Character/Button_LB.DDS", 80, 60,
//		COMMAND_LB, g_vec3One
//	);
//
//	//サブ武器２のアイコン
//	InitSpriteRender(
//		m_playerUI.m_weaponRender[enMaxWeapons_Sub2],
//		"Assets/sprite/InGame/Character/BowArrow.DDS", 290, 290,
//		m_weaponIconPos[enMaxWeapons_Sub2], m_weaponIconScale[enMaxWeapons_Sub2]
//	);
//	m_weaponSprits[enMaxWeapons_Sub2].m_weaponRender = &m_playerUI.m_weaponRender[enMaxWeapons_Sub2];
//
//	//サブ武器２のフレーム
//	InitSpriteRender(
//		m_playerUI.m_sub2WeaponFlameRender, "Assets/sprite/InGame/Character/SubWeaponFlame.DDS", 156, 155,
//		m_weaponIconPos[enMaxWeapons_Sub2], g_vec3One * 0.9f
//	);
//	//サブ武器２のコマンド
//	InitSpriteRender(
//		m_playerUI.m_sub2WeaponCommandRender, "Assets/sprite/InGame/Character/Button_RB.DDS", 80, 60,
//		COMMAND_RB, g_vec3One
//	);
//
//	//メイン武器(ソード＆シールド)の耐久値の背景
//	InitSpriteRender(m_playerUI.m_weaponEndranceRender[enMaxWeapons_Main],
//		"Assets/sprite/InGame/Character/Shield_Endurance.DDS", 134, 132,
//		ENDURANCE_SPRITE_POS, g_vec3One);
//	m_weaponSprits[enMaxWeapons_Main].m_weaponEndranceRender = &m_playerUI.m_weaponEndranceRender[enMaxWeapons_Main];
//	//サブ武器２(ボウ＆アロー)の耐久値の背景
//	InitSpriteRender(m_playerUI.m_weaponEndranceRender[enMaxWeapons_Sub2],
//		"Assets/sprite/InGame/Character/ArrowStock.DDS", 177, 182,
//		ENDURANCE_SPRITE_POS, g_vec3One);
//	m_weaponSprits[enMaxWeapons_Sub2].m_weaponEndranceRender = &m_playerUI.m_weaponEndranceRender[enMaxWeapons_Sub2];
//
//	//耐久力のフォント
//	InitFontRender(
//		m_playerUI.m_weaponEndranceFont, ENDURANCE_FONT_POS, 1.5f
//	);
//	//耐久値のオフセット
//	m_playerUI.m_weaponEndranceFont.SetOffset({ 20.0f,0.0f });
//
//}

//void GameUI::InitMonsterUI()
//{
//	//HPの値
//	InitFontRender(m_monsterUI.m_hpFont, BOSS_HP_FONT_POS, 1.1f);
//
//	//確認用
//	InitFontRender(m_monsterUI.m_AccumulationDamageFont, { 0.0f, 500.0f });
//
//	//ボスのHPのフレーム
//	InitSpriteRender(
//		m_monsterUI.m_hpFlameRender,
//		"Assets/sprite/InGame/Character/HP_Flame_Boss.DDS", 1000, 60, BOSS_HP_FLAME_POS
//	);
//
//	//ボスのHPバー
//	InitSpriteRender(
//		m_monsterUI.m_hpFrontRender,
//		"Assets/sprite/InGame/Character/HP_Front_Boss.DDS", 978, 47, BOSS_HP_FRONT_POS
//	);
//	//ピボットの設定
//	m_monsterUI.m_hpFrontRender.SetPivot(GAUGE_PIBOT);
//
//	//ボスの白いHPバー
//	InitSpriteRender(
//		m_monsterUI.m_hpWhiteRender,
//		"Assets/sprite/InGame/Character/HP_White_Boss.DDS", 978, 47, BOSS_HP_FRONT_POS
//	);
//	//ピボットの設定
//	m_monsterUI.m_hpWhiteRender.SetPivot(GAUGE_PIBOT);
//
//	//HPバーの裏側
//	InitSpriteRender(
//		m_monsterUI.m_hpBackRender,
//		"Assets/sprite/InGame/Character/HP_Back_Boss.DDS", 978, 47, BOSS_HP_BACK_POS
//	);
//
//	//スーパーアーマーのフレーム
//	InitSpriteRender(
//		m_monsterUI.m_superArmor_FlameRender,
//		"Assets/sprite/InGame/Character/SuperArmor_Flame.DDS", 670, 26, BOSS_SUPERARMOR_POS
//	);
//
//	//スーパーアーマーの変動するバー
//	InitSpriteRender(
//		m_monsterUI.m_superArmor_FrontBarRender,
//		"Assets/sprite/InGame/Character/SuperArmor_Front.DDS", 666, 24, BOSS_SUPERARMOR_FLONT_BAR_POS
//	);
//	//ピボットの設定
//	m_monsterUI.m_superArmor_FrontBarRender.SetPivot(GAUGE_PIBOT);
//
//	//スーパーアーマーの裏のバー
//	InitSpriteRender(
//		m_monsterUI.m_superArmor_BackBarRender,
//		"Assets/sprite/InGame/Character/SuperArmor_Back.DDS", 664, 22, BOSS_SUPERARMOR_POS
//	);
//
//}



void GameUI::InitSpriteRender(
	SpriteRender& spriterender,
	const char* filePath,
	const float width, const float height,
	Vector3 position,
	Vector3 scale,
	Quaternion rotation
)
{
	//初期化
	spriterender.Init(filePath, width, height);
	//座標、サイズ、回転の設定と更新
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