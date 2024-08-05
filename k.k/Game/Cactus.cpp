#include "stdafx.h"
#include "Cactus.h"

#include "EnemyObjectPool.h"

#include "Brave.h"


Cactus::~Cactus()
{
}

bool Cactus::Start()
{
	//モデルを初期化していないなら
	if (!m_modelRender.IsInit())
	{
		m_modelRender.Init(
			"Assets/modelData/character/Cactus/Cactus.tkm",
			L"Assets/shader/ToonTextrue/lamp_Cactus.DDS");
	}

	m_modelRender.SetTransform(m_position, m_rotation, m_scale);

	//ステータスを初期化
	m_status.InitCommonEnemyStatus("Cactus");


	//キャラコンを初期化していないなら
	if (!m_charaCon.IsInited())
	{
		m_charaCon.Init(20.0f, 40.0f, m_position);
	}
	else
	{
		m_charaCon.SetPosition(m_position);
	}

	//コンポーネントを初期化していないなら
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
	//基本的なコンポーネントをセッティング
	SettingDefaultComponent();
	//このクラスで追加したいコンポーネントをセッティング
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
