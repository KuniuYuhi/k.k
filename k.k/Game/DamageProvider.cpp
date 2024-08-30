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
