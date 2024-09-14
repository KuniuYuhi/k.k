#pragma once



class PlayerGameUI;
class SummonerUI;
class GameStatusUI;

class GameUI:public IGameObject
{
public:
	GameUI();
	~GameUI();


	bool Start();
	void Update();
	void Render(RenderContext& rc);

	
private:

private:


	PlayerGameUI* m_playerGameUI = nullptr;


	SummonerUI* m_summonerUI = nullptr;


	GameStatusUI* m_gameStatusUI = nullptr;

};

