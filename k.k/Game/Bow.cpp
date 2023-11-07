#include "stdafx.h"
#include "Bow.h"
#include "Brave.h"
#include "Arrow.h"

namespace {
	//���킪���[��Ԃ̎��̍��W
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
	//�E�҂̃C���X�^���X��T��
	m_brave = FindGO<Brave>("brave");

	//��̃I�u�W�F�N�g�𐶐�
	m_arrow = NewGO<Arrow>(0, "arrow");
	m_arrow->SetBow(this);
	//��������Ă���̂ŁA�ێ��t���O���Z�b�g
	SetStockArrowFlag(true);

	InitModel();
	InitCollision();

	//����
	SetWeaponState(enWeaponState_Stowed);
	SetStowedFlag(true);
	//�h��^�C�v�̐ݒ�
	SetEnDefendTipe(enDefendTipe_avoid);

	return true;
}

void Bow::Update()
{
	//���[��ԂȂ�
	if (GetStowedFlag() == true)
	{
		return;
	}

	if (GetStockArrowFlag() != true)
	{
		m_arrow = NewGO<Arrow>(0, "arrow");
		m_arrow->SetBow(this);
		//��������Ă���̂ŁA�ێ��t���O���Z�b�g
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

bool Bow::IsHitCollision()
{
	return false;
}

void Bow::ProcessSkillAttack()
{
}

void Bow::InitModel()
{
	//�|���f���̏�����
	m_modelBow.Init("Assets/modelData/character/Player/NewHero/Bow.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ
	);

	//�|�Ɩ�̍��W�ɑΉ�����{�[��ID���擾
	//������Ԃ̎��̃{�[��ID
	m_armedBowBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_l");
	m_armedArrowBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_r");

}

void Bow::MoveArmed()
{
	//�|�̃��[���h���W��ݒ�
	m_bowMatrix = 
		m_brave->GetModelRender().GetBone(m_armedBowBoonId)->GetWorldMatrix();
	m_modelBow.SetWorldMatrix(m_bowMatrix);
	//��̃��[���h���W��ݒ�
	m_arrowMatrix = 
		m_brave->GetModelRender().GetBone(m_armedArrowBoonId)->GetWorldMatrix();
	m_modelBow.SetWorldMatrix(m_bowMatrix);

	if (m_arrow != nullptr)
	{
		//��̃X�e�[�g��ݒ�
		m_arrow->SetWeaponState(enWeaponState_Armed);
	}
	
}

void Bow::MoveStowed()
{
	//�|�̍��W��ݒ�
	m_bowPos = STOWEDS_POSITION;
	m_modelBow.SetPosition(m_bowPos);

	if (m_arrow != nullptr)
	{
		//��̃X�e�[�g��ݒ�
		m_arrow->SetWeaponState(enWeaponState_Stowed);
	}
	

	SetStowedFlag(true);
}

void Bow::ProcessLongRangeAttack()
{
	if (m_arrow != nullptr)
	{
		//��𔭎�
		m_arrow->SetShotFlag(true);
		//�E�҂̑O�������擾
		m_arrow->SetForward(m_brave->GetForward());
		//
		m_arrow->ApplyMatrixToLocalPosition();
		//�ˌ��J�n���W��ݒ�
		m_arrow->SetShotStartPosition(m_arrow->GetPosition());
		//���������̂ŁA���̖��
		SetStockArrowFlag(false);
		m_arrow = nullptr;
	}
}
