#pragma once

namespace {
	const int SUMMON_MONSTERS_CONUT = 5;
}

class BattlePhase:public IGameObject
{
public:
	BattlePhase();
	~BattlePhase();

	bool Start();
	void Update();

private:
	
	/// <summary>
	/// モンスターの召喚する処理
	/// </summary>
	void ProcessSummonMonsters();

	/// <summary>
	/// モブモンスターを生成
	/// </summary>
	/// <param name="createCount">生成する数</param>
	void CreateMobMonsters(const int createCount);

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
	/// 難易度：簡単の処理
	/// </summary>
	void processEasyDifficulty(const int summonMonstersCount);
	/// <summary>
	/// 難易度：普通の処理
	/// </summary>
	void processNormalDifficulty(const int summonMonstersCount);
	/// <summary>
	/// 難易度：難しいの処理
	/// </summary>
	void processHardDifficulty(const int summonMonstersCount);

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

	ModelRender model[SUMMON_MONSTERS_CONUT];

	Vector3 m_createPos[SUMMON_MONSTERS_CONUT];

	float m_incantationTimer = 0.0f;

	

};

