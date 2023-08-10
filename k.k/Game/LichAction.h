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
		enAttack_Summon
	};

	EnActionNumber m_enActionNumber;

	struct Action
	{
		int m_eval = 0;
		EnActionNumber m_enAction;
	};

	//次の行動
	/*enum enNextAction
	{
		enNextAction_None,
		enNextAction_1,
		enNextAction_2,
		enNextAction_3
	};

	struct Action
	{
		int m_eval = 0;
		enNextAction m_enNextAction;
	};*/

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
	
	
	void CalcEvalIdle(EnActionNumber m_enActionNumber/*Action& action*/);

	void CalcEvalAttack1(EnActionNumber m_enActionNumber/*Action& action*/);

	void CalcEvalAttack2(EnActionNumber m_enActionNumber/*Action& action*/);

	void CalcEvalSummon(EnActionNumber m_enActionNumber/*Action& action*/);

	void CalcEvalDarkMeteorite(EnActionNumber m_enActionNumber/*Action& action*/);


	

	Player* m_player = nullptr;
	Lich* m_lich = nullptr;

	const int m_actionAmount = 5;

	EnActionNumber m_oldActionNumber = enAttack_None;

	//構造体にしたい
	//行動の数分の評価値
	int m_eval[5];

	Action m_action[5] = { 0 };


	

};

