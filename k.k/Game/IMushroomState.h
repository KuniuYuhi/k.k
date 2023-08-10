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
	/// ��Ԃ��Ǘ�����
	/// </summary>
	virtual void ManageState() = 0;
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	virtual void PlayAnimation() = 0;


protected:
	Mushroom* m_mushroom = nullptr;
};

