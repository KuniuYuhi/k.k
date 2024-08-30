#pragma once
#include "IMobEnemyState.h"
#include "BeholderEyeStateContext.h"

class BeholderEye;
class BeholderEyeStateContext;

class BeholderEyeState_Hit : public IMobEnemyState
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="slime">�ڋʂ̃C���X�^���X</param>
	/// <param name="stateContest">�X�e�[�g�R���e�L�X�g</param>
	BeholderEyeState_Hit(BeholderEye* beholderEye, BeholderEyeStateContext* stateContest)
	{
		m_beholderEye = beholderEye;
		m_stateCotext = stateContest;
	}

	~BeholderEyeState_Hit();

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
	BeholderEye* m_beholderEye = nullptr;
	BeholderEyeStateContext* m_stateCotext = nullptr;

	bool m_playAnimationResetFlag = true;

};
