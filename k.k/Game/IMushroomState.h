#pragma once

class Mushroom;
class IMushroomState
{
public:
	IMushroomState(Mushroom* mushroom)
	{
		m_mushroom = mushroom;
	}

	virtual ~IMushroomState() {}

	/// <summary>
	/// 状態を管理する
	/// </summary>
	virtual void ManageState() = 0;
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	virtual void PlayAnimation() = 0;


protected:
	Mushroom* m_mushroom = nullptr;
};

