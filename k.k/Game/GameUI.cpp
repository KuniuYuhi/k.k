#include "stdafx.h"
#include "GameUI.h"
#include "Game.h"
#include "Player.h"
#include "Lich.h"


//todo MPも白いバーいれる？

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
	/// プレイヤー側
	/// </summary>

	const Vector2 HP_OR_MP_PIBOT = { 0.0f,0.5f };							//HPかMPのピボット

	//ステータスバー
	const Vector3 MAIN_ICON_POS = { -835.0f, -411.0f, 0.0f };
	const Vector3 MAIN_ICON_BASE_POS = { -835.0f, -411.0f, 0.0f };
	const Vector3 MAIN_STATUS_BAR = { -504.0f, -432.0f, 0.0f };
	const Vector3 MAIN_HP_FRONT_BAR = { -706.0f, -469.0f, 0.0f };
	const Vector3 MAIN_HP_BACK_BAR = { -439.0f, -468.0f, 0.0f };
	const Vector3 MAIN_MP_FRONT_BAR = { -708.5f, -394.0f, 0.0f };
	const Vector3 MAIN_MP_BACK_BAR = { -467.0f, -394.0f, 0.0f };
	const Vector2 HP_FONT_POS = { -704.0f,-425.0f };
	const Vector2 MP_FONT_POS = { -704.0f, -354.0f };

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
	//プレイヤーのUI
	InitPlayerUI();
	//モンスターのUI
	InitMonsterUI();

	//制限時間
	InitFontRender(m_TimerFont, TIMER_POS, 1.1f);
	m_TimerFont.SetOffset(TIMER_OFFSET);

	//制限時間の枠
	InitSpriteRender(
		m_TimeFlameRender, "Assets/sprite/InGame/Character/TimeFlame2.DDS", 500, 124, TIME_FLAME_POS, TIME_FLAME_SIZE
	);

	m_oldMainCharaHP = m_player->GetNowActorStatus().hp;

	return true;
}

void GameUI::Update()
{
	//プレイヤーのUIの処理
	PlayerUIUpdate();
	//ボスのUIの処理
	MonsterUIUpdate();


}

void GameUI::PlayerUIUpdate()
{
	//制限時間
	TimerUIUpdate();
	//ステータス
	UpdateMainStatus();
	//キャラアイコン
	UpdateCharaIcon();
	//ウェポン
	UpdateWeapon();
}

void GameUI::UpdateMainStatus()
{
	//HPの処理
	ProcessPlayerHp();
	//MPの処理
	ProcessPlayerMp();

	
}

void GameUI::UpdateCharaIcon()
{
	//やられたキャラのアイコンをグレースケールにする
	if (m_player->GetNowActorDieFlag()==true)
	{
		m_playerUI.m_characterIconRender.SetGrayScale(true);
	}
}

