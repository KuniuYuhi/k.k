#pragma once

/*
*@brief 進行状況の管理
*/

#include "AllGameSceneState.h"

class GameSceneContext;

/// <summary>
/// シングルトンパターンで作成
/// </summary>
class GameSceneManager:public Noncopyable
{
private:
	GameSceneManager();
	~GameSceneManager();

public:

	/// <summary>
	/// 勝敗
	/// </summary>
	enum EnBattleOutCome
	{
		enBattleOutCome_PlayerWin,
		enBattleOutCome_PlayerLose,
		enBattleOutCome_None
	};



	static GameSceneManager* m_instance; //唯一のインスタンスのアドレスを記録する変数。
	
	/// <summary>
	/// インスタンスを生成。ゲームシーンを設定
	/// </summary>
	/// <param name="setGameSceneState"></param>
	static void CreateInstanceAndSetGameSceneState(EnGameSceneState setGameSceneState);

	/// <summary>
	/// インスタンスを取得。
	/// </summary>
	/// <returns></returns>
	static GameSceneManager* GetInstance()
	{
		return m_instance;
	}

	/// <summary>
	/// インスタンスを削除
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	/// <summary>
	/// ゲームシーンコンテキストを生成
	/// </summary>
	/// <param name="setGameSceneState"></param>
	void CreateGameSceneContext(EnGameSceneState setGameSceneState);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// ステートを切り替える
	/// </summary>
	/// <param name="cangeGameSceneState"></param>
	void ChangeGameSceneState(EnGameSceneState cangeGameSceneState);


	/// <summary>
	/// シーン切り替え可能かフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetIsSceneChangeableFlag(bool flag)
	{
		m_isSceneChangeable = flag;
	}

	/// <summary>
	/// シーン切り替え可能かフラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& GetIsSceneChangeableFlag() const
	{
		return m_isSceneChangeable;
	}

	/// <summary>
	/// 現在のゲームシーンステートを取得する
	/// </summary>
	/// <returns></returns>
	const EnGameSceneState& GetCurrentGameSceneState();




	/// <summary>
	/// バトルの勝敗を設定する
	/// </summary>
	/// <param name="setBattleOutCome"></param>
	void SetBattleOutCome(EnBattleOutCome setBattleOutCome)
	{
		m_battleOutCome = setBattleOutCome;
	}

	/// <summary>
	/// バトルの勝敗を取得する
	/// </summary>
	/// <returns></returns>
	const EnBattleOutCome& GetBattleOutCome()
	{
		return m_battleOutCome;
	}



private:

	std::unique_ptr<GameSceneContext> m_gameSceneContext = nullptr;		//ゲームシーンコンテキスト
																		//をスマートポインタで生成

	bool m_isSceneChangeable = false;				//シーンが切り替え可能か


	EnBattleOutCome m_battleOutCome = enBattleOutCome_None;

};

