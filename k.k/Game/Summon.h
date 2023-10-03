#pragma once

class Lich;
class AIActor;

class Slime;
class TurtleShell;
class Mushroom;
class Cactus;

class Summon:public IGameObject
{
public:
	Summon();
	~Summon();

	bool Start();
	void Update();

	/// <summary>
	/// �~����̓_�̍��W���v�Z���Ċi�[����
	/// </summary>
	/// <param name="center">�~�̒��S</param>
	/// <param name="radius">�~�̔��a</param>
	/// <param name="numPoints">��������_�̐�</param>
	void CalcCirclePoints(Vector3 center, float radius, int numPoints);

	/// <summary>
	/// �����X�^�[�̏���������W�����߂�
	/// </summary>
	void SetSummonMonsterPos();

	/// <summary>
	/// �����X�^�[����
	/// </summary>
	void SummonMonster(Vector3 summonPosition);

	/// <summary>
	/// �����X�^�[���������邩�̃t���O�̐ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetSummonStartFlag(bool flag)
	{
		m_isSummonStartFlag = flag;
	}

	/// <summary>
	/// ���X�g�̍��W����G�t�F�N�g�𐶐�����
	/// </summary>
	void SetCircleEffect();

	void SetLich(Lich* lich)
	{
		m_lich = lich;
	}

	enum EnMonster
	{
		enMonster_Slime,
		enMonster_Mushroom,
		enMonster_Cactus,
		enMonster_TurtleShell,
		enMonster_Num
	};

	void SetFirstSummonFlag(bool flag)
	{
		m_firstSummonFlag = flag;
	}


private:
	Lich* m_lich = nullptr;

	Slime* m_slime = nullptr;
	TurtleShell* m_turtleShell = nullptr;
	Mushroom* m_mushroom = nullptr;
	Cactus* m_cactus = nullptr;

	Vector3 m_lichPosition = g_vec3Zero;

	//�������郂���X�^�[�̐�
	int m_createMonsters = 0;

	int m_divMonster = 4;

	int m_summonTurtleShellCount = 2;			//�Ƃ��b���̏�����������̐�

	std::vector<Vector3> m_summonPositions;

	bool m_firstSummonFlag = false;

	bool m_isSummonStartFlag = false;				//�����X�^�[���������邩�̃t���O
};

