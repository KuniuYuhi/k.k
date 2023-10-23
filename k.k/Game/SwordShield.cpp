#include "stdafx.h"
#include "SwordShield.h"
#include "Brave.h"

namespace {
	//���킪���[��Ԃ̎��̍��W
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
	//�E�҂̃C���X�^���X��T��
	m_brave = FindGO<Brave>("brave");

	InitModel();

	//����
	SetWeaponState(enWeaponState_Armed);
	SetStowedFlag(false);
	//�h��^�C�v�̐ݒ�
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
	//�����f���̏�����
	m_modelSword.Init("Assets/modelData/character/Player/NewHero/OneHandSword.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ);
	
	//�����f���̏�����
	m_modelShield.Init("Assets/modelData/character/Player/NewHero/Shield.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ);
	//���Ə��̍��W�ɑΉ�����{�[��ID���擾
	//������Ԃ̎��̃{�[��ID
	m_armedSwordBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_r");
	m_armedShieldBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_l");
	//���[��Ԃ̎��̃{�[��ID
	m_stowedSwordBoonId = m_brave->GetModelRender().FindBoneID(L"weaponPosition_r");
	m_stowedShieldBoonId= m_brave->GetModelRender().FindBoneID(L"weaponPosition_l");
}

void SwordShield::MoveWeapon()
{
	switch (m_enWeaponState)
	{
	case IWeapon::enWeaponState_Stowed://���[���
		MoveStowed();
		break;
	case IWeapon::enWeaponState_Armed://�������
		MoveArmed();
		break;
	case IWeapon::enWeaponState_None://�Ȃ�
		break;
	default:
		break;
	}
}

void SwordShield::MoveArmed()
{
	//���Ə��̃��[���h���W��ݒ�
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
	//���Ə��̃��[���h���W��ݒ�
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
