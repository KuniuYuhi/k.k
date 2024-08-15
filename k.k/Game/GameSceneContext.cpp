#include "stdafx.h"
#include "GameSceneContext.h"


#include "IGameSceneState.h"

#include "GameScene_Title.h"
#include "GameScene_GameStart.h"
#include "GameScene_AppearanceBoss.h"
#include "GameScene_Game.h"
#include "GameScene_Pause.h"
#include "GameScene_Result.h"

void GameSceneContext::Init(EnGameSceneState startGameSceneState)
{

	//�X�e�[�g��؂�ւ���
	ChangeState(startGameSceneState);

}

void GameSceneContext::UpdateGameSceneState()
{
	if (m_currentGameScene == nullptr)
	{
		return;
	}

	m_currentGameScene->Update();
}

void GameSceneContext::ChangeState(EnGameSceneState changeGameSceneState)
{
	//���g������Ȃ�폜
	if (m_currentGameScene != nullptr)
	{
		//�폜�O�̏���
		m_currentGameScene->Exit();
		//�폜
		delete m_currentGameScene;
	}

	//�X�e�[�g�𐶐�����
	switch (changeGameSceneState)
	{
	case enGameSceneState_Title:
		m_currentGameScene = new GameScene_Title();
		break;
	case enGameSceneState_GameStart:
		m_currentGameScene = new GameScene_GameStart();
		break;
	case enGameSceneState_AppearanceBoss:
		m_currentGameScene = new GameScene_AppearanceBoss();
		break;
	case enGameSceneState_Game:
		m_currentGameScene = new GameScene_Game();
		break;
	case enGameSceneState_Pause:
		m_currentGameScene = new GameScene_Pause();
		break;
	case enGameSceneState_Result:
		m_currentGameScene = new GameScene_Result();
		break;
	default:
		std::abort();
		break;
	}

	//�O�̃X�e�[�g��ۑ�
	m_previousGameSceneState = m_currentGameSceneState;
	//���݂̃X�e�[�g�������̃X�e�[�g�ɂ���
	m_currentGameSceneState = changeGameSceneState;

	//�X�e�[�g�J�ڎ��̍ŏ��̏���
	m_currentGameScene->Entry();

}