void GameUI::UpdateWeapon()
{
	//入れ替え確定してから処理
	if (m_player->GetChangeWeaponCompleteFlag() == true)
	{
		if (m_player->GetChangeTargetUseWeapon() == enWeapon_Sub)
		{
			//切り替え対象の武器と武器のスプライトを入れ替える
			ChangeWeapon(m_weaponSprits[enWeapon_Sub]);
		}
		else
		{
			//切り替え対象の武器と武器のスプライトを入れ替える
			ChangeWeapon(m_weaponSprits[enWeapon_Sub2]);
		}
		
		//切り替えたので、フラグをリセット
		m_player->SetChangeWeaponCompleteFlag(false);
	}

	for (int num = 0; num < enWeapon_num; num++)
	{
		m_weaponSprits[num].m_weaponSprite->SetPosition(m_weaponIconPos[num]);
		m_weaponSprits[num].m_weaponSprite->SetScale(m_weaponIconScale[num]);
		m_weaponSprits[num].m_weaponSprite->Update();
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
	//ボスがいないなら処理しない
	if (m_lich == nullptr)
	{
		return;
	}

	//ボスのHPの処理
	ProcessBossHP();

	
}

void GameUI::TimerUIUpdate()
{
	//分の取得
	int minute = m_game->GetMinute();
	//秒の取得
	int second = m_game->GetSecond();

	wchar_t time[256];
	swprintf_s(time, 256, L"%d:%02d", minute, second);
	
	m_TimerFont.SetText(time);
}

void GameUI::DrawPlayerUI(RenderContext& rc)
{
	//メインHP
	m_playerUI.m_hpBackRender.Draw(rc);

	if (m_playerHpWhiteScale.x > 0.0f)
	{
		m_playerUI.m_hpWhiteRender.Draw(rc);
	}

	
	m_playerUI.m_hpFrontRender.Draw(rc);
	//メインMP
	m_playerUI.m_mpBackRender.Draw(rc);
	m_playerUI.m_mpFrontRender.Draw(rc);
	
	//メインステータスバー
	m_playerUI.m_statusBarRender.Draw(rc);
	//メインアイコンベース
	m_playerUI.m_iconBaseRender.Draw(rc);
	//HPとMPのフォント
	m_playerUI.m_hpFont.Draw(rc);
	m_playerUI.m_mpFont.Draw(rc);


	//メイン武器のフレーム
	m_playerUI.m_mainWeaponFlameRender.Draw(rc);
	////サブ武器１のフレーム
	m_playerUI.m_subWeaponFlameRender.Draw(rc);
	////サブ武器１のコマンド
	m_playerUI.m_subWeaponCommandRender.Draw(rc);
	////サブ武器２のフレーム
	m_playerUI.m_sub2WeaponFlameRender.Draw(rc);
	////サブ武器２のコマンド
	m_playerUI.m_sub2WeaponCommandRender.Draw(rc);

	//武器のアイコン
	for (int num = 0; num < enWeapon_num; num++)
	{
		m_weaponSprits[num].m_weaponSprite->Draw(rc);
	}
}

void GameUI::DrawMonsterUI(RenderContext& rc)
{
	if (m_lich == nullptr)
	{
		return;
	}
	//アイコン
	m_monsterUI.m_IconRender.Draw(rc);

	//HP
	

	m_monsterUI.m_HpBackRender.Draw(rc);
	if (m_BossHpWhiteScale.x > 0.0f)
	{
		m_monsterUI.m_HpWhiteRender.Draw(rc);
	}
	m_monsterUI.m_HpFrontRender.Draw(rc);

	//HPのフレーム
	m_monsterUI.m_HpFlameRender.Draw(rc);

	//HPのフォント
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

	//HPの値
	InitFontRender(m_playerUI.m_hpFont, HP_FONT_POS, 1.3f);
	//MPの値
	InitFontRender(m_playerUI.m_mpFont, MP_FONT_POS, 1.2f);

	//キャラアイコン
	InitSpriteRender(
		m_playerUI.m_characterIconRender, "Assets/sprite/InGame/Character/Icon_Hero.DDS", 219, 219, MAIN_ICON_POS);

	//アイコンベース
	InitSpriteRender(
		m_playerUI.m_iconBaseRender, "Assets/sprite/InGame/Character/Icon_Base_Main.DDS", 250, 250, MAIN_ICON_BASE_POS);

	//ステータスバー
	InitSpriteRender(
		m_playerUI.m_statusBarRender, "Assets/sprite/InGame/Character/StatusBar_Main.DDS", 720, 206, MAIN_STATUS_BAR);

	//HPバー
	InitSpriteRender(
		m_playerUI.m_hpFrontRender, "Assets/sprite/InGame/Character/HP_Front_Main.DDS", 550, 72, MAIN_HP_FRONT_BAR);
	//ピボットの設定
	m_playerUI.m_hpFrontRender.SetPivot(HP_OR_MP_PIBOT);

	//白いHPバー
	InitSpriteRender(
		m_playerUI.m_hpWhiteRender, "Assets/sprite/InGame/Character/HP_White.DDS", 550, 72, MAIN_HP_FRONT_BAR);
	//ピボットの設定
	m_playerUI.m_hpWhiteRender.SetPivot(HP_OR_MP_PIBOT);

	//HPバーの裏側
	InitSpriteRender(
		m_playerUI.m_hpBackRender, "Assets/sprite/InGame/Character/HP_Back_Main.DDS", 585, 63, MAIN_HP_BACK_BAR);

	//MPバー
	InitSpriteRender(
		m_playerUI.m_mpFrontRender, "Assets/sprite/InGame/Character/MP_Front_Main.DDS", 483, 53, MAIN_MP_FRONT_BAR);
	//ピボットの設定
	m_playerUI.m_mpFrontRender.SetPivot(HP_OR_MP_PIBOT);

	//MPバーの裏側
	InitSpriteRender(
		m_playerUI.m_mpBackRender, "Assets/sprite/InGame/Character/MP_Back_Main.DDS", 483, 53, MAIN_MP_BACK_BAR);

	//スキル１のフレーム
	InitSpriteRender(
		m_playerUI.m_Skill_1FlameRender, "Assets/sprite/InGame/Character/SkillFlame.DDS", 181, 181, SKILL_1__POS, g_vec3One);


	//メイン武器のアイコン
	InitSpriteRender(
		m_playerUI.m_weaponRender[enWeapon_Main],
		"Assets/sprite/InGame/Character/SwordShield.DDS", 256, 256,
		m_weaponIconPos[enWeapon_Main], m_weaponIconScale[enWeapon_Main]
	);
	m_weaponSprits[enWeapon_Main].m_weaponSprite = &m_playerUI.m_weaponRender[enWeapon_Main];

	//メイン武器のフレーム
	InitSpriteRender(
		m_playerUI.m_mainWeaponFlameRender, "Assets/sprite/InGame/Character/MainWeaponFlame.DDS", 238, 237,
		m_weaponIconPos[enWeapon_Main], g_vec3One*1.1f
	);

	//サブ武器１のアイコン
	InitSpriteRender(
		m_playerUI.m_weaponRender[enWeapon_Sub], "Assets/sprite/InGame/Character/GreatSword.DDS", 300, 300,
		m_weaponIconPos[enWeapon_Sub], m_weaponIconScale[enWeapon_Sub]
	);
	m_weaponSprits[enWeapon_Sub].m_weaponSprite = &m_playerUI.m_weaponRender[enWeapon_Sub];

	//サブ武器１のフレーム
	InitSpriteRender(
		m_playerUI.m_subWeaponFlameRender, "Assets/sprite/InGame/Character/SubWeaponFlame.DDS", 156, 155,
		m_weaponIconPos[enWeapon_Sub], g_vec3One * 0.9f
	);
	//サブ武器１のコマンド
	InitSpriteRender(
		m_playerUI.m_subWeaponCommandRender, "Assets/sprite/InGame/Character/SkillKye.DDS", 60, 56,
		{ 750.0f,38.0f,0.0f }, g_vec3One
	);

	//サブ武器２のアイコン
	InitSpriteRender(
		m_playerUI.m_weaponRender[enWeapon_Sub2], "Assets/sprite/InGame/Character/BowArrow.DDS", 290, 290,
		m_weaponIconPos[enWeapon_Sub2], m_weaponIconScale[enWeapon_Sub2]
	);
	m_weaponSprits[enWeapon_Sub2].m_weaponSprite = &m_playerUI.m_weaponRender[enWeapon_Sub2];

	//サブ武器２のフレーム
	InitSpriteRender(
		m_playerUI.m_sub2WeaponFlameRender, "Assets/sprite/InGame/Character/SubWeaponFlame.DDS", 156, 155,
		m_weaponIconPos[enWeapon_Sub2], g_vec3One * 0.9f
	);
	//サブ武器２のコマンド
	Quaternion rot;
	rot.SetRotationDegZ(180.0f);
	InitSpriteRender(
		m_playerUI.m_sub2WeaponCommandRender, "Assets/sprite/InGame/Character/SkillKye.DDS", 60, 56,
		{ 750.0f,-110.0f,0.0f }, g_vec3One, rot
	);	
}

void GameUI::InitMonsterUI()
{
	//HPの値
	InitFontRender(m_monsterUI.m_hpFont, BOSS_HP_FONT_POS, 1.1f);

	//確認用
	InitFontRender(m_monsterUI.m_AccumulationDamageFont, { 0.0f, 500.0f });
	
	//ボスのアイコン
	InitSpriteRender(
		m_monsterUI.m_IconRender, "Assets/sprite/InGame/Character/Icon_Lich.DDS", 180, 180, BOSS_ICON_POS, g_vec3One * 0.8f);

	//ボスのHPのフレーム
	InitSpriteRender(
		m_monsterUI.m_HpFlameRender, "Assets/sprite/InGame/Character/HP_Flame_Boss.DDS", 1000, 60, BOSS_HP_FLAME_POS);

	//ボスのHPバー
	InitSpriteRender(
		m_monsterUI.m_HpFrontRender, "Assets/sprite/InGame/Character/HP_Front_Boss.DDS", 978, 47, BOSS_HP_FRONT_POS);
	//ピボットの設定
	m_monsterUI.m_HpFrontRender.SetPivot(HP_OR_MP_PIBOT);
	
	//ボスの白いHPバー
	InitSpriteRender(
		m_monsterUI.m_HpWhiteRender, "Assets/sprite/InGame/Character/HP_White_Boss.DDS", 978, 47, BOSS_HP_FRONT_POS);
	//ピボットの設定
	m_monsterUI.m_HpWhiteRender.SetPivot(HP_OR_MP_PIBOT);

	//HPバーの裏側
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

void GameUI::ProcessPlayerHp()
{
	//HPバーの減っていく割合。
	Vector3 HpScale = Vector3::One;
	HpScale = CalcGaugeScale(m_player->GetNowActorStatus().maxHp, m_player->GetNowActorStatus().hp);
	m_playerUI.m_hpFrontRender.SetScale(HpScale);
	//現在のフレームのHPと前フレームのHPのサイズが違うなら
	if (HpScale.x < m_oldPlayerHpScale.x)
	{
		m_playerHpWhiteScale = m_oldPlayerHpScale;
	}

	//白いHPバーを減らすための補間率を計算。
	//HPが多いほど遅く少なくなるほど速くする
	float num = m_player->GetNowActorStatus().hp / m_player->GetNowActorStatus().maxHp;
	m_playerLerpSpeed = Math::Lerp(num, WHITE_HP_LERP_START, WHITE_HP_LERP_END);

	//白いHPバーの減っていく割合
	m_playerHpWhiteScale = Math::Lerp(
		g_gameTime->GetFrameDeltaTime() * m_playerLerpSpeed,
		m_playerHpWhiteScale, HP_SCALE_END_POS);
	m_playerUI.m_hpWhiteRender.SetScale(m_playerHpWhiteScale);

	//HPフォント
	int HpFont = m_player->GetNowActorStatus().hp;
	wchar_t HP[255];
	swprintf_s(HP, 255, L"HP      %3d", HpFont);
	m_playerUI.m_hpFont.SetText(HP);

	//前フレームのHpスケールを設定
	m_oldPlayerHpScale = HpScale;

	//更新
	m_playerUI.m_hpFrontRender.Update();
	m_playerUI.m_hpWhiteRender.Update();
}

void GameUI::ProcessPlayerMp()
{
	//MPバーの減っていく割合。
	Vector3 MpScale = Vector3::One;
	MpScale = CalcGaugeScale(m_player->GetNowActorStatus().maxMp, m_player->GetNowActorStatus().mp);
	m_playerUI.m_mpFrontRender.SetScale(MpScale);




	//MPフォント
	int MpFont = m_player->GetNowActorStatus().mp;
	wchar_t MP[255];
	swprintf_s(MP, 255, L"MP     %3d", MpFont);
	m_playerUI.m_mpFont.SetText(MP);

	//更新
	m_playerUI.m_mpFrontRender.Update();
}

void GameUI::ProcessBossHP()
{
	//HPバーの減っていく割合。
	Vector3 HpScale = Vector3::One;
	HpScale = CalcGaugeScale(m_lich->GetStatus().maxHp, m_lich->GetStatus().hp);
	m_monsterUI.m_HpFrontRender.SetScale(HpScale);
	//現在のフレームのHPと前フレームのHPのサイズが違うなら
	if (HpScale.x < m_oldBossHpScale.x)
	{
		m_BossHpWhiteScale = m_oldBossHpScale;
	}

	//白いHPバーを減らすための補間率を計算。
	//HPが多いほど遅く少なくなるほど速くする
	float num = m_lich->GetStatus().hp / m_lich->GetStatus().maxHp;
	m_bossLerpSpeed = Math::Lerp(num, WHITE_HP_LERP_START, WHITE_HP_LERP_END);

	//白いHPバーの減っていく割合
	m_BossHpWhiteScale = Math::Lerp(
		g_gameTime->GetFrameDeltaTime() * m_bossLerpSpeed,
		m_BossHpWhiteScale, HP_SCALE_END_POS);
	m_monsterUI.m_HpWhiteRender.SetScale(m_BossHpWhiteScale);

	//ボスのHPの表示
	int NowActorMP = m_lich->GetStatus().hp;
	int NowActorMaxMP = m_lich->GetStatus().maxHp;
	wchar_t MP[255];
	swprintf_s(MP, 255, L"HP %3d/%d", NowActorMP, NowActorMaxMP);
	m_monsterUI.m_hpFont.SetText(MP);

	/*int a = m_lich->GetAccumulationDamage();
	int b = m_lich->GetHitCount();
	wchar_t A[255];
	swprintf_s(A, 255, L"%3d%3d回", a,b);
	m_monsterUI.m_AccumulationDamageFont.SetText(A);*/

	m_oldBossHpScale = HpScale;

	m_monsterUI.m_HpFrontRender.Update();
	m_monsterUI.m_HpWhiteRender.Update();
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

