#pragma once

/*
*@brief 進行状況の管理
*/



class GameManager :public Noncopyable
{
private:
	GameManager();
	~GameManager();
public:
	/// <summary>
	/// インスタンスの作成。
	/// </summary>
	static void CreateInstance()
	{
		m_instance = new GameManager;
	}
	/// <summary>
	/// インスタンスの破棄。
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}
	/// <summary>
	/// インスタンスを取得。
	/// </summary>
	/// <returns></returns>
	static GameManager* GetInstance()
	{
		return m_instance;
	}


public:
	//ゲームのシーン
	enum EnSeenState
	{
		enSeen_Tittle,
		enSeen_Battle,
		enSeen_GameClear,
		enSeen_GameOver,
		enSeen_Result,
		enSeen_Feed
	};


	static GameManager* m_instance;		//唯一のインスタンスのアドレスを記録する変数。



};



