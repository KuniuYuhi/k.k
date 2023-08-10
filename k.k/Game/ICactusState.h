#pragma once

class Cactus;
class ICactusState
{
public:
	ICactusState(Cactus* cactus)
	{
		m_cactus = cactus;
	}

	virtual ~ICactusState(){}

	/// <summary>
	/// 状態を管理する
	/// </summary>
	virtual void ManageState() = 0;
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	virtual void PlayAnimation() = 0;


protected:
	Cactus* m_cactus = nullptr;
};

