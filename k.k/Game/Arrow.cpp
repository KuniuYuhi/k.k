#include "stdafx.h"
#include "Arrow.h"
#include "Bow.h"
#include "Player.h"
#include "Boss.h"
#include "CharactersInfoManager.h"


//todo�@�����]�����Ĕ�΂�
//todo �O��������]�ɓK�����Đ��`��Ԃŉ�]

namespace {
	//���킪���[��Ԃ̎��̍��W
	const Vector3 STOWEDS_POSITION = { 0.0f,-500.0f,0.0f };
	//�ʏ�U���̓����蔻��̃T�C�Y
	const Vector3 ARROW_NORMAL_COLLISION_SIZE = { 70.0f,30.0f,10.0f };
	//�X�L���U���̓����蔻��̃T�C�Y
	const Vector3 ARROW_Skill_COLLISION_SIZE = { 150.0f,14.0f,20.0f };

	const float SKILL_RADIUS = 60.0f;

	const float ADD_FORWARD = 30.0f;

	const float SKILL_DELETE_RANGE = 600.0f;
	const float SKILL_ARROW_SPEED = 1000.0f;

	const float DEFAULT_DELETE_RANGE = 400.0f;	//������鋗��
	const float DEFAULT_ARROW_SPEED = 450.0f;

	const float GRAVITY = 11.8f;					//�d��

	const float HITTABLE_TIMER_LIMMIT = 0.1f;
}

Arrow::Arrow()
{
}

Arrow::~Arrow()
{
	DeleteGO(m_arrowCollision);

	if (m_arrowAttackEffect != nullptr)
	{
		m_arrowAttackEffect->Stop();
	}
}

bool Arrow::Start()
{
	//�E�҂̃C���X�^���X��T��
	m_player = FindGO<Player>("player");

	//���f����������
	InitModel();
	
	//����
	//�|�Ɠ�����Ԃ�ݒ�
	SetWeaponState(m_bow->GetBowEnWeaponState());

	return true;
}

void Arrow::Update()
{
	if (m_shotFlag != true)
	{
		MoveWeapon();
	}
	else
	{
		//�ˌ����̈ړ�����
		ProcessLongRangeAttack();

		
	}

	m_modelArrow.Update();
}

void Arrow::MoveWeapon()
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

void Arrow::MoveArmed()
{
	//��̍��W��ݒ�
	m_arrowMatrix = m_bow->GetArrowMatrix();
	m_modelArrow.SetWorldMatrix(m_arrowMatrix);
}

void Arrow::MoveStowed()
{
	//��̍��W��ݒ�
	m_arrowPos = STOWEDS_POSITION;
	m_modelArrow.SetPosition(m_arrowPos);
}

bool Arrow::IsHitCollision()
{
	//�{�X�Ƃ̓����蔻��
	if (m_arrowCollision->IsHit(
		CharactersInfoManager::GetInstance()->GetBossInstance()->GetCharacterController()
	))
	{
		//�q�b�g����
		return true;
	}

	//todo �������ꂽ�u�Ԃ��ƃL�����R��������������Ă��Ȃ��̂ŃG���[�ɂȂ�

	//���u�����X�^�[�̃��X�g�̎擾
	const auto& mobMonsters = 
		CharactersInfoManager::GetInstance()->GetMobMonsters();
	//���u�����X�^�[�Ƃ̓����蔻��
	for (auto mobMonster : mobMonsters)
	{
		if (m_arrowCollision->IsHit(
			mobMonster->GetCharacterController()))
		{
			//�q�b�g����
			return true;
		}
	}

	//�q�b�g���Ȃ�����
	return false;
}

void Arrow::ProcessLongRangeAttack()
{
	switch (m_enShotPatternState)
	{
	case Arrow::enShotPatternState_Normal:
		NormalShot();
		break;
	case Arrow::enShotPatternState_Skill:
		SkillShot();
		break;
	default:
		break;
	}

	m_arrowCollision->SetPosition(m_arrowPos);
	m_arrowCollision->Update();
}

