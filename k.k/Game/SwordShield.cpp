#include "stdafx.h"
#include "SwordShield.h"
#include "Brave.h"

//todo �A�j���[�V�����C�x���g�ǉ�
//�W�����v���������

namespace {
	//���킪���[��Ԃ̎��̍��W
	const Vector3 STOWEDS_POSITION = { 0.0f,-500.0f,0.0f };

	const Vector3 SWORD_COLLISION_SIZE = { 12.0f,100.0f,5.0f };
	const Vector3 SHIELD_COLLISION_SIZE = { 22.0f,40.0f,16.0f };

	const float SKILL_RADIUS = 60.0f;

	const float ADD_FORWARD = 30.0f;

	//�X�e�[�^�X
	const int POWER = 30;
	const int ENDURANCE = 70;		//����̑ϋv��(���̑ϋv��)�B

	const float MOVE_FORWARD_SPEED = 200.0f;

	const float ATTACK_1COMBO_KNOCKBACK_POWER = 120.0f;
	const float ATTACK_2COMBO_KNOCKBACK_POWER = 130.0f;
	const float ATTACK_3COMBO_KNOCKBACK_POWER = 140.0f;
	const float SKILL_KNOCKBACK_POWER = 180.0f;
}

SwordShield::SwordShield()
{
	SetMoveForwardSpeed(MOVE_FORWARD_SPEED);
	SetWeaponPower(POWER);

	m_knockPower_1combo = ATTACK_1COMBO_KNOCKBACK_POWER;
	m_knockPower_2combo = ATTACK_2COMBO_KNOCKBACK_POWER;
	m_knockPower_3combo = ATTACK_3COMBO_KNOCKBACK_POWER;
	m_knockPower_Skill = SKILL_KNOCKBACK_POWER;
}

SwordShield::~SwordShield()
{
	DeleteGO(m_swordCollision);
	DeleteGO(m_shieldCollision);
}

bool SwordShield::Start()
{
	//����̃X�e�[�^�X������
	m_status.InitWeaponStatus(
		POWER, ENDURANCE
	);

	//�E�҂̃C���X�^���X��T��
	m_brave = FindGO<Brave>("brave");

	InitModel();
	InitCollision();

	//�h��^�C�v�̐ݒ�
	SetEnDefendTipe(enDefendTipe_Defence);

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_brave->GetModelRender().AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	return true;
}

void SwordShield::Update()
{
	//���[��ԂȂ�
	if (GetStowedFlag() == true)
	{
		return;
	}

	MoveWeapon();

	m_modelSword.Update();
	m_modelShield.Update();
	m_swordCollision->Update();
	m_shieldCollision->Update();
}

bool SwordShield::IsHitCollision()
{
	//�_�[�N�{�[���̓����蔻��
	const auto& DarkBallCollisions = g_collisionObjectManager->FindCollisionObjects("darkball");
	//�R���W�����̔z���for���ŉ�
	for (auto collision : DarkBallCollisions)
	{
		//���g�̃L�����R���ƏՓ˂�����
		if (collision->IsHit(m_shieldCollision) == true)
		{	
			return true;
		}
	}

	//�_�[�N�E�H�[���̓����蔻��
	const auto& DarkWallCollisions = g_collisionObjectManager->FindCollisionObjects("DarkWall");
	//�R���W�����̔z���for���ŉ�
	for (auto collision : DarkWallCollisions)
	{
		//���g�̃L�����R���ƏՓ˂�����
		if (collision->IsHit(m_shieldCollision) == true)
		{	
			return true;
		}
	}

	//���e�I�̓����蔻��
	const auto& MeteoCollisions = g_collisionObjectManager->FindCollisionObjects("meteo");
	//�R���W�����̔z���for���ŉ�
	for (auto collision : MeteoCollisions)
	{
		//���g�̃L�����R���ƏՓ˂�����
		if (collision->IsHit(m_shieldCollision) == true)
		{
			return true;
		}
	}
	//���e�I�̔����̓����蔻��
	const auto& MeteoExplosionCollisions = g_collisionObjectManager->FindCollisionObjects("explosion");
	//�R���W�����̔z���for���ŉ�
	for (auto collision : MeteoExplosionCollisions)
	{
		//���g�̃L�����R���ƏՓ˂�����
		if (collision->IsHit(m_shieldCollision) == true)
		{	
			return true;
		}
	}

	//�����X�^�[�̍U���̓����蔻��
	const auto& MonsterCollisions = g_collisionObjectManager->FindCollisionObjects("monsterattack");
	//�R���W�����̔z���for���ŉ�
	for (auto collision : MonsterCollisions)
	{
		//���g�̃L�����R���ƏՓ˂�����
		if (collision->IsHit(m_shieldCollision) == true)
		{
			return true;
		}
	}

	//�_�[�N���e�I�̍U���̓����蔻��
	const auto& DarkMeteoCollisions = g_collisionObjectManager->FindCollisionObjects("bigmeteo");
	//�R���W�����̔z���for���ŉ�
	for (auto collision : DarkMeteoCollisions)
	{
		//���g�̃L�����R���ƏՓ˂�����
		if (collision->IsHit(m_shieldCollision) == true)
		{	
			return true;
		}
	}

	return false;
}

