#include "stdafx.h"
#include "Bow.h"
#include "Brave.h"
#include "Arrow.h"

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
	if (m_arrow != nullptr)
	{
		DeleteGO(m_arrow);
	}
}

bool Bow::Start()
{
	//勇者のインスタンスを探す
	m_brave = FindGO<Brave>("brave");

	//矢のオブジェクトを生成
	m_arrow = NewGO<Arrow>(0, "arrow");
	m_arrow->SetBow(this);
	//矢を持っているので、保持フラグをセット
	SetStockArrowFlag(true);

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

	if (GetStockArrowFlag() != true)
	{
		m_arrow = NewGO<Arrow>(0, "arrow");
		m_arrow->SetBow(this);
		//矢を持っているので、保持フラグをセット
		SetStockArrowFlag(true);
	}

	MoveWeapon();

	m_modelBow.Update();
}

void Bow::Render(RenderContext& rc)
{
	if (GetWeaponState() == enWeaponState_Stowed)
	{
		return;
	}

	m_modelBow.Draw(rc);
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
	//弓モデルの初期化
	m_modelBow.Init("Assets/modelData/character/Player/NewHero/Bow.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ
	);

	//弓と矢の座標に対応するボーンIDを取得
	//装備状態の時のボーンID
	m_armedBowBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_l");
	m_armedArrowBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_r");

}

void Bow::MoveArmed()
{
	//弓のワールド座標を設定
	m_bowMatrix = 
		m_brave->GetModelRender().GetBone(m_armedBowBoonId)->GetWorldMatrix();
	m_modelBow.SetWorldMatrix(m_bowMatrix);
	//矢のワールド座標を設定
	m_arrowMatrix = 
		m_brave->GetModelRender().GetBone(m_armedArrowBoonId)->GetWorldMatrix();
	m_modelBow.SetWorldMatrix(m_bowMatrix);

	if (m_arrow != nullptr)
	{
		//矢のステートを設定
		m_arrow->SetWeaponState(enWeaponState_Armed);
	}
	
}

void Bow::MoveStowed()
{
	//弓の座標を設定
	m_bowPos = STOWEDS_POSITION;
	m_modelBow.SetPosition(m_bowPos);

	if (m_arrow != nullptr)
	{
		//矢のステートを設定
		m_arrow->SetWeaponState(enWeaponState_Stowed);
	}
	

	SetStowedFlag(true);
}

void Bow::ProcessLongRangeAttack()
{
	if (m_arrow != nullptr)
	{
		//矢を発射
		m_arrow->SetShotFlag(true);
		//勇者の前方向を取得
		m_arrow->SetForward(m_brave->GetForward());
		//
		m_arrow->ApplyMatrixToLocalPosition();
		//射撃開始座標を設定
		m_arrow->SetShotStartPosition(m_arrow->GetPosition());
		//矢を放ったので、今の矢を
		SetStockArrowFlag(false);
		m_arrow = nullptr;
	}
}