void Arrow::SetShotArrowSetting(
	bool shotFlag, 
	Vector3 forward, 
	Vector3 shotStartPosition, 
	float angle,
	EnShotPatternState shotPatternState
)
{
	//�V���b�g�t���O��ݒ�
	SetShotFlag(shotFlag);
	//�O������ݒ�
	SetForward(forward);
	//���[���h���W�����[�J�����W�ɓK��
	ApplyMatrixToLocalPosition();
	//�V���b�g�J�n���W�̐ݒ�
	SetShotStartPosition(m_arrowPos, angle);
	//�V���b�g�X�e�[�g�̐ݒ�
	SetShotPatternState(shotPatternState);
	//�����蔻��̏�����
	SelectInitCollision(shotPatternState);
	//�����n�_�̐ݒ�
	SetTargetPosition();
	//�ʏ�U���̐ݒ�Ȃ�
	if (shotPatternState == enShotPatternState_Normal)
	{
		//�ʏ�U���ɕK�v�ȏ��̐ݒ�
		SetNormalShotInfo();
	}

	//�U�����@�ɂ��G�t�F�N�g�Đ�
	PlayArrowEffect();
}

void Arrow::SetTargetPosition()
{
	m_targetPosition = m_shotStartPosition;
	m_targetPosition += m_forward * DEFAULT_DELETE_RANGE;
	m_targetPosition.y = 0.0f;
}

void Arrow::InitModel()
{
	//��f���̏�����
	m_modelArrow.Init("Assets/modelData/character/Player/NewHero/Arrow.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ
	);
}

void Arrow::PlayArrowEffect()
{
	Quaternion rot = g_quatIdentity;
	rot.SetRotationYFromDirectionXZ(m_forward);

	if (m_enShotPatternState==enShotPatternState_Normal)
	{
		m_arrowAttackEffect = NewGO<EffectEmitter>(0);
		m_arrowAttackEffect->Init(InitEffect::enEffect_Arrow);
		m_arrowAttackEffect->Play();
		m_arrowAttackEffect->SetScale(g_vec3One * 12.0f);
		
	}
	else
	{
		m_arrowAttackEffect = NewGO<EffectEmitter>(0);
		m_arrowAttackEffect->Init(InitEffect::enEffect_BowArrowSkillShot);
		m_arrowAttackEffect->Play();
		m_arrowAttackEffect->SetScale(g_vec3One * 10.0f);
	}

	m_arrowAttackEffect->SetPosition(m_shotStartPosition);
	m_arrowAttackEffect->SetRotation(rot);
	m_arrowAttackEffect->Update();

	
}

void Arrow::SelectInitCollision(EnShotPatternState shotPatternState)
{
	switch (shotPatternState)
	{
	case Arrow::enShotPatternState_Normal:
		InitCollision(
			"Attack",m_arrowPos,g_quatIdentity, ARROW_NORMAL_COLLISION_SIZE);
		break;
	case Arrow::enShotPatternState_Skill:
		InitCollision(
			"skillAttack", m_arrowPos, g_quatIdentity, ARROW_Skill_COLLISION_SIZE);
		break;
	default:
		break;
	}
}

void Arrow::InitCollision(
	const char* collisionName,
	Vector3 createPos,
	Quaternion rotation,
	Vector3 collisionSize
)
{
	//��̒ʏ�U���p�̓����蔻��̐���
	m_arrowCollision = NewGO<CollisionObject>(0, collisionName);
	m_arrowCollision->CreateBox(
		createPos,
		rotation,
		collisionSize
	);
	m_arrowCollision->SetIsEnableAutoDelete(false);
	m_arrowCollision->SetWorldMatrix(m_arrowMatrix);
	m_arrowCollision->Update();
}

void Arrow::NormalShot()
{
	//�R���W�������G�ɓ���������
	if (IsHitCollision() == true)
	{
		ProcessDelete();
		return;
	}
	
	//��������`���ړ�����
	//��������܂ł̐������ԓ��Ȃ�
	if (m_deleteTimer < m_flightDuration)
	{
		//��̈ړ�����
		MoveNormalShot();
		//��̉�]�����@����ĂȂ�
		RoatationNormalShot();
		//��������܂ł̃^�C�}�[�����Z
		m_deleteTimer += g_gameTime->GetFrameDeltaTime() * 6.0f;
		
	}
	else
	{
		//����
		DeleteGO(this);
	}
	
	//��̃��[���h���W���擾
	Matrix arrowMatrix = m_arrowMatrix;
	//�s��ɍ��W��K��
	ApplyVector3ToMatirx(arrowMatrix, m_arrowPos);
	//���f���̍s���ݒ�
	m_modelArrow.SetWorldMatrix(arrowMatrix);

	//�G�t�F�N�g�̍��W�̐ݒ�ƍX�V
	m_arrowAttackEffect->SetPosition(m_arrowPos);
	m_arrowAttackEffect->Update();

	//�O�t���[���̖�̍��W���擾
	m_oldArrowPos = m_arrowPos;
}

