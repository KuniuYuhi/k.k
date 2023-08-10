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
	/// ��Ԃ��Ǘ�����
	/// </summary>
	virtual void ManageState() = 0;
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	virtual void PlayAnimation() = 0;


protected:
	Cactus* m_cactus = nullptr;
};

