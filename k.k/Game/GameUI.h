#pragma once
#include "MyWeapon.h"

class Game;
class Player;
class Boss;

class GameUI:public IGameObject
{
public:
	GameUI();
	~GameUI();

	struct WeaponSprits
	{
		SpriteRender* m_weaponRender = nullptr;
		SpriteRender* m_weaponEndranceRender = nullptr;
	};

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// プレイヤーインスタンスの取得
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player)
	{
		m_player = player;
	}

	/// <summary>
	/// ボスインスタンスの取得
	/// </summary>
	/// <param name="boss"></param>
	void SetBoss(Boss* boss)
	{
		m_boss = boss;
	}

	/// <summary>
	/// ゲームインスタンスの取得
	/// </summary>
	/// <param name="game"></param>
	void SetGame(Game* game)
	{
		m_game = game;
	}

	
private:
	/// <summary>
	/// プレイヤーのUIの初期化
	/// </summary>
	void InitPlayerUI();
	/// <summary>
	/// モンスターのUIの初期化
	/// </summary>
	void InitMonsterUI();
	/// <summary>
	/// フェーズのUIの初期化
	/// </summary>
	void InitPhaseUI();

	/// <summary>
	/// プレイヤーUIの処理
	/// </summary>
	void PlayerUIUpdate();
	/// <summary>
	/// メインキャラのステータスの更新処理
	/// </summary>
	void UpdateMainStatus();
	
	/// <summary>
	/// 武器の更新処理
	/// </summary>
	void UpdateWeapon();
	/// <summary>
	/// ゲージのスケールを計算する
	/// </summary>
	/// <param name="Maxvalue">最大値</param>
	/// <param name="value">計算したい値</param>
	/// <returns>サイズ。Xの値が変換した状態が返ってくる</returns>
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
	/// プレイヤーのHPの処理
	/// </summary>
	void ProcessPlayerHp();
	
	/// <summary>
	/// ボスのHPの処理
	/// </summary>
	void ProcessBossHP();

	/// <summary>
	/// ボスのスーパーアーマーの処理
	/// </summary>
	void ProcessBossSuperArmor();

	/// <summary>
	/// フェーズの処理
	/// </summary>
	void ProcessPhase();
	/// <summary>
	/// 現在のフェーズのモンスターの数のテキストを設定
	/// </summary>
	void CalcOffsetForNowPhaseMonsters(int monsters);

	/// <summary>
	/// 武器切り替え処理
	/// </summary>
	void ChangeWeaponAction(WeaponSprits& changeWeaponSprite);

	/// <summary>
	/// 耐久値のフォントの処理
	/// </summary>
	void ProcessWeaponEndranceFont();

	/// <summary>
	/// 引数の桁数によってオフセットを計算する
	/// </summary>
	/// <param name="num">オフセットしたい数</param>
	/// <param name="xOffset">X方向にずらす量</param>
	/// <param name="yOffset">Y方向にずらす量</param>
	Vector2 CalcNumberCount(float num, float xOffset, float yOffset);

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

	struct MonsterUI
	{
		FontRender m_hpFont;					//HPの値
		FontRender m_AccumulationDamageFont;	//

		SpriteRender m_hpFlameRender;			//HPのフレーム
		SpriteRender m_hpFrontRender;			//変動するHPバー
		SpriteRender m_hpWhiteRender;			//遅れて減らす白いHPバー
		SpriteRender m_hpBackRender;			//HPバーの裏側

		SpriteRender m_superArmor_FlameRender;		//スーパーアーマーのフレーム
		SpriteRender m_superArmor_FrontBarRender;	//スーパーアーマーの表の変動するのバー
		SpriteRender m_superArmor_BackBarRender;	//スーパーアーマーの後ろのバー

	};

	//プレイヤーのUI
	struct PlayerUI
	{
		FontRender m_hpFont;						//HPの値
		
		SpriteRender m_hpFlameRender;			//ステータスバー

		SpriteRender m_hpFrontRender;			//変動するHPバー
		SpriteRender m_hpBackRender;			//HPバーの裏側
		SpriteRender m_hpWhiteRender;			//遅れて減らす白いHPバー

		SpriteRender m_mainWeaponFlameRender;		//メイン武器のフレーム

		SpriteRender m_subWeaponFlameRender;		//サブ武器１のフレーム
		SpriteRender m_subWeaponCommandRender;		//サブ武器１のコマンド

		SpriteRender m_sub2WeaponFlameRender;		//サブ武器２のフレーム
		SpriteRender m_sub2WeaponCommandRender;		//サブ武器２のコマンド

		SpriteRender m_weaponRender[enMaxWeapons_num];
		SpriteRender m_weaponEndranceRender[enMaxWeapons_num];
		FontRender m_weaponEndranceFont;

	};

	WeaponSprits m_weaponSprits[enMaxWeapons_num];

	Vector3 m_weaponIconPos[enMaxWeapons_num] = {
		{720.0f,-330.0f,0.0f},
		{850.0f,38.0f,0.0f},
		{850.0f,-110.0f,0.0f}
	};
	Vector3 m_weaponIconScale[enMaxWeapons_num] = {
		g_vec3One * 0.8f,
		g_vec3One * 0.5f,
		g_vec3One * 0.5f
	};


	FontRender m_timerFont;				//制限時間
	SpriteRender m_timeFlameRender;		//制限時間の枠

	

	struct phaseUI
	{
		SpriteRender m_phaseFlameRender;	//フェーズのフレーム
		FontRender m_nowPhaseFont;			//現在のフェーズ数
		FontRender m_nowPhaseMonstersFont;	//現在のフェーズのモンスターの数
		SpriteRender m_phaseTimeFlameRender;
		SpriteRender m_phaseTimeBarRender;

	};



	Game* m_game = nullptr;
	Player* m_player = nullptr;
	Boss* m_boss = nullptr;

	PlayerUI m_playerUI;			//プレイヤーのUI
	MonsterUI m_monsterUI;			//モンスターのUI
	phaseUI m_phaseUI;				//フェーズのUI

	Vector3 m_playerWhiteHpScale = g_vec3One;		//プレイヤーの白いHPバー

	Vector3 m_bossHpWhiteScale = g_vec3One;			//ボスの白いHPバー

	float m_gaugeTimer = 0.0f;

	float m_wipeSize = 0;

	

	float m_nowPlayerWhiteHp = 0.0f;
	float m_nowBossWhiteHp = 0.0f;

	int m_oldPhaseMonstersNum = 0;



};

