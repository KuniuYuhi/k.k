#include "stdafx.h"
#include "WeaponBase.h"

#include "MobEnemyBase.h"
#include "EnemyManager.h"


void WeaponBase::ChangeStowedState()
{
	SetCurrentWeaponState(enStowed);
}

void WeaponBase::ChangeArmedState()
{
	SetCurrentWeaponState(enArmed);
}

Vector3 WeaponBase::CalcAutoAimAtTarget(
	Vector3 origin, Vector3 nowDirection, float searchRadius, float maxDot)
{
	Vector3 newDirection = nowDirection;

	//���u�G�l�~�[���X�g���v���C���[�ɋ߂����ɂ��ꂩ����
	EnemyManager::GetInstance()->SortEnemiesByDistanceToTarget(origin);
	//�G�l�~�[���X�g���擾
	std::vector<MobEnemyBase*> tempList = EnemyManager::GetInstance()->GetMobEnemyList();

	//���X�g�̒��g���Ȃ��Ȃ珈�������Ȃ�
	if (tempList.size() == 0) return newDirection;

	for (auto enemy : tempList)
	{
		//�v���C���[����G�Ɍ������x�N�g�����v�Z
		Vector3 diff = enemy->GetPosition() - origin;
		//�x�N�g���̒��������ׂ�͈͊O�Ȃ�
		if (diff.Length() > searchRadius)
		{
			//�����߂��ɓG�����Ȃ��̂�for�����ʂ���
			break;
		}
		//�x�N�g���𐳋K��
		diff.Normalize();
		//���̕����ƌv�Z�����x�N�g���̓��ς𒲂ׁA�ǂ̂��炢�x�N�g�������Ă��邩���ׂ�
		float dot = Dot(nowDirection, diff);
		//�����̓��ς�莗�Ă�����V�������W�Ƃ���
		if (dot >= maxDot)
		{
			newDirection = diff;
		}
	}

	return newDirection;
}
