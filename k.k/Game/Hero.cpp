#include "stdafx.h"
#include "Hero.h"

namespace {
	int MAXHP = 200;
	int ATK = 50;
	float SPEED = 100.0f;
	const char* NAME = "Hero";
}

Hero::Hero()
{
}

Hero::~Hero()
{
}

bool Hero::Start()
{
	//ステータスの初期化
	m_status.InitStatus(
	MAXHP,
	ATK,
	SPEED,
	NAME
	);

	InitModel();


	return true;
}

void Hero::InitModel()
{
	//アニメーションクリップをロードする。
	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/Player/Idle_Normal.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Run].Load("Assets/animData/character/Player/SprintFWD_Battle.tka");
	m_animationClip[enAnimClip_Run].SetLoopFlag(true);

	m_modelRender.Init(
		"Assets/modelData/character/Player/Hero.tkm",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);

	//m_modelRender.SetPosition(50.0f, 0.0f, 50.0f);

	m_modelRender.Update();


	m_charaCon.Init(12.0f, 33.0f, g_vec3Zero);
}

void Hero::Update()
{
	// アニメーションの切り替え。
	if (g_pad[0]->IsPress(enButtonA)) {
		m_modelRender.PlayAnimation(enAnimClip_Idle, 0.6f);
		m_dashFlag = false;
	}
	if (g_pad[0]->IsPress(enButtonB)) {
		m_modelRender.PlayAnimation(enAnimClip_Run, 0.2f);
		m_dashFlag = true;
	}

	m_position = m_charaCon.Execute(m_moveSpeed = Move(m_status), 1.0f / 60.0f);
	Rotation();

	SetTransFormModel(m_modelRender);

	//m_modelRender.SetPosition(m_position);
	//m_modelRender.SetRotation(Rotation());
	m_modelRender.Update();
}

void Hero::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
