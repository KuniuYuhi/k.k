#pragma once
#include "IGameSceneState.h"

class Game;

class GameScene_Game : public IGameSceneState
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




private:
	Game* m_game = nullptr;


};

