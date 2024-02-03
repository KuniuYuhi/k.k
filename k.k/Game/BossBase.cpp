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
	//スーパーアーマーのポイントを計算
	if (m_status.CalcSuperArmorPoint(value, addOrSub) == true)
	{
		//スーパーアーマーが割れた、割れている
		SetBreakSuperArmorFlag(true);
	}
	else
	{
		//スーパーアーマーは割れていない
		SetBreakSuperArmorFlag(false);
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
