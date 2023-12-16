#include "stdafx.h"
#include "BossBase.h"

namespace {
	const int MAX_FLINCH_PROBABILITY = 10;

	const int FLINCH_PROBABILITY = 5;
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
	if (addOrSub == true)
	{
		AddSuperArmorPoint(value);
	}
	else
	{
		SubSuperArmorPoint(value);
	}
}

void BossBase::AddSuperArmorPoint(float addValue)
{
	//���Z
	m_superArmorPoint += addValue;
	if (m_superArmorPoint >= m_maxSuperArmorPoint)
	{
		//�X�[�p�[�A�[�}�[��
		SetBreakSuperArmorFlag(false);
		m_superArmorPoint = m_maxSuperArmorPoint;
	}
}

void BossBase::SubSuperArmorPoint(float subValue)
{
	//���Z
	m_superArmorPoint -= subValue;
	if (m_superArmorPoint <= 0)
	{
		//�X�[�p�[�A�[�}�[�����ꂽ
		SetBreakSuperArmorFlag(true);
		m_superArmorPoint = 0;
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
