#include "stdafx.h"
#include "CharactersInfoManager.h"

#include "Boss.h"

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

	//�����X�^�[�̔ԍ��̃J�E���g
	int monsterNumberCount = 0;

	//�ǂ������郂���X�^�[�̔z��B�����X�^�[�̑�����m_mobMonsters�̔z��̔ԍ�
	//��s
	std::vector<std::vector<float>> array =
	{ {-1.0f,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f },
	{ -1.0f,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f } };
	

	for (auto monster : m_mobMonsters)
	{
		//�v���C���[���烂���X�^�[�Ɍ������x�N�g�����v�Z
		Vector3 diff = monster->GetPosition() - m_playerInstance->GetPosition();

		//���������ȉ��Ȃ�
		if (diff.Length() < MONSTER_NEAR_PLAYER_DISTANCE)
		{
			//�x�N�g���ƃ����X�^�[�̔z��̔ԍ���ݒ�
			array[0][m_monstersNearPlayerCount] = diff.Length();
			array[1][m_monstersNearPlayerCount] = monsterNumberCount;

			//�J�E���g�����Z
			m_monstersNearPlayerCount++;
		}
		//�J�E���g���Z
		monsterNumberCount++;
	}

	//todo�I�����������߂ɔz���-1���i�[


	//�v���C���[�ɋ߂������X�^�[�����Ȃ������珈�������Ȃ�
	if (m_monstersNearPlayerCount == 0)
	{
		return;
	}

	//���X�g�������X�^�[���߂����ɓ���ւ���
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
		}
	}
	//�v���C���[�ɋ߂Â��郂���X�^�[�̐��J��Ԃ�
	for (int i = 0; i < m_monstersNearPlayerCount; i++)
	{
		//�v���C���[��ǂ������邱�Ƃ��ł��鐔
		if (i < NUM_ENABLE_CHASE_PLAYER)
		{
			//���̃����X�^�[�͒ǂ���������
			m_mobMonsters[array[1][i]]->SetEnableChasePlayerFag(true);
		}
		else
		{
			//�ǂ��������鐔�𒴂�����I��
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

void CharactersInfoManager::SetAllMonsterDamgeHitFlag(bool flag)
{
	//�{�X�̃t���O��ݒ�
	m_bossInstance->SetDamageHitEnableFlag(flag);

	//�����X�^�[�̐���0�Ȃ珈�����Ȃ�
	if (m_mobMonsters.size() == 0)
	{
		return;
	}

	for (auto monster : m_mobMonsters)
	{
		//�����X�^�[�̃t���O��ݒ�
		monster->SetDamageHitEnableFlag(flag);
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
