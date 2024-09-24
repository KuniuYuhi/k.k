#include "stdafx.h"
#include "DamageProvider.h"
#include "CharacterBase.h"
#include "IMagicBall.h"

DamageProvider::DamageProvider()
{
}

DamageProvider::~DamageProvider()
{
}

void DamageProvider::Start()
{
}

void DamageProvider::UpdateComponent()
{
	m_isHit = false;
}

Vector3 DamageProvider::GetProviderPostion()
{
	if (m_characterBase != nullptr)
	{
		return m_characterBase->GetPosition();
	}
	else return m_magicBall->GetPosition();
	
	
}

void DamageProvider::AdjustPowerRandomly()
{
	int randomAdjustment = rand() % 21 - 10;

	m_damageInfo.attackPower += randomAdjustment;

	if (m_damageInfo.attackPower <= 0)
	{
		m_damageInfo.attackPower = 1;
	}
}

void DamageProvider::CalcFinalKnockBackPattern(int knockBackToleranceLevel)
{
	//�m�b�N�o�b�N�̃��x�����v�Z
	int finalRevel = 
		static_cast<int>(m_damageInfo.knockBackPattern) - knockBackToleranceLevel;

	

	if (finalRevel > enKBPattern_VerticalLaunch)
	{
		m_damageInfo.knockBackPattern = enKBPattern_VerticalLaunch;
		return;
	}
	else if (finalRevel < enKBPattern_ShortGroundedRetreat)
	{
		m_damageInfo.knockBackPattern = enKBPattern_ShortGroundedRetreat;
		return;
	}

	//�^��ϊ�
	EnKnockBackPattern finalKBPattern = static_cast<EnKnockBackPattern>(finalRevel);

	//�v�Z�����m�b�N�o�b�N�̃p�^�[����ݒ�
	m_damageInfo.knockBackPattern = finalKBPattern;

}
