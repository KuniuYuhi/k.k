#include "stdafx.h"
#include "TurtleShell.h"

namespace {
	const float ANGLE = 100.0f;				//����p

	//�X�e�[�^�X
	int MAXHP = 200;
	int MAXMP = 500;
	int ATK = 15;
	float SPEED = 70.0f;
	const char* NAME = "TurtleShell";
}

TurtleShell::TurtleShell()
{
	m_angle = ANGLE;
}

TurtleShell::~TurtleShell()
{
}

//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
struct IsForestResult :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//�Փ˃t���O�B
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//�n�ʂƂԂ����ĂȂ�������B
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_forest) {
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

bool TurtleShell::Start()
{
	m_status.InitStatus(
		MAXHP,
		MAXMP,
		ATK,
		SPEED,
		NAME
	);

	InitModel();

	//�܂������A�j���[�V�����B���̌�s��



	//�@�������������B
	srand((unsigned)time(NULL));

	//6����8�͈̔͂̃C���^�[�o��
	m_angleChangeTime = rand() % 3 + 6;

	return true;
}

void TurtleShell::InitModel()
{
	m_modelRender.Init("Assets/modelData/character/TurtleShell/TurtleShell.tkm");


	m_charaCon.Init(
		22.0f,
		4.0f,
		m_position
	);

	//���W�̐ݒ�
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void TurtleShell::Update()
{
	//�v���C���[���{�X�����ꂽ�������

	AngleChangeTimeIntarval(m_angleChangeTime);

	Move();
	Rotation();


	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void TurtleShell::Move()
{
	//�͈͓��Ƀv���C���[�����Ȃ�������
	if (IsFindPlayer(m_distanceToPlayer) != true)
	{
		//���b�Ԋu�Ō������x�N�g����ς���
		if (m_angleChangeTimeFlag == false)
		{
			m_direction = SetDirection();
			m_angleChangeTimeFlag = true;
		}

		m_moveSpeed = m_direction * m_status.defaultSpeed;
		m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);

	}
	//������
	else
	{
		Vector3 toPlayerDir = m_targetPosition;
		toPlayerDir.Normalize();
		//�^�[�Q�b�g�Ɍ������x�N�g���ƑO�����̓��ς��v�Z����
		float t = toPlayerDir.Dot(m_forward);
		//���ς̌��ʂ�acos�֐��ɓn���āAm_enemyFowrad��toPlayerDir�̂Ȃ��p�x�����߂�B
		float angle = acos(t);
		//����p����
		if (fabsf(angle) < Math::DegToRad(m_angle))
		{
			//�ǂ�������
			m_moveSpeed = calcVelocity(m_status);
			m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
		}
	}

	//�ǂɂԂ������甽�]
	if (IsBumpedForest() == true)
	{
		m_direction *= -1.0f;
		m_moveSpeed = m_direction * m_status.defaultSpeed;
		m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
		return;
	}

}

Vector3 TurtleShell::SetDirection()
{
	Vector3 randomPos = g_vec3Zero;
	randomPos.y = 0.0f;
	float X = rand() % (21 - 10) + 1;
	float Z = rand() % (21 - 10) + 1;
	randomPos.x += X;
	randomPos.z += Z;
	randomPos.Normalize();

	return randomPos;
}

bool TurtleShell::IsBumpedForest()
{
	Vector3 pos1 = m_position;
	Vector3 pos2 = m_position;
	pos1.Normalize();
	pos2 += pos1 * 20.0f;

	SphereCollider m_sphereCollider;
	m_sphereCollider.Create(1.0f);

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//�n�_�̓G�l�~�[�̍��W�B
	start.setOrigin(btVector3(m_position.x, m_position.y, m_position.z));
	//�I�_�̓v���C���[�̍��W�B
	end.setOrigin(btVector3(
		pos2.x, pos2.y, pos2.z));

	//�ǂ̔����Ԃ�
	IsForestResult callback_Forest;
	//�R���C�_�[���n�_����I�_�܂œ������āB
	//�ǂƏՓ˂��邩�ǂ����𒲂ׂ�B
	PhysicsWorld::GetInstance()->ConvexSweepTest(
		(const btConvexShape*)m_sphereCollider.GetBody(),
		start, end, callback_Forest);
	//�X�ɏՓ˂���
	if (callback_Forest.isHit == true)
	{
		return true;
	}
	else
	{
		//�Փ˂��Ȃ�����
		return false;
	}
}

void TurtleShell::Damage(int attack)
{
}

bool TurtleShell::RotationOnly()
{
	return false;
}

void TurtleShell::ManageState()
{
}

void TurtleShell::PlayAnimation()
{

}

void TurtleShell::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
