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
}

bool GameUI::Start()
{
	//プレイヤーのUIの初期化
	InitPlayerUI();
	//モンスターのUIの初期化
	InitMonsterUI();
	//フェーズUIの初期化
	InitPhaseUI();

	//制限時間
	InitFontRender(m_timerFont, TIMER_POS, 1.1f);
	m_timerFont.SetOffset(TIMER_OFFSET);

	//制限時間の枠
	InitSpriteRender(
		m_timeFlameRender, "Assets/sprite/InGame/Character/TimeFlame2.DDS", 500, 124, TIME_FLAME_POS, TIME_FLAME_SIZE
	);


	



	m_nowPlayerWhiteHp = m_player->GetNowActorStatus().GetMaxHp();

	return true;
}

void GameUI::Update()
{
	//制限時間
	TimerUIUpdate();
	//フェーズの処理
	ProcessPhase();
	//プレイヤーのUIの処理
	PlayerUIUpdate();
	//ボスのUIの処理
	MonsterUIUpdate();
}

void GameUI::PlayerUIUpdate()
{
	//ステータス
	UpdateMainStatus();
	//ウェポン
	UpdateWeapon();
}

void GameUI::MonsterUIUpdate()
{
	//ボスがいないなら処理しない
	if (m_boss == nullptr)
	{
		return;
	}

	//ボスのHPの処理
	ProcessBossHP();
	//ボスのスーパーアーマーの処理
	ProcessBossSuperArmor();
}

void GameUI::UpdateMainStatus()
{
	//HPの処理
	ProcessPlayerHp();
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

	//耐久値のフォントの処理
	ProcessWeaponEndranceFont();
	
	//画像の更新
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
	//減っていく割合を計算
	scale.x = value / Maxvalue;
	return scale;
}

void GameUI::TimerUIUpdate()
{
	//分の取得
	int minute = GameManager::GetInstance()->GetMinute();
	//秒の取得
	int second = GameManager::GetInstance()->GetSecond();

	wchar_t time[256];
	swprintf_s(time, 256, L"%d:%02d", minute, second);
	//テキストを設定
	m_timerFont.SetText(time);
}

void GameUI::ProcessPlayerHp()
{
	//最大HPと現在のHPを取得
	int maxHP = m_player->GetNowActorStatus().GetMaxHp();
	int nowHP = m_player->GetNowActorStatus().GetHp();

	//HPバーの減っていく割合。
	Vector3 HpScale = Vector3::One;
	//HPのスケールを計算
	HpScale = CalcGaugeScale(maxHP, nowHP);
	//スケールを設定
	m_playerUI.m_hpFrontRender.SetScale(HpScale);
	//HPフォントの設定
	wchar_t HP[255];
	//現在のHPを表示
	swprintf_s(HP, 255, L"HP      %3d", nowHP);
	//テキストを設定
	m_playerUI.m_hpFont.SetText(HP);

	//白いHPが現在のHPより小さいなら
	if (m_nowPlayerWhiteHp < nowHP)
	{
		//白いHPに現在のHPを代入する
		m_nowPlayerWhiteHp = nowHP;
	}
	else
	{
		//白いHPを減らす
		m_nowPlayerWhiteHp -= g_gameTime->GetFrameDeltaTime() * WHITE_HP_SPEED;
	}
	//白いHPのスケールを計算
	m_playerWhiteHpScale = CalcGaugeScale(maxHP, m_nowPlayerWhiteHp);
	//スケールを設定
	m_playerUI.m_hpWhiteRender.SetScale(m_playerWhiteHpScale);

	//更新
	m_playerUI.m_hpFrontRender.Update();
	m_playerUI.m_hpWhiteRender.Update();
}