void SwordShield::ProcessSkillAttack()
{
	//�����蔻��𐶐�������W��ݒ�
	m_skillAttackPosition = g_vec3Zero;
	//���̃��[���h���W���x�N�g���ɏ�Z
	m_swordMatrix.Apply(m_skillAttackPosition);
	m_skillAttackPosition.y = 0.0f;
	Vector3 forward;
	forward = m_brave->GetForward();
	forward *= ADD_FORWARD;
	//�O�������𑫂�
	m_skillAttackPosition += forward;

	//�X�L���U�����̓����蔻��̐���
	auto skillCollision = NewGO<CollisionObject>(0, "skillAttack");
	skillCollision->CreateSphere(
		m_skillAttackPosition,
		g_quatIdentity,
		SKILL_RADIUS
	);
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
}

void SwordShield::InitCollision()
{
	InitSwordCollision();
	InitShieldCollision();
}

void SwordShield::InitSwordCollision()
{
	//���̓����蔻��
	m_swordCollision = NewGO<CollisionObject>(0, "Attack");
	/*m_swordCollision->CreateMesh(
		g_vec3Zero,
		g_quatIdentity,
		m_modelSword.GetModel(),
		m_brave->GetModelRender().GetBone(m_armedSwordBoonId)->GetWorldMatrix()
	);*/
	m_swordCollision->CreateBox(
		STOWEDS_POSITION,
		Quaternion(0.0f, 90.0f, 180.0f, 1.0f),
		SWORD_COLLISION_SIZE
	);
	m_swordCollision->SetIsEnableAutoDelete(false);
	m_swordCollision->SetIsEnable(false);
}

void SwordShield::InitShieldCollision()
{
	//���̓����蔻��
	m_shieldCollision = NewGO<CollisionObject>(0, "defence");
	m_shieldCollision->CreateBox(
		STOWEDS_POSITION,
		Quaternion(0.0f, 90.0f, 180.0f, 1.0f),
		SHIELD_COLLISION_SIZE
	);
	m_shieldCollision->SetIsEnableAutoDelete(false);
	m_shieldCollision->SetIsEnable(false);
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
	Vector3 swordPos = g_vec3Zero;
	//���Ə��̃��[���h���W��ݒ�
	m_swordMatrix =
		m_brave->GetModelRender().GetBone(m_armedSwordBoonId)->GetWorldMatrix();
	m_modelSword.SetWorldMatrix(m_swordMatrix);


	m_shieldMatrix =
		m_brave->GetModelRender().GetBone(m_armedShieldBoonId)->GetWorldMatrix();
	m_modelShield.SetWorldMatrix(m_shieldMatrix);


	//�h��\����Y�{�^���������Ă���Ԃ͖h��
	if (GetIsDefendEnableFlag()!=false && 
		g_pad[0]->IsPress(enButtonY) == true)
	{
		//�h�䂵�Ă���Ԃ͉�]�\
		SetRotationDelectionFlag(true);
	}
	else
	{
		//�h�䂵�Ă��Ȃ��̂Ńt���O�����Z�b�g
		SetRotationDelectionFlag(false);
	}



	//�����蔻��̗L�����������̏���
	if (m_brave->GetIsCollisionPossibleFlag() == true)
	{
		m_swordCollision->SetIsEnable(true);
	}
	else if (m_swordCollision->IsEnable() != false)
	{
		m_swordCollision->SetIsEnable(false);
	}

	m_swordCollision->SetWorldMatrix(m_swordMatrix);
	m_shieldCollision->SetWorldMatrix(m_shieldMatrix);
}

void SwordShield::MoveStowed()
{
	m_swordPos = STOWEDS_POSITION;
	m_shieldPos = STOWEDS_POSITION;

	m_modelSword.SetPosition(m_swordPos);
	m_modelShield.SetPosition(m_shieldPos);
	//�����蔻��̍��W�̐ݒ�
	m_swordCollision->SetPosition(m_swordPos);
	m_shieldCollision->SetPosition(m_shieldPos);
	//�����蔻��̖�����
	m_swordCollision->SetIsEnable(false);
	m_shieldCollision->SetIsEnable(false);
	SetStowedFlag(true);
}

void SwordShield::ProcessRising()
{
	m_skillMovePos = g_vec3AxisY;
	float addYPos = 0.0f;
	//�㏸����
	addYPos +=
		g_gameTime->GetFrameDeltaTime() * GetJampSpeed();
	m_skillMovePos.y += addYPos;
	//�v���C���[�̍��W���X�V
	m_brave->ExecutePosition(m_skillMovePos);
}

void SwordShield::ProcessFall()
{
	m_skillMovePos = g_vec3AxisY;
	float addYPos = 0.0f;
	//�㏸����
	addYPos +=
		g_gameTime->GetFrameDeltaTime() * GetJampSpeed() * 1.2f;
	m_skillMovePos.y -= addYPos;

	//�v���C���[�̍��W���X�V
	m_brave->ExecutePosition(m_skillMovePos);
}

void SwordShield::Render(RenderContext& rc)
{
	if (GetWeaponState() == enWeaponState_Stowed)
	{
		return;
	}

	m_modelSword.Draw(rc);

	//�h��\�łȂ��Ȃ�\�����Ȃ�
	if (GetIsDefendEnableFlag() != true)
	{
		return;
	}
	m_modelShield.Draw(rc);
}

void SwordShield::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�X�L���̃W�����v����
	if (wcscmp(eventName, L"RisingSword") == 0)
	{
		//�L�[�t���[����Jamp�̊ԏ�����������
		ProcessRising();
	}
	//�X�L���̃W�����v����
	if (wcscmp(eventName, L"FallSword") == 0)
	{
		//�L�[�t���[����Jamp�̊ԏ�����������
		ProcessFall();
	}
}
