#pragma once

#include "AllGameSceneState.h"
//#include "IGameSceneState.h"

class IGameSceneState;

class GameSceneContext
{
public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="startGameSceneState"></param>
	void Init(EnGameSceneState startGameSceneState);

	/// <summary>
	/// 現在のゲームシーンステートの更新処理を行う
	/// </summary>
	void UpdateGameSceneState();

	/// <summary>
	/// ステートを切り替える
	/// </summary>
	/// <param name="changeGameSceneState"></param>
	void ChangeState(EnGameSceneState changeGameSceneState);

	/// <summary>
	/// 現在のゲームシーンを取得する
	/// </summary>
	/// <returns></returns>
	IGameSceneState* GetCurrentGameScene()
	{
		return m_currentGameScene;
	}


	/// <summary>
	/// 現在のゲームシーンステートを取得する
	/// </summary>
	/// <returns></returns>
	EnGameSceneState GetCurrentGameSceneState()
	{
		return m_currentGameSceneState;
	}
	



private:
	IGameSceneState* m_currentGameScene = nullptr;			//現在のシーンステート

	EnGameSceneState m_currentGameSceneState = enGameSceneState_None;
	EnGameSceneState m_previousGameSceneState = enGameSceneState_None;


};

