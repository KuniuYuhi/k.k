#pragma once
#include "IGameSceneState.h"


class EntryBoss;

class GameScene_AppearanceBoss :
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

private:
	EntryBoss* m_entryBoss = nullptr;

};

