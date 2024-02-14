#include "stdafx.h"
#include "DarkSpearObj.h"
#include "InitEffect.h"
#include "GameManager.h"

namespace {
	const float EFFECT_SIZE = 7.0f;

	const float DELETE_TIMETR_LIMMIT = 2.0f;

	const int ATTACK = 25;

	const float SCALE_Small = 1.5f;
	const float SCALE_Medium = 2.0f;
	const float SCALE_Large = 3.0f;

	const float DEFAULT_MUL_SIZE = 0.5f;
	const float MUL_CURRENT_SIZE = 3.0f;
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

	if (m_rangeEffect != nullptr)
	{
		m_rangeEffect->Stop();
	}
}

bool DarkSpearObj::Start()
{
	//�U���͂̐ݒ�
	SetAttack(ATTACK);

	PlayEffect();

	m_darkSpearModel.Init("Assets/modelData/character/Lich/DarkSpear.tkm",
		L"Assets/shader/ToonTextrue/lamp_Slime.DDS");
	m_darkSpearModel.SetScale(m_maxScale);
	m_darkSpearModel.SetPosition(m_position);
	m_darkSpearModel.Update();

    return true;
}

void DarkSpearObj::Update()
{
	//�������鎞�ԂɂȂ��������
	if (m_deleteTimer > DELETE_TIMETR_LIMMIT)
	{
		DeleteGO(this);
	}
	else
	{
		m_deleteTimer += g_gameTime->GetFrameDeltaTime();
	}
	//�T�C�Y��ύX����
	ChangeSizeDarkSpearObj();

}

void DarkSpearObj::SetMaxScale(EnMaxScaleSize maxScaleSize)
{
	switch (maxScaleSize)
	{
	case DarkSpearObj::enMaxScaleSize_Small:
		m_maxScale *= SCALE_Small;
		break;
	case DarkSpearObj::enMaxScaleSize_Medium:
		m_maxScale *= SCALE_Medium;
		break;
	case DarkSpearObj::enMaxScaleSize_Large:
		m_maxScale *= SCALE_Large;
		break;
	default:std::abort();
		break;
	}
}

void DarkSpearObj::SetMaxScale(int sizeNumer)
{
	switch (sizeNumer)
	{
	case DarkSpearObj::enMaxScaleSize_Small:
		m_maxScale *= SCALE_Small;
		break;
	case DarkSpearObj::enMaxScaleSize_Medium:
		m_maxScale *= SCALE_Medium;
		break;
	case DarkSpearObj::enMaxScaleSize_Large:
		m_maxScale *= SCALE_Large;
		break;
	default:std::abort();
		break;
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

void DarkSpearObj::CreateCollision()
{
	m_collision = NewGO<CollisionObject>(0, GetCollisionName());
	m_collision->CreateMesh(
		g_vec3Zero, 
		g_quatIdentity,
		m_darkSpearModel.GetModel(), 
		m_darkSpearModel.GetModel().GetWorldMatrix()
	);
	//�����蔻��𐶐������쐬�҂̐ݒ�
	m_collision->SetCreatorName(GetName());
	//�����ō폜����悤�ɂ���
	m_collision->SetIsEnableAutoDelete(true);
	m_collision->Update();

}

void DarkSpearObj::ChangeSizeDarkSpearObj()
{
	//���݂̃T�C�Y�����߂�
	float mulSize = DEFAULT_MUL_SIZE * sin(m_deleteTimer * MUL_CURRENT_SIZE);

	float currentSize = m_maxScale * mulSize * MUL_CURRENT_SIZE;

	m_scale = g_vec3One * currentSize;

	//�T�C�Y��0�ȏ�Ȃ�g�嗦�̍X�V�Ɠ����蔻��𐶐�
	if (currentSize > 0.0f)
	{
		//�����蔻�萶��
		CreateCollision();
		//�g�嗦�̍X�V
		m_darkSpearModel.SetScale(m_scale);
		m_darkSpearModel.Update();
	}
}

void DarkSpearObj::Render(RenderContext& rc)
{
	//������x�̑傫���Ȃ烂�f����`��
	if (m_scale.y > 0.0f)
	{
		m_darkSpearModel.Draw(rc);
	}
}
