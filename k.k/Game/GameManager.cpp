#include "stdafx.h"
#include "GameManager.h"

GameManager* GameManager::m_instance = nullptr;

GameManager::GameManager()
{
	if (m_instance != nullptr) {
		//�C���X�^���X�����łɍ���Ă���B
		std::abort();
	}
	m_instance = this;
}

GameManager::~GameManager()
{

}
