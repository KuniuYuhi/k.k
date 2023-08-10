#pragma once

class Player;
class Lich;




///���b�`�̍s�������߂�N���X�B�]���l�Ō��܂�
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

	//���̍s��
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
	/// �v���C���[�̃C���X�^���X���擾
	/// </summary>
	/// <param name="player"></param>
	void GetPlayer(Player* player)
	{
		m_player = player;
	}
	/// <summary>
	/// ���b�`�̃C���X�^���X���擾
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

	//�\���̂ɂ�����
	//�s���̐����̕]���l
	int m_eval[5];

	Action m_action[5] = { 0 };


	

};

