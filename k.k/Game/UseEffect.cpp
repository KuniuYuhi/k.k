#include "stdafx.h"
#include "UseEffect.h"

#include "CharacterBase.h"


UseEffect::UseEffect()
{
}

UseEffect::~UseEffect()
{
	m_effect->Stop();
	m_effect = nullptr;
	m_character = nullptr;

}

bool UseEffect::Start()
{


	return true;
}

void UseEffect::Update()
{
	if (m_effect->IsPlay() == false)
	{
		Delete();
		return;
	}


	if (m_isAutoMove)
	{
		ChaseMoveForSpecifiedPosition();
	}

}

void UseEffect::PlayEffect(
	EnEFK initEffectNumber, 
	Vector3 position, Vector3 SetScale, Quaternion rotation,
	bool isAutoMove
)
{
	m_effect = NewGO<EffectEmitter>(0);
	m_effect->Init(initEffectNumber);
	m_effect->Play();
	m_effect->SetPosition(position);
	m_effect->SetScale(SetScale);
	m_effect->SetRotation(rotation);
	m_effect->Update();

	m_isAutoMove = isAutoMove;
}

void UseEffect::PlayEffect(
	EnEFK initEffectNumber, 
	CharacterBase* characterBase, 
	Vector3 position, Vector3 SetScale, Quaternion rotation,
	bool isAutoMove
)
{
	m_character = characterBase;

	m_effect = NewGO<EffectEmitter>(0);
	m_effect->Init(initEffectNumber);
	m_effect->Play();
	m_effect->SetPosition(position);
	m_effect->SetScale(SetScale);
	m_effect->SetRotation(rotation);
	m_effect->Update();

	m_isAutoMove = isAutoMove;
}

void UseEffect::PlayEffect(
	EnEFK initEffectNumber, 
	CharacterBase* characterBase, int boonId, 
	Vector3 position, Vector3 SetScale, Quaternion rotation,
	bool isAutoMove
)
{
	m_character = characterBase;
	m_boonId = boonId;

	m_effect = NewGO<EffectEmitter>(0);
	m_effect->Init(initEffectNumber);
	m_effect->Play();
	m_effect->SetPosition(position);
	m_effect->SetScale(SetScale);
	m_effect->SetRotation(rotation);
	m_effect->Update();

	m_isAutoMove = isAutoMove;
}

void UseEffect::Delete()
{
	m_effect->Stop();
	DeleteGO(this);
}

void UseEffect::SetMovePosition(Vector3 position)
{
	m_effect->SetPosition(position);
	m_effect->Update();
}

void UseEffect::ChaseMoveForCenterPosition()
{
}

void UseEffect::ChaseMoveForSpecifiedPosition()
{
	Vector3 position = g_vec3Zero;
	Matrix m =
	m_character->GetModelRender().GetBone(m_boonId)->GetWorldMatrix();

	m.Apply(position);

	m_effect->SetPosition(position);
	m_effect->Update();

}
