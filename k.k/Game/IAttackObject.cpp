#include "stdafx.h"
#include "IAttackObject.h"

namespace {
	const int ADD_ATTACK_MAX = 5;
}

const int IAttackObject::GetAttack()
{
	//�U���͂�m_attack�`+5+m_attack�͈̔͂ɂ���

	int attack = m_attack;

	attack = rand() % ADD_ATTACK_MAX + m_attack;

	return attack;
}
