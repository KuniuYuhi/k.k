#include "stdafx.h"
#include "GameSceneManager.h"

#include "GameSceneContext.h"



GameSceneManager* GameSceneManager::m_instance = nullptr;

GameSceneManager::GameSceneManager()
{
	if (m_instance != nullptr) {
		//�C���X�^���X�����łɍ���Ă���B
		std::abort();
	}
	m_instance = this;

}

GameSceneManager::~GameSceneManager()
{
	m_instance = nullptr;
}

void GameSceneManager::CreateInstanceAndSetGameSceneState(EnGameSceneState setGameSceneState)
{
	//�Q�[���V�[���𐶐�
	m_instance = new GameSceneManager();
	//�Q�[���V�[���R���e�L�X�g�𐶐�
	m_instance->CreateGameSceneContext(setGameSceneState);
}

void GameSceneManager::CreateGameSceneContext(EnGameSceneState setGameSceneState)
{
	m_gameSceneContext = std::make_unique<GameSceneContext>();
	//�����������ŊǗ�����X�e�[�g��ݒ�
	m_gameSceneContext.get()->Init(setGameSceneState);
}

void GameSceneManager::Update()
{
	if (m_gameSceneContext == nullptr)
	{
		return;
	}
	//���݂̃V�[���̍X�V���������s
	m_gameSceneContext.get()->UpdateGameSceneState();
}

void GameSceneManager::ChangeGameSceneState(EnGameSceneState cangeGameSceneState)
{
	m_gameSceneContext.get()->ChangeState(cangeGameSceneState);
}

const EnGameSceneState& GameSceneManager::GetCurrentGameSceneState()
{
	return m_gameSceneContext.get()->GetCurrentGameSceneState();
}
