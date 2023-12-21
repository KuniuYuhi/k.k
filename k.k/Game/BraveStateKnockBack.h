#pragma once
#include "IBraveState.h"

class BraveStateKnockBack :public IBraveState
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="brave"></param>
	BraveStateKnockBack(Brave* brave)
		:IBraveState(brave)
	{
	}

	/// <summary>
	/// �E�҂̏�ԑJ�ڊǗ�
	/// </summary>
	void ManageState();
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	void PlayAnimation();

private:

	/// <summary>
	/// �d�����Ă��邩�B�^�C�}�[�Ōv�Z
	/// </summary>
	/// <returns></returns>
	bool IsStiffness();

private:
	float m_stiffnessTimer = 0.0f;
	bool m_knockBackEndFlag = false;
	
};
