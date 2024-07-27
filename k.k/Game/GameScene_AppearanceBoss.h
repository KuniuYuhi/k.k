#pragma once
#include "IGameSceneState.h"


class EntryBoss;

class GameScene_AppearanceBoss :
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

private:
	EntryBoss* m_entryBoss = nullptr;

};

