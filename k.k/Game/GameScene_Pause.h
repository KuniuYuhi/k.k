#pragma once
#include "IGameSceneState.h"
class GameScene_Pause :
    public IGameSceneState
{
public:
	/// <summary>
	/// �X�e�[�g�J�n���̏���
	/// </summary>
	void Entry() override;

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update() override;

	/// <summary>
	/// �X�e�[�g�I�����̏���
	/// </summary>
	void Exit() override;
};

