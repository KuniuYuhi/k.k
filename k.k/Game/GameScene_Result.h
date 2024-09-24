#pragma once
#include "IGameSceneState.h"


class ResultSeen;
class GameFinishCamera;

class GameScene_Result : public IGameSceneState
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

	void PlayerWinProcess();

	void PlayerLoseProcess();


	void CreateResult();



private:

	ResultSeen* m_resultSeen = nullptr;
	GameFinishCamera* m_gameFinishCamera = nullptr;

};

