#include "stdafx.h"
#include "Mimic.h"

#include "MobEnemyMovement.h"

#include "Brave.h"

#include "EnemyObjectPool.h"

#include "EnemyManager.h"



Mimic::~Mimic()
{
}

bool Mimic::Start()
{
	//モデルを初期化していないなら
	if (!m_modelRender.IsInit())
	{
		//モデルを初期化
		m_modelRender.Init(
			"Assets/modelData/character/Mimic/Mimic.tkm",
			L"Assets/shader/ToonTextrue/lamp_Slime.DDS"
		);
	}

	//トランスフォームの設定
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);



	if (m_charaCon == nullptr)
	{
		CreateCharacterController();
		m_charaCon.get()->Init(20.0f, 40.0f, m_position);
	}
	else
	{
		m_charaCon.get()->SetPosition(m_position);
	}

	//ステータスを初期化
	m_status.InitCommonEnemyStatus("Mimic");

	//コンポーネントを初期化していないなら
	if (!m_isSettingComponents)
	{
		InitComponents();
	}


	EnemyManager::GetInstance()->AddMobEnemyToList(this);

	m_player = FindGO<Brave>("Brave");


	return true;
}

void Mimic::Update()
{
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		ReleaseThis();
		return;
	}


	ChaseMovement(m_player->GetPosition());

	Rotation();

	m_modelRender.SetTransform(m_position, m_rotation, m_scale);

	m_modelRender.Update();
}

void Mimic::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

void Mimic::InitComponents()
{
	//基本的なコンポーネントをセッティング
	SettingDefaultComponent();
	//このクラスで追加したいコンポーネントをセッティング
	AddMoreComponent();

	m_isSettingComponents = true;
}

void Mimic::ReleaseThis()
{
	//todo キャラコンの位置の変更、非アクティブ化時の処理

	//
	EnemyManager::GetInstance()->RemoveMobEnemyToList(this);

	m_charaCon.reset();

	EnemyObjectPool::GetInstance()->OnRelease("Mimic", this);
}

void Mimic::AddMoreComponent()
{
}

void Mimic::CreateCollisionObject()
{
}
