#include "stdafx.h"
#include "GameManager.h"
#include "CharactersInfoManager.h"

#include "BattleStart.h"
#include "Player.h"
#include "Lich.h"

namespace {
	const float SECOND_MAX = 60.0f;
}

GameManager* GameManager::m_instance = nullptr;

GameManager::GameManager(EnGameSeenState startGameSeenState)
{
	if (m_instance != nullptr) {
		//�C���X�^���X�����łɍ���Ă���B
		std::abort();
	}
	m_instance = this;
	//�Q�[���J�n���̃X�e�[�g��ݒ�
	m_enGameSeenState = startGameSeenState;
}

GameManager::~GameManager()
{
	//�L�������}�l�[�W���[�폜
	CharactersInfoManager::DeleteInstance();

	m_instance = nullptr;
}

void GameManager::Init()
{
	//�L�����N�^�[�̏��}�l�[�W���[�̐���
	CharactersInfoManager::CreateInstance();
}

void GameManager::Execute()
{


	CalcTimeLimmit();
}

void GameManager::CreateBattleStartClass()
{
	//�o�g���X�^�[�g�N���X����
	BattleStart* battleStart = NewGO<BattleStart>(0, "battlestart");
}

void GameManager::CreatePlayerClass()
{
	Player* player = NewGO<Player>(0, "player");
	CharactersInfoManager::GetInstance()->SetPlayerInstance(player);
}

void GameManager::CreateBoss()
{
	Lich* lich = NewGO<Lich>(0, "lich");
	CharactersInfoManager::GetInstance()->SetLichInstance(lich);
}



void GameManager::CalcTimeLimmit()
{


	//0�b�ȉ��Ȃ�
	if (m_second <= 0) {
		//1�����炷
		m_minute--;
		//����0���Ȃ�A�b��0�ɂ���
		if (m_minute < 0) {
			m_second = 0.0f;
			m_minute = 0.0f;
			//�������ԂɒB����
			//�Q�[���I�[�o�[�X�e�[�g��ݒ�
			SetGameSeenState(enGameSeenState_GameOver);
		}
		//60�b�ɖ߂�
		else
		{
			m_second = SECOND_MAX;
		}
	}
	else
	{
		//�b�����炷
		m_second -= g_gameTime->GetFrameDeltaTime();
	}
}




