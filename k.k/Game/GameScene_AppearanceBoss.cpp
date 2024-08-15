#include "stdafx.h"
#include "GameScene_AppearanceBoss.h"

#include "EntryBoss.h"
#include "Game.h"

#include "GameSceneManager.h"



void GameScene_AppearanceBoss::Entry()
{
	//�{�X�o��N���X����
	m_entryBoss = NewGO<EntryBoss>(0, "entryboss");
	
	Game* game = FindGO<Game>("game");
	game->CreatePlayerAndCamera();

	

}

void GameScene_AppearanceBoss::Update()
{
	//���Ԍo�߂��X�L�b�v���ꂽ�玟�̃X�e�[�g�ɐ؂�ւ���
	if (GameSceneManager::GetInstance()->GetIsSceneChangeableFlag())
	{
		//�V�[���؂�ւ��\���t���O�����Z�b�g
		GameSceneManager::GetInstance()->SetIsSceneChangeableFlag(false);
		//�Q�[���V�[���ɐ؂�ւ�
		GameSceneManager::GetInstance()->ChangeGameSceneState(enGameSceneState_Game);
		return;
	}

}

void GameScene_AppearanceBoss::Exit()
{
	//�{�X�o��N���X�폜
	DeleteGO(m_entryBoss);
}
