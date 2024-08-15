#include "stdafx.h"
#include "GameScene_Game.h"

#include "GameSceneManager.h"

#include "Fade.h"

#include "GameCamera.h"

#include "Game.h"

namespace {
	const float DEFAULT_BATTLE_BGM = 2.0f;

}

void GameScene_Game::Entry()
{
	//�t�F�[�h�A�E�g����

	Fade* fade = FindGO<Fade>("fade");
	fade->StartFadeOut(3.0f);

	GameCamera* gameCamera = FindGO<GameCamera>("gameCamera");
	//�Q�[���J�n���̃J�����̐ݒ�
	gameCamera->SetBattleStartCamera();

	//todo �����ŃQ�[���N���X���쐬


	//�{�X�𐶐�
	m_game = FindGO<Game>("game");
	m_game->CreateBoss();
	m_game->CreateGameUI();
	m_game->CreateBattlePhase();

	//BGM�̉��ʂ��グ��
		//BGM�̍Đ�
	g_soundManager->InitAndPlaySoundSource(enSoundName_BattleBGM, DEFAULT_BATTLE_BGM, false, true, true);

}

void GameScene_Game::Update()
{
	//���s����������X�e�[�g��ύX
	if (GameSceneManager::GetInstance()->GetBattleOutCome() !=
		GameSceneManager::enBattleOutCome_None)
	{

		g_soundManager->StopSound(enSoundName_BattleBGM);

		//�V�[���؂�ւ��\���t���O�����Z�b�g
		GameSceneManager::GetInstance()->SetIsSceneChangeableFlag(false);
		//���U���g�V�[���ɐ؂�ւ�
		GameSceneManager::GetInstance()->ChangeGameSceneState(enGameSceneState_Result);
		return;
	}


	//
	m_game->UpdateGameSystem();

}

void GameScene_Game::Exit()
{
	//

}

