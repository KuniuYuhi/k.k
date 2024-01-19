#include "stdafx.h"
#include "CharactersInfoManager.h"

namespace {
	const float MONSTER_NEAR_PLAYER_DISTANCE = 350.0f;

	const int NUM_MONSTERS = 15;

	const int NUM_ENABLE_CHASE_PLAYER = 3;		//�v���C���[��ǂ��������郂���X�^�[�̐�

}

CharactersInfoManager* CharactersInfoManager::m_charactersInfoInstance = nullptr;

CharactersInfoManager::CharactersInfoManager()
{
	if (m_charactersInfoInstance != nullptr) {
		//�C���X�^���X�����łɍ���Ă���B
		std::abort();
	}
	m_charactersInfoInstance = this;
}

CharactersInfoManager::~CharactersInfoManager()
{
	m_charactersInfoInstance = nullptr;
}

void CharactersInfoManager::Execute()
{
	SearchMonstersNearPlayer();
}

void CharactersInfoManager::SearchMonstersNearPlayer()
{
	//�J�E���g��0�ɂ���
	m_monstersNearPlayerCount = 0;

	//�v���C���[�̃C���X�^���X���Ȃ��Ȃ珈�����Ȃ�
	if (m_playerInstance == nullptr)
	{
		return;
	}

	//�����X�^�[�̐���0�Ȃ珈�����Ȃ�
	if (m_mobMonsters.size() == 0)
	{
		return;
	}


	//�ǂ������郂���X�^�[�̔z��B�����X�^�[�̑�����m_mobMonsters�̔z��̔ԍ�
	//�s��
	//float chaseMonsters[NUM_MONSTERS][2];
	//�����X�^�[�̔ԍ��̃J�E���g
	int monsterNumberCount = 0;

	//��s
	std::vector<std::vector<float>> array =
	{ {-1.0f,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f },
	{ -1.0f,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f } };
	

	for (auto monster : m_mobMonsters)
	{
		//���͈̔͊O�Ȃ�l��-�P�ɂ���
		//chaseMonsters[m_monstersNearPlayerCount][0] = -1.0f;
		//chaseMonsters[m_monstersNearPlayerCount][1] = -1;

		//�v���C���[���烂���X�^�[�Ɍ������x�N�g�����v�Z
		Vector3 diff = monster->GetPosition() - m_playerInstance->GetPosition();

		//���������ȉ��Ȃ�
		if (diff.Length() < MONSTER_NEAR_PLAYER_DISTANCE)
		{
			//���W�ƃ����X�^�[�̔ԍ���ۑ�
			//chaseMonsters[m_monstersNearPlayerCount][0] = diff.Length();
			//chaseMonsters[m_monstersNearPlayerCount][1] = monsterNumberCount;
		
			array[0][m_monstersNearPlayerCount] = diff.Length();
			array[1][m_monstersNearPlayerCount] = monsterNumberCount;

			//�J�E���g�����Z
			m_monstersNearPlayerCount++;
		}
		

		//�J�E���g���Z
		monsterNumberCount++;
	}

	//�v���C���[�ɋ߂������X�^�[�����Ȃ������珈�������Ȃ�
	if (m_monstersNearPlayerCount == 0)
	{
		return;
	}


	for (int i = 0; i < m_monstersNearPlayerCount; i++)
	{
		for (int j = i + 1; j < m_monstersNearPlayerCount; j++)
		{
			if (array[0][i] > array[0][j])
			{
				//�����̓���ւ�
				int temp = array[0][i];
				array[0][i] = array[0][j];
				array[0][j] = temp;
				//�ԍ��̓���ւ�
				temp = array[1][i];
				array[1][i] = array[1][j];
				array[1][j] = temp;

			}

			//if (chaseMonsters[i][0] > chaseMonsters[j][0])
			//{
			//	//�����̓���ւ�
			//	int temp = chaseMonsters[i][0];
			//	chaseMonsters[i][0] = chaseMonsters[j][0];
			//	chaseMonsters[j][0] = temp;
			//	//�ԍ��̓���ւ�
			//	temp = chaseMonsters[i][1];
			//	chaseMonsters[i][1] = chaseMonsters[j][1];
			//	chaseMonsters[j][1] = temp;

			//}
		}
	}


	



	//for (int i = 0; i < NUM_ENABLE_CHASE_PLAYER; i++)
	//{		
	//	//-�P�̎��͂������X�g���Ȃ��̂ŏ������I���
	//	/*if (chaseMonsters[i][1] == -1)
	//	{
	//		break;
	//	}*/

	//	//���̃����X�^�[�̓v���C���[��ǂ���������I
	//	//m_mobMonsters[chaseMonsters[i][1]]->SetEnableChasePlayerFag(true);
	//}
	

	for (int i = 0; i < m_monstersNearPlayerCount; i++)
	{
		if (i < NUM_ENABLE_CHASE_PLAYER)
		{
			//int a = 0;
			m_mobMonsters[array[1][i]]->SetEnableChasePlayerFag(true);
		}
		else
		{
			return;
		}
	}
}

void CharactersInfoManager::SortMonsterToPlayerLength(float array[][2], int maxValue)
{
	for (int i = 0; i < maxValue; i++)
	{
		for (int j = i + 1; j < maxValue; j++)
		{
			//
			if (array[i][0] > array[j][0])
			{
				//�����̓���ւ�
				int temp = array[i][0];
				array[i][0] = array[j][0];
				array[j][0] = temp;
				//�ԍ��̓���ւ�
				temp = array[i][1];
				array[i][1] = array[j][1];
				array[j][1] = temp;

			}
		}
	}
}

bool CharactersInfoManager::SearchMonsterNearPlayer(
	MobMonster* mobMonster)
{
	Vector3 diff= mobMonster->GetPosition() - m_playerInstance->GetPosition();

	//���������ȉ��Ȃ�
	if (diff.Length() < MONSTER_NEAR_PLAYER_DISTANCE)
	{
		//�J�E���g�����Z
		m_monstersNearPlayerCount++;
		//�v���C���[�ɋ߂�
		return true;
	}
	//�v���C���[�ɋ߂��Ȃ�
	return false;
}
