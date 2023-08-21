#include "stdafx.h"
#include "GameUI.h"
#include "Game.h"
#include "Player.h"
#include "Lich.h"

namespace {

	const Vector3 BOSS_ICON_POS = { -613.0f,450.0f,0.0f };

	const Vector3 BOSS_HP_FLAME_POS = { 0.0f,450.0f,0.0f };
	const Vector3 BOSS_HP_FRONT_POS = { 0.0f,450.0f,0.0f };
	const Vector3 BOSS_HP_BACK_POS = { 0.0f,450.0f,0.0f };

	/// <summary>
	/// プレイヤー側
	/// </summary>

	const Vector2 HP_OR_MP_PIBOT = { 0.0f,0.5f };							//HPかMPのピボット

	const Vector3 MAIN_ICON_POS = { -301.0f, -411.0f, 0.0f };
	const Vector3 MAIN_ICON_BASE_POS = { -301.0f, -411.0f, 0.0f };

	const Vector3 SUB_ICON_POS = { -863.0f, -219.0f, 0.0f };
	const Vector3 SUB_ICON_BASE_POS = { -863.0f, -219.0f, 0.0f };
	const Vector3 SUB_ICON_SCALE = { 0.65f, 0.65f, 0.65f };

	const Vector3 MAIN_STATUS_BAR = { 36.0f, -432.0f, 0.0f };
	const Vector3 SUB_STATUS_BAR = { -682.0f, -215.0f, 0.0f };

	const Vector3 MAIN_HP_FRONT_BAR = { -166.0f, -469.0f, 0.0f };
	const Vector3 MAIN_HP_BACK_BAR = { 101.0f, -468.0f, 0.0f };
	const Vector3 MAIN_MP_FRONT_BAR = { -168.5f, -394.0f, 0.0f };
	const Vector3 MAIN_MP_BACK_BAR = { 73.0f, -394.0f, 0.0f };

	const Vector3 SUB_HP_FRONT_BAR = { -771.0f, -233.0f, 0.0f };
	const Vector3 SUB_HP_BACK_BAR = { -651.0f, -233.0f, 0.0f };
	const Vector3 SUB_MP_FRONT_BAR = { -770.0f, -198.0f, 0.0f };
	const Vector3 SUB_MP_BACK_BAR = { -667.0f, -198.0f, 0.0f };
	//+10+10
	const Vector3 SKILL_CENTER_POS = { 755.0f,-396.0f,0.0f };
	const Vector3 SKILL_1__POS = { 592.0f,-376.0f,0.0f };
	const Vector3 SKILL_2__POS = { 755.0f,-217.0f,0.0f };


	const Vector3 ICON_LERP_CENTER_POS = { -571.0f,300.0f,0.0f };

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

	m_oldMainCharaHP = m_player->GetNowActorStatus().hp;

	return true;
}

void GameUI::Update()
{

	PlayerUIUpdate();

	MonsterUIUpdate();


}

void GameUI::PlayerUIUpdate()
{
	if (ChangeCharacterIcon() == true)
	{
		return;
	}



	UpdateMainStatus();
	UpdateSubStatus();

	UpdateCharaIcon();

}

void GameUI::UpdateMainStatus()
{
	//線形補間
	//HPバーの減っていく割合。
	Vector3 HpScale = Vector3::One;
	HpScale = CalcGaugeScale(m_player->GetNowActorStatus().maxHp, m_player->GetNowActorStatus().hp);

	//前フレームのメインキャラのHPと現在のフレームのメインキャラのHPが違うなら
	//if (m_oldMainCharaHP != m_player->GetNowActorStatus().hp)
	//{
	//	float nowHp = m_player->GetNowActorStatus().hp;
	//	float HpScaleX = 0.0f;
	//	HpScaleX = Math::Lerp(m_gaugeTimer, m_oldMainCharaHP, nowHp);
	//	Vector3 HpScale = Vector3::One;
	//	HpScale.x -= HpScaleX;
	//	m_gaugeTimer += g_gameTime->GetFrameDeltaTime();
	//}
	//else
	//{
	//	//前フレームのメインキャラのHPをセーブ
	//	m_oldMainCharaHP = m_player->GetNowActorStatus().hp;
	//	m_gaugeTimer = 0.0f;
	//}
	
	m_playerUI.m_MainHpFrontRender.SetScale(HpScale);

	//MPバーの減っていく割合。
	Vector3 MpScale = Vector3::One;
	MpScale = CalcGaugeScale(m_player->GetNowActorStatus().maxMp, m_player->GetNowActorStatus().mp);
	m_playerUI.m_MainMpFrontRender.SetScale(MpScale);
	
	//更新
	m_playerUI.m_MainHpFrontRender.Update();
	m_playerUI.m_MainMpFrontRender.Update();
}

