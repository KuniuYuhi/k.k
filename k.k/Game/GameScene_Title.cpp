#include "stdafx.h"
#include "GameScene_Title.h"

#include "GameSceneManager.h"

#include "Title.h"

#include "Fade.h"


void GameScene_Title::Entry()
{
	//�V�[���؂�ւ��\���t���O�����Z�b�g
	GameSceneManager::GetInstance()->SetIsSceneChangeableFlag(false);

	Title* m_title = NewGO<Title>(0, "title");

	Fade* fade = FindGO<Fade>("fade");
	fade->StartFadeOut(5.0f);

}

void GameScene_Title::Update()
{
	//�o�g���X�^�[�g�̏������I������玟�̃X�e�[�g�ɑJ��
	if (GameSceneManager::GetInstance()->GetIsSceneChangeableFlag())
	{
		//�V�[���؂�ւ��\���t���O�����Z�b�g
		GameSceneManager::GetInstance()->SetIsSceneChangeableFlag(false);
		//�{�X�o��V�[���ɐ؂�ւ�
		GameSceneManager::GetInstance()->ChangeGameSceneState(enGameSceneState_GameStart);
		return;
	}
}

void GameScene_Title::Exit()
{
}
