#include "stdafx.h"
#include "GameScene_GameStart.h"

#include "GameSceneManager.h"

#include "Game.h"
#include "BattleStart.h"


void GameScene_GameStart::Entry()
{
	//�V�[���؂�ւ��\���t���O�����Z�b�g
	GameSceneManager::GetInstance()->SetIsSceneChangeableFlag(false);


	Game* game = NewGO<Game>(0, "game");

	//�o�g���X�^�[�g�N���X�쐬
	m_battleStart = NewGO<BattleStart>(0, "battlestart");
}

void GameScene_GameStart::Update()
{
	//�o�g���X�^�[�g�̏������I������玟�̃X�e�[�g�ɑJ��
	if (GameSceneManager::GetInstance()->GetIsSceneChangeableFlag())
	{
		//�V�[���؂�ւ��\���t���O�����Z�b�g
		GameSceneManager::GetInstance()->SetIsSceneChangeableFlag(false);
		//�{�X�o��V�[���ɐ؂�ւ�
		GameSceneManager::GetInstance()->ChangeGameSceneState(enGameSceneState_AppearanceBoss);
		return;
	}

}

void GameScene_GameStart::Exit()
{
	//�o�g���X�^�[�g�N���X���폜
	DeleteGO(m_battleStart);
}
