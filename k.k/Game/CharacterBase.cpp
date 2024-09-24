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

void CharacterBase::CreateDamageFont(
	int hitDamage, DamageFont::EnDamageActor hitActor
)
{
	DamageFont* damagefont = NewGO<DamageFont>(0, "damagefont");
	damagefont->Setting(
		hitActor,
		hitDamage,
		m_position
	);
}

