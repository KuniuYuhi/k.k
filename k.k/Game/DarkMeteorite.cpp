#include "stdafx.h"
#include "DarkMeteorite.h"
#include "Meteo.h"
#include "Player.h"
#include "Lich.h"
#include "InitEffect.h"

namespace {
	const int MAX_LENGTH = 10;
	const int MIN_LENGTH = -10;

	const float BIGMETEO_SPEED = 200.0f;

	const float Y_DOWN = -200.0f;

	const float GROUND_CHECK_Y_UP = 20.0f;
	const float GROUND_CHECK_Y_DOWN = -20.0f;

	const float BIGMETEO_GROUND_CHECK_Y_UP = -100.0f;
	const float BIGMETEO_GROUND_CHECK_Y_DOWN = -130.0f;

	const float WIND_SCALE = 30.0f;

	const float DARKMETEO_SHOT_TIMER = 5.0f;

	const float NEAR_LENGTH = 150.0f;
}

DarkMeteorite::DarkMeteorite()
{

	
}

DarkMeteorite::~DarkMeteorite()
{
	DeleteGO(m_collision);

	if (m_darkMeteoriteEffect != nullptr)
	{
		m_darkMeteoriteEffect->Stop();
	}
	if (m_windEffect != nullptr)
	{
		m_windEffect->Stop();
	}

	if (m_gameEndFlag == true)
	{
		if (m_meteos.size() != 0)
		{
			QueryGOs<Meteo>("meteo", [&](Meteo* meteo_) {
				DeleteGO(meteo_);
				return true;
				});
		}
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
	m_model.Init(
		"Assets/modelData/character/Lich/Effect/Meteo.tkm", 
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		nullptr, 
		0, enModelUpAxisZ, 
		false, 
		false 
		);

	m_model.SetTransform(m_position, m_rotation, g_vec3One);
	m_model.Update();

	m_player = FindGO<Player>("player");

	//�r�b�O���e�I�̓����蔻�萶��
	CreateCollision();

	m_darkMeteoriteEffect = NewGO<EffectEmitter>(0);
	m_darkMeteoriteEffect->Init(InitEffect::enEffect_DarkMeteorite);
	m_darkMeteoriteEffect->Play();
	m_darkMeteoriteEffect->SetPosition(m_position);
	

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
	Vector3 effectScale = m_scale * 1.8f;
	m_darkMeteoriteEffect->SetScale(effectScale);
	m_darkMeteoriteEffect->Update();

	//���̃T�C�Y�ɂȂ����珈�������Ȃ�
	if (m_maxScale.x <= m_scale.x)
	{
		m_sizeUpFlag = true;
	}
}

void DarkMeteorite::ProcessShot()
{
	//�V���b�g�t���O�������Ă��Ȃ��Ȃ珈�����Ȃ�
	if (m_ShotStartFlag != true)
	{
		return;
	}

	//�^�C�}�[���������ԂɒB���Ă��Ȃ��Ȃ甲���o��
	if (CreateMeteoTimer() != true)
	{
		return;
	}

	//�������鐔
	if (m_meteoCounter < m_createMeteoCount)
	{
		while (true)
		{
			if (ShotMeteo() == true)
			{
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
			
		}
		else
		{
			//����ȊO�Ȃ�_�[�N���e�I�A�N�V�����̏I���
			m_shotEndFlag = true;
			//m_enShotState = enShotState_End;

		}
	}
}

bool DarkMeteorite::ShotMeteo()
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

		//�����Ō�̃��e�I�Ȃ�
		if (m_meteoCounter >= m_createMeteoCount && m_lastBigMeteoShotFlag == true)
		{
			//�_�[�N���e�I�𗎂Ƃ����̂��߂Ɏ��̃^�C�}�[�𒷂�����
			m_createTime = DARKMETEO_SHOT_TIMER;
		}
		return true;
	}
	return false;
}

