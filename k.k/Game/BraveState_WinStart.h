#pragma once
#include "IBraveState.h"
class BraveState_WinStart :
    public IBraveState
{
public:
	BraveState_WinStart(Brave* brave, BraveStateContext* stateContext)
		:IBraveState(brave,stateContext)
	{
	}

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

};

