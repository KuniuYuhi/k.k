#pragma once

#include "AllGameSceneState.h"
//#include "IGameSceneState.h"

class IGameSceneState;

class GameSceneContext
{
public:

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="startGameSceneState"></param>
	void Init(EnGameSceneState startGameSceneState);

	/// <summary>
	/// ���݂̃Q�[���V�[���X�e�[�g�̍X�V�������s��
	/// </summary>
	void UpdateGameSceneState();

	/// <summary>
	/// �X�e�[�g��؂�ւ���
	/// </summary>
	/// <param name="changeGameSceneState"></param>
	void ChangeState(EnGameSceneState changeGameSceneState);

	/// <summary>
	/// ���݂̃Q�[���V�[�����擾����
	/// </summary>
	/// <returns></returns>
	IGameSceneState* GetCurrentGameScene()
	{
		return m_currentGameScene;
	}


	/// <summary>
	/// ���݂̃Q�[���V�[���X�e�[�g���擾����
	/// </summary>
	/// <returns></returns>
	EnGameSceneState GetCurrentGameSceneState()
	{
		return m_currentGameSceneState;
	}
	



private:
	IGameSceneState* m_currentGameScene = nullptr;			//���݂̃V�[���X�e�[�g

	EnGameSceneState m_currentGameSceneState = enGameSceneState_None;
	EnGameSceneState m_previousGameSceneState = enGameSceneState_None;


};

