#pragma once

class Brave;

/// <summary>
/// 勇者のステートの基底クラス
/// </summary>
class IBraveState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="brave"></param>
	IBraveState(Brave* brave)
	{
		m_brave = brave;
	}
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IBraveState(){}

	/// <summary>
	/// 状態を管理する
	/// </summary>
	virtual void ManageState() = 0;
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	virtual void PlayAnimation() = 0;
protected:
	Brave* m_brave = nullptr; // プレイヤー

};

