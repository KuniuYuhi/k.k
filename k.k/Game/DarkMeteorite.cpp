#include "stdafx.h"
#include "DarkMeteorite.h"
#include "Meteo.h"

DarkMeteorite::DarkMeteorite()
{
}

DarkMeteorite::~DarkMeteorite()
{
}

bool DarkMeteorite::Start()
{
	m_model.Init("Assets/modelData/character/Slime/slime.tkm");


	m_model.SetTransform(m_position, m_rotation, g_vec3One);
	m_model.Update();

	return true;
}

void DarkMeteorite::Update()
{
	//���̃T�C�Y�ɂȂ����珈�������Ȃ�
	SizeUp();
	
	Shot();


	m_model.Update();
}

void DarkMeteorite::SizeUp()
{
	//���̃T�C�Y�ɂȂ����珈�������Ȃ�
	if (m_sizeUpFlag != false)
	{
		return;
	}

	m_scale *= 1.0f + g_gameTime->GetFrameDeltaTime() * 1.5f;
	m_model.SetScale(m_scale);
	//���̃T�C�Y�ɂȂ����珈�������Ȃ�
	if (m_maxScale.x <= m_scale.x)
	{
		m_sizeUpFlag = true;
	}
}

void DarkMeteorite::Shot()
{
	//�V���b�g�t���O����������
	if (m_ShotStartFlag != true)
	{
		return;
	}

	//�^�C�}�[���������ԂɒB���Ă��Ȃ��Ȃ甲���o��
	if (CreateTimer() != true)
	{
		return;
	}
	//�������鐔
	if (m_meteoCounter < m_createMeteoCount)
	{
		//�^�[�Q�b�g�����߂ă��e�I�𐶐�
		Vector3 createPositon = m_targetPosition;

		float X = rand() % 1201 - 600;
		float Z = rand() % 1201 - 600;

		createPositon.x += X;
		createPositon.z += Z;

		CreateMeteo(createPositon);

		m_meteoCounter++;

		

	}
	//�S�Ẵ��e�I�𐶐�������
	else
	{
		m_shotEndFlag = true;
	}
	
}

void DarkMeteorite::CreateMeteo(Vector3 targetposition)
{
	Meteo* meteo = NewGO<Meteo>(0, "meteo");
	meteo->SetPosition(m_position);
	meteo->SetRotation(m_rotation);
	meteo->SetTargetPosition(targetposition);
}

bool DarkMeteorite::CreateTimer()
{
	if (m_createTime < m_createTimer)
	{
		m_createTimer = 0.0f;
		return true;
	}
	else
	{
		m_createTimer += g_gameTime->GetFrameDeltaTime();
		return false;
	}
}

void DarkMeteorite::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}
