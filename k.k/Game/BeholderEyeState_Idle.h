#pragma once
#include "IMobEnemyState.h"
#include "BeholderEyeStateContext.h"

class BeholderEye;
class BeholderEyeStateContext;

class BeholderEyeState_Idle : public IMobEnemyState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="slime">目玉のインスタンス</param>
	/// <param name="stateContest">ステートコンテキスト</param>
	BeholderEyeState_Idle(BeholderEye* beholderEye, BeholderEyeStateContext* stateContest)
	{
		m_beholderEye = beholderEye;
		m_stateCotext = stateContest;
	}

	~BeholderEyeState_Idle();

	/// <summary>
	/// アニメーションを再生
	/// </summary>
	void PlayAnimation() override;
	/// <summary>
	/// 入った時の処理
	/// </summary>
	void Entry() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Ubdate() override;
	/// <summary>
	/// 終了時の処理
	/// </summary>
	void Exit() override;


private:
	BeholderEye* m_beholderEye = nullptr;
	BeholderEyeStateContext* m_stateCotext = nullptr;
};

