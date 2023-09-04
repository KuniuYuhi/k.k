#pragma once

class Player;
class Lich;

///リッチの行動を決めるクラス。評価値で決まる
class LichAction:public Noncopyable
{
public:
	LichAction(Lich* lich)
	{
		m_lich = lich;
	}

	~LichAction(){}

	enum EnActionNumber
	{
		enAttack_None,
		enAttack_1,
		enAttack_2,
		enAttack_DarkMeteorite,
		enAttack_Summon,
		enAttack_AngryMode
	};

	EnActionNumber m_enActionNumber;

	struct Action
	{
		int m_eval = 0;					//評価値
		EnActionNumber m_enAction;		//行動
		int m_priority = 0;				//優先度
	};

	/// <summary>
	/// それぞれの行動の優先度を設定する
	/// </summary>
	void SettingPriority();

	int NextAction();

	int CalcMaxEvalNumber();

	/// <summary>
	/// プレイヤーのインスタンスを取得
	/// </summary>
	/// <param name="player"></param>
	void GetPlayer(Player* player)
	{
		m_player = player;
	}
	/// <summary>
	/// リッチのインスタンスを取得
	/// </summary>
	void GetLich(Lich* lich)
	{
		m_lich = lich;
	}
	
	


private:
	
	
	void CalcEvalIdle(EnActionNumber m_enActionNumber);

	void CalcEvalAttack1(EnActionNumber m_enActionNumber);

	void CalcEvalAttack2(EnActionNumber m_enActionNumber);

	void CalcEvalSummon(EnActionNumber m_enActionNumber);

	void CalcEvalDarkMeteorite(EnActionNumber m_enActionNumber);

	void CalcEvalAngryMode(EnActionNumber m_enActionNumber);
	

	Player* m_player = nullptr;
	Lich* m_lich = nullptr;

	const int m_actionAmount = 6;

	EnActionNumber m_oldActionNumber = enAttack_None;

	EnActionNumber m_firstActionNumber = enAttack_Summon;			//最初の行動

	//構造体にしたい
	//行動の数分の評価値
	int m_eval[6];

	Action m_action[6] = { 0 };


	

};