void GameUI::ProcessBossHP()
{
	//最大HPと現在のHPを取得
	int maxHP = m_boss->GetStatus().GetMaxHp();
	int nowHP = m_boss->GetStatus().GetHp();

	//HPバーの減っていく割合。
	Vector3 HpScale = Vector3::One;
	//HPのスケールを計算
	HpScale = CalcGaugeScale(maxHP, nowHP);
	//スケールを設定
	m_monsterUI.m_hpFrontRender.SetScale(HpScale);
	//ボスのHPの表示
	wchar_t MP[255];
	//現在のHPを表示
	swprintf_s(MP, 255, L"HP %3d/%d", nowHP, maxHP);
	//テキストを設定
	m_monsterUI.m_hpFont.SetText(MP);

	/*int a = m_lich->GetAccumulationDamage();
	int b = m_lich->GetHitCount();
	wchar_t A[255];
	swprintf_s(A, 255, L"%3d%3d回", a,b);
	m_monsterUI.m_AccumulationDamageFont.SetText(A);*/


	//白いHPが現在のHPより小さいなら
	if (m_nowBossWhiteHp < nowHP)
	{
		//白いHPに現在のHPを代入する
		m_nowBossWhiteHp = nowHP;
	}
	else
	{
		//白いHPを減らす
		m_nowBossWhiteHp -= g_gameTime->GetFrameDeltaTime() * BOSS_WHITE_HP_SPEED;
	}
	//白いHPのスケールを計算
	m_bossHpWhiteScale = CalcGaugeScale(maxHP, m_nowBossWhiteHp);
	//スケールを設定
	m_monsterUI.m_hpWhiteRender.SetScale(m_bossHpWhiteScale);


	
	//更新
	m_monsterUI.m_hpFrontRender.Update();
	m_monsterUI.m_hpWhiteRender.Update();
}

void GameUI::ProcessBossSuperArmor()
{
	//スーパーアーマーのバーの減っていく割合。
	Vector3 spuerArmorScale = Vector3::One;
	spuerArmorScale = CalcGaugeScale(
		m_boss->GetStatus().GetMaxSuperArmorPoint(), m_boss->GetStatus().GetSuperArmorPoint());
	m_monsterUI.m_superArmor_FrontBarRender.SetScale(spuerArmorScale);

	//スーパーアーマーがブレイクしている時は、ゲージの色を暗くする
	if (m_boss->GetSuperArmorBreakFlag() == true)
	{
		m_monsterUI.m_superArmor_FrontBarRender.SetMulColor(SUPERSARMOR_GRAY_COLOR);
	}
	else
	{
		m_monsterUI.m_superArmor_FrontBarRender.SetMulColor(g_vec4White);
	}

	//更新
	m_monsterUI.m_superArmor_FrontBarRender.Update();
}

void GameUI::ProcessPhase()
{
	//現在のフェーズを取得
	int PhaseNumber = GameManager::GetInstance()->GetNowPhaseState();
	wchar_t NowPhase[255];
	swprintf_s(NowPhase, 255, L"%d", PhaseNumber+1);
	//テキストを設定
	m_phaseUI.m_nowPhaseFont.SetText(NowPhase);


	//現在の敵の数を取得
	int monsterNum = CharactersInfoManager::GetInstance()->GetMobMonsters().size();
	wchar_t NowMonsterNum[255];
	swprintf_s(NowMonsterNum, 255, L"%d", monsterNum);
	//テキストを設定
	m_phaseUI.m_nowPhaseMonstersFont.SetText(NowMonsterNum);
	//テキストのオフセット量を計算
	CalcOffsetForNowPhaseMonsters(monsterNum);


	//タイマーの現在の値と最大値を取得
	float nowTimer = GameManager::GetInstance()->GetPhaseTimer();
	float timerLimmit = GameManager::GetInstance()->GetNowPhaseTimerLimmit();

	//タイムバーの減っていく割合。
	Vector3 phaseTimerScale = Vector3::One;
	//タイムバーのスケールを計算
	phaseTimerScale = CalcGaugeScale(timerLimmit, nowTimer);
	//スケールを設定
	m_phaseUI.m_phaseTimeBarRender.SetScale(phaseTimerScale);
	m_phaseUI.m_phaseTimeBarRender.Update();
}

