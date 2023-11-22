#include "stdafx.h"
#include "CharactersInfoManager.h"


CharactersInfoManager* CharactersInfoManager::m_charactersInfoInstance = nullptr;

CharactersInfoManager::CharactersInfoManager()
{
	if (m_charactersInfoInstance != nullptr) {
		//インスタンスがすでに作られている。
		std::abort();
	}
	m_charactersInfoInstance = this;
}

CharactersInfoManager::~CharactersInfoManager()
{
	m_charactersInfoInstance = nullptr;
}
