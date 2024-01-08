#include "stdafx.h"
#include "CharactersInfoManager.h"

namespace {
	const float MONSTER_NEAR_PLAYER_DISTANCE = 350.0f;
}

CharactersInfoManager* CharactersInfoManager::m_charactersInfoInstance = nullptr;

CharactersInfoManager::CharactersInfoManager()
{
	if (m_charactersInfoInstance != nullptr) {
		//インスタンスがすでに作られている。
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
	//カウントを0にする
	m_monstersNearPlayerCount = 0;

	if (m_playerInstance == nullptr)
	{
		return;
	}

	//モンスターの数が0なら
	if (m_mobMonsters.size() == 0)
	{
		return;
	}

	//
	//for (auto monster : m_mobMonsters)
	//{
	//	//プレイヤーからモンスターに向かうベクトルを計算
	//	Vector3 diff = monster->GetPosition() - m_playerInstance->GetPosition();
	//	//距離が一定以下なら
	//	if (diff.Length() < MONSTER_NEAR_PLAYER_DISTANCE)
	//	{
	//		//カウントを加算
	//		m_monstersNearPlayerCount++;
	//	}
	//}

}

bool CharactersInfoManager::SearchMonsterNearPlayer(
	MobMonster* mobMonster)
{
	Vector3 diff= mobMonster->GetPosition() - m_playerInstance->GetPosition();

	//距離が一定以下なら
	if (diff.Length() < MONSTER_NEAR_PLAYER_DISTANCE)
	{
		//カウントを加算
		m_monstersNearPlayerCount++;
		//プレイヤーに近い
		return true;
	}
	//プレイヤーに近くない
	return false;
}
