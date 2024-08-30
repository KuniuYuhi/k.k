#include "stdafx.h"
#include "IMagicBall.h"
#include "DamageProvider.h"

void IMagicBall::DefaultSettingComponents()
{
	AddComponent<DamageProvider>();
	m_damageProvider = GetComponent<DamageProvider>();
	//自身のインスタンスを設定
	m_damageProvider->SetProviderMagicBallInstance(this);
}

void IMagicBall::SetShotMagicBallParameters(
	Vector3 startPosition, Vector3 forward, CharacterBase* chaseCharacter)
{
	m_startPosition = startPosition;
	m_position = m_startPosition;
	m_forward = forward;
	m_chaseCharacter = chaseCharacter;
}
