#include "stdafx.h"
#include "BigSword.h"
#include "Brave.h"

namespace {
	//���킪���[��Ԃ̎��̍��W
	const Vector3 STOWEDS_POSITION = { 0.0f,-500.0f,0.0f };

	const Vector3 BIG_SWORD_COLLISION_SIZE = { 18.0f,100.0f,8.0f };

	const float SKILL_RADIUS = 80.0f;

	const float ADD_FORWARD = 8.0f;

	//�X�e�[�^�X
	const int POWER = 40;
	const int ENDURANCE = -1;		//����̑ϋv��(�ϋv�͊T�O�Ȃ�)

	const float HITTABLE_TIME = 0.5f;

	const float MOVE_FORWARD_SPEED = 150.0f;
}

struct IsGroundResult :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//�Փ˃t���O�B
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//�n�ʂƂԂ����ĂȂ�������B
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Ground) {
			//�Փ˂����͕̂ǂł͂Ȃ��B
			isHit = false;
			return 0.0f;
		}
		else
		{
			//�n�ʂƂԂ�������B
		//�t���O��true�ɁB
			isHit = true;
			return 0.0f;
		}
	}
};

BigSword::BigSword()
{
	SetMoveForwardSpeed(MOVE_FORWARD_SPEED);
	SetWeaponPower(POWER);
}

BigSword::~BigSword()
{
	DeleteGO(m_bigSwordCollision);
}

bool BigSword::Start()
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
	SetEnDefendTipe(enDefendTipe_avoid);

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_brave->GetModelRender().AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	return true;
}

void BigSword::Update()
{
	//���[��Ԃ̎��͏������Ȃ�
	if (GetStowedFlag() == true)
	{
		return;
	}

	//�q�b�g�\�����f����
	m_hitDelection.IsHittable(HITTABLE_TIME);

	MoveWeapon();

	m_modelBigSword.Update();
	m_bigSwordCollision->Update();
}

void BigSword::ProcessSkillAttack()
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

void BigSword::InitCollision()
{
	m_bigSwordCollision = NewGO<CollisionObject>(0, "Attack");
	m_bigSwordCollision->CreateBox(
		STOWEDS_POSITION,
		Quaternion(0.0f, 90.0f, 180.0f, 1.0f),
		BIG_SWORD_COLLISION_SIZE
	);
	m_bigSwordCollision->SetIsEnableAutoDelete(false);
	m_bigSwordCollision->SetIsEnable(false);
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

	
	//�����蔻��̗L�����������̏���
	if (m_brave->GetIsCollisionPossibleFlag() == true)
	{
		m_bigSwordCollision->SetIsEnable(true);
	}
	else if (m_bigSwordCollision->IsEnable() != false)
	{
		m_bigSwordCollision->SetIsEnable(false);
	}

	Matrix collisionMatrix = m_swordMatrix;

	//collisionMatrix.

	m_bigSwordCollision->SetWorldMatrix(m_swordMatrix);

}

void BigSword::MoveStowed()
{
	m_swordPos = STOWEDS_POSITION;
	m_modelBigSword.SetPosition(m_swordPos);
	//�����蔻��̍��W�̐ݒ�
	m_bigSwordCollision->SetPosition(m_swordPos);
	//�����蔻��̖�����
	m_bigSwordCollision->SetIsEnable(false);
	SetStowedFlag(true);
}

void BigSword::ProcessRising()
{
	m_skillMovePos = g_vec3AxisY;
	float addYPos = 0.0f;
	//�㏸����
	addYPos +=
		g_gameTime->GetFrameDeltaTime() * GetJampSpeed();
	m_skillMovePos.y += addYPos;
	//�v���C���[�̍��W���X�V
	m_brave->ExecutePosition(m_skillMovePos);
	//�㏸�������̍��W��ݒ�
	m_maxRisingPosition = m_brave->GetPosition();
}

void BigSword::ProcessFall()
{
	m_skillMovePos = g_vec3AxisY;
	float addYPos = 0.0f;
	//�㏸����
	addYPos +=
		g_gameTime->GetFrameDeltaTime() * GetJampSpeed()*1.2f;
	m_skillMovePos.y -= addYPos;

	//�v���C���[�̍��W���X�V
	m_brave->ExecutePosition(m_skillMovePos);

	//�n�ʂƂ̏Փ˔�����Ƃ�
	if (IsGround() == true)
	{
		//���W��n�ʂɍ��킹��
		Vector3 Zero = m_brave->GetPosition();
		Zero.y = 0.0f;
		m_brave->SetPosition(Zero);
	}
}

bool BigSword::IsGround()
{
	Vector3 endPosition = m_brave->GetPosition();

	CCapsuleCollider m_capsuleCollider;
	m_capsuleCollider.Init(20.0f, 30.0f);
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//�n�_�̓G�l�~�[�̍��W�B
	start.setOrigin(btVector3(
		m_maxRisingPosition.x, m_maxRisingPosition.y, m_maxRisingPosition.z));
	//�I�_�̓v���C���[�̍��W�B
	end.setOrigin(btVector3(
		endPosition.x*1.1f, endPosition.y * 1.1f, endPosition.z * 1.1f));
	//�ǂ̔����Ԃ�
	IsGroundResult callback_Ground;
	//�R���C�_�[���n�_����I�_�܂œ������āB
	//�ǂƏՓ˂��邩�ǂ����𒲂ׂ�B
	PhysicsWorld::GetInstance()->ConvexSweepTest(
		(const btConvexShape*)m_capsuleCollider.GetBody(),
		start, end, callback_Ground);
	//�n�ʂɏՓ˂���
	if (callback_Ground.isHit == true)
	{
		return true;
	}
	else
	{
		//�n�ʂł͂Ȃ�����
		return false;
	}
}

void BigSword::Render(RenderContext& rc)
{
	if (GetWeaponState() == enWeaponState_Stowed)
	{
		return;
	}
	m_modelBigSword.Draw(rc);
}

void BigSword::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�X�L���̃W�����v����
	if (wcscmp(eventName, L"RisingGreatSword") == 0)
	{
		//�L�[�t���[����Jamp�̊ԏ�����������
		ProcessRising();
	}
	//�X�L���̃W�����v����
	if (wcscmp(eventName, L"FallGreatSword") == 0)
	{
		//�L�[�t���[����Jamp�̊ԏ�����������
		ProcessFall();
	}
}