void Arrow::SkillShot()
{
	//�G�Ƀ_���[�W��^������^�C�~���O��ݒ�
	if (m_player->GetHittableFlag() != true&&
		m_hitDelection.IsHittable(HITTABLE_TIMER_LIMMIT))
	{
		//���i�q�b�g�U���\
		m_player->SetHittableFlag(true);
		//���i�q�b�g�\����t���O�����Z�b�g
		m_hitDelection.SetHittableFlag(false);
	}

	//�ˌ��J�n���W���猻�݂̍��W�Ɍ������x�N�g�����v�Z
	Vector3 diff = m_arrowPos - m_shotStartPosition;
	//����R���ł��鋗���Ȃ�
	if (diff.Length() > SKILL_DELETE_RANGE)
	{
		//����
		DeleteGO(this);
	}
	//��̍��W��ݒ�
	m_arrowPos += (m_forward * SKILL_ARROW_SPEED) * g_gameTime->GetFrameDeltaTime();

	Matrix arrowMatrix = m_arrowMatrix;
	//�s��ɍ��W��K��
	ApplyVector3ToMatirx(arrowMatrix, m_arrowPos);
	//�s���ݒ�
	m_modelArrow.SetWorldMatrix(arrowMatrix);
	//�G�t�F�N�g�̍��W�̐ݒ�ƍX�V
	m_arrowAttackEffect->SetPosition(m_arrowPos);
	m_arrowAttackEffect->Update();
}

void Arrow::ApplyVector3ToMatirx(Matrix& baseMatrix, Vector3 position)
{
	Matrix matrix = baseMatrix;
	baseMatrix.m[3][0] = position.x;
	baseMatrix.m[3][1] = position.y;
	baseMatrix.m[3][2] = position.z;
}

void Arrow::SetNormalShotInfo()
{
	//1.�ڕW�Ɍ����������̌v�Z
	Vector3 targetDistance = m_targetPosition - m_shotStartPosition;
	float distance = targetDistance.Length();
	//2.�����x�̌v�Z
	float verocity = distance / (sin(Math::DegToRad(2 * m_angle)) / GRAVITY);
	//3.�����x�̕���
	m_shotArrowVerocity.x = sqrt(verocity) * cos(Math::DegToRad(m_angle));
	m_shotArrowVerocity.y = sqrt(verocity) * sin(Math::DegToRad(m_angle));
	//4.��s���Ԃ̌v�Z
	m_flightDuration = distance / m_shotArrowVerocity.x;

	m_oldArrowPos = m_arrowPos;
}

void Arrow::MoveNormalShot()
{
	float X = m_forward.x * m_shotArrowVerocity.x *
		g_gameTime->GetFrameDeltaTime() * 10.0f;
	float Z = m_forward.z * m_shotArrowVerocity.x *
		g_gameTime->GetFrameDeltaTime() * 10.0f;

	//�V�������W
	m_arrowPos += {
		X,
			(m_shotArrowVerocity.y - (GRAVITY * m_deleteTimer))* g_gameTime->GetFrameDeltaTime() * 4.0f,
			Z
	};
}

void Arrow::RoatationNormalShot()
{
	//��]
	Vector3 Axis;
	Axis.Cross(m_shotStartPosition, m_arrowPos);

	//�p�x�̌v�Z
	//����
	Vector3 dot1 = m_shotStartPosition;
	Vector3 dot2 = m_arrowPos;

	float vec1 = m_shotStartPosition.Length();
	float vec2 = m_arrowPos.Length();

	dot1.Normalize();
	dot2.Normalize();
	float dotProduct = Dot(dot1, dot2);
	//�m����(�x�N�g���̑傫��)

	//�A�[�N�R�T�C���̌v�Z
	float acos = std::acos(dotProduct / (vec1 * vec2));
	//���W�A������x�ɕϊ�
	float rotationAngle = Math::RadToDeg(acos);
}

void Arrow::ProcessDelete()
{
	//�����X�^�[�Ƀ_���[�W��^������悤�Ƀt���O�����Z�b�g
	CharactersInfoManager::GetInstance()->SetAllMonsterDamgeHitFlag(true);
	//�����蔻��̍폜
	DeleteGO(m_arrowCollision);
	//����������̂ō폜
	DeleteGO(this);
}

void Arrow::Render(RenderContext& rc)
{
	//���[��ԂȂ�\�����Ȃ�
	if (GetWeaponState() == enWeaponState_Stowed)
	{
		return;
	}

	m_modelArrow.Draw(rc);
}
