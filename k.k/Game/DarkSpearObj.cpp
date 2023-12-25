#include "stdafx.h"
#include "DarkSpearObj.h"
#include "InitEffect.h"
#include "GameManager.h"

namespace {
	const float EFFECT_SIZE = 10.0f;
}

DarkSpearObj::DarkSpearObj()
{
}

DarkSpearObj::~DarkSpearObj()
{
	if (m_darkSpearEffect != nullptr)
	{
		m_darkSpearEffect->Stop();
	}
}

bool DarkSpearObj::Start()
{
	PlayEffect();

    return true;
}

void DarkSpearObj::Update()
{
	if (GameManager::GetInstance()->GetOutComeState() !=
		GameManager::enOutComeState_None)
	{
		DeleteGO(this);
		return;
	}

	if (m_darkSpearEffect->IsPlay() != true)
	{
		DeleteGO(this);
	}

}

void DarkSpearObj::PlayEffect()
{
	m_darkSpearEffect = NewGO<EffectEmitter>(0);
	m_darkSpearEffect->Init(InitEffect::enEffect_DarkSpear);
	m_darkSpearEffect->Play();
	m_darkSpearEffect->SetScale(g_vec3One * EFFECT_SIZE);
	m_darkSpearEffect->SetPosition(m_position);
	m_darkSpearEffect->Update();
}