Vector3 DarkMeteorite::SetMeteoTargetPosition()
{
	//�@�������������B
	srand((unsigned)time(NULL));

	//�^�[�Q�b�g�����߂ă��e�I�𐶐�
	//���݂̃t���[���̃^�[�Q�b�g�̍��W���擾
	m_targetPosition = m_player->GetPosition();
	/*Vector3 forward = m_player->GetForward();
	forward.Normalize();
	forward *= 100.0f;
	*/
	Vector3 createPositon = m_targetPosition;
	float X = (rand() % (MAX_LENGTH - (MIN_LENGTH) + 1)) + (MIN_LENGTH);
	float Z = (rand() % (MAX_LENGTH - (MIN_LENGTH) + 1)) + (MIN_LENGTH);
	createPositon.x += X;
	createPositon.z += Z;

	return createPositon;
}

void DarkMeteorite::CreateCollision()
{
	//�{�̂̓����蔻��̐���
	m_collision = NewGO<CollisionObject>(0, "bigmeteo");
	m_collision->CreateSphere(
		m_position,
		g_quatIdentity,
		120.0f
	);
	m_collision->SetIsEnableAutoDelete(false);
	m_collision->SetPosition(m_position);
	m_collision->Update();
}

void DarkMeteorite::CreateMeteo(Vector3 targetposition)
{
	//���e�I�̐����ƃ��X�g�Ɋi�[
	Vector3 createPos = m_position;
	createPos.y += 70.0f;
	Meteo* meteo = NewGO<Meteo>(0, "meteo");
	meteo->SetPosition(createPos);
	meteo->SetRotation(m_rotation);
	meteo->SetTargetPosition(targetposition);
	//���X�g�ɒǉ��B
	m_meteos.emplace_back(meteo);
	//���e�I�������Đ�
	g_soundManager->InitAndPlaySoundSource(enSoundName_Boss_Meteo_Shot, g_soundManager->GetSEVolume());
}

bool DarkMeteorite::CreateMeteoTimer()
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

bool DarkMeteorite::IsHitWall()
{
	Vector3 addPos = m_moveSpeed;
	addPos.Normalize();
	Vector3 pos2 = m_position;
	pos2.Add(addPos * 3.0f);

	BoxCollider m_boxCollider;
	m_boxCollider.Create(Vector3(1.0f, 1.0f, 1.0f));

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//�n�_�̓G�l�~�[�̍��W�B
	start.setOrigin(btVector3(m_position.x, 0.0f, m_position.z));
	//�I�_�̓v���C���[�̍��W�B
	end.setOrigin(btVector3(pos2.x, 0.0f, pos2.z));

	//�ǂ̔����Ԃ�
	IsWallResult isWallResult;
	//�R���C�_�[���n�_����I�_�܂œ������āB
	//�ǂƏՓ˂��邩�ǂ����𒲂ׂ�B
	PhysicsWorld::GetInstance()->ConvexSweepTest(
		(const btConvexShape*)m_boxCollider.GetBody(),
		start, end, isWallResult);
	//�ǂɏՓ˂���
	if (isWallResult.isHit == true)
	{
		return true;
	}
	else
	{
		//�ǂł͂Ȃ�����
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
		ProcessShot();
		break;
	case DarkMeteorite::enShotState_BigMeteo:
		DarkMeteoMoveManageState();
		break;
	default:
		DeleteGO(this);
		break;
	}
}

void DarkMeteorite::DarkMeteoMoveManageState()
{
	switch (m_darkMeteoMoveState)
	{
	case DarkMeteorite::enDarkMeteoMoveState_fall:
		OnProcessFallStateTransition();
		break;
	case DarkMeteorite::enDarkMeteoMoveState_Chase:
		OnProcessChaseStateTransition();
		break;
	case DarkMeteorite::enDarkMeteoMoveState_straight:
		OnProcessStraightStateTransition();
		break;
	default:
		break;
	}
}

