#include "stdafx.h"
#include "BossBase.h"

namespace {
	const int MAX_FLINCH_PROBABILITY = 10;

	const int FLINCH_PROBABILITY = 7;
}

BossBase::BossBase()
{
}

BossBase::~BossBase()
{
}

void BossBase::HitNormalAttack()
{
}

void BossBase::HitSkillAttack()
{
}

void BossBase::CalcSuperArmor(bool addOrSub, float value)
{
	//�X�[�p�[�A�[�}�[�̃|�C���g���v�Z
	if (m_status.CalcSuperArmorPoint(value, addOrSub) == true)
	{
		//�X�[�p�[�A�[�}�[�����ꂽ�A����Ă���
		SetBreakSuperArmorFlag(true);
	}
	else
	{
		//�X�[�p�[�A�[�}�[�͊���Ă��Ȃ�
		SetBreakSuperArmorFlag(false);
	}

}

bool BossBase::IsFlinch()
{
	//�X�[�p�[�A�[�}�[���u���C�N���Ă��Ȃ��Ȃ珈�����Ȃ�
	if (GetBreakSuperArmorFlag() != true)
	{
		return false;
	}

	int value = rand() % MAX_FLINCH_PROBABILITY;
	if (value > FLINCH_PROBABILITY)
	{
		//����
		return true;
	}
	//���܂Ȃ�
	return false;
	
}
