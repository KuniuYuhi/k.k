#include "stdafx.h"
#include "Cactus.h"

#include "EnemyObjectPool.h"

#include "Brave.h"


Cactus::~Cactus()
{
}

bool Cactus::Start()
{
	//���f�������������Ă��Ȃ��Ȃ�
	if (!m_modelRender.IsInit())
	{
		m_modelRender.Init(
			"Assets/modelData/character/Cactus/Cactus.tkm",
			L"Assets/shader/ToonTextrue/lamp_Cactus.DDS");
	}

	m_modelRender.SetTransform(m_position, m_rotation, m_scale);

	//�X�e�[�^�X��������
	m_status.InitCommonEnemyStatus("Cactus");


	//�L�����R�������������Ă��Ȃ��Ȃ�
	if (!m_charaCon.IsInited())
	{
		m_charaCon.Init(20.0f, 40.0f, m_position);
	}
	else
	{
		m_charaCon.SetPosition(m_position);
	}

	//�R���|�[�l���g�����������Ă��Ȃ��Ȃ�
	if (!m_isSettingComponents)
	{
		InitComponents();
	}

	m_player = FindGO<Brave>("Brave");

	return true;
}

void Cactus::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		ReleaseThis();
		return;
	}

	ChaseMovement(m_player->GetPosition());

	Rotation();

	m_modelRender.SetTransform(m_position, m_rotation, m_scale);

	m_modelRender.Update();
}

void Cactus::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

void Cactus::InitComponents()
{
	//��{�I�ȃR���|�[�l���g���Z�b�e�B���O
	SettingDefaultComponent();
	//���̃N���X�Œǉ��������R���|�[�l���g���Z�b�e�B���O
	AddMoreComponent();

	m_isSettingComponents = true;
}

void Cactus::ReleaseThis()
{
	m_charaCon.SetPosition(m_deactivatePosition);
	m_charaCon.Execute(m_deactivatePosition, g_gameTime->GetFrameDeltaTime());
	EnemyObjectPool::GetInstance()->OnRelease("Cactus", this);
}

void Cactus::AddMoreComponent()
{
}
