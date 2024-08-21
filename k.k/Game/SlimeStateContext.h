#pragma once

//�X���C���̃X�e�[�g
#include "SlimeInfo.h"

class Slime;
class IMobEnemyState;

using namespace SlimeStates;

/// <summary>
/// ���u�G�l�~�[�F�X���C���̃X�e�[�g�Ǘ��N���X
/// </summary>
class SlimeStateContext
{
public:
	SlimeStateContext();
	~SlimeStateContext();


	/// <summary>
	/// ������
	/// </summary>
	/// <param name="slime">�X���C���̃C���X�^���X</param>
	/// <param name="startState">�ŏ��̃X�e�[�g</param>
	void Init(Slime* slime, EnSlimeState startState);

	/// <summary>
	/// �X�e�[�g��؂�ւ���
	/// </summary>
	/// <param name="changeState">�؂�ւ������X�e�[�g</param>
	void ChangeSlimeState(Slime* slime, EnSlimeState changeState);

	/// <summary>
	/// ���݂̃X�e�[�g�̍X�V����
	/// </summary>
	void UpdateCurrentState();
	/// <summary>
	/// ���݂̃X�e�[�g�̃A�j���[�V�����̍Đ�
	/// </summary>
	void PlayAnimationCurrentState();



	EnSlimeState GetCurrentSlimeState()
	{
		return m_enCurrentSlimeState;
	}

protected:

	EnSlimeState m_enCurrentSlimeState = enSlimeState_Idle;
	EnSlimeState m_enPreviousSlimeState = enSlimeState_Idle;

	IMobEnemyState* m_currentSlimeState = nullptr;		//���݂̃X�e�[�g

};

