#include "stdafx.h"
#include "DamageProvider.h"
#include "CharacterBase.h"

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

Vector3 DamageProvider::GetProviderCharacterPostion()
{
	return m_characterBase->GetPosition();
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
