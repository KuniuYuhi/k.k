#include "stdafx.h"
#include "GameSceneManager.h"

#include "GameSceneContext.h"



GameSceneManager* GameSceneManager::m_instance = nullptr;


namespace {

	const float TIME_LIMIT = 5.0f;		//��������
	const float SECONT_MAX = 60.0f;
}

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

	//�^�C�}�[�̍X�V
	UpdateGameTimer();
}

void GameSceneManager::ChangeGameSceneState(EnGameSceneState cangeGameSceneState)
{
	m_gameSceneContext.get()->ChangeState(cangeGameSceneState);
}

const EnGameSceneState& GameSceneManager::GetCurrentGameSceneState()
{
	return m_gameSceneContext.get()->GetCurrentGameSceneState();
}



void GameSceneManager::StartGameTimer()
{
	m_isStartGameTimer = true;
	//�������Ԃ�ݒ�
	m_gameTimer.SetMinute(TIME_LIMIT);

	m_isTimeOver = false;
}

void GameSceneManager::UpdateGameTimer()
{
	//�X�^�[�g���Ă��Ȃ��Ȃ珈�����Ȃ�
	if (!m_isStartGameTimer) return;

	//���Ԑ؂�Ȃ珈�����Ȃ�
	if (m_isTimeOver) return;

	CalcGameTimer();

}

void GameSceneManager::CalcGameTimer()
{
	//0�b�ȉ��Ȃ�
	if (m_gameTimer.GetSecond() <= 0) {
		//1�����炷
		m_gameTimer.SubMinute(1.0f);
		//����0���Ȃ�A�b��0�ɂ���
		if (m_gameTimer.GetMinute() < 0) {
			m_gameTimer.SetSecond(0.0f);
			m_gameTimer.SetMinute(0.0f);
			
			//���Ԑ؂�I�I�I
			m_isTimeOver = true;
			//���s��ݒ�i�v���C���[�̕����I�I�j
			SetOutComPlayerLose();
		}
		//60�b�ɖ߂�
		else
		{
			m_gameTimer.SetSecond(SECONT_MAX);
		}
	}
	else
	{
		//�b�����炷
		m_gameTimer.SubSecond(g_gameTime->GetFrameDeltaTime());
	}


}
