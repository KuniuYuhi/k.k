#pragma once
#include "IBraveState.h"
class BraveState_SkillStart :
    public IBraveState
{
public:
	BraveState_SkillStart(Brave* brave, BraveStateContext* stateContext)
		:IBraveState(brave,stateContext)
	{
	}

	/// <summary>
	/// �A�j���[�V�������Đ�
	/// </summary>
	void PlayAnimation() override;
	/// <summary>
	/// ���������̏���
	/// </summary>
	void Entry() override;
	/// <summary>
	/// �X�V����
	/// </summary>
	void Ubdate() override;
	/// <summary>
	/// �I�����̏���
	/// </summary>
	void Exit() override;
};
