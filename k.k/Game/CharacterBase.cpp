#include "stdafx.h"
#include "CharacterBase.h"

void CharacterBase::CreateCharacterController()
{
	m_charaCon = std::make_unique<CharacterController>();
}
