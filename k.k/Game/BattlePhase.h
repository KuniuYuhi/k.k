#pragma once

class MobMonster;

class BattlePhase:public IGameObject
{
public:
	BattlePhase();
	~BattlePhase();

	bool Start() override;
	void Update() override;

private:
	
	/// <summary>
	/// モンスターの召喚する処理
	/// </summary>
	void ProcessSummonMonsters();

	/// <summary>
	/// モブモンスターの生成する数を決める
	/// </summary>
	/// <returns></returns>
	void SetMobMonsterSpawnCount();

	/// <summary>
	/// モブモンスターを生成
	/// </summary>
	void CreateMobMonsters();

	/// <summary>
	/// モブモンスターを削除
	/// </summary>
	void DeleteMobMonsters();

	/// <summary>
	/// 召喚するモンスターの座標の設定
	/// </summary>
	/// <param name="radius">生成する範囲の円の半径</param>
	void SetSummonRandomPosition(const int radius);

	/// <summary>
	/// 他の座標と距離が空いているか
	/// </summary>
	/// <param name="count">調べたい配列の番号</param>
	/// <param name="distance">他の座標との間隔</param>
	/// <returns></returns>
	bool IsWithInDistances(int count,float distance);

	/// <summary>
	/// 現在のフェーズによって難易度を変更
	/// </summary>
	/// <param name="summonMonstersCount">召喚するモンスターの数</param>
	void AdjustDifficultyByPhase(const int summonMonstersCount);
	
	/// <summary>
	/// 詠唱中か(召喚の準備)
	/// </summary>
	/// <returns></returns>
	bool IsIncantation();

	/// <summary>
	/// 魔法陣の生成
	/// </summary>
	/// <param name="monsterCount">魔法陣を生成する数(モンスターの数と同じ)。</param>
	void CreateMagicCircleEffect(const int monsterCount);

	/// <summary>
	/// 召喚された瞬間の光の処理
	/// </summary>
	/// <param name="createPos"></param>
	void CreateSummonLightEffect(Vector3 createPos);

	/// <summary>
	/// 優先するモブモンスターの番号を取得。どのモブモンスターを選ばれないこともある
	/// </summary>
	/// <returns></returns>
	const int GetMobMonsterPriority();

	/// <summary>
	/// モブモンスターを生成する
	/// </summary>
	/// <param name="priorityNumber">生成するモンスターの番号。優先度。0以上ならこの番号のモンスターしか生成しない</param>
	/// <returns></returns>
	MobMonster* GenerateMobMonster(int priorityMobMonsterNumber = -1);


private:
	/// <summary>
	/// モンスター召喚のステップ
	/// </summary>
	enum EnSummonMonsterStep
	{
		enSummonMonsterStep_SettingMagicCircle,
		enSummonMonsterStep_SummonMonsters,
	};
	EnSummonMonsterStep m_enSummonMonsterStep = enSummonMonsterStep_SettingMagicCircle;

	std::vector<Vector3> m_createPositions;

	float m_incantationTimer = 0.0f;

	int m_mobMonsterSpawnCount = 0;

};

