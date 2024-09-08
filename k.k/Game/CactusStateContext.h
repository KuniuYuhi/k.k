#pragma once

#include "CactusInfo.h"

class Cactus;
class IMobEnemyState;

using namespace CactusStates;

/// <summary>
/// ���u�G�l�~�[�F�T�{�e���̃X�e�[�g�Ǘ��N���X
/// </summary>
class CactusStateContext
{
public:
	CactusStateContext();
	~CactusStateContext();


	/// <summary>
	/// ������
	/// </summary>
	/// <param name="slime">�X���C���̃C���X�^���X</param>
	/// <param name="startState">�ŏ��̃X�e�[�g</param>
	void Init(Cactus* cactus, EnCactusState startState);

	/// <summary>
	/// �X�e�[�g��؂�ւ���
	/// </summary>
	/// <param name="changeState">�؂�ւ������X�e�[�g</param>
	void ChangeCactusState(Cactus* cactus, EnCactusState changeState);

	/// <summary>
	/// ���݂̃X�e�[�g�̍X�V����
	/// </summary>
	void UpdateCurrentState();
	/// <summary>
	/// ���݂̃X�e�[�g�̃A�j���[�V�����̍Đ�
	/// </summary>
	void PlayAnimationCurrentState();



	EnCactusState GetCurrentSlimeState()
	{
		return m_enCurrentSlimeState;
	}

protected:

	EnCactusState m_enCurrentSlimeState = enCactusState_Idle;
	EnCactusState m_enPreviousSlimeState = enCactusState_Idle;

	IMobEnemyState* m_currentState = nullptr;		//���݂̃X�e�[�g

};