void GameUI::UpdateSubStatus()
{
	//HPバーの減っていく割合。
	Vector3 HpScale = Vector3::One;
	HpScale = CalcGaugeScale(m_player->GetSubActorStatus().maxHp, m_player->GetSubActorStatus().hp);
	m_playerUI.m_SubHpFrontRender.SetScale(HpScale);

	//MPバーの減っていく割合。
	Vector3 MpScale = Vector3::One;
	MpScale = CalcGaugeScale(m_player->GetSubActorStatus().maxMp, m_player->GetSubActorStatus().mp);
	m_playerUI.m_SubMpFrontRender.SetScale(MpScale);

	//更新
	m_playerUI.m_SubHpFrontRender.Update();
	m_playerUI.m_SubMpFrontRender.Update();
}

bool GameUI::ChangeCharacterIcon()
{
	//1になったら終わり
	if (m_charaIconChangeTimer >= 1.0f)
	{
		m_player->SetChangCharacterFlagForGameUI(false);
		m_charaIconChangeTimer = 0.0f;
		return false;
	}

	//キャラが切り替わったら
	if (m_player->GetChangCharacterFlagForGameUI() != true)
	{
		return false;
	}

	m_charaIconChangeTimer += g_gameTime->GetFrameDeltaTime()*4.0f;
	if (m_charaIconChangeTimer > 1.0f)
	{
		m_charaIconChangeTimer = 1.0f;
	}

	//メインからサブ
	Vector3 Mpos1;
	Mpos1.Lerp(m_charaIconChangeTimer, MAIN_ICON_POS, ICON_LERP_CENTER_POS);
	Vector3 Mpos2;
	Mpos2.Lerp(m_charaIconChangeTimer, ICON_LERP_CENTER_POS, SUB_ICON_POS);
	Vector3 MainToSub;
	MainToSub.Lerp(m_charaIconChangeTimer, Mpos1, Mpos2);
	//アイコンのスケール
	Vector3 scaleDown;
	scaleDown.Lerp(m_charaIconChangeTimer, g_vec3One, SUB_ICON_SCALE);

	//サブからメイン
	Vector3 Spos1;
	Spos1.Lerp(m_charaIconChangeTimer, SUB_ICON_POS, ICON_LERP_CENTER_POS);
	Vector3 Spos2;
	Spos2.Lerp(m_charaIconChangeTimer, ICON_LERP_CENTER_POS, MAIN_ICON_POS);
	Vector3 SubToMain;
	SubToMain.Lerp(m_charaIconChangeTimer, SUB_ICON_POS, MAIN_ICON_POS);
	//アイコンのスケール
	Vector3 scaleUp;
	scaleUp.Lerp(m_charaIconChangeTimer, SUB_ICON_SCALE, g_vec3One);

	if (m_player->GetActiveCharacter() != Player::enHero)
	{
		m_playerUI.m_MainIconRender.SetScale(scaleDown);
		m_playerUI.m_MainIconRender.SetPosition(MainToSub);
	}
	else
	{
		m_playerUI.m_SubIconRender.SetScale(scaleDown);
		m_playerUI.m_SubIconRender.SetPosition(MainToSub);
	}

	if (m_player->GetActiveCharacter() == Player::enHero)
	{
		m_playerUI.m_MainIconRender.SetScale(scaleUp);
		m_playerUI.m_MainIconRender.SetPosition(SubToMain);
	}
	else
	{
		m_playerUI.m_SubIconRender.SetScale(scaleUp);
		m_playerUI.m_SubIconRender.SetPosition(SubToMain);
	}

	
	m_playerUI.m_MainIconRender.Update();
	m_playerUI.m_SubIconRender.Update();

	
	
	return true;
	

}

void GameUI::UpdateCharaIcon()
{
	m_playerUI.m_MainIconRender.SetGrayScale(true);


	/*if (m_player->GetNowActorDieFlag()==true)
	{
		m_playerUI.m_MainIconRender.SetGrayScale(true);
	}

	if (m_player->GetSubActorDieFlag()==true)
	{
		m_playerUI.m_SubIconRender.SetGrayScale(true);
	}*/

}

Vector3 GameUI::CalcGaugeScale(float Maxvalue, float value)
{
	Vector3 scale = g_vec3One;
	scale.x = value / Maxvalue;
	return scale;
}

