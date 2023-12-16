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
	//加算
	m_superArmorPoint += addValue;
	if (m_superArmorPoint >= m_maxSuperArmorPoint)
	{
		//スーパーアーマー回復
		SetBreakSuperArmorFlag(false);
		m_superArmorPoint = m_maxSuperArmorPoint;
	}
}

void BossBase::SubSuperArmorPoint(float subValue)
{
	//減算
	m_superArmorPoint -= subValue;
	if (m_superArmorPoint <= 0)
	{
		//スーパーアーマーが割れた
		SetBreakSuperArmorFlag(true);
		m_superArmorPoint = 0;
	}
}

bool BossBase::IsFlinch()
{
	//スーパーアーマーがブレイクしていないなら処理しない
	if (GetBreakSuperArmorFlag() != true)
	{
		return false;
	}

	int value = rand() % MAX_FLINCH_PROBABILITY;
	if (value > FLINCH_PROBABILITY)
	{
		//怯む
		return true;
	}
	//怯まない
	return false;
	
}
