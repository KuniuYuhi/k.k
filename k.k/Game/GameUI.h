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
	/// モンスターのUIの処理
	/// </summary>
	void MonsterUIUpdate();

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

private:

	struct MonsterUI
	{
		FontRender m_hpFont;
		FontRender m_AccumulationDamageFont;
	};

	struct PlayerUI
	{
		FontRender m_hpFont;
		FontRender m_mpFont;
	};

	Game* m_game = nullptr;
	Player* m_player = nullptr;
	Lich* m_lich = nullptr;

	PlayerUI m_playerUI;			//プレイヤーの情報のUI
	MonsterUI m_monsterUI;			//モンスターの情報のUI


};

