#include "stdafx.h"
#include "SwordShield.h"
#include "Brave.h"

namespace {

	const Vector3 SWORD_COLLISION_SIZE = { 90.0f,20.0f,10.0f };
	const Vector3 SHIELD_COLLISION_SIZE = { 28.0f,50.0f,16.0f };
}

SwordShield::SwordShield()
{
}

SwordShield::~SwordShield()
{
	DeleteGO(m_swordCollision);
	DeleteGO(m_shieldCollision);


}

bool SwordShield::Start()
{

	m_brave = FindGO<Brave>("Brave");

	//����������
	Init();

	return true;
}

void SwordShield::Update()
{


	if (m_enWeaponState == enArmed)
	{
		MoveArmed();
	}



	m_swordModelRender.Update();
	m_shieldModelRender.Update();

	m_swordCollision->Update();
	m_shieldCollision->Update();

}


void SwordShield::Init()
{
	//�����f���̓ǂݍ���
	m_swordModelRender.Init("Assets/modelData/character/Player/NewHero/OneHandSword2.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,0,enModelUpAxisZ);
	//���̒��S�̃{�[��ID���擾
	m_swordCenterBoonId = m_swordModelRender.FindBoneID(L"Center");


	//�����f���̓ǂݍ���
	m_shieldModelRender.Init("Assets/modelData/character/Player/NewHero/Shield.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0, 0, enModelUpAxisZ);


	//���Ə��̍��W�ɑΉ�����{�[��ID���擾
	//������Ԃ̎��̃{�[��ID
	m_armedSwordBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_r");
	m_armedShieldBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_l");


	InitCollision();

	//���킪���[��ԂȂ�
	if (m_enWeaponState == enStowed)
	{
		ChangeStowedState();
	}
	//���킪������ԂȂ�
	else
	{
		ChangeArmedState();
	}


	

}

void SwordShield::ChangeStowedState()
{
	SetCurrentWeaponState(enStowed);

	m_swordModelRender.SetPosition(m_stowedPosition);
	m_shieldModelRender.SetPosition(m_stowedPosition);

	if(m_swordCollision != nullptr) m_swordCollision->SetPosition(m_stowedPosition);

	if (m_shieldCollision != nullptr) m_shieldCollision->SetPosition(m_stowedPosition);

}

void SwordShield::ChangeArmedState()
{
	SetCurrentWeaponState(enArmed);
}

void SwordShield::InitCollision()
{
	//���̓����蔻��
	m_swordCollision = 
		NewGO<CollisionObject>
		(0, g_collisionObjectManager->m_attackCollisionName
		);
	m_swordCollision->CreateBox(
		m_stowedPosition,
		Quaternion::Identity,
		SWORD_COLLISION_SIZE
	);
	//�����蔻��������ō폜���Ȃ��悤�ɂ���
	m_swordCollision->SetIsEnableAutoDelete(false);
	//�����蔻��𖳌���
	m_swordCollision->SetIsEnable(false);


	//���̓����蔻��
	m_shieldCollision = NewGO<CollisionObject>(0, "defence");
	m_shieldCollision->CreateBox(
		m_stowedPosition,
		Quaternion(0.0f, 90.0f, 180.0f, 1.0f),
		SHIELD_COLLISION_SIZE
	);
	m_shieldCollision->SetIsEnableAutoDelete(false);
	m_shieldCollision->SetIsEnable(false);
}

void SwordShield::AttackAction()
{
}

void SwordShield::MoveArmed()
{
	Vector3 swordPos = g_vec3Zero;
	//���̃��[���h���W��ݒ�
	m_swordMatrix =
		m_brave->GetModelRender().GetBone(m_armedSwordBoonId)->GetWorldMatrix();
	m_swordModelRender.SetWorldMatrix(m_swordMatrix);

	//���̃��[���h���W��ݒ�
	m_shieldMatrix =
		m_brave->GetModelRender().GetBone(m_armedShieldBoonId)->GetWorldMatrix();
	m_shieldModelRender.SetWorldMatrix(m_shieldMatrix);

	//�R���W�����̃��[���h���W��ݒ�
	//���̒��S�̃{�[�����烏�[���h���W���擾���ăR���W�����̃��[���h���W�ɐݒ�
	Matrix matrix;
	matrix = m_swordModelRender.GetBone(m_swordCenterBoonId)->GetWorldMatrix();
	m_swordCollision->SetWorldMatrix(matrix);
	//���̃��[���h���W��ݒ�
	m_shieldCollision->SetWorldMatrix(m_shieldMatrix);
}


void SwordShield::Render(RenderContext& rc)
{
	m_swordModelRender.Draw(rc);
	m_shieldModelRender.Draw(rc);

}