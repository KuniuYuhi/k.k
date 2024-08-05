#include "stdafx.h"
#include "EnemyObjectPool.h"

#include "EnemyBase.h"

//////////////////////////////////////////////
//���u�G�l�~�[����
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
	//��A�N�e�B�u��
	slime->Deactivate();
	slimeQueue.push(slime);
	//�C���T�[�g
	m_objectPoolQueue.insert(std::make_pair("Slime", slimeQueue));
	//�܌X���C�����L���[�ɒǉ�
	for (int i = 0; i < 10; i++)
	{
		OnCreateEnemy<Slime>("Slime");
	}



	//�J�N�^�X�̃L���[��}��
	std::queue<EnemyBase*> cactusQueue;
	EnemyBase* cactus = NewGO<Cactus>(0, "Cactus");
	//��A�N�e�B�u��
	cactus->Deactivate();
	cactusQueue.push(cactus);
	//�C���T�[�g
	m_objectPoolQueue.insert(std::make_pair("Cactus", cactusQueue));
	//�܌J�N�^�X���L���[�ɒǉ�
	for (int i = 0; i < 10; i++)
	{
		OnCreateEnemy<Cactus>("Cactus");
	}

}