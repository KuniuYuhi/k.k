#include "stdafx.h"
#include "EnemyObjectPool.h"

#include "EnemyBase.h"

#include "EnemyManager.h"

//////////////////////////////////////////////
//���u�G�l�~�[����
//////////////////////////////////////////////
#include "Slime.h"
#include "Cactus.h"
#include "BeholderEye.h"
#include "Mimic.h"
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

	//todo ���̃G�l�~�[������
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
	//���f��������ɓǂݍ���
	slime->InitModel();
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
	//���f��������ɓǂݍ���
	cactus->InitModel();
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

	//�ڋʂ̃L���[��}��
	std::queue<EnemyBase*> beholderEyeQueue;
	EnemyBase* beholderEye = NewGO<BeholderEye>(0, "BeholderEye");
	//���f��������ɓǂݍ���
	beholderEye->InitModel();
	//��A�N�e�B�u��
	beholderEye->Deactivate();
	beholderEyeQueue.push(beholderEye);
	//�C���T�[�g
	m_objectPoolQueue.insert(std::make_pair("BeholderEye", beholderEyeQueue));
	//�܌ڋʂ��L���[�ɒǉ�
	for (int i = 0; i < 10; i++)
	{
		OnCreateEnemy<BeholderEye>("BeholderEye");
	}


	//�~�~�b�N�̃L���[��}��
	std::queue<EnemyBase*> mimicQueue;
	EnemyBase* mimic = NewGO<Mimic>(0, "Mimic");
	//���f��������ɓǂݍ���
	slime->InitModel();
	//��A�N�e�B�u��
	mimic->Deactivate();
	mimicQueue.push(mimic);
	//�C���T�[�g
	m_objectPoolQueue.insert(std::make_pair("Mimic", mimicQueue));
	//�܌ڋʂ��L���[�ɒǉ�
	for (int i = 0; i < 10; i++)
	{
		OnCreateEnemy<Mimic>("Mimic");
	}


	//Start�֐��ŃG�l�~�[�}�l�[�W���[�̃��X�g�ɒǉ����Ă����̂�
	//���Z�b�g���Ă���
	EnemyManager::GetInstance()->CrearMobEnemyList();

}