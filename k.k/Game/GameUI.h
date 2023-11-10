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
	/// メインキャラのステータスの更新処理
	/// </summary>
	void UpdateMainStatus();
	/// <summary>
	/// キャラアイコンの更新処理
	/// </summary>
	void UpdateCharaIcon();
	/// <summary>
	/// HPやMPなどのゲージのスケールを計算する
	/// </summary>
	/// <param name="Maxvalue">最大値</param>
	/// <param name="value">計算したい値</param>
	/// <returns>サイズ</returns>
	Vector3 CalcGaugeScale(float Maxvalue, float value);

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
	/// プレイヤーインスタンスの取得
	/// </summary>
	/// <param name="player"></param>
	void GetPlayer(Player* player)
	{
		m_player = player;
	}
	/// <summary>
	/// リッチ(ボス)のインスタンスの取得
	/// </summary>
	/// <param name="lich"></param>
	void GetLich(Lich* lich)
	{
		m_lich = lich;
	}
	/// <summary>
	/// ゲームインスタンスの取得
	/// </summary>
	/// <param name="game"></param>
	void GetGame(Game* game)
	{
		m_game = game;
	}

	/// <summary>
	/// スプライトレンダーの初期化。
	/// </summary>
	/// <param name="spriterender">初期化したいスプライトレンダー</param>
	/// <param name="filePath">画像のファイルパス</param>
	/// <param name="width">画像の幅</param>
	/// <param name="height">画像の高さ</param>
	/// <param name="position">座標</param>
	/// <param name="scale">サイズ</param>
	/// <param name="rotation">回転</param>
	void InitSpriteRender(
		SpriteRender& spriterender,
		const char* filePath,
		const float width, const float height,
		Vector3 position,
		Vector3 scale = g_vec3One,
		Quaternion rotation = g_quatIdentity
	);
	/// <summary>
	/// フォントレンダーの初期化。
	/// </summary>
	/// <param name="fontRender">初期化したいフォントレンダー</param>
	/// <param name="position">座標</param>
	/// <param name="color">カラー</param>
	/// <param name="scale">サイズ</param>
	/// <param name="isShadowParam">影(輪郭)をつけるか</param>
	/// <param name="shadowOffset">オフセット量</param>
	/// <param name="shadowColor">影のカラー</param>
	void InitFontRender(
		FontRender& fontRender,
		Vector2 position,
		float scale = 1.0f,
		Vector4 color = g_vec4White,
		bool isShadowParam = true,
		float shadowOffset = 1.8f,
		Vector4 shadowColor = g_vec4Black
	);

private:
	/// <summary>
	/// プレイヤーのHPの処理
	/// </summary>
	void ProcessPlayerHp();
	/// <summary>
	/// プレイヤーのMPの処理
	/// </summary>
	void ProcessPlayerMp();

	/// <summary>
	/// ボスのHPの処理
	/// </summary>
	void ProcessBossHP();

private:

	struct MonsterUI
	{
		FontRender m_hpFont;					//HPの値
		FontRender m_AccumulationDamageFont;	//

		SpriteRender m_IconRender;				//キャラアイコン
		SpriteRender m_HpFlameRender;			//HPのフレーム
		SpriteRender m_HpFrontRender;			//変動するHPバー
		SpriteRender m_HpWhiteRender;			//遅れて減らす白いHPバー
		SpriteRender m_HpBackRender;			//HPバーの裏側
	};

	//プレイヤーのUI
	struct PlayerUI
	{
		FontRender m_hpFont;						//HPの値
		FontRender m_mpFont;						//MPの値

		SpriteRender m_characterIconRender;			//キャラアイコン

		SpriteRender m_iconBaseRender;				//キャラアイコンのベース

		SpriteRender m_statusBarRender;			//ステータスバー

		SpriteRender m_hpFrontRender;			//変動するHPバー
		SpriteRender m_hpBackRender;			//HPバーの裏側
		SpriteRender m_hpWhiteRender;			//遅れて減らす白いHPバー
		SpriteRender m_mpFrontRender;			//変動するMPバー
		SpriteRender m_mpBackRender;			//MPバーの裏側

		SpriteRender m_SkillCenterRender;			//スキル関連の中心
		SpriteRender m_Skill_1FlameRender;			//スキル1のフレーム
		SpriteRender m_Skill_1FlameInsideRender;	//スキル1のフレームの内側

		SpriteRender m_SkillButtonXRender;			//スキルを発動できるボタン
		
	};

	FontRender m_TimerFont;				//制限時間
	SpriteRender m_TimeFlameRender;		//制限時間の枠

	Game* m_game = nullptr;
	Player* m_player = nullptr;
	Lich* m_lich = nullptr;

	PlayerUI m_playerUI;			//プレイヤーの情報のUI
	MonsterUI m_monsterUI;			//モンスターの情報のUI

	Vector3 m_playerHpWhiteScale = g_vec3One;		//プレイヤーの白いHPバー
	Vector3 m_oldPlayerHpScale = g_vec3One;	//前フレームのプレイヤーのHPバー

	Vector3 m_BossHpWhiteScale = g_vec3One;			//ボスの白いHPバー
	Vector3 m_oldBossHpScale = g_vec3One;		//前フレームのボスのHPバー

	Vector3 m_playerMpWhiteScale = g_vec3One;		//プレイヤーの白いMPバー

	float m_oldMainCharaHP = 0.0f;				//前フレームのメインキャラのHP

	float m_gaugeTimer = 0.0f;

	float m_charaIconChangeTimer = 0.0f;
	
	bool m_coolTimeDrawFlag = true;

	float m_wipeSize = 0;

	float m_bossLerpSpeed = 0.0f;
	float m_playerLerpSpeed = 0.0f;







};

