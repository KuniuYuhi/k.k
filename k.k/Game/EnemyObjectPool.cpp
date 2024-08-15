#include "stdafx.h"
#include "EnemyObjectPool.h"

#include "EnemyBase.h"

//////////////////////////////////////////////
//モブエネミーたち
//////////////////////////////////////////////
#include "Slime.h"
#include "Cactus.h"
//////////////////////////////////////////////

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
	//中身を全てクリア
	m_objectPoolQueue.clear();
}

void EnemyObjectPool::CreateInstance()
{
	m_instance = new EnemyObjectPool();
}

void EnemyObjectPool::Init()
{
	//スライムのキューを挿入
	std::queue<EnemyBase*> slimeQueue;
	EnemyBase* slime = NewGO<Slime>(0, "Slime");
	//非アクティブ化
	slime->Deactivate();
	slimeQueue.push(slime);
	//インサート
	m_objectPoolQueue.insert(std::make_pair("Slime", slimeQueue));
	//五個スライムをキューに追加
	for (int i = 0; i < 10; i++)
	{
		OnCreateEnemy<Slime>("Slime");
	}



	//カクタスのキューを挿入
	std::queue<EnemyBase*> cactusQueue;
	EnemyBase* cactus = NewGO<Cactus>(0, "Cactus");
	//非アクティブ化
	cactus->Deactivate();
	cactusQueue.push(cactus);
	//インサート
	m_objectPoolQueue.insert(std::make_pair("Cactus", cactusQueue));
	//五個カクタスをキューに追加
	for (int i = 0; i < 10; i++)
	{
		OnCreateEnemy<Cactus>("Cactus");
	}

}