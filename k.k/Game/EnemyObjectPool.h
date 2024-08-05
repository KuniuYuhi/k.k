#pragma once
#include "IObjectPool.h"

#include "EnemyBase.h"

class EnemyBase;
class Slime;



/// <summary>
/// エネミーのオブジェクトプール管理クラス。シングルトンパターン
/// </summary>
class EnemyObjectPool : public Noncopyable
{
private:
	EnemyObjectPool();
	~EnemyObjectPool();

public:

	static EnemyObjectPool* m_instance;		//エネミーオブジェクトプールの唯一のインスタンス

	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns></returns>
	static EnemyObjectPool* GetInstance()
	{
		return m_instance;
	}

	/// <summary>
	/// 
	/// </summary>
	static void CreateInstance();

	/// <summary>
	/// インスタンスを削除
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();



	/// <summary>
	/// オブジェクトを生成してキューに格納。
	/// EnemyBaseクラスを継承しているオブジェクトのみ格納できます。
	/// オブジェクトは非アクティブ化されます。
	/// </summary>
	/// <typeparam name="T">オブジェクトのクラス</typeparam>
	/// <param name="saveKeyName">オブジェクトを保存するペアのキーの名前</param>
	template <typename T>
	void OnCreateEnemy(const char* saveKeyName)
	{
		//オブジェクトを生成
		EnemyBase* createobj = NewGO<T>(0, saveKeyName);

		//IGameObjectを継承しているなら取得
		IGameObject* targetObj = dynamic_cast<IGameObject*>(createobj);
		////IGameObjectが継承されていないならエラー
		if (targetObj == nullptr) std::abort();

		//非アクティブ化する
		targetObj->Deactivate();


		//オブジェクトプールの中に引数のオブジェクトの名前があるか調べる
		auto it = m_objectPoolQueue.find(saveKeyName);
		if (it != m_objectPoolQueue.end())
		{
			//キューにプッシュ
			it->second.push(createobj);
		}
	}


	/// <summary>
	/// キューからオブジェクトを取得する
	/// </summary>
	/// <param name="getKeyName">取得するオブジェクトが格納されているキー</param>
	/// <returns></returns>
	
	/// <summary>
	/// エネミーをゲットする
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="getKeyName"></param>
	/// <returns></returns>
	template <typename T>
	EnemyBase* OnGetEnemy(const char* getKeyName)
	{
		//キューからキーに対応するオブジェクトを取得
		std::queue<EnemyBase*>& getQueue = m_objectPoolQueue.at(getKeyName);

		//キューが空なら新しく生成して格納
		if (getQueue.empty())
		{
			//エネミーを生成
			OnCreateEnemy<T>(getKeyName);
		}
		//一番上から取ってくる
		auto* getEnemy = getQueue.front();

		//ゲットしたいクラスを持っているか調べる
		getEnemy = dynamic_cast<T*>(getEnemy);

		if (getEnemy == nullptr)
		{
			return nullptr;
		}

		//アクティブ化する
		getEnemy->Activate();
		//既にスタート関数の処理をしているならやり直す
		if (getEnemy->IsStart())
		{
			getEnemy->ResetStartFlag();
		}

		//キューから取り出す
		getQueue.pop();

		return getEnemy;
	}

	/// <summary>
	/// オブジェクトをキューに返す。返すときに非アクティブ化されます。
	/// </summary>
	/// <param name="keyName">格納するキーの名前</param>
	/// <param name="releaseObj">格納するオブジェクト</param>
	void OnRelease(const char* keyName, EnemyBase* releaseObj)
	{
		//キューからキーに対応するオブジェクトを取得
		std::queue<EnemyBase*>& getQueue = m_objectPoolQueue.at(keyName);
		//
		if (releaseObj == nullptr)
		{
			return;
		}
		//格納する前に非アクティブ化する
		releaseObj->Deactivate();
		//キューに格納する
		getQueue.push(releaseObj);
	}

	void OnDestroy(const char* keyName)
	{
		//キューからキーに対応するオブジェクトを取得
		std::queue<EnemyBase*>& getQueue = m_objectPoolQueue.at(keyName);

		if (getQueue.empty())
		{
			return;
		}

		while (!getQueue.empty())
		{
			if (auto* getEnemy = getQueue.front())
			{
				getEnemy->Dead();
			}
			getQueue.pop();
		}
	}

	/// <summary>
	/// オブジェクトプールの中のアクティブなオブジェクトの数を調べる
	/// </summary>
	/// <returns>アクティブオブジェクトの数</returns>
	const int GetActiveObjCount()
	{
		int activeCount = 0;

		for (auto& pool: m_objectPoolQueue)
		{
			std::queue<EnemyBase*> getPool = pool.second;

			while (!getPool.empty())
			{
				EnemyBase* enemy = getPool.front();

				getPool.pop();

				if (enemy->IsActive())
				{
					activeCount++;
				}
			}
		}

		return activeCount;
	}


private:

	int m_initPoolSize = 10;

	std::map<const char*, std::queue<EnemyBase*>> m_objectPoolQueue;	//オブジェクトプールのマップ



};

