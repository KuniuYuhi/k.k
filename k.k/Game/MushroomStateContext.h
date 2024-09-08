#pragma once

#include "MushroomInfo.h"

class Mushroom;
class IMobEnemyState;

using namespace MushroomStates;

/// <summary>
/// ���u�G�l�~�[�F�L�m�R�̃X�e�[�g�Ǘ��N���X
/// </summary>
class MushroomStateContext
{
public:
	MushroomStateContext();
	~MushroomStateContext();


	/// <summary>
	/// ������
	/// </summary>
	/// <param name="slime">�L�m�R�̃C���X�^���X</param>
	/// <param name="startState">�ŏ��̃X�e�[�g</param>
	void Init(Mushroom* mushroom, EnMushroomState startState);

	/// <summary>
	/// �X�e�[�g��؂�ւ���
	/// </summary>
	/// <param name="changeState">�؂�ւ������X�e�[�g</param>
	void ChangeMushroomState(Mushroom* mushroom, EnMushroomState changeState);

	/// <summary>
	/// ���݂̃X�e�[�g�̍X�V����
	/// </summary>
	void UpdateCurrentState();
	/// <summary>
	/// ���݂̃X�e�[�g�̃A�j���[�V�����̍Đ�
	/// </summary>
	void PlayAnimationCurrentState();



	EnMushroomState GetCurrentSlimeState()
	{
		return m_enCurrentSlimeState;
	}

protected:

	EnMushroomState m_enCurrentSlimeState = enMushroomState_Idle;
	EnMushroomState m_enPreviousSlimeState = enMushroomState_Idle;

	IMobEnemyState* m_currentState = nullptr;		//���݂̃X�e�[�g

};

