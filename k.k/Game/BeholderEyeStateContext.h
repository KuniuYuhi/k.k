#pragma once
#include "BeholderEyeInfo.h"

class BeholderEye;
class IMobEnemyState;

using namespace BeholderEyeStates;


class BeholderEyeStateContext
{
public:
	BeholderEyeStateContext();
	~BeholderEyeStateContext();


	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="beholderEye">目玉のインスタンス</param>
	/// <param name="startState">最初のステート</param>
	void Init(BeholderEye* beholderEye, EnBeholderEyeState startState);

	/// <summary>
	/// ステートを切り替える
	/// </summary>
	/// <param name="changeState">切り替えたいステート</param>
	void ChangeBeholderEyeState(BeholderEye* beholderEye, EnBeholderEyeState changeState);

	/// <summary>
	/// 現在のステートの更新処理
	/// </summary>
	void UpdateCurrentState();
	/// <summary>
	/// 現在のステートのアニメーションの再生
	/// </summary>
	void PlayAnimationCurrentState();



	EnBeholderEyeState GetCurrentSlimeState()
	{
		return m_enCurrentSlimeState;
	}

protected:

	EnBeholderEyeState m_enCurrentSlimeState = enBeholderEyeState_Idle;
	EnBeholderEyeState m_enPreviousSlimeState = enBeholderEyeState_Idle;

	IMobEnemyState* m_currentState = nullptr;		//現在のステート

};

