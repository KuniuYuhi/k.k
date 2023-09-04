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
		enAttack_Summon,
		enAttack_AngryMode
	};

	EnActionNumber m_enActionNumber;

	struct Action
	{
		int m_eval = 0;					//�]���l
		EnActionNumber m_enAction;		//�s��
		int m_priority = 0;				//�D��x
	};

	/// <summary>
	/// ���ꂼ��̍s���̗D��x��ݒ肷��
	/// </summary>
	void SettingPriority();

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

	EnActionNumber m_firstActionNumber = enAttack_Summon;			//�ŏ��̍s��

	//�\���̂ɂ�����
	//�s���̐����̕]���l
	int m_eval[6];

	Action m_action[6] = { 0 };


	

};