void GameUI::MonsterUIUpdate()
{
	if (m_lich == nullptr)
	{
		return;
	}

	//ボスのHPの表示
	int NowActorMP = m_lich->GetStatus().hp;
	int NowActorMaxMP = m_lich->GetStatus().maxHp;
	wchar_t MP[255];
	swprintf_s(MP, 255, L"ボス HP %3d/%d", NowActorMP, NowActorMaxMP);
	m_monsterUI.m_hpFont.SetText(MP);

	int a = m_lich->GetAccumulationDamage();
	int b = m_lich->GetHitCount();
	wchar_t A[255];
	swprintf_s(A, 255, L"%3d%3d回", a,b);
	m_monsterUI.m_AccumulationDamageFont.SetText(A);

}

void GameUI::DrawPlayerUI(RenderContext& rc)
{
	//m_playerUI.m_hpFont.Draw(rc);
	//m_playerUI.m_mpFont.Draw(rc);
	//メインHP
	m_playerUI.m_MainHpBackRender.Draw(rc);
	m_playerUI.m_MainHpFrontRender.Draw(rc);
	//メインMP
	m_playerUI.m_MainMpBackRender.Draw(rc);
	m_playerUI.m_MainMpFrontRender.Draw(rc);
	//サブHP
	m_playerUI.m_SubHpBackRender.Draw(rc);
	m_playerUI.m_SubHpFrontRender.Draw(rc);
	//サブMP
	m_playerUI.m_SubMpBackRender.Draw(rc);
	m_playerUI.m_SubMpFrontRender.Draw(rc);


	//メインステータスバー
	m_playerUI.m_MainStatusBarRender.Draw(rc);
	//サブステータスバー
	m_playerUI.m_SubStatusBarRender.Draw(rc);
	//メインアイコンベース
	m_playerUI.m_MainIconBaseRender.Draw(rc);
	//サブアイコンベース
	m_playerUI.m_SubIconBaseRender.Draw(rc);
	//スキルの真ん中の◇
	m_playerUI.m_SkillCenterRender.Draw(rc);
	//スキル１のフレーム
	m_playerUI.m_Skill_1FlameRender.Draw(rc);
	//スキル２のフレーム
	m_playerUI.m_Skill_2FlameRender.Draw(rc);
	//スキル１の内側のフレーム
	m_playerUI.m_Skill_1FlameInsideRender.Draw(rc);
	//スキル２の内側のフレーム
	m_playerUI.m_Skill_2FlameInsideRender.Draw(rc);

	//メインアイコン
	m_playerUI.m_MainIconRender.Draw(rc);
	//サブアイコン
	m_playerUI.m_SubIconRender.Draw(rc);

	//キャラによって切り替える
	if (m_player->GetActiveCharacter() == Player::enHero)
	{
		//ヒーローのスキル
		m_playerUI.m_SkillRotarySlashRender.Draw(rc);
		m_playerUI.m_SkillPowerUpRender.Draw(rc);
	}
	else
	{
		//ウィザードのスキル
		m_playerUI.m_SkillFlamePillarRender.Draw(rc);
		m_playerUI.m_SkillFireBallRender.Draw(rc);
	}
	
	

}

void GameUI::DrawMonsterUI(RenderContext& rc)
{
	if (m_lich == nullptr)
	{
		return;
	}

	m_monsterUI.m_hpFont.Draw(rc);
	m_monsterUI.m_AccumulationDamageFont.Draw(rc);

	//アイコン
	m_monsterUI.m_IconRender.Draw(rc);
	//HPのフレーム
	m_monsterUI.m_HpFlameRender.Draw(rc);
	//HP
	m_monsterUI.m_HpBackRender.Draw(rc);
	m_monsterUI.m_HpFrontRender.Draw(rc);
}

void GameUI::Render(RenderContext& rc)
{
	DrawPlayerUI(rc);
	DrawMonsterUI(rc);
}

