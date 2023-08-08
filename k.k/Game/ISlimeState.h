#pragma once

class Slime;
/// <summary>
/// スライムステートの基底クラス
/// </summary>
class ISlimeState
{
public:
	ISlimeState(Slime* slime)
	{
		m_slime = slime;
	}

	virtual ~ISlimeState() {}

	/// <summary>
	/// 状態を管理する
	/// </summary>
	virtual void ManageState() = 0;
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	virtual void PlayAnimation() = 0;

protected:
	Slime* m_slime = nullptr;
};

