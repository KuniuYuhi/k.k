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
	/// 円周上の点の座標を計算して格納する
	/// </summary>
	/// <param name="center">円の中心</param>
	/// <param name="radius">円の半径</param>
	/// <param name="numPoints">分割する点の数</param>
	void CalcCirclePoints(Vector3 center, float radius, int numPoints);

	/// <summary>
	/// モンスターの召喚する座標を決める
	/// </summary>
	void SetSummonMonsterPos();

	/// <summary>
	/// モンスター召喚
	/// </summary>
	void SummonMonster(Vector3 summonPosition);



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

	//生成するモンスターの数
	int m_createMonsters = 6;

	int m_divMonster = 4;

	int m_summonTurtleShellCount = 2;

	std::vector<Vector3> m_summonPositions;

	bool m_firstSummonFlag = false;
};

