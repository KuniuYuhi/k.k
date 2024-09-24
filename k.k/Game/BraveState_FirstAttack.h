#pragma once
#include "IBraveState.h"

class PlayerController;

class BraveState_FirstAttack :
    public IBraveState
{
public:
	BraveState_FirstAttack(Brave* brave, BraveStateContext* stateContext)
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


private:

	PlayerController* m_playerController = nullptr;

	bool m_isTrigger = false;

	bool m_isNextNormalAttackProgress = false;	//次の攻撃に進むのか

};

