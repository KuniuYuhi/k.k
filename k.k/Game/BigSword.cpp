#include "stdafx.h"
#include "BigSword.h"
#include "Brave.h"

namespace {
	//���킪���[��Ԃ̎��̍��W
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
	//�E�҂̃C���X�^���X��T��
	m_brave = FindGO<Brave>("brave");

	InitModel();

	//����
	SetWeaponState(enWeaponState_Stowed);
	SetStowedFlag(true);
	//�h��^�C�v�̐ݒ�
	SetEnDefendTipe(enDefendTipe_avoid);

	return true;
}

void BigSword::Update()
{
	//���[��Ԃ̎��͏������Ȃ�
	if (GetStowedFlag() == true)
	{
		return;
	}

	MoveWeapon();

	m_modelBigSword.Update();
}

void BigSword::InitModel()
{
	//�����f���̏�����
	m_modelBigSword.Init("Assets/modelData/character/Player/NewHero/BigSowrd.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ);

	//���Ə��̍��W�ɑΉ�����{�[��ID���擾
	//������Ԃ̎��̃{�[��ID
	m_armedSwordBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_l");
}

void BigSword::MoveWeapon()
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

void BigSword::MoveArmed()
{
	//���̃��[���h���W��ݒ�
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


