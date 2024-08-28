#pragma once

class MobEnemyBase;

/// <summary>
/// エネミーの情報を管理するマネージャー
/// </summary>
class EnemyManager:public Noncopyable
{
public:

	EnemyManager();
	~EnemyManager();

	static EnemyManager* m_instance;

	/// <summary>
	/// インスタンスを生成
	/// </summary>
	static void CreateInstance();

	/// <summary>
	/// インスタンスを削除
	/// </summary>
	static void DeleteInstance();

	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns></returns>
	static EnemyManager* GetInstance()
	{
		return m_instance;
	}

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// モブエネミーが近づける距離を管理
	/// </summary>
	void ControlEnemyDistances(Vector3 targetPosition);


	/// <summary>
	/// モブエネミーをリストに追加
	/// </summary>
	/// <param name="addMobEnemy"></param>
	void AddMobEnemyToList(MobEnemyBase* addMobEnemy)
	{
		m_mobEnemyList.emplace_back(addMobEnemy);
	}
	/// <summary>
	/// モブエネミーリストをクリア
	/// </summary>
	void CrearMobEnemyList()
	{
		m_mobEnemyList.clear();
	}



	std::vector<MobEnemyBase*> GetMobEnemyList()
	{
		return m_mobEnemyList;
	}


	/// <summary>
	/// モブエネミーをリストから削除
	/// </summary>
	/// <param name="removeMobEnemy"></param>
	void RemoveMobEnemyToList(MobEnemyBase* removeMobEnemy)
	{
		std::vector<MobEnemyBase*>::iterator it = std::find(
			m_mobEnemyList.begin(), // モブモンスターのリストの最初
			m_mobEnemyList.end(),   // モブモンスターのリストの最後
			removeMobEnemy                     // 消したいアクター
		);
		if (it != m_mobEnemyList.end()) {
			m_mobEnemyList.erase(it);
		}
	}

	/// <summary>
	/// エネミーのリストをターゲットに向かう距離に近い順に入れ替える
	/// </summary>
	/// <param name="targetPosition"></param>
	void SortEnemiesByDistanceToTarget(Vector3 targetPosition);

private:

	







private:

	//ボスエネミー情報

	std::vector<MobEnemyBase*> m_mobEnemyList;		//モブエネミーリスト


	int m_approachMobEnemyAmount = 5;				//接近できるモブエネミーの数

	
};

