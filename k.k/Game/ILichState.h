#pragma once

class Lich;
/// <summary>
/// リッチステートの基底クラス
/// </summary>
class ILichState
{
public:
	ILichState(Lich* lich)
	{
		m_lich = lich;
	}

	virtual ~ILichState(){}

	/// <summary>
	/// 状態を管理する
	/// </summary>
	virtual void ManageState() = 0;
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	virtual void PlayAnimation() = 0;

protected:
	Lich* m_lich = nullptr;
};

