#pragma once

#include "BraveAnimClipAndStateInfo.h"

class Brave;
class BraveStateContext;

/// <summary>
/// プレイヤーのステートの基底クラス
/// </summary>
class IBraveState
{
public:

	IBraveState(Brave* brave, BraveStateContext* stateContext)
	{
		m_brave = brave;
		m_stateCotext = stateContext;
	}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IBraveState() {}

	/// <summary>
	/// アニメーションを再生
	/// </summary>
	virtual void PlayAnimation() = 0;
	/// <summary>
	/// 入った時の処理
	/// </summary>
	virtual void Entry() = 0;
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Ubdate() = 0;
	/// <summary>
	/// 終了時の処理
	/// </summary>
	virtual void Exit() = 0;


protected:

	//プレイヤーのインスタンス
	Brave* m_brave = nullptr;
	//ステートコンテキストのインスタンス
	BraveStateContext* m_stateCotext = nullptr;

};
