#pragma once
#include "IBraveState.h"

class BraveStateDefend :public IBraveState
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="brave"></param>
	BraveStateDefend(Brave* brave)
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
	/// ���ȂǂŖh���Ƃ��̏���
	/// </summary>
	void ProcessDefence();
	/// <summary>
	/// �������Ƃ��̏���
	/// </summary>
	void ProcessAvoid();
};