void GameUI::CalcOffsetForNowPhaseMonsters(int monsters)
{
	//桁が変わらないならオフセット処理をしない
	if (monsters == m_oldPhaseMonstersNum)
	{
		return;
	}
	//敵の数が二桁と一桁があるのでオフセットを計算
	Vector2 offset = CalcNumberCount(monsters, NOW_PHASE_MONSTERS_DEFAULT_X_OFFSET, NOW_PHASE_MONSTERS_DEFAULT_Y_OFFSET);
	//オフセットを設定
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

	//wchar_t型をstring型に変換
	//std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	//std::string str = converter.to_bytes(endrance);
	////文字列の長さを取得
	//int length= str.length();
	//m_playerUI.m_weaponEndranceFont.SetOffset({ length/2.0f ,0.0f });

	m_playerUI.m_weaponEndranceFont.SetText(endrance);

	int harfEndrance = m_player->GetNowWeaponMaxEndrance() / 2;

	//半分より上なら白色にする
	if (m_player->GetNowWeaponEndrance() > harfEndrance)
	{
		m_playerUI.m_weaponEndranceFont.SetColor(g_vec4White);
	}
	//耐久値の半分の半分(四分の一)だったら赤色にする
	else if (m_player->GetNowWeaponEndrance() <= harfEndrance / 2)
	{
		m_playerUI.m_weaponEndranceFont.SetColor(g_vec4Red);
	}
	//半分以下なら黄色にする
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
		//引数の桁数を取得
		digitCount = static_cast<int>(log10(num) + 1);
	}

	//オフセットを計算
	Vector2 offset = g_vec2Zero;
	//Xオフセットを計算
	offset.x = xOffset * digitCount;
	//Yオフセットを計算
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
	//メインHP
	m_playerUI.m_hpBackRender.Draw(rc);

	if (m_playerWhiteHpScale.x > 0.0f)
	{
		m_playerUI.m_hpWhiteRender.Draw(rc);
	}


	m_playerUI.m_hpFrontRender.Draw(rc);

	//メイン
	m_playerUI.m_hpFlameRender.Draw(rc);

	//HPのフォント
	m_playerUI.m_hpFont.Draw(rc);


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
	//耐久値の背景
	for (int num = 0; num < enWeapon_num; num++)
	{
		m_weaponSprits[num].m_weaponRender->Draw(rc);
	}

	if (m_weaponSprits[enWeapon_Main].m_weaponEndranceRender != nullptr)
	{
		m_weaponSprits[enWeapon_Main].m_weaponEndranceRender->Draw(rc);
	}

	//武器の耐久値が-１より大きかったら描画
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

	//HPのフレーム
	m_monsterUI.m_hpFlameRender.Draw(rc);

	//HPのフォント
	m_monsterUI.m_hpFont.Draw(rc);
	m_monsterUI.m_AccumulationDamageFont.Draw(rc);

	//スーパーアーマー
	m_monsterUI.m_superArmor_FlameRender.Draw(rc);
	m_monsterUI.m_superArmor_BackBarRender.Draw(rc);
	m_monsterUI.m_superArmor_FrontBarRender.Draw(rc);
}

