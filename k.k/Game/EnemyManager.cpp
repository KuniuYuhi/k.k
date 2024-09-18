#include "stdafx.h"
#include "EnemyManager.h"
#include "MobEnemyBase.h"

#include "Summoner.h"

EnemyManager* EnemyManager::m_instance = nullptr;

EnemyManager::EnemyManager()
{
	if (m_instance != nullptr) {
		//インスタンスがすでに作られている。
		std::abort();
	}
	m_instance = this;
}

EnemyManager::~EnemyManager()
{
	m_instance = nullptr;
}

void EnemyManager::CreateInstance()
{
	m_instance = new EnemyManager();
}

void EnemyManager::DeleteInstance()
{
	delete m_instance;
	m_instance = nullptr;
}

void EnemyManager::Update()
{

}

void EnemyManager::ControlEnemyDistances(Vector3 targetPosition)
{
	//モブエネミーリストの中身がないなら処理しない
	if (m_mobEnemyList.size() == 0) return;

	//全エネミーの待機フラグを立てておく
	for (auto enemy : m_mobEnemyList)
	{
		enemy->SetWaitingFlag(true);
	}
	
	//ソートしてプレイヤーに対する距離が近い順に並び替える
	SortEnemiesByDistanceToTarget(targetPosition);
	
	
	for (int num = 0; num < m_approachMobEnemyAmount; num++)
	{
		//追いかけるエネミーの数がリストより多くなったら抜け出す
		if (m_mobEnemyList.size() <= num) break;

		//
		if (!m_mobEnemyList[num]->IsActive()) continue;

		//近くまで追いかけてエネミーの待機フラグを下げておく
		m_mobEnemyList[num]->SetWaitingFlag(false);
	}

}

void EnemyManager::SortEnemiesByDistanceToTarget(Vector3 targetPosition)
{
	std::sort(m_mobEnemyList.begin(), m_mobEnemyList.end(),
		[targetPosition](MobEnemyBase* a, MobEnemyBase* b)
		{
			return a->GetDistanceToTargetPositionValue(targetPosition) < b->GetDistanceToTargetPositionValue(targetPosition);
		});
}

int EnemyManager::GetNearbyEnemyCount(Vector3 targetPosition, float radius)
{
	int count = 0;

	for (auto enemy: m_mobEnemyList)
	{
		//ターゲットまでの距離が半径以下ならカウント加算
		if (enemy->GetDistanceToTargetPositionValue(targetPosition) <= radius)
		{
			count++;
		}
	}

	return count;
}

void EnemyManager::DeleteAllEnemy(bool isPlayDieEffect)
{
	for (auto enemy : m_mobEnemyList)
	{
		//削除時の処理
		enemy->DieFlomOutside(isPlayDieEffect);
	}

	m_mobEnemyList.clear();

}

void EnemyManager::WinProcessAllEnemy()
{
	for (auto enemy : m_mobEnemyList)
	{
		//勝利時の処理
		enemy->WinProcess();
	}

	Summoner* summoner = FindGO<Summoner>("Summoner");

	if (summoner != nullptr)
	{
		//勝利時の処理
		summoner->WinProcess();
	}

}

void EnemyManager::GameOutComeProcess()
{
	for (auto enemy : m_mobEnemyList)
	{
		//強制的に待機状態にする
		enemy->ForceChangeStateIdle();
	}


}
