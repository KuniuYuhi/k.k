#include "stdafx.h"
#include "CharacterBase.h"

void CharacterBase::CreateCharacterController()
{
	m_charaCon = std::make_unique<CharacterController>();
}

void CharacterBase::SetRotateDirection(Vector3 direction)
{
	direction.Normalize();
	m_rotateDirection = direction;
}

