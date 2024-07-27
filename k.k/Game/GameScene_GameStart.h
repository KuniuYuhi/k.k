#pragma once
#include "IGameSceneState.h"

class BattleStart;

class GameScene_GameStart : public IGameSceneState
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
	BattleStart* m_battleStart = nullptr;


};

