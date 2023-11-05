#include "stdafx.h"
#include "Bow.h"
#include "Brave.h"

namespace {
	//武器が収納状態の時の座標
	const Vector3 STOWEDS_POSITION = { 0.0f,-500.0f,0.0f };

	const Vector3 ARROW_COLLISION_SIZE = { 22.0f,40.0f,16.0f };

	const float SKILL_RADIUS = 60.0f;

	const float ADD_FORWARD = 30.0f;
}

Bow::Bow()
{
}

Bow::~Bow()
{
	DeleteGO(m_arrowCollision);
}

bool Bow::Start()
{
	//勇者のインスタンスを探す
	m_brave = FindGO<Brave>("brave");

	InitModel();
	InitCollision();

	//装備
	SetWeaponState(enWeaponState_Stowed);
	SetStowedFlag(true);
	//防御タイプの設定
	SetEnDefendTipe(enDefendTipe_avoid);

	return true;
}

void Bow::Update()
{
	//収納状態なら
	if (GetStowedFlag() == true)
	{
		return;
	}

	MoveWeapon();

	m_modelBow.Update();
	m_modelArrow.Update();
	m_arrowCollision->Update();

}

void Bow::Render(RenderContext& rc)
{
	if (GetWeaponState() == enWeaponState_Stowed)
	{
		return;
	}

	m_modelBow.Draw(rc);
	m_modelArrow.Draw(rc);
}

void Bow::MoveWeapon()
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

bool Bow::IsHitCollision()
{
	return false;
}

void Bow::ProcessSkillAttack()
{
}

void Bow::InitModel()
{
	//剣モデルの初期化
	m_modelBow.Init("Assets/modelData/character/Player/NewHero/Bow.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ
	);

	//盾モデルの初期化
	m_modelArrow.Init("Assets/modelData/character/Player/NewHero/Arrow.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ
	);
	//剣と盾の座標に対応するボーンIDを取得
	//装備状態の時のボーンID
	m_armedBowBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_l");
	m_armedArrowBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_r");

}

void Bow::InitCollision()
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

void Bow::MoveArmed()
{
	m_bowMatrix = 
		m_brave->GetModelRender().GetBone(m_armedBowBoonId)->GetWorldMatrix();
	m_modelBow.SetWorldMatrix(m_bowMatrix);
	m_arrowMatrix =
		m_brave->GetModelRender().GetBone(m_armedArrowBoonId)->GetWorldMatrix();
	m_modelArrow.SetWorldMatrix(m_arrowMatrix);
}

void Bow::MoveStowed()
{
	m_bowPos = STOWEDS_POSITION;
	m_arrowPos = STOWEDS_POSITION;

	m_modelBow.SetPosition(m_bowPos);
	m_modelArrow.SetPosition(m_arrowPos);
	//当たり判定の座標設定
	m_arrowCollision->SetPosition(m_arrowPos);
	//当たり判定の無効化
	m_arrowCollision->SetIsEnable(false);
	SetStowedFlag(true);
}
