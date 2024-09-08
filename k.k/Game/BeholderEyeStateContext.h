#pragma once
#include "BeholderEyeInfo.h"

class BeholderEye;
class IMobEnemyState;

using namespace BeholderEyeStates;


class BeholderEyeStateContext
{
public:
	BeholderEyeStateContext();
	~BeholderEyeStateContext();


	/// <summary>
	/// ������
	/// </summary>
	/// <param name="beholderEye">�ڋʂ̃C���X�^���X</param>
	/// <param name="startState">�ŏ��̃X�e�[�g</param>
	void Init(BeholderEye* beholderEye, EnBeholderEyeState startState);

	/// <summary>
	/// �X�e�[�g��؂�ւ���
	/// </summary>
	/// <param name="changeState">�؂�ւ������X�e�[�g</param>
	void ChangeBeholderEyeState(BeholderEye* beholderEye, EnBeholderEyeState changeState);

	/// <summary>
	/// ���݂̃X�e�[�g�̍X�V����
	/// </summary>
	void UpdateCurrentState();
	/// <summary>
	/// ���݂̃X�e�[�g�̃A�j���[�V�����̍Đ�
	/// </summary>
	void PlayAnimationCurrentState();



	EnBeholderEyeState GetCurrentSlimeState()
	{
		return m_enCurrentSlimeState;
	}

protected:

	EnBeholderEyeState m_enCurrentSlimeState = enBeholderEyeState_Idle;
	EnBeholderEyeState m_enPreviousSlimeState = enBeholderEyeState_Idle;

	IMobEnemyState* m_currentState = nullptr;		//���݂̃X�e�[�g

};

