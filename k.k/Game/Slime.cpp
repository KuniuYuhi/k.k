#include "stdafx.h"
#include "Slime.h"

#include "MobEnemyMovement.h"

#include "Brave.h"

#include "EnemyObjectPool.h"

#include "EnemyManager.h"

//#include "SlimeStateContext.h"


Slime::~Slime()
{
}

bool Slime::Start()
{
	
	LoadAnimationClip();

	Init();
	
	//キャラコン初期化
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

	//エネミーマネージャーのリストに自身を入れる
	EnemyManager::GetInstance()->AddMobEnemyToList(this);


	//ステートコンテキストの初期化
	//最初のステートは出現ステート
	m_slimeContext = std::make_unique<SlimeStateContext>();

	m_slimeContext.get()->Init(this, enSlimeState_Appear);

	m_player = FindGO<Brave>("Brave");

	return true;
}

void Slime::Init()
{
	//モデルを初期化していないなら
	if (!m_modelRender.IsInit())
	{
		//モデルを初期化
		m_modelRender.Init(
			"Assets/modelData/character/Slime/slime.tkm",
			L"Assets/shader/ToonTextrue/lamp_Slime.DDS",
			m_animationClip,
			enSlimeAnimClip_num,
			enModelUpAxisZ
		);
	}

	//トランスフォームの設定
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
}

void Slime::LoadAnimationClip()
{
	m_animationClip[enSlimeAnimClip_Idle].Load("Assets/animData/character/Slime/Idle_Normal.tka");
	m_animationClip[enSlimeAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enSlimeAnimClip_Run].Load("Assets/animData/character/Slime/Run.tka");
	m_animationClip[enSlimeAnimClip_Run].SetLoopFlag(true);
	m_animationClip[enSlimeAnimClip_Attack].Load("Assets/animData/character/Slime/Attack1.tka");
	m_animationClip[enSlimeAnimClip_Attack].SetLoopFlag(false);
	m_animationClip[enSlimeAnimClip_Hit].Load("Assets/animData/character/Slime/Damege.tka");
	m_animationClip[enSlimeAnimClip_Hit].SetLoopFlag(false);
	m_animationClip[enSlimeAnimClip_Die].Load("Assets/animData/character/Slime/Die.tka");
	m_animationClip[enSlimeAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enSlimeAnimClip_Victory].Load("Assets/animData/character/Slime/Victory.tka");
	m_animationClip[enSlimeAnimClip_Victory].SetLoopFlag(true);
	m_animationClip[enSlimeAnimClip_Appear].Load("Assets/animData/character/Slime/Appear.tka");
	m_animationClip[enSlimeAnimClip_Appear].SetLoopFlag(false);
}

void Slime::InitComponents()
{
	//基本的なコンポーネントをセッティング
	SettingDefaultComponent();
	//このクラスで追加したいコンポーネントをセッティング
	AddMoreComponent();

	m_isSettingComponents = true;
}

void Slime::ReleaseThis()
{
	//todo キャラコンの位置の変更、非アクティブ化時の処理

	//
	EnemyManager::GetInstance()->RemoveMobEnemyToList(this);

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
		//ReleaseThis();
		//return;
	}


	ChaseMovement(m_player->GetPosition());
	
	Rotation();

	//コンテキストの処理
	m_slimeContext.get()->UpdateCurrentState();
	m_slimeContext.get()->PlayAnimationCurrentState();

	m_modelRender.SetTransform(m_position, m_rotation, m_scale);

	m_modelRender.Update();
}




void Slime::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}