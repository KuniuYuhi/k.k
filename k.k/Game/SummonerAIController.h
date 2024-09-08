#pragma once
#include "IComponent.h"


class Summoner;
class Brave;

/// <summary>
/// サモナーのAIコントローラー。アクションの細かい処理を記載
/// </summary>
class SummonerAIController :public IComponent
{
public:

	SummonerAIController();
	~SummonerAIController();

	void Start() override;

	void UpdateComponent() override;


	/// <summary>
	/// ワープ座標をステージのレベルファイルから読み込む
	/// </summary>
	void InitWarpPointsFromStageLevel();

	/// <summary>
	/// ワープ先の座標を決定
	/// </summary>
	void DecisionWarpPoint();
	/// <summary>
	/// ワープ処理
	/// </summary>
	void WarpProcess();

	/// <summary>
	/// ショックウェーブの当たり判定作成
	/// </summary>
	void CreateShockWaveCollision();

	/// <summary>
	/// コンボ攻撃の当たり判定を生成
	/// </summary>
	void CreateComboAttackCollision();
	/// <summary>
	///	三コンボ目の当たり判定の生成
	/// </summary>
	void CreateThirdComboCollision();

	/// <summary>
	/// ダークボールの生成
	/// </summary>
	void CreateDarkBall();

	/// <summary>
	/// ダークメテオの落下位置を決定
	/// </summary>
	void DecisionDarkMeteoriteFallPoint();
	/// <summary>
	/// ダークメテオを生成
	/// </summary>
	void CreateDarkMeteorite();

private:
	

private:
	Summoner* m_summoner = nullptr;		//サモナーのインスタンス

	Brave* m_player = nullptr;

	std::vector<Vector3> m_warpPointsList;		//ワープ先座標をまとめたリスト

	CollisionObject* m_shockWaveCollision = nullptr;	//ショックウェーブ用コリジョン

	CollisionObject* m_comboAttackCollision = nullptr;	//コンボ攻撃用コリジョン


	Vector3 m_warpPosition = g_vec3Zero;			//ワープ座標

	Vector3 m_darkMeteoriteFallPoint = g_vec3Zero;		//ダークメテオ落下地点



	int m_darkBallCreateCount = 0;		//ダークボール生成数カウント

};

