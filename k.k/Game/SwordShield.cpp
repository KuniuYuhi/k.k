#include "stdafx.h"
#include "SwordShield.h"
#include "Brave.h"

namespace {
	//武器が収納状態の時の座標
	const Vector3 STOWEDS_POSITION = { 0.0f,-500.0f,0.0f };
}

SwordShield::SwordShield()
{
}

SwordShield::~SwordShield()
{
}

bool SwordShield::Start()
{
	//勇者のインスタンスを探す
	m_brave = FindGO<Brave>("brave");

	InitModel();

	//装備
	SetWeaponState(enWeaponState_Armed);
	SetStowedFlag(false);
	//防御タイプの設定
	SetEnDefendTipe(enDefendTipe_Defence);

	return true;
}

void SwordShield::Update()
{

	if (GetStowedFlag() == true)
	{
		return;
	}

	MoveWeapon();

	m_modelSword.Update();
	m_modelShield.Update();
}

void SwordShield::InitModel()
{
	//剣モデルの初期化
	m_modelSword.Init("Assets/modelData/character/Player/NewHero/OneHandSword.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ);
	
	//盾モデルの初期化
	m_modelShield.Init("Assets/modelData/character/Player/NewHero/Shield.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ);
	//剣と盾の座標に対応するボーンIDを取得
	//装備状態の時のボーンID
	m_armedSwordBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_r");
	m_armedShieldBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_l");
	//収納状態の時のボーンID
	m_stowedSwordBoonId = m_brave->GetModelRender().FindBoneID(L"weaponPosition_r");
	m_stowedShieldBoonId= m_brave->GetModelRender().FindBoneID(L"weaponPosition_l");
}

void SwordShield::MoveWeapon()
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

void SwordShield::MoveArmed()
{
	//剣と盾のワールド座標を設定
	m_swordMatrix =
		m_brave->GetModelRender().GetBone(m_armedSwordBoonId)->GetWorldMatrix();
	m_modelSword.SetWorldMatrix(m_swordMatrix);
	m_shieldMatrix =
		m_brave->GetModelRender().GetBone(m_armedShieldBoonId)->GetWorldMatrix();
	m_modelShield.SetWorldMatrix(m_shieldMatrix);
}

void SwordShield::MoveStowed()
{
	m_swordPos = STOWEDS_POSITION;
	m_shieldPos = STOWEDS_POSITION;

	m_modelSword.SetPosition(m_swordPos);
	m_modelShield.SetPosition(m_shieldPos);

	SetStowedFlag(true);
	//剣と盾のワールド座標を設定
	/*m_swordMatrix =
		m_brave->GetModelRender().GetBone(m_stowedSwordBoonId)->GetWorldMatrix();
	m_modelSword.SetWorldMatrix(m_swordMatrix);
	m_shieldMatrix =
		m_brave->GetModelRender().GetBone(m_stowedShieldBoonId)->GetWorldMatrix();
	m_modelShield.SetWorldMatrix(m_shieldMatrix);*/
}

void SwordShield::Render(RenderContext& rc)
{
	if (GetWeaponState() == enWeaponState_Stowed)
	{
		return;
	}

	m_modelSword.Draw(rc);
	m_modelShield.Draw(rc);
}
