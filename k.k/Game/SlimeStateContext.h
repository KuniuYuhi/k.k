#pragma once

//スライムのステート
#include "SlimeInfo.h"

class Slime;
class IMobEnemyState;

using namespace SlimeStates;

/// <summary>
/// モブエネミー：スライムのステート管理クラス
/// </summary>
class SlimeStateContext
{
public:
	SlimeStateContext();
	~SlimeStateContext();


	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="slime">スライムのインスタンス</param>
	/// <param name="startState">最初のステート</param>
	void Init(Slime* slime, EnSlimeState startState);

	/// <summary>
	/// ステートを切り替える
	/// </summary>
	/// <param name="changeState">切り替えたいステート</param>
	void ChangeSlimeState(Slime* slime, EnSlimeState changeState);

	/// <summary>
	/// 現在のステートの更新処理
	/// </summary>
	void UpdateCurrentState();
	/// <summary>
	/// 現在のステートのアニメーションの再生
	/// </summary>
	void PlayAnimationCurrentState();



	EnSlimeState GetCurrentSlimeState()
	{
		return m_enCurrentSlimeState;
	}

protected:

	EnSlimeState m_enCurrentSlimeState = enSlimeState_Idle;
	EnSlimeState m_enPreviousSlimeState = enSlimeState_Idle;

	IMobEnemyState* m_currentSlimeState = nullptr;		//現在のステート

};

