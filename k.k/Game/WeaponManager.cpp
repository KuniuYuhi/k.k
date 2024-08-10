#include "stdafx.h"
#include "WeaponManager.h"

WeaponManager* WeaponManager::m_instance = nullptr;

WeaponManager::WeaponManager()
{
	if (m_instance != nullptr)
	{
		std::abort();
	}
	m_instance = this;
}

WeaponManager::~WeaponManager()
{
	m_instance = nullptr;
}

void WeaponManager::CreateInstance()
{
	m_instance = new WeaponManager();
}