void DarkMeteorite::OnProcessFallStateTransition()
{
	//�^�[�Q�b�g�̌������x�N�g�����v�Z
	SetToTargetDirection();
	//���K��
	m_moveSpeed.Normalize();
	//��]
	m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
	//���x��������
	m_moveSpeed *= BIGMETEO_SPEED;
	//�n�ʂɂ��Ă��Ȃ��Ȃ�
	if (IsHitGround(m_position, BIGMETEO_GROUND_CHECK_Y_UP, BIGMETEO_GROUND_CHECK_Y_DOWN) != true)
	{
		//�n�ʂɂ��܂ł̏���
		//�΂߉��Ɉړ�
		m_moveSpeed.y = Y_DOWN;
	}
	else
	{
		//�n�ʂɂ���
		//�S�Ă̋ʂ������I�����
		//m_shotEndFlag = true;
		m_moveSpeed.y = 0.0f;
		//���G�t�F�N�g����
		m_windEffect = NewGO<EffectEmitter>(0);
		m_windEffect->Init(InitEffect::enEffect_DarkMeteorite_Wind);
		m_windEffect->Play();
		m_windEffect->SetPosition(m_position);
		m_windEffect->SetScale(g_vec3One * WIND_SCALE);
		m_windEffect->Update();

		//���̃X�e�[�g�ɑJ��
		m_darkMeteoMoveState = enDarkMeteoMoveState_Chase;
	}

	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime() * 1.5f;
	m_model.SetPosition(m_position);
	m_collision->SetPosition(m_position);
	m_collision->Update();
	m_darkMeteoriteEffect->SetPosition(m_position);
	m_darkMeteoriteEffect->Update();
}

void DarkMeteorite::OnProcessChaseStateTransition()
{
	//�^�[�Q�b�g�̌������x�N�g�����v�Z
	SetToTargetDirection();
	//���K��
	m_moveSpeed.Normalize();
	//��]
	m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
	//���x��������
	m_moveSpeed *= BIGMETEO_SPEED;
	//�ǂƂ̏Փ˔���
	//todo ���܂ɏ����Ȃ��̂Ő������Ԃ����߂�
	m_moveSpeed.y = 0.0f;
	//�ǂɂԂ�����������
	if (IsHitWall() == true)
	{
		//�Ԃ������̂Ŕ���
		//�����I���
		m_shotEndFlag = true;
		//�_�[�N���e�I�̈ړ��X�e�[�g�̏��������Ȃ��悤�ɂ���
		m_darkMeteoMoveState = enDarkMeteoMoveState_None;
		//�V���b�g�X�e�[�g
		m_enShotState = enShotState_End;
		return;
	}

	//�v���C���[�Ƃ̋������߂������珈������߂�
	if (IsNearDistanceToPlayer() == true)
	{
		//���̃X�e�[�g�ɑJ��
		m_darkMeteoMoveState = enDarkMeteoMoveState_straight;
		return;
	}

	SetTRS();
}

void DarkMeteorite::OnProcessStraightStateTransition()
{
	//�ǂƂ̏Փ˔���
	//todo ���܂ɏ����Ȃ��̂Ő������Ԃ����߂�
	m_moveSpeed.y = 0.0f;
	//�ǂɂԂ�����������
	if (IsHitWall() == true)
	{
		//�Ԃ������̂Ŕ���
		//�����I���
		m_shotEndFlag = true;
		//�_�[�N���e�I�̈ړ��X�e�[�g�̏��������Ȃ��悤�ɂ���
		m_darkMeteoMoveState = enDarkMeteoMoveState_None;
		//�V���b�g�X�e�[�g
		m_enShotState = enShotState_End;
		return;
	}

	SetTRS();
}

void DarkMeteorite::SetToTargetDirection()
{
	//�^�[�Q�b�g�̌������x�N�g�����v�Z
	m_targetPosition = m_player->GetPosition();
	m_moveSpeed = m_targetPosition - m_position;
}

bool DarkMeteorite::IsNearDistanceToPlayer()
{
	//�v���C���[�ɓ����������̔���
	Vector3 diff = m_position - m_player->GetPosition();
	if (diff.Length() < NEAR_LENGTH)
	{
		return true;
	}
	return false;
}

void DarkMeteorite::SetTRS()
{
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime() * 1.5f;
	m_model.SetPosition(m_position);
	m_model.SetRotation(m_rotation);
	m_collision->SetPosition(m_position);
	m_collision->Update();
	m_darkMeteoriteEffect->SetPosition(m_position);
	m_darkMeteoriteEffect->Update();

	if (m_windEffect != nullptr)
	{
		Vector3 pos = m_position;
		pos.y = 0.0f;
		m_windEffect->SetPosition(pos);
		m_windEffect->SetRotation(m_rotation);
		m_windEffect->Update();
	}
}

void DarkMeteorite::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}