void GameUI::InitPlayerUI()
{
	//HPの値
	InitFontRender(m_playerUI.m_hpFont, HP_FONT_POS, 1.3f);

	//ステータスバー
	InitSpriteRender(
		m_playerUI.m_hpFlameRender, "Assets/sprite/InGame/Character/Player_HP_Flame.DDS", 608, 88, MAIN_STATUS_BAR);

	//HPバー
	InitSpriteRender(
		m_playerUI.m_hpFrontRender, "Assets/sprite/InGame/Character/Player_HP_Front.DDS", 588, 78, MAIN_HP_BAR);
	//ピボットの設定
	m_playerUI.m_hpFrontRender.SetPivot(GAUGE_PIBOT);

	//白いHPバー
	InitSpriteRender(
		m_playerUI.m_hpWhiteRender, "Assets/sprite/InGame/Character/Player_HP_White.DDS", 588, 78, MAIN_HP_BAR);
	//ピボットの設定
	m_playerUI.m_hpWhiteRender.SetPivot(GAUGE_PIBOT);

	//HPバーの裏側
	InitSpriteRender(
		m_playerUI.m_hpBackRender, "Assets/sprite/InGame/Character/Player_HP_Back.DDS", 600, 80, MAIN_STATUS_BAR);

	//メイン武器のアイコン
	InitSpriteRender(
		m_playerUI.m_weaponRender[enWeapon_Main],
		"Assets/sprite/InGame/Character/SwordShield.DDS", 256, 256,
		m_weaponIconPos[enWeapon_Main], m_weaponIconScale[enWeapon_Main]
	);
	m_weaponSprits[enWeapon_Main].m_weaponRender = &m_playerUI.m_weaponRender[enWeapon_Main];

	//メイン武器のフレーム
	InitSpriteRender(
		m_playerUI.m_mainWeaponFlameRender, "Assets/sprite/InGame/Character/MainWeaponFlame.DDS", 238, 237,
		m_weaponIconPos[enWeapon_Main], g_vec3One * 1.1f
	);

	//サブ武器１のアイコン
	InitSpriteRender(
		m_playerUI.m_weaponRender[enWeapon_Sub],
		"Assets/sprite/InGame/Character/GreatSword.DDS", 300, 300,
		m_weaponIconPos[enWeapon_Sub], m_weaponIconScale[enWeapon_Sub]
	);
	m_weaponSprits[enWeapon_Sub].m_weaponRender = &m_playerUI.m_weaponRender[enWeapon_Sub];

	//サブ武器１のフレーム
	InitSpriteRender(
		m_playerUI.m_subWeaponFlameRender, "Assets/sprite/InGame/Character/SubWeaponFlame.DDS", 156, 155,
		m_weaponIconPos[enWeapon_Sub], g_vec3One * 0.9f
	);
	//サブ武器１のコマンド
	InitSpriteRender(
		m_playerUI.m_subWeaponCommandRender, "Assets/sprite/InGame/Character/Button_LB.DDS", 80, 60,
		COMMAND_LB, g_vec3One
	);

	//サブ武器２のアイコン
	InitSpriteRender(
		m_playerUI.m_weaponRender[enWeapon_Sub2],
		"Assets/sprite/InGame/Character/BowArrow.DDS", 290, 290,
		m_weaponIconPos[enWeapon_Sub2], m_weaponIconScale[enWeapon_Sub2]
	);
	m_weaponSprits[enWeapon_Sub2].m_weaponRender = &m_playerUI.m_weaponRender[enWeapon_Sub2];

	//サブ武器２のフレーム
	InitSpriteRender(
		m_playerUI.m_sub2WeaponFlameRender, "Assets/sprite/InGame/Character/SubWeaponFlame.DDS", 156, 155,
		m_weaponIconPos[enWeapon_Sub2], g_vec3One * 0.9f
	);
	//サブ武器２のコマンド
	InitSpriteRender(
		m_playerUI.m_sub2WeaponCommandRender, "Assets/sprite/InGame/Character/Button_RB.DDS", 80, 60,
		COMMAND_RB, g_vec3One
	);

	//メイン武器(ソード＆シールド)の耐久値の背景
	InitSpriteRender(m_playerUI.m_weaponEndranceRender[enWeapon_Main],
		"Assets/sprite/InGame/Character/Shield_Endurance.DDS", 134, 132,
		ENDURANCE_SPRITE_POS, g_vec3One);
	m_weaponSprits[enWeapon_Main].m_weaponEndranceRender = &m_playerUI.m_weaponEndranceRender[enWeapon_Main];
	//サブ武器２(ボウ＆アロー)の耐久値の背景
	InitSpriteRender(m_playerUI.m_weaponEndranceRender[enWeapon_Sub2],
		"Assets/sprite/InGame/Character/ArrowStock.DDS", 177, 182,
		ENDURANCE_SPRITE_POS, g_vec3One);
	m_weaponSprits[enWeapon_Sub2].m_weaponEndranceRender = &m_playerUI.m_weaponEndranceRender[enWeapon_Sub2];

	//耐久力のフォント
	InitFontRender(
		m_playerUI.m_weaponEndranceFont, ENDURANCE_FONT_POS, 1.5f
	);
	//耐久値のオフセット
	m_playerUI.m_weaponEndranceFont.SetOffset({ 20.0f,0.0f });

}

