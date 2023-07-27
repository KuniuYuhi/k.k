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

	enum EnActionNumber
	{
		enAttack_None,
		enAttack_1,
		enAttack_2,

	};

	enum enNextAction
	{
		enNextAction_None,
		enNextAction_1,
		enNextAction_2,
	};

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
	
	struct Action
	{
		int m_eval = 0;
		enNextAction m_enNextAction = enNextAction_None;
	};


private:
	
	void CalcEvalIdle(Action& action);

	void CalcEvalAttack1(Action& action);

	void CalcEvalAttack2(Action& action);

	void CalcEvalSummon(Action& action);






	Player* m_player = nullptr;
	Lich* m_lich = nullptr;

	const int m_actionAmount = 3;



	Action m_action[];


	

};

