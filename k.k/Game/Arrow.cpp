#include "stdafx.h"
#include "Arrow.h"
#include "Bow.h"

namespace {
	//武器が収納状態の時の座標
	const Vector3 STOWEDS_POSITION = { 0.0f,-500.0f,0.0f };

	const Vector3 ARROW_COLLISION_SIZE = { 22.0f,40.0f,16.0f };

	const float SKILL_RADIUS = 60.0f;

	const float ADD_FORWARD = 30.0f;
}

Arrow::Arrow()
{
}

Arrow::~Arrow()
{
}

bool Arrow::Start()
{
	InitModel();
	//InitCollision();

	//装備
	SetWeaponState(m_bow->GetBowEnWeaponState());

	return true;
}

void Arrow::Update()
{
	if (m_shotFlag != true)
	{
		MoveWeapon();
	}
	else
	{
		//射撃時の移動処理
		ProcessLongRangeAttack();
	}

	

	ArrowUpdate();
}

void Arrow::MoveWeapon()
{
	switch (m_enWeaponState)
	{
	case IWeapon::enWeaponState_Stowed://収納状態
		MoveStowed();
		break;
	case IWeapon::enWeaponState_Armed://装備状態
		MoveArmed();
		break;
	case IWeapon::enWeaponState_None://なし
		break;
	default:
		break;
	}
}

void Arrow::MoveArmed()
{
	//矢の座標を設定
	m_arrowMatrix = m_bow->GetArrowMatrix();
	m_modelArrow.SetWorldMatrix(m_arrowMatrix);
}

void Arrow::MoveStowed()
{
	//矢の座標を設定
	m_arrowPos = STOWEDS_POSITION;
	m_modelArrow.SetPosition(m_arrowPos);
}

void Arrow::ProcessLongRangeAttack()
{
	//射撃開始座標から現在の座標に向かうベクトルを計算
	Vector3 diff = m_arrowPos - m_shotStartPosition;

	if (diff.Length() > 200.0f)
	{
		DeleteGO(this);
	}
	//矢の座標を設定
	m_arrowPos += (m_forward * 300.0f) * g_gameTime->GetFrameDeltaTime();
	m_modelArrow.SetPosition(m_arrowPos);
}

void Arrow::InitModel()
{
	//矢モデルの初期化
	m_modelArrow.Init("Assets/modelData/character/Player/NewHero/Arrow.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ
	);
}

void Arrow::InitCollision()
{
	//矢の当たり判定の生成
	m_arrowCollision = NewGO<CollisionObject>(0, "Attack");
	m_arrowCollision->CreateBox(
		STOWEDS_POSITION,
		g_quatIdentity,
		ARROW_COLLISION_SIZE
	);
	m_arrowCollision->SetIsEnableAutoDelete(false);
	m_arrowCollision->SetIsEnable(false);
}

void Arrow::Render(RenderContext& rc)
{
	if (GetWeaponState() == enWeaponState_Stowed)
	{
		return;
	}

	m_modelArrow.Draw(rc);
}
