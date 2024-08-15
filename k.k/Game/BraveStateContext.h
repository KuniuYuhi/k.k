#pragma once

#include "BraveAnimClipAndStateInfo.h"

using namespace BraveState;

class Brave;
class IBraveState;

/// <summary>
/// 勇者のステートを管理するクラス
/// </summary>
class BraveStateContext
{
public:
	BraveStateContext();
	~BraveStateContext();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="brave">勇者のインスタンス</param>
	/// <param name="startState">最初のステート</param>
	void Init(Brave* brave,EnBraveState startState);

	/// <summary>
	/// ステートを切り替える
	/// </summary>
	/// <param name="changeState">切り替えたいステート</param>
	void ChangeBraveState(EnBraveState changeState);

	/// <summary>
	/// ステートを切り替える
	/// </summary>
	/// <param name="changeState">切り替えたいステート番号</param>
	void ChangeBraveState(int changeStateNum);

	/// <summary>
	/// 現在のステートを更新
	/// </summary>
	void UpdateCurrentState();

	/// <summary>
	/// 現在のステートのアニメーションを再生
	/// </summary>
	void PlayAnimationCurrentState();
	

private:

	std::map<EnBraveState, IBraveState*> m_braveStateMap;


	EnBraveState m_EnCurrentBraveState = enBraveState_Idle;		//現在のステートを示す
	EnBraveState m_EnPreviousBraveState = enBraveState_Idle;	//前のステートを示す

	IBraveState* m_currentBraveState = nullptr;		//現在動いているステート
};

