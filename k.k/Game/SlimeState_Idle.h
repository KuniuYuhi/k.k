#pragma once
#include "IMobEnemyState.h"
#include "SlimeStateContext.h"

class Slime;
class SlimeStateContext;

class SlimeState_Idle : public IMobEnemyState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="slime">スライムのインスタンス</param>
	/// <param name="stateContest">ステートコンテキスト</param>
	SlimeState_Idle(Slime* slime, SlimeStateContext* stateContest)
	{
		m_slime = slime;
		m_stateCotext = stateContest;
	}

	~SlimeState_Idle();

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
	Slime* m_slime = nullptr;
	SlimeStateContext* m_stateCotext = nullptr;

};

