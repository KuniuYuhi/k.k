#include "stdafx.h"
#include "Slime.h"

#include "MobEnemyMovement.h"

#include "Brave.h"

#include "EnemyObjectPool.h"

Slime::~Slime()
{
}

bool Slime::Start()
{
	//モデルを初期化していないなら
	if (!m_modelRender.IsInit())
	{
		//モデルを初期化
		m_modelRender.Init(
			"Assets/modelData/character/Slime/slime.tkm",
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
	m_status.InitCommonEnemyStatus("Slime");

	//コンポーネントを初期化していないなら
	if (!m_isSettingComponents)
	{
		InitComponents();
	}


	m_player = FindGO<Brave>("Brave");


	return true;
}

void Slime::ReleaseThis()
{
	//todo キャラコンの位置の変更、非アクティブ化時の処理


	m_charaCon.reset();
	
	EnemyObjectPool::GetInstance()->OnRelease("Slime", this);
}

void Slime::AddMoreComponent()
{

}

void Slime::CreateCollisionObject()
{

}

void Slime::Update()
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


void Slime::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

void Slime::InitComponents()
{
	//基本的なコンポーネントをセッティング
	SettingDefaultComponent();
	//このクラスで追加したいコンポーネントをセッティング
	AddMoreComponent();

	m_isSettingComponents = true;
}
