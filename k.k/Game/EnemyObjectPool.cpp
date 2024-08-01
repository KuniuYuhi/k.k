#include "stdafx.h"
#include "EnemyObjectPool.h"

#include "EnemyBase.h"

#include "Slime.h"



EnemyObjectPool* EnemyObjectPool::m_instance = nullptr;

EnemyObjectPool::EnemyObjectPool()
{
	if (m_instance != nullptr)
	{
		std::abort();
	}
	m_instance = this;
}

EnemyObjectPool::~EnemyObjectPool()
{
	//���g��S�ăN���A
	m_objectPoolQueue.clear();
}

void EnemyObjectPool::CreateInstance()
{
	m_instance = new EnemyObjectPool();
}

void EnemyObjectPool::Init()
{
	//�X���C���̃L���[��}��
	std::queue<EnemyBase*> slimeQueue;

	EnemyBase* slime = NewGO<Slime>(0, "Slime");
	slime->Deactivate();
	slimeQueue.push(slime);

	m_objectPoolQueue.insert(std::make_pair("Slime", slimeQueue));



	for (int i = 0; i < 5; i++)
	{
		OnCreateEnemy<Slime>("Slime");
	}

}