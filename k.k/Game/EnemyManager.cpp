#include "stdafx.h"
#include "EnemyManager.h"
#include "MobEnemyBase.h"

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
			return a->CalcDistanceToTargetPosition(targetPosition) < b->CalcDistanceToTargetPosition(targetPosition);
		});
}