void GameUI::InitPlayerUI()
{
	//キャラアイコン
	m_playerUI.m_MainIconRender.Init("Assets/sprite/InGame/Character/Icon_Hero.DDS",219,219);
	SettingSpriteRender(
		m_playerUI.m_MainIconRender, MAIN_ICON_POS, g_vec3One, g_quatIdentity);
	m_playerUI.m_SubIconRender.Init("Assets/sprite/InGame/Character/Icon_Wizard.DDS", 219, 219);
	SettingSpriteRender(
		m_playerUI.m_SubIconRender, SUB_ICON_POS, Vector3(0.65f,0.65f,0.65f), g_quatIdentity);

	//アイコンベース
	m_playerUI.m_MainIconBaseRender.Init("Assets/sprite/InGame/Character/Icon_Base_Main.DDS", 250, 250);
	SettingSpriteRender(
		m_playerUI.m_MainIconBaseRender, MAIN_ICON_BASE_POS, g_vec3One, g_quatIdentity);
	m_playerUI.m_SubIconBaseRender.Init("Assets/sprite/InGame/Character/Icon_Base_Sub.DDS", 185, 185);
	SettingSpriteRender(
		m_playerUI.m_SubIconBaseRender, SUB_ICON_BASE_POS, Vector3(0.9f, 0.9f, 0.9f), g_quatIdentity);

	//ステータスバー
	m_playerUI.m_MainStatusBarRender.Init("Assets/sprite/InGame/Character/StatusBar_Main.DDS", 720, 206);
	SettingSpriteRender(
		m_playerUI.m_MainStatusBarRender, MAIN_STATUS_BAR, g_vec3One, g_quatIdentity);
	m_playerUI.m_SubStatusBarRender.Init("Assets/sprite/InGame/Character/StatusBar_Sub.DDS", 320, 104);
	SettingSpriteRender(
		m_playerUI.m_SubStatusBarRender, SUB_STATUS_BAR, g_vec3One, g_quatIdentity);

	//メインHP
	m_playerUI.m_MainHpFrontRender.Init("Assets/sprite/InGame/Character/HP_Front_Main.DDS", 550, 72);
	m_playerUI.m_MainHpFrontRender.SetPivot(HP_OR_MP_PIBOT);
	SettingSpriteRender(
		m_playerUI.m_MainHpFrontRender, MAIN_HP_FRONT_BAR, g_vec3One, g_quatIdentity);
	m_playerUI.m_MainHpBackRender.Init("Assets/sprite/InGame/Character/HP_Back_Main.DDS", 585, 63);
	SettingSpriteRender(
		m_playerUI.m_MainHpBackRender, MAIN_HP_BACK_BAR, g_vec3One, g_quatIdentity);

	//メインMP
	m_playerUI.m_MainMpFrontRender.Init("Assets/sprite/InGame/Character/MP_Front_Main.DDS", 483, 53);
	m_playerUI.m_MainMpFrontRender.SetPivot(HP_OR_MP_PIBOT);
	SettingSpriteRender(
		m_playerUI.m_MainMpFrontRender, MAIN_MP_FRONT_BAR, g_vec3One, g_quatIdentity);
	m_playerUI.m_MainMpBackRender.Init("Assets/sprite/InGame/Character/MP_Back_Main.DDS", 483, 53);
	SettingSpriteRender(
		m_playerUI.m_MainMpBackRender, MAIN_MP_BACK_BAR, g_vec3One, g_quatIdentity);

	//サブHP
	m_playerUI.m_SubHpFrontRender.Init("Assets/sprite/InGame/Character/HP_Front_Sub.DDS", 242, 32);
	m_playerUI.m_SubHpFrontRender.SetPivot(HP_OR_MP_PIBOT);
	SettingSpriteRender(
		m_playerUI.m_SubHpFrontRender, SUB_HP_FRONT_BAR, g_vec3One, g_quatIdentity);
	m_playerUI.m_SubHpBackRender.Init("Assets/sprite/InGame/Character/HP_Back_Sub.DDS", 244, 32);
	SettingSpriteRender(
		m_playerUI.m_SubHpBackRender, SUB_HP_BACK_BAR, g_vec3One, g_quatIdentity);
	//サブMP
	m_playerUI.m_SubMpFrontRender.Init("Assets/sprite/InGame/Character/MP_Front_Sub.DDS", 206, 31);
	m_playerUI.m_SubMpFrontRender.SetPivot(HP_OR_MP_PIBOT);
	SettingSpriteRender(
		m_playerUI.m_SubMpFrontRender, SUB_MP_FRONT_BAR, g_vec3One, g_quatIdentity);
	m_playerUI.m_SubMpBackRender.Init("Assets/sprite/InGame/Character/MP_Back_Sub.DDS", 206, 31);
	SettingSpriteRender(
		m_playerUI.m_SubMpBackRender, SUB_MP_BACK_BAR, g_vec3One, g_quatIdentity);

	//スキルの真ん中の◇
	m_playerUI.m_SkillCenterRender.Init("Assets/sprite/InGame/Character/Skill_Center.DDS", 400, 400);
	SettingSpriteRender(
		m_playerUI.m_SkillCenterRender, SKILL_CENTER_POS, Vector3(0.7f,0.7f,0.7f), g_quatIdentity);
	//スキル１のフレーム
	m_playerUI.m_Skill_1FlameRender.Init("Assets/sprite/InGame/Character/Skill_Flame.DDS", 285, 285);
	SettingSpriteRender(
		m_playerUI.m_Skill_1FlameRender, SKILL_1__POS, Vector3(0.7f, 0.7f, 0.7f), g_quatIdentity);
	//スキル１の内側のフレーム
	m_playerUI.m_Skill_1FlameInsideRender.Init("Assets/sprite/InGame/Character/Skill_Flame_Inside.DDS",262, 262);
	SettingSpriteRender(
		m_playerUI.m_Skill_1FlameInsideRender, SKILL_1__POS, Vector3(0.7f, 0.7f, 0.7f), g_quatIdentity);
	//スキル２のフレーム
	m_playerUI.m_Skill_2FlameRender.Init("Assets/sprite/InGame/Character/Skill_Flame.DDS", 285, 285);
	SettingSpriteRender(
		m_playerUI.m_Skill_2FlameRender, SKILL_2__POS, Vector3(0.7f, 0.7f, 0.7f), g_quatIdentity);
	//スキル２の内側のフレーム
	m_playerUI.m_Skill_2FlameInsideRender.Init("Assets/sprite/InGame/Character/Skill_Flame_Inside.DDS", 262, 262);
	SettingSpriteRender(
		m_playerUI.m_Skill_2FlameInsideRender, SKILL_2__POS, Vector3(0.7f, 0.7f, 0.7f), g_quatIdentity);

	//ヒーローのスキル１　回転斬り
	m_playerUI.m_SkillRotarySlashRender.Init("Assets/sprite/InGame/Character/Icon_RotarySlash.DDS", 224, 224);
	SettingSpriteRender(
		m_playerUI.m_SkillRotarySlashRender, SKILL_1__POS, Vector3(0.7f, 0.7f, 0.7f), g_quatIdentity);
	//ヒーローのスキル２　パワーアップ
	m_playerUI.m_SkillPowerUpRender.Init("Assets/sprite/InGame/Character/Icon_PowerUp.DDS", 262, 262);
	SettingSpriteRender(
		m_playerUI.m_SkillPowerUpRender, SKILL_2__POS, Vector3(0.7f, 0.7f, 0.7f), g_quatIdentity);

	//ウィザードのスキル１　フレイムピラー
	m_playerUI.m_SkillFlamePillarRender.Init("Assets/sprite/InGame/Character/Icon_Flamepillar.DDS", 224, 224);
	SettingSpriteRender(
		m_playerUI.m_SkillFlamePillarRender, SKILL_1__POS, Vector3(0.7f, 0.7f, 0.7f), g_quatIdentity);
	//ウィザードのスキル２　ファイヤーボール
	m_playerUI.m_SkillFireBallRender.Init("Assets/sprite/InGame/Character/Icon_FireBall.DDS", 224, 224);
	SettingSpriteRender(
		m_playerUI.m_SkillFireBallRender, SKILL_2__POS, Vector3(0.7f, 0.7f, 0.7f), g_quatIdentity);


}

