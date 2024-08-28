#include "stdafx.h"
#include "EnemyManager.h"
#include "MobEnemyBase.h"

EnemyManager* EnemyManager::m_instance = nullptr;

EnemyManager::EnemyManager()
{
	if (m_instance != nullptr) {
		//�C���X�^���X�����łɍ���Ă���B
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
	//���u�G�l�~�[���X�g�̒��g���Ȃ��Ȃ珈�����Ȃ�
	if (m_mobEnemyList.size() == 0) return;

	//�S�G�l�~�[�̑ҋ@�t���O�𗧂ĂĂ���
	for (auto enemy : m_mobEnemyList)
	{
		enemy->SetWaitingFlag(true);
	}
	
	//�\�[�g���ăv���C���[�ɑ΂��鋗�����߂����ɕ��ёւ���
	SortEnemiesByDistanceToTarget(targetPosition);
	
	
	for (int num = 0; num < m_approachMobEnemyAmount; num++)
	{
		//�ǂ�������G�l�~�[�̐������X�g��葽���Ȃ����甲���o��
		if (m_mobEnemyList.size() <= num) break;

		//
		if (!m_mobEnemyList[num]->IsActive()) continue;

		//�߂��܂Œǂ������ăG�l�~�[�̑ҋ@�t���O�������Ă���
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