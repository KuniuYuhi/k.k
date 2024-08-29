#pragma once

#include "CactusInfo.h"

class Cactus;
class IMobEnemyState;

using namespace CactusStates;

/// <summary>
/// モブエネミー：サボテンのステート管理クラス
/// </summary>
class CactusStateContext
{
public:
	CactusStateContext();
	~CactusStateContext();


	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="slime">スライムのインスタンス</param>
	/// <param name="startState">最初のステート</param>
	void Init(Cactus* cactus, EnCactusState startState);

	/// <summary>
	/// ステートを切り替える
	/// </summary>
	/// <param name="changeState">切り替えたいステート</param>
	void ChangeCactusState(Cactus* cactus, EnCactusState changeState);

	/// <summary>
	/// 現在のステートの更新処理
	/// </summary>
	void UpdateCurrentState();
	/// <summary>
	/// 現在のステートのアニメーションの再生
	/// </summary>
	void PlayAnimationCurrentState();



	EnCactusState GetCurrentSlimeState()
	{
		return m_enCurrentSlimeState;
	}

protected:

	EnCactusState m_enCurrentSlimeState = enCactusState_Idle;
	EnCactusState m_enPreviousSlimeState = enCactusState_Idle;

	IMobEnemyState* m_currentState = nullptr;		//現在のステート

};

