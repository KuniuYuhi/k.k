#include "stdafx.h"
#include "GameScene_Result.h"

#include "Result.h"
#include "GameFinishCamera.h"
#include "Game.h"

#include "GameSceneManager.h"


void GameScene_Result::Entry()
{
	//�V�[���؂�ւ��\���t���O�����Z�b�g
	GameSceneManager::GetInstance()->SetIsSceneChangeableFlag(false);

	//�����������������ɂ���ď�����ς���

	switch (GameSceneManager::GetInstance()->GetBattleOutCome())
	{
	case GameSceneManager::enBattleOutCome_PlayerWin:
		//�v���C���[�����������I
		PlayerWinProcess();
		break;
	case GameSceneManager::enBattleOutCome_PlayerLose:
		//�v���C���[���s�k�����c
		PlayerLoseProcess();
		break;
	default:
		std::abort();
		break;
	}


	//���U���g�V�[���𐶐�
	//CreateResult();

	
}

void GameScene_Result::Update()
{
	//���U���g�Ń��[�f�B���O�𐶐�����
	//�S���������I�������

	//
	if (GameSceneManager::GetInstance()->GetIsSceneChangeableFlag())
	{

		//�V�[���؂�ւ��\���t���O�����Z�b�g
		GameSceneManager::GetInstance()->SetIsSceneChangeableFlag(false);
		//�^�C�g���V�[���ɐ؂�ւ�
		GameSceneManager::GetInstance()->ChangeGameSceneState(enGameSceneState_Title);
		return;
	}


}

void GameScene_Result::Exit()
{
	if (m_gameFinishCamera != nullptr)
	{
		DeleteGO(m_gameFinishCamera);
	}
	
	Game* game = FindGO<Game>("game");
	game->DeleteThis();

	//���U���g�V�[�����폜
	DeleteGO(m_resultSeen);

	//���s�����Z�b�g����
	GameSceneManager::GetInstance()->SetBattleOutCome(GameSceneManager::enBattleOutCome_None);

}



void GameScene_Result::PlayerWinProcess()
{
	//�Q�[���J�����̍폜
	Game* game = FindGO<Game>("game");

	game->DeleteGameCamera();

	//�Q�[���t�B�j�b�V���J�����̐���
	m_gameFinishCamera = NewGO<GameFinishCamera>(0, "gamefinishcamera");

	//BGM�����������Ă���

	
}

void GameScene_Result::PlayerLoseProcess()
{
	//BGM�����������Ă���

	CreateResult();
}

void GameScene_Result::CreateResult()
{
	m_resultSeen = NewGO<ResultSeen>(0, "result");


}