void GameUI::InitMonsterUI()
{
	m_monsterUI.m_hpFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_monsterUI.m_hpFont.SetScale(1.5f);
	m_monsterUI.m_hpFont.SetPosition(-800.0f, 500.0f);

	m_monsterUI.m_AccumulationDamageFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_monsterUI.m_AccumulationDamageFont.SetScale(1.5f);
	m_monsterUI.m_AccumulationDamageFont.SetPosition(0.0f, 500.0f);

	//ボスのアイコン
	m_monsterUI.m_IconRender.Init("Assets/sprite/InGame/Character/Icon_Lich.DDS", 180, 180);
	SettingSpriteRender(
		m_monsterUI.m_IconRender, BOSS_ICON_POS, g_vec3One, g_quatIdentity);
	//ボスのHPのフレーム
	m_monsterUI.m_HpFlameRender.Init("Assets/sprite/InGame/Character/HP_Flame_Boss.DDS", 1000, 80);
	SettingSpriteRender(
		m_monsterUI.m_HpFlameRender, BOSS_HP_FLAME_POS, g_vec3One, g_quatIdentity);
	//ボスのHP
	m_monsterUI.m_HpFrontRender.Init("Assets/sprite/InGame/Character/HP_Front_Boss.DDS", 978, 57);
	SettingSpriteRender(
		m_monsterUI.m_HpFrontRender, BOSS_HP_FRONT_POS, g_vec3One, g_quatIdentity);
	m_monsterUI.m_HpBackRender.Init("Assets/sprite/InGame/Character/HP_Back_Boss.DDS", 978, 57);
	SettingSpriteRender(
		m_monsterUI.m_HpBackRender, BOSS_HP_BACK_POS, g_vec3One, g_quatIdentity);
}

