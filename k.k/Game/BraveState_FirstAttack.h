#pragma once
#include "IBraveState.h"

class PlayerController;

class BraveState_FirstAttack :
    public IBraveState
{
public:
	BraveState_FirstAttack(Brave* brave, BraveStateContext* stateContext)
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


private:

	PlayerController* m_playerController = nullptr;

	bool m_isTrigger = false;

	bool m_isNextNormalAttackProgress = false;	//���̍U���ɐi�ނ̂�

};

