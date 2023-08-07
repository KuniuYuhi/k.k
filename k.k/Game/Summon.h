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



	void SetLich(Lich* lich)
	{
		m_lich = lich;
	}

	enum EnMonster
	{
		enMonster_Slime,
		enMonster_TurtleShell,
		enMonster_Mushroom,
		enMonster_Cactus,
		enMonster_Num
	};


private:
	Lich* m_lich = nullptr;

	Slime* m_slime = nullptr;
	TurtleShell* m_turtleShell = nullptr;
	Mushroom* m_mushroom = nullptr;
	Cactus* m_cactus = nullptr;

	Vector3 m_lichPosition = g_vec3Zero;

	//�������郂���X�^�[�̐�
	int m_createMonsters = 2;

	std::vector<Vector3> m_summonPositions;

	std::vector<AIActor*> m_monsters;

};

