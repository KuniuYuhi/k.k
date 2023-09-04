#pragma once

class Game;
class Player;
class Lich;

class GameUI:public IGameObject
{
public:
	GameUI();
	~GameUI();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void InitPlayerUI();

	void InitMonsterUI();

	/// <summary>
	/// プレイヤーUIの処理
	/// </summary>
	void PlayerUIUpdate();
	/// <summary>
	/// メインキャラのステータスの更新
	/// </summary>
	void UpdateMainStatus();
	/// <summary>
	/// サブキャラのステータスの更新
	/// </summary>
	void UpdateSubStatus();
	/// <summary>
	/// キャラアイコンの切り替え
	/// </summary>
	bool ChangeCharacterIcon();
	/// <summary>
	/// 
	/// </summary>
	void UpdateCharaIcon();
	/// <summary>
	/// HPやMPなどのゲージのスケールを計算する
	/// </summary>
	/// <param name="Maxvalue">最大値</param>
	/// <param name="value">計算したい値</param>
	/// <returns>サイズ</returns>
	Vector3 CalcGaugeScale(float Maxvalue, float value);

	void CalcChangeCharaIconCoolTime();


	/// <summary>
	/// モンスターのUIの処理
	/// </summary>
	void MonsterUIUpdate();

	/// <summary>
	/// タイマーのUIの処理
	/// </summary>
	void TimerUIUpdate();

	void DrawPlayerUI(RenderContext& rc);

	void DrawMonsterUI(RenderContext& rc);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="player"></param>
	void GetPlayer(Player* player)
	{
		m_player = player;
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="lich"></param>
	void GetLich(Lich* lich)
	{
		m_lich = lich;
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="game"></param>
	void GetGame(Game* game)
	{
		m_game = game;
	}

	/// <summary>
	/// スプライトの座標、サイズ、回転を設定
	/// </summary>
	/// <param name="spriterender">スプライトレンダー</param>
	/// <param name="position">座標</param>
	/// <param name="scale">サイズ</param>
	/// <param name="rotation">回転</param>
	void SettingSpriteRender(
		SpriteRender& spriterender, Vector3 position, Vector3 scale, Quaternion rotation)
	{
		spriterender.SetPosition(position);
		spriterender.SetScale(scale);
		spriterender.SetRotation(rotation);
		spriterender.Update();
	}

private:

	struct MonsterUI
	{
		FontRender m_hpFont;
		FontRender m_AccumulationDamageFont;

		SpriteRender m_IconRender;
		SpriteRender m_HpFlameRender;
		SpriteRender m_HpFrontRender;
		SpriteRender m_HpBackRender;
	};

	struct PlayerUI
	{
		FontRender m_hpFont;
		FontRender m_mpFont;

		SpriteRender m_MainIconRender;
		SpriteRender m_SubIconRender;

		SpriteRender m_MainIconBaseRender;
		SpriteRender m_SubIconBaseRender;

		SpriteRender m_MainStatusBarRender;
		SpriteRender m_SubStatusBarRender;

		SpriteRender m_MainHpFrontRender;
		SpriteRender m_MainHpBackRender;
		SpriteRender m_MainMpFrontRender;
		SpriteRender m_MainMpBackRender;

		SpriteRender m_SubHpFrontRender;
		SpriteRender m_SubHpBackRender;
		SpriteRender m_SubMpFrontRender;
		SpriteRender m_SubMpBackRender;

		SpriteRender m_SkillCenterRender;
		SpriteRender m_Skill_1FlameRender;
		SpriteRender m_Skill_2FlameRender;
		SpriteRender m_Skill_1FlameInsideRender;
		SpriteRender m_Skill_2FlameInsideRender;

		SpriteRender m_SkillButtonXRender;
		SpriteRender m_SkillButtonYRender;

		SpriteRender m_SkillPowerUpRender;
		SpriteRender m_SkillRotarySlashRender;

		SpriteRender m_SkillFireBallRender;
		SpriteRender m_SkillFlamePillarRender;

		SpriteRender m_ChangeCharacterIconRender;
		SpriteRender m_ChangeCharacterIconBlackRender;
		FontRender m_ChangeCharacterCoolTimeFont;

		SpriteRender m_TimeFlameRender;

	};

	FontRender m_TimerFont;				//制限時間

	Game* m_game = nullptr;
	Player* m_player = nullptr;
	Lich* m_lich = nullptr;

	PlayerUI m_playerUI;			//プレイヤーの情報のUI
	MonsterUI m_monsterUI;			//モンスターの情報のUI


	float m_oldMainCharaHP = 0.0f;				//前フレームのメインキャラのHP

	float m_gaugeTimer = 0.0f;

	float m_charaIconChangeTimer = 0.0f;
	
	bool m_coolTimeDrawFlag = true;

	float m_wipeSize = 0;

};

