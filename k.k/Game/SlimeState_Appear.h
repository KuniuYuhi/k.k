#pragma once
#include "IMobEnemyState.h"
#include "SlimeStateContext.h"

class Slime;
class SlimeStateContext;

class SlimeState_Appear : public IMobEnemyState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="slime">スライムのインスタンス</param>
	/// <param name="stateContest">ステートコンテキスト</param>
	SlimeState_Appear(Slime* slime, SlimeStateContext* stateContest)
	{
		m_slime = slime;
		m_stateCotext = stateContest;
	}

	~SlimeState_Appear();

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

