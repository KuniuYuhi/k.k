#include "stdafx.h"
#include "EnemyObjectPool.h"

#include "EnemyBase.h"

#include "EnemyManager.h"

//////////////////////////////////////////////
//モブエネミーたち
//////////////////////////////////////////////
#include "MobEnemyheaderFiles.h"
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
	//中のエネミーを消す
	//DeleteQueueObject();


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
	//モデルだけ先に読み込む
	slime->InitModel();
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

	//キノコのキューを挿入
	std::queue<EnemyBase*> mushroomQueue;
	EnemyBase* mushroom = NewGO<Mushroom>(0, "Mushroom");
	//モデルだけ先に読み込む
	mushroom->InitModel();
	//非アクティブ化
	mushroom->Deactivate();
	mushroomQueue.push(mushroom);
	//インサート
	m_objectPoolQueue.insert(std::make_pair("Mushroom", mushroomQueue));
	//五個キノコをキューに追加
	for (int i = 0; i < 10; i++)
	{
		OnCreateEnemy<Mushroom>("Mushroom");
	}

	//カクタスのキューを挿入
	std::queue<EnemyBase*> cactusQueue;
	EnemyBase* cactus = NewGO<Cactus>(0, "Cactus");
	//モデルだけ先に読み込む
	cactus->InitModel();
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

	//目玉のキューを挿入
	std::queue<EnemyBase*> beholderEyeQueue;
	EnemyBase* beholderEye = NewGO<BeholderEye>(0, "BeholderEye");
	//モデルだけ先に読み込む
	beholderEye->InitModel();
	//非アクティブ化
	beholderEye->Deactivate();
	beholderEyeQueue.push(beholderEye);
	//インサート
	m_objectPoolQueue.insert(std::make_pair("BeholderEye", beholderEyeQueue));
	//五個目玉をキューに追加
	for (int i = 0; i < 10; i++)
	{
		OnCreateEnemy<BeholderEye>("BeholderEye");
	}


	//ミミックのキューを挿入
	std::queue<EnemyBase*> mimicQueue;
	EnemyBase* mimic = NewGO<Mimic>(0, "Mimic");
	//モデルだけ先に読み込む
	slime->InitModel();
	//非アクティブ化
	mimic->Deactivate();
	mimicQueue.push(mimic);
	//インサート
	m_objectPoolQueue.insert(std::make_pair("Mimic", mimicQueue));
	//五個目玉をキューに追加
	for (int i = 0; i < 10; i++)
	{
		OnCreateEnemy<Mimic>("Mimic");
	}


	//Start関数でエネミーマネージャーのリストに追加していたので
	//リセットしておく
	EnemyManager::GetInstance()->CrearMobEnemyList();

}

void EnemyObjectPool::DeleteQueueObject()
{
	for (auto& pair : m_objectPoolQueue) {
		//キューを持ってくる
		std::queue<EnemyBase*>& enemyQueue = pair.second;

		//キューの中身がなくなるまで繰り返す
		while (!enemyQueue.empty()) {
			EnemyBase* enemy = enemyQueue.front();
			//キューから出す
			enemyQueue.pop();

			//エネミーを削除
			DeleteGO(enemy);
		}
	}
}
