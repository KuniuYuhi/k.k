#pragma once

#include "MushroomInfo.h"

class Mushroom;
class IMobEnemyState;

using namespace MushroomStates;

/// <summary>
/// モブエネミー：キノコのステート管理クラス
/// </summary>
class MushroomStateContext
{
public:
	MushroomStateContext();
	~MushroomStateContext();


	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="slime">キノコのインスタンス</param>
	/// <param name="startState">最初のステート</param>
	void Init(Mushroom* mushroom, EnMushroomState startState);

	/// <summary>
	/// ステートを切り替える
	/// </summary>
	/// <param name="changeState">切り替えたいステート</param>
	void ChangeMushroomState(Mushroom* mushroom, EnMushroomState changeState);

	/// <summary>
	/// 現在のステートの更新処理
	/// </summary>
	void UpdateCurrentState();
	/// <summary>
	/// 現在のステートのアニメーションの再生
	/// </summary>
	void PlayAnimationCurrentState();



	EnMushroomState GetCurrentSlimeState()
	{
		return m_enCurrentSlimeState;
	}

protected:

	EnMushroomState m_enCurrentSlimeState = enMushroomState_Idle;
	EnMushroomState m_enPreviousSlimeState = enMushroomState_Idle;

	IMobEnemyState* m_currentState = nullptr;		//現在のステート

};

