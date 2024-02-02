#pragma once

#include "MobMonster.h"

class MobMonster;	//モブモンスター
class Player;		//プレイヤー
class Boss;			//ボス

//todo 取得するのがポインタである必要があるのか

class CharactersInfoManager:public Noncopyable
{
private:
	CharactersInfoManager();
	~CharactersInfoManager();

public:
	/// <summary>
	/// インスタンスの作成。
	/// </summary>
	static void CreateInstance()
	{
		m_charactersInfoInstance = new CharactersInfoManager;
	}
	/// <summary>
	/// インスタンスの破棄。
	/// </summary>
	static void DeleteInstance()
	{


		delete m_charactersInfoInstance;
		m_charactersInfoInstance = nullptr;
	}
	/// <summary>
	/// インスタンスの取得。
	/// キャラクター情報マネージャーの関数を使うときはこの関数を経由する
	/// </summary>
	/// <returns></returns>
	static CharactersInfoManager* GetInstance()
	{
		return m_charactersInfoInstance;
	}

	/// <summary>
	/// プレイヤーインスタンスの設定
	/// </summary>
	/// <param name="brave"></param>
	void SetPlayerInstance(Player* player)
	{
		//既にインスタンスが代入されているなら
		if (m_playerInstance != nullptr)
		{
			std::abort();
		}
		m_playerInstance = player;
	}
	/// <summary>
	/// プレイヤーインスタンスの取得
	/// </summary>
	/// <returns></returns>
	Player* GetPlayerInstance()
	{
		return m_playerInstance;
	}
	/// <summary>
	/// プレイヤーインスタンスの削除
	/// </summary>
	void DeletePlayerInstance()
	{
		//DeleteGO(m_playerInstance);
	}

	/// <summary>
	/// ボスインスタンスの設定
	/// </summary>
	/// <param name="boss"></param>
	void SetBossInstance(Boss* boss)
	{
		//既にインスタンスが代入されているなら
		if (m_bossInstance != nullptr)
		{
			std::abort();
		}
		m_bossInstance = boss;
	}
	/// <summary>
	/// ボスインスタンスの取得
	/// </summary>
	/// <returns></returns>
	Boss* GetBossInstance()
	{
		return m_bossInstance;
	}

	/// <summary>
	/// ボスインスタンスの削除
	/// </summary>
	void DeleteBossInstance()
	{
		//DeleteGO(m_bossInstance);
	}


	/// <summary>
	/// モブモンスターのリストに情報を追加
	/// </summary>
	/// <param name="mobMonster"></param>
	void AddMobMonsterFromList(MobMonster* mobMonster)
	{
		m_mobMonsters.emplace_back(mobMonster);
	}
	/// <summary>
	/// 指定したモブモンスターをリストから削除
	/// </summary>
	/// <param name="mobMonster"></param>
	void RemoveMobMonsterFormList(MobMonster* mobMonster)
	{
		std::vector<MobMonster*>::iterator it = std::find(
			m_mobMonsters.begin(), // モブモンスターのリストの最初
			m_mobMonsters.end(),   // モブモンスターのリストの最後
			mobMonster                     // 消したいアクター
		);
		if (it != m_mobMonsters.end()) {
			m_mobMonsters.erase(it);
		}

	}
	/// <summary>
	/// モブモンスターのリストの取得
	/// </summary>
	/// <returns></returns>
	std::vector<MobMonster*>& GetMobMonsters()
	{
		return m_mobMonsters;
	}

	/// <summary>
	/// 指定した番号のモンスターを取得
	/// </summary>
	/// <param name="number"></param>
	/// <returns></returns>
	MobMonster* GetMobMonster(int number)
	{
		return m_mobMonsters[number];
	}

	/// <summary>
	/// プレイヤーに近いモンスターの数を取得
	/// </summary>
	/// <returns></returns>
	const int& GetMonstersNearPlayerCount() const
	{
		return m_monstersNearPlayerCount;
	}

	/// <summary>
	/// 毎フレーム行う処理
	/// </summary>
	void Execute();

	/// <summary>
	/// 指定したモンスターがプレイヤーに近いか調べる
	/// </summary>
	/// <param name="mobMonster"></param>
	bool SearchMonsterNearPlayer(MobMonster* mobMonster);

	/// <summary>
	/// 全てのモンスター(ボス含む)のダメージを受けられるかのフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetAllMonsterDamgeHitFlag(bool flag);

private:

	/// <summary>
	/// プレイヤーに近いモンスターの数を調べる
	/// </summary>
	void SearchMonstersNearPlayer();

	
	void SortMonsterToPlayerLength(float array[][2], int maxValue);


	

private:
	Boss* m_bossInstance = nullptr;
	Player* m_playerInstance = nullptr;		//プレイヤーのインスタンス

	std::vector<MobMonster*> m_mobMonsters;			//モブモンスターのリスト

	int m_monstersNearPlayerCount = 0;


	static CharactersInfoManager* m_charactersInfoInstance;

};

