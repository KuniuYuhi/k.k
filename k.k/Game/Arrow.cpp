#include "stdafx.h"
#include "Arrow.h"
#include "Bow.h"


//todo�@�����]�����Ĕ�΂�
//todo �O��������]�ɓK�����Đ��`��Ԃŉ�]

namespace {
	//���킪���[��Ԃ̎��̍��W
	const Vector3 STOWEDS_POSITION = { 0.0f,-500.0f,0.0f };
	//�ʏ�U���̓����蔻��̃T�C�Y
	const Vector3 ARROW_NORMAL_COLLISION_SIZE = { 100.0f,10.0f,10.0f };
	//�X�L���U���̓����蔻��̃T�C�Y
	const Vector3 ARROW_Skill_COLLISION_SIZE = { 150.0f,14.0f,20.0f };

	const float SKILL_RADIUS = 60.0f;

	const float ADD_FORWARD = 30.0f;

	const float SKILL_DELETE_RANGE = 600.0f;
	const float SKILL_ARROW_SPEED = 550.0f;

	const float DEFAULT_DELETE_RANGE = 400.0f;	//������鋗��
	const float DEFAULT_ARROW_SPEED = 450.0f;

	const float GRAVITY = 11.8f;					//�d��
}

Arrow::Arrow()
{
}

Arrow::~Arrow()
{
	DeleteGO(m_arrowCollision);
}

bool Arrow::Start()
{
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
	SetShotFlag(shotFlag);
	SetForward(forward);
	ApplyMatrixToLocalPosition();
	SetShotStartPosition(m_arrowPos, angle);
	SetShotPatternState(shotPatternState);
	//�����蔻��̏�����
	SelectInitCollision(shotPatternState);
	//�����n�_�̐ݒ�
	SetTargetPosition();

	//1.�ڕW�Ɍ����������̌v�Z
	Vector3 targetDistance = m_targetPosition - m_shotStartPosition;
	float distance = targetDistance.Length();
	//2.�����x�̌v�Z
	float verocity = distance / (sin(Math::DegToRad(2 * m_angle)) / GRAVITY);
	//3.�����x�̕���
	m_shotArrowVerocity.Vx = sqrt(verocity) * cos(Math::DegToRad(m_angle));
	m_shotArrowVerocity.Vy = sqrt(verocity) * sin(Math::DegToRad(m_angle));
	//4.��s���Ԃ̌v�Z
	m_flightDuration = distance / m_shotArrowVerocity.Vx;
	
	m_oldArrowPos = m_arrowPos;
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
	//m_arrowCollision->SetIsEnable(false);

	m_arrowCollision->SetWorldMatrix(m_arrowMatrix);
	m_arrowCollision->Update();
}

void Arrow::NormalShot()
{
	//�U�����q�b�g������
	if (m_bow->GetAttackHitFlag() == true)
	{
		//�q�b�g�����̂ŁA�U�����q�b�g�������̃t���O�����Z�b�g
		m_bow->SetAttackHitFlag(false);
		DeleteGO(this);
	}
	//6.�ړ�
	if (m_deleteTimer < m_flightDuration)
	{
		float X = m_forward.x * m_shotArrowVerocity.Vx * 
			g_gameTime->GetFrameDeltaTime() * 10.0f;
		float Z = m_forward.z * m_shotArrowVerocity.Vx * 
			g_gameTime->GetFrameDeltaTime() * 10.0f;

		//�V�������W
		m_arrowPos += {
			X,
			(m_shotArrowVerocity.Vy - (GRAVITY * m_deleteTimer)) * g_gameTime->GetFrameDeltaTime()*4.0f,
			Z
		};
		m_deleteTimer += g_gameTime->GetFrameDeltaTime() * 6.0f;

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
		float dotProduct = Dot(dot1,dot2);
		//�m����(�x�N�g���̑傫��)
		
		//�A�[�N�R�T�C���̌v�Z
		float acos = std::acos(dotProduct / (vec1 * vec2));
		//���W�A������x�ɕϊ�
		float rotationAngle = Math::RadToDeg(acos);

		//////////////////////////////////////////////////

		Quaternion rot = g_quatIdentity;
		//rot.SetRotation(m_arrowMatrix);
		Vector3 a = m_arrowPos;
		a.Normalize();
		Vector3 direction = m_arrowPos - m_shotStartPosition;
		/*direction.x *= a.x;
		direction.y *= a.y;
		direction.z *= a.z;*/

		//rot.SetRotation(direction, g_vec3AxisX);

		m_arrowMatrix.Apply(rot);

		m_rotation = rot;
	}
	else
	{
		//����
		DeleteGO(this);
	}
	//m_modelArrow.SetWorldMatrix(aa);
	
	m_modelArrow.SetPosition(m_arrowPos);
	//m_modelArrow.SetRotation(m_rotation);

	//�O�t���[���̖�̍��W���擾
	m_oldArrowPos = m_arrowPos;
}

void Arrow::SkillShot()
{
	//�G�Ƀ_���[�W��^������^�C�~���O��ݒ�



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
	m_modelArrow.SetPosition(m_arrowPos);
}

void Arrow::Render(RenderContext& rc)
{
	if (GetWeaponState() == enWeaponState_Stowed)
	{
		return;
	}

	m_modelArrow.Draw(rc);
}
