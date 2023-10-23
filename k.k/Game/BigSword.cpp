#include "stdafx.h"
#include "BigSword.h"
#include "Brave.h"

namespace {
	//武器が収納状態の時の座標
	const Vector3 STOWEDS_POSITION = { 0.0f,-500.0f,0.0f };
}

BigSword::BigSword()
{
}

BigSword::~BigSword()
{
}

bool BigSword::Start()
{
	//勇者のインスタンスを探す
	m_brave = FindGO<Brave>("brave");

	InitModel();

	//装備
	SetWeaponState(enWeaponState_Stowed);
	SetStowedFlag(true);
	//防御タイプの設定
	SetEnDefendTipe(enDefendTipe_avoid);

	return true;
}

void BigSword::Update()
{
	//収納状態の時は処理しない
	if (GetStowedFlag() == true)
	{
		return;
	}

	MoveWeapon();

	m_modelBigSword.Update();
}

void BigSword::InitModel()
{
	//剣モデルの初期化
	m_modelBigSword.Init("Assets/modelData/character/Player/NewHero/BigSowrd.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ);

	//剣と盾の座標に対応するボーンIDを取得
	//装備状態の時のボーンID
	m_armedSwordBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_l");
}

void BigSword::MoveWeapon()
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

void BigSword::MoveArmed()
{
	//剣のワールド座標を設定
	m_swordMatrix =
		m_brave->GetModelRender().GetBone(m_armedSwordBoonId)->GetWorldMatrix();
	m_modelBigSword.SetWorldMatrix(m_swordMatrix);
}

void BigSword::MoveStowed()
{
	m_swordPos = STOWEDS_POSITION;
	m_modelBigSword.SetPosition(m_swordPos);
}

void BigSword::Render(RenderContext& rc)
{
	if (GetWeaponState() == enWeaponState_Stowed)
	{
		return;
	}
	m_modelBigSword.Draw(rc);
}


