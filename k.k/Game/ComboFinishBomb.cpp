#include "stdafx.h"
#include "ComboFinishBomb.h"
#include "InitEffect.h"

namespace {
	const int ATTACK = 30;
	const float COLLISION_RADIUS = 80.0f;
	const float EFFECT_SIZE = 10.0f;
}

ComboFinishBomb::ComboFinishBomb()
{
}

ComboFinishBomb::~ComboFinishBomb()
{

}

bool ComboFinishBomb::Start()
{
	//�U���͂̐ݒ�
	SetAttack(ATTACK);
	//�G�t�F�N�g�Đ�
	PlayBombEffect();
	//�����蔻��쐬
	CreatCollision();

	return true;
}

void ComboFinishBomb::Update()
{
	DeleteGO(this);
}

void ComboFinishBomb::PlayBombEffect()
{
	//�����G�t�F�N�g�Đ�
	EffectEmitter* explosionEffect = NewGO<EffectEmitter>(0);
	explosionEffect->Init(InitEffect::enEffect_DarkBall_Explosion);
	explosionEffect->Play();
	explosionEffect->SetScale(g_vec3One * EFFECT_SIZE);
	explosionEffect->SetPosition(m_position);
	explosionEffect->Update();

}

void ComboFinishBomb::CreatCollision()
{
	CollisionObject* collision = NewGO<CollisionObject>(0, GetCollisionName());
	collision->CreateSphere(
		m_position,g_quatIdentity, COLLISION_RADIUS
	);
	collision->SetCreatorName(GetName());
	//�����ō폜����悤�ɂ���
	collision->SetIsEnableAutoDelete(true);
	collision->Update();
}
