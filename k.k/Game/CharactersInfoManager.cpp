#include "stdafx.h"
#include "CharactersInfoManager.h"

namespace {
	const float MONSTER_NEAR_PLAYER_DISTANCE = 350.0f;
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

	if (m_playerInstance == nullptr)
	{
		return;
	}

	//�����X�^�[�̐���0�Ȃ�
	if (m_mobMonsters.size() == 0)
	{
		return;
	}

	//
	//for (auto monster : m_mobMonsters)
	//{
	//	//�v���C���[���烂���X�^�[�Ɍ������x�N�g�����v�Z
	//	Vector3 diff = monster->GetPosition() - m_playerInstance->GetPosition();
	//	//���������ȉ��Ȃ�
	//	if (diff.Length() < MONSTER_NEAR_PLAYER_DISTANCE)
	//	{
	//		//�J�E���g�����Z
	//		m_monstersNearPlayerCount++;
	//	}
	//}

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
