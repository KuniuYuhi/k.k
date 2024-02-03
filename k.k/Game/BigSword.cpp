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
	const int SKILL_POWER = 70;
	const int ENDURANCE = -1;		//����̑ϋv��(�ϋv�͊T�O�Ȃ�)

	const float HITTABLE_TIME = 0.5f;

	const float MOVE_FORWARD_SPEED = 150.0f;

	const float ATTACK_1COMBO_KNOCKBACK_POWER = 160.0f;
	const float ATTACK_2COMBO_KNOCKBACK_POWER = 175.0f;
	const float ATTACK_3COMBO_KNOCKBACK_POWER = 210.0f;
	const float SKILL_KNOCKBACK_POWER = 320.0f;

	const float NORMAL_ATTACK_1_EFFECT_ANGLE = -270.0f;
	const float NORMAL_ATTACK_2_EFFECT_ANGLE = 80.0f;

	const float NORMAL_ATTACK_1_2_EFFECT_SIZE = 11.0f;
	const float NORMAL_ATTACK_3_EFFECT_SIZE = 18.0f;

	const float SKILL_ATTACK_EFFECT_SIZE = 10.0f;
	const float SKILL_ATTACK_RISING_EFFECT_SIZE = 10.0f;
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

	m_knockPower_1combo = ATTACK_1COMBO_KNOCKBACK_POWER;
	m_knockPower_2combo = ATTACK_2COMBO_KNOCKBACK_POWER;
	m_knockPower_3combo = ATTACK_3COMBO_KNOCKBACK_POWER;
	m_knockPower_Skill = SKILL_KNOCKBACK_POWER;
}

BigSword::~BigSword()
{
	DeleteGO(m_bigSwordCollision);
}

bool BigSword::Start()
{
	//����̃X�e�[�^�X������
	m_status.InitWeaponStatus(GetName());

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

	//���i�q�b�g�\�����f����B���̕���ŏ������r���ŏI����Ă��邩������Ȃ�����
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
	//
	
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

void BigSword::PlaySkillAttackEffect()
{
	//�G�t�F�N�g�Đ��̂��߂̍��W�Ɖ�]�̐ݒ�
	Vector3 pos;
	Quaternion rot;
	SettingEffectInfo(pos, rot, 0.0f);
	//�G�t�F�N�g�Đ�
	PlayEffect(
		InitEffect::enEffect_GreatSwordSkillAttack,
		pos, SKILL_ATTACK_EFFECT_SIZE
	);
}

void BigSword::SettingEffectInfo(Vector3& effectPos, Quaternion& rot, float angle)
{
	effectPos = g_vec3Zero;
	m_swordMatrix.Apply(effectPos);
	rot = g_quatIdentity;
	rot.SetRotationYFromDirectionXZ(m_brave->GetForward());
	rot.AddRotationDegZ(angle);
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
	//�ʏ�U���P�̃A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"GreatSwordPlayCombo1Effect") == 0)
	{
		Vector3 pos;
		Quaternion rot;
		SettingEffectInfo(pos, rot, NORMAL_ATTACK_1_EFFECT_ANGLE);
		//�G�t�F�N�g�Đ�
		PlayEffect(InitEffect::enEffect_GreatSwordCombo12,
			pos, NORMAL_ATTACK_1_2_EFFECT_SIZE, rot
		);
		//���Đ�
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_GreatSwordCombo_1_2,
			g_soundManager->GetSEVolume()
		);
	}
	//�ʏ�U���Q�̃A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"GreatSwordPlayCombo2Effect") == 0)
	{
		Vector3 pos;
		Quaternion rot;
		SettingEffectInfo(pos, rot, NORMAL_ATTACK_2_EFFECT_ANGLE);
		//�G�t�F�N�g�Đ�
		PlayEffect(InitEffect::enEffect_GreatSwordCombo12,
			pos, NORMAL_ATTACK_1_2_EFFECT_SIZE, rot
		);
		//���Đ�
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_GreatSwordCombo_1_2,
			g_soundManager->GetSEVolume()
		);
	}
	//�ʏ�U���R�̃A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"GreatSwordPlayCombo3Effect") == 0)
	{
		Vector3 pos;
		Quaternion rot;
		SettingEffectInfo(pos, rot, 0.0f);
		//�G�t�F�N�g�Đ�
		PlayEffect(InitEffect::enEffect_GreatSwordCombo3,
			pos, NORMAL_ATTACK_3_EFFECT_SIZE, rot
		);
	}
	//�X�L���̏㏸�A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"GreatSwordSkillRising") == 0)
	{
		//�G�t�F�N�g�Đ�
		PlayEffect(InitEffect::enEffect_GreatSwordSkillRising,
			m_brave->GetPosition(), SKILL_ATTACK_RISING_EFFECT_SIZE
		);
		//���Đ�
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_GreatSwordSkillRising,
			g_soundManager->GetSEVolume()
		);
	}
	//�X�L���U���̃A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"GreatSwordSkillAttack") == 0)
	{
		//���C������̃X�L���U������
		ProcessSkillAttack();
		//�X�L���U���̃G�t�F�N�g�Đ�
		PlaySkillAttackEffect();
		//���Đ�
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_GreatSwordSkillAttack,
			g_soundManager->GetSEVolume()
		);
	}
	
}


