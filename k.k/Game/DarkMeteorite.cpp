#include "stdafx.h"
#include "DarkMeteorite.h"
#include "Meteo.h"
#include "Player.h"

DarkMeteorite::DarkMeteorite()
{
}

DarkMeteorite::~DarkMeteorite()
{
}

//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
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

bool DarkMeteorite::Start()
{
	m_model.Init("Assets/modelData/character/Slime/slime.tkm");


	m_model.SetTransform(m_position, m_rotation, g_vec3One);
	m_model.Update();

	m_player = FindGO<Player>("player");

	return true;
}

void DarkMeteorite::Update()
{
	//���̃T�C�Y�ɂȂ����珈�������Ȃ�
	SizeUp();
	
	Shot();


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
	//�V���b�g�t���O����������
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
			if (IsHitGround(HitPosition) == true)
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
		m_shotEndFlag = true;
	}
	
}

Vector3 DarkMeteorite::SetMeteoTargetPosition()
{
	//�^�[�Q�b�g�����߂ă��e�I�𐶐�
	//���݂̃t���[���̃^�[�Q�b�g�̍��W���擾
	m_targetPosition = m_player->GetPosition();

	Vector3 createPositon = m_targetPosition;
	float X = rand() % 601 - 300;
	float Z = rand() % 601 - 300;
	createPositon.x += X;
	createPositon.z += Z;

	return createPositon;
}

void DarkMeteorite::CreateMeteo(Vector3 targetposition)
{
	Meteo* meteo = NewGO<Meteo>(0, "meteo");
	meteo->SetPosition(m_position);
	meteo->SetRotation(m_rotation);
	meteo->SetTargetPosition(targetposition);
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

bool DarkMeteorite::IsHitGround(Vector3 targetposition)
{
	BoxCollider m_boxCollider; 
	m_boxCollider.Create(Vector3(5.0f, 5.0f, 5.0f));

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//�n�_�̓G�l�~�[�̍��W�B
	start.setOrigin(btVector3(targetposition.x, targetposition.y + 20.0f, targetposition.z));
	//�I�_�̓v���C���[�̍��W�B
	end.setOrigin(btVector3(targetposition.x, targetposition.y - 20.0f, targetposition.z));

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

void DarkMeteorite::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}
