#pragma once
#include "IGameSceneState.h"


class ResultSeen;
class GameFinishCamera;

class GameScene_Result : public IGameSceneState
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

	void PlayerWinProcess();

	void PlayerLoseProcess();


	void CreateResult();



private:

	ResultSeen* m_resultSeen = nullptr;
	GameFinishCamera* m_gameFinishCamera = nullptr;

};

