#pragma once

class MobEnemyBase;
class BossEnemyBase;

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

	/// <summary>
	/// モブエネミーリストを取得
	/// </summary>
	/// <returns></returns>
	std::vector<MobEnemyBase*> GetMobEnemyList()
	{
		return m_mobEnemyList;
	}


	/// <summary>
	/// ボスエネミーのインスタンスを設定
	/// </summary>
	/// <param name="bossEnemy"></param>
	void SetBossEnemyInstance(BossEnemyBase* bossEnemy)
	{
		m_bossEnemy = bossEnemy;
	}
	/// <summary>
	/// ボスエネミーのインスタンスを取得
	/// </summary>
	/// <returns></returns>
	BossEnemyBase* GetBossEnemyInstance()
	{
		return m_bossEnemy;
	}

	/// <summary>
	/// ボスエネミーのインスタンスがnullptrか？
	/// </summary>
	/// <returns>nullptrならtrue</returns>
	bool IsNullptrBossEnemyInstance()
	{
		if (m_bossEnemy != nullptr)
		{
			return false;
		}
		return true;
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



	int GetNearbyEnemyCount(Vector3 targetPosition, float radius);


	/// <summary>
	/// 全てのエネミーを削除する
	/// </summary>
	/// <param name="isPlayDieEffect">モブエネミーの死亡エフェクトを再生するか</param>
	void DeleteAllEnemy(bool isPlayDieEffect = false);

	/// <summary>
	/// エネミーの勝利時の処理を実行する
	/// </summary>
	void WinProcessAllEnemy();

	/// <summary>
	/// 
	/// </summary>
	void GameOutComeProcess();

private:

	







private:

	//ボスエネミー情報
	BossEnemyBase* m_bossEnemy = nullptr;			//ボスエネミーインスタンス

	std::vector<MobEnemyBase*> m_mobEnemyList;		//モブエネミーリスト


	int m_approachMobEnemyAmount = 5;				//接近できるモブエネミーの数

	
};

