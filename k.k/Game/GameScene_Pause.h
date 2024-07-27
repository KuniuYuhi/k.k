#pragma once
#include "IGameSceneState.h"
class GameScene_Pause :
    public IGameSceneState
{
public:
	/// <summary>
	/// ステート開始時の処理
	/// </summary>
	void Entry() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// ステート終了時の処理
	/// </summary>
	void Exit() override;
};

