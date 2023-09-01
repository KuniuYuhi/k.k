#include "stdafx.h"
#include "DarkMeteorite.h"
#include "Meteo.h"
#include "Player.h"
#include "Lich.h"

namespace {
	const int MAX_LENGTH = 50;
	const int MIN_LENGTH = -50;

	const float BIGMETEO_SPEED = 200.0f;

	const float Y_DOWN = -150.0f;

	const float GROUND_CHECK_Y_UP = 20.0f;
	const float GROUND_CHECK_Y_DOWN = -20.0f;

	const float BIGMETEO_GROUND_CHECK_Y_UP = 90.0f;
	const float BIGMETEO_GROUND_CHECK_Y_DOWN = 120.0f;
}

DarkMeteorite::DarkMeteorite()
{
}

DarkMeteorite::~DarkMeteorite()
{
	DeleteGO(m_collision);

	if (m_meteos.size() != 0)
	{
		QueryGOs<Meteo>("meteo", [&](Meteo* meteo_) {
			DeleteGO(meteo_);
			return true;
			});
	}
}

//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�n�ʗp)
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

//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
struct IsWallResult :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//�Փ˃t���O�B
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//�n�ʂƂԂ����ĂȂ�������B
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall) {
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


bool DarkMeteorite::Start()
{
	m_model.Init("Assets/modelData/character/Lich/Effect/DarkBall.tkm", nullptr, 0, enModelUpAxisZ, false, false, false);


	m_model.SetTransform(m_position, m_rotation, g_vec3One);
	m_model.Update();

	m_player = FindGO<Player>("player");

	return true;
}

void DarkMeteorite::Update()
{

	ShotManageState();

	


	m_model.Update();
}

void DarkMeteorite::SizeUp()
{
	//���̃T�C�Y�ɂȂ����珈�������Ȃ�
	if (m_sizeUpFlag != false)
	{
		return;
	}

	m_scale *= 1.0f + g_gameTime->GetFrameDeltaTime() * 1.5f;
	m_model.SetScale(m_scale);
	//���̃T�C�Y�ɂȂ����珈�������Ȃ�
	if (m_maxScale.x <= m_scale.x)
	{
		m_sizeUpFlag = true;
	}
}

void DarkMeteorite::Shot()
{
	//�V���b�g�t���O�������Ă��Ȃ��Ȃ珈�����Ȃ�
	if (m_ShotStartFlag != true)
	{
		return;
	}

	//�^�C�}�[���������ԂɒB���Ă��Ȃ��Ȃ甲���o��
	if (CreateTimer() != true)
	{
		return;
	}

	//�������鐔
	if (m_meteoCounter < m_createMeteoCount)
	{
		//
		while (true)
		{
			//���e�I���Փ˂�����W
			Vector3 HitPosition = g_vec3Zero;
			HitPosition = SetMeteoTargetPosition();
			//�Փ˂�����W���n�ʂȂ�
			if (IsHitGround(HitPosition, GROUND_CHECK_Y_UP, GROUND_CHECK_Y_DOWN) == true)
			{
				//���e�I�𐶐�����
				CreateMeteo(HitPosition);
				//�J�E���^�[�𑝂₷
				m_meteoCounter++;
				break;
			}
		}	
	}
	//�S�Ẵ��e�I�𐶐�������
	else
	{
		//�Ō�ɑ傫�ȃ��e�I�����t���O�������Ă�����
		if (m_lastBigMeteoShotFlag == true)
		{
			//�r�b�O���e�I�����Ƃ��̃^�[�Q�b�g�����߂�
			m_targetPosition = m_player->GetPosition();
			m_moveSpeed = m_targetPosition - m_position;
			//���K��
			m_moveSpeed.Normalize();
			m_moveSpeed *= BIGMETEO_SPEED;
			//�X�e�[�g��ς���
			m_enShotState = enShotState_BigMeteo;
			//�r�b�O���e�I�̓����蔻�萶��
			CreateCollision();
		}
		else
		{
			//����ȊO�Ȃ�_�[�N���e�I�I���
			m_shotEndFlag = true;

		}
	}


	
}

Vector3 DarkMeteorite::SetMeteoTargetPosition()
{
	//�@�������������B
	srand((unsigned)time(NULL));

	//�^�[�Q�b�g�����߂ă��e�I�𐶐�
	//���݂̃t���[���̃^�[�Q�b�g�̍��W���擾
	m_targetPosition = m_player->GetPosition();

	Vector3 createPositon = m_targetPosition;
	/*float X = rand() % 601 - 300;
	float Z = rand() % 601 - 300;*/
	float X = (rand() % (MAX_LENGTH - (MIN_LENGTH) + 1)) + (MIN_LENGTH);
	float Z = (rand() % (MAX_LENGTH - (MIN_LENGTH) + 1)) + (MIN_LENGTH);
	createPositon.x += X;
	createPositon.z += Z;

	return createPositon;
}

void DarkMeteorite::CreateCollision()
{
	m_collision = NewGO<CollisionObject>(0, "bigmeteo");
	m_collision->CreateSphere(
		m_position,
		g_quatIdentity,
		100.0f
	);
	m_collision->SetIsEnableAutoDelete(false);
	m_collision->SetPosition(m_position);
	m_collision->Update();
}

void DarkMeteorite::CreateMeteo(Vector3 targetposition)
{
	Vector3 createPos = m_position;
	createPos.y += 70.0f;
	Meteo* meteo = NewGO<Meteo>(0, "meteo");
	meteo->SetPosition(createPos);
	meteo->SetRotation(m_rotation);
	meteo->SetTargetPosition(targetposition);
	//���X�g�ɒǉ��B
	m_meteos.emplace_back(meteo);
}

bool DarkMeteorite::CreateTimer()
{
	if (m_createTime < m_createTimer)
	{
		m_createTimer = 0.0f;
		return true;
	}
	else
	{
		m_createTimer += g_gameTime->GetFrameDeltaTime();
		return false;
	}
}

bool DarkMeteorite::IsHitGround(Vector3 targetposition,float up, float down)
{
	BoxCollider m_boxCollider; 
	m_boxCollider.Create(Vector3(5.0f, 5.0f, 5.0f));
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//�n�_�̓G�l�~�[�̍��W�B
	start.setOrigin(btVector3(targetposition.x, targetposition.y + up, targetposition.z));
	//�I�_�̓v���C���[�̍��W�B
	end.setOrigin(btVector3(targetposition.x, targetposition.y + down, targetposition.z));
	//�ǂ̔����Ԃ�
	IsGroundResult callback_Ground;
	//�R���C�_�[���n�_����I�_�܂œ������āB
	//�ǂƏՓ˂��邩�ǂ����𒲂ׂ�B
	PhysicsWorld::GetInstance()->ConvexSweepTest(
		(const btConvexShape*)m_boxCollider.GetBody(), 
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

bool DarkMeteorite::IsHitWall(Vector3 pos1, Vector3 pos2)
{
	BoxCollider m_boxCollider;
	m_boxCollider.Create(Vector3(10.0f, 10.0f, 10.0f));

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//�n�_�̓G�l�~�[�̍��W�B
	start.setOrigin(btVector3(pos1.x, pos1.y, pos1.z));
	//�I�_�̓v���C���[�̍��W�B
	end.setOrigin(btVector3(pos2.x, pos2.y, pos2.z));

	//�ǂ̔����Ԃ�
	IsGroundResult callback_Ground;
	//�R���C�_�[���n�_����I�_�܂œ������āB
	//�ǂƏՓ˂��邩�ǂ����𒲂ׂ�B
	PhysicsWorld::GetInstance()->ConvexSweepTest(
		(const btConvexShape*)m_boxCollider.GetBody(),
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

void DarkMeteorite::ShotManageState()
{
	switch (m_enShotState)
	{
	case DarkMeteorite::enShotState_Meteo:
		//���̃T�C�Y�ɂȂ����珈�������Ȃ�
		SizeUp();
		//���e�I��������
		Shot();
		break;
	case DarkMeteorite::enShotState_BigMeteo:

		move();
		break;
	default:
		break;
	}
}

void DarkMeteorite::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}

void DarkMeteorite::move()
{
	if (m_bigMeteoMoveCount > 0)
	{
		m_targetPosition = m_player->GetPosition();
		m_moveSpeed = m_targetPosition - m_position;
		//���K��
		m_moveSpeed.Normalize();
		m_moveSpeed *= BIGMETEO_SPEED;
		//�J�E���g�����炷
		m_bigMeteoMoveCount-=g_gameTime->GetFrameDeltaTime();
	}
	

	if (m_isBigMeteoYDownFlag == false)
	{
		//�n�ʂɂ��Ă��Ȃ��Ԏ΂߉��Ɉړ�
		if (IsHitGround(m_position, BIGMETEO_GROUND_CHECK_Y_UP, BIGMETEO_GROUND_CHECK_Y_DOWN) != true)
		{
			m_moveSpeed.y = Y_DOWN;
		}
		else
		{
			m_isBigMeteoYDownFlag = true;
			m_moveSpeed.y = 0.0f;
		}
	}
	else
	{
		Vector3 addPos = m_moveSpeed;
		addPos.Normalize();
		Vector3 pos2 = m_position + (addPos * 0.65f);
		//�ǂƂ̏Փ˔���
		if (IsHitWall(m_position, pos2) == true)
		{
			//����
			//�����I���
			m_shotEndFlag = true;
			m_enShotState = enShotState_End;
		}
	}
	
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime()*1.5f;

	m_model.SetPosition(m_position);

	m_collision->SetPosition(m_position);
	m_collision->Update();
}