void GameUI::InitMonsterUI()
{
	//HPの値
	InitFontRender(m_monsterUI.m_hpFont, BOSS_HP_FONT_POS, 1.1f);

	//確認用
	InitFontRender(m_monsterUI.m_AccumulationDamageFont, { 0.0f, 500.0f });

	//ボスのHPのフレーム
	InitSpriteRender(
		m_monsterUI.m_hpFlameRender,
		"Assets/sprite/InGame/Character/HP_Flame_Boss.DDS", 1000, 60, BOSS_HP_FLAME_POS
	);

	//ボスのHPバー
	InitSpriteRender(
		m_monsterUI.m_hpFrontRender,
		"Assets/sprite/InGame/Character/HP_Front_Boss.DDS", 978, 47, BOSS_HP_FRONT_POS
	);
	//ピボットの設定
	m_monsterUI.m_hpFrontRender.SetPivot(GAUGE_PIBOT);

	//ボスの白いHPバー
	InitSpriteRender(
		m_monsterUI.m_hpWhiteRender,
		"Assets/sprite/InGame/Character/HP_White_Boss.DDS", 978, 47, BOSS_HP_FRONT_POS
	);
	//ピボットの設定
	m_monsterUI.m_hpWhiteRender.SetPivot(GAUGE_PIBOT);

	//HPバーの裏側
	InitSpriteRender(
		m_monsterUI.m_hpBackRender,
		"Assets/sprite/InGame/Character/HP_Back_Boss.DDS", 978, 47, BOSS_HP_BACK_POS
	);

	//スーパーアーマーのフレーム
	InitSpriteRender(
		m_monsterUI.m_superArmor_FlameRender,
		"Assets/sprite/InGame/Character/SuperArmor_Flame.DDS", 670, 26, BOSS_SUPERARMOR_POS
	);

	//スーパーアーマーの変動するバー
	InitSpriteRender(
		m_monsterUI.m_superArmor_FrontBarRender,
		"Assets/sprite/InGame/Character/SuperArmor_Front.DDS", 666, 24, BOSS_SUPERARMOR_FLONT_BAR_POS
	);
	//ピボットの設定
	m_monsterUI.m_superArmor_FrontBarRender.SetPivot(GAUGE_PIBOT);

	//スーパーアーマーの裏のバー
	InitSpriteRender(
		m_monsterUI.m_superArmor_BackBarRender,
		"Assets/sprite/InGame/Character/SuperArmor_Back.DDS", 664, 22, BOSS_SUPERARMOR_POS
	);

}

void GameUI::InitPhaseUI()
{
	//フェーズのフレーム
	InitSpriteRender(
		m_phaseUI.m_phaseFlameRender,
		"Assets/sprite/InGame/Wave/Wave_Flame.DDS", 500, 300,
		PHASE_FLAME_POS, g_vec3One
	);
	//現在のフェーズの数の文字
	InitFontRender(
		m_phaseUI.m_nowPhaseFont, NOW_PHASE_FONT_POS, NOW_PHASE_SIZE);
	//現在のフェーズのモンスターの数
	InitFontRender(
		m_phaseUI.m_nowPhaseMonstersFont, NOW_PHASE_MONSTERS_POS, NOW_PHASE_MONSTERS_SIZE);
	//オフセットを計算
	Vector2 offset = CalcNumberCount(1, NOW_PHASE_MONSTERS_DEFAULT_X_OFFSET, NOW_PHASE_MONSTERS_DEFAULT_Y_OFFSET);
	//オフセットを設定
	m_phaseUI.m_nowPhaseMonstersFont.SetOffset(offset);

	//フェーズの残り時間のフレーム
	InitSpriteRender(
		m_phaseUI.m_phaseTimeFlameRender,
		"Assets/sprite/InGame/Wave/Wave_time_Flame.DDS", 350, 46,
		PHASE_TIME_FLAME_POS, g_vec3One
	);
	//フェーズの残り時間のバー
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