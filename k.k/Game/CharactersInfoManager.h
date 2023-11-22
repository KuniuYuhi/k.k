#pragma once

#include "MobMonster.h"

class Brave;		//勇者クラス
class Lich;			//リッチ(ボス)
class MobMonster;	//モブモンスター

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
	/// 勇者インスタンスの設定
	/// </summary>
	/// <param name="brave"></param>
	void SetBraveInstance(Brave* brave)
	{
		//既にインスタンスが代入されているなら
		if (m_braveInstance != nullptr)
		{
			std::abort();
		}
		m_braveInstance = brave;
	}
	/// <summary>
	/// 勇者インスタンスの取得
	/// </summary>
	/// <returns></returns>
	Brave* GetBraveInstance()
	{
		return m_braveInstance;
	}

	/// <summary>
	/// リッチインスタンスの設定
	/// </summary>
	/// <param name="brave"></param>
	void SetLichInstance(Lich* lich)
	{
		//既にインスタンスが代入されているなら
		if (m_lichInstance != nullptr)
		{
			std::abort();
		}
		m_lichInstance = lich;
	}
	/// <summary>
	/// リッチインスタンスの取得
	/// </summary>
	/// <returns></returns>
	Lich* GetLichInstance()
	{
		return m_lichInstance;
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


private:
	Brave* m_braveInstance;			//勇者のインスタンス
	Lich* m_lichInstance;			//リッチのインスタンス

	std::vector<MobMonster*> m_mobMonsters;			//モブモンスターのリスト


	static CharactersInfoManager* m_charactersInfoInstance;

};

