#include "stdafx.h"
#include "MobMonster.h"
#include "InitEffect.h"
#include "Lich.h"

//todo�@�V�����e������

namespace {
	const float DEAD_EFFECT_SIZE = 2.0f;
	const float HIT_EFFECT_SIZE = 15.0f;

	const float FIND_DISTANCE = 200.0f;
}

//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
struct IsForestResult :public btCollisionWorld::ConvexResultCallback
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

bool MobMonster::RotationOnly()
{
	if (isRotationEntable() != true)
	{
		return true;
	}
	return false;
}

void MobMonster::Move(CharacterController& charaCon)
{
	if (isAnimationEntable() != true)
	{
		return;
	}
	//�U�����͏������Ȃ�
	if (IsAttackEntable() != true)
	{
		return;
	}
	
	//���E�Ƀ^�[�Q�b�g����������
	if (IsFindPlayer(m_distanceToPlayer) == true)
	{
		Vector3 toPlayerDir = m_toTarget;
		//����p���Ƀ^�[�Q�b�g��������
		if (IsInFieldOfView(toPlayerDir, m_forward, m_angle) == true)
		{
			m_chasePlayerFlag = true;
			m_direction = m_targetPosition;
			//toPlayerDir.Normalize();
			////�ǂ�������
			//m_direction = toPlayerDir;
			/*m_moveSpeed = CalcVelocity(m_status, m_direction*(-1.0f),true);
			m_SaveMoveSpeed = m_moveSpeed;*/
		}
		else
		{
			//����p���ɂ͂��Ȃ����U���\�����ɂ���Ȃ�
			if (IsFindPlayer(FIND_DISTANCE) == true)
			{
				m_chasePlayerFlag = true;
				m_direction = m_targetPosition;
				/*m_moveSpeed = CalcVelocity(m_status, m_targetPosition,true);
				m_SaveMoveSpeed = m_moveSpeed;*/
			}
		}
	}
	else
	{
		//�����_���ȕ����Ɉړ�
		m_chasePlayerFlag = false;
		//���b�Ԋu�Ō������x�N�g����ς���
		if (m_angleChangeTimeFlag == false)
		{
			m_direction = SetRamdomDirection(m_angleRange);

			m_direction = m_position + (m_direction * 1000.0f);

			m_angleChangeTimeFlag = true;
		}
		
		/*m_moveSpeed = m_direction * m_status.defaultSpeed;
		m_SaveMoveSpeed = m_moveSpeed;*/
	}

	//�ǂɂԂ������甽�]
	if (IsBumpedForest(m_pos2Length) == true)
	{
		//�ړ���������𔽓]����
		m_direction *= -1.0f;
		/*m_moveSpeed = m_direction * m_status.defaultSpeed;
		m_SaveMoveSpeed = m_moveSpeed;

		m_position = charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);*/
		return;
	}


	//�v���C���[�Ƃ̋������߂��Ȃ��Ȃ�ړ�����
	if (IsFindPlayer(m_stayRange) != true)
	{
		//�ړ����x�̌v�Z
		m_moveSpeed = CalcVelocity(m_status, m_direction, m_chasePlayerFlag);
		m_SaveMoveSpeed = m_moveSpeed;
		//�e������
		//IsBumpedMonster();
		//�͂����p���[������������
		SubPassPower();
		//�͂����͂����킹��
		//m_moveSpeed += m_passPower;
		//���W���ړ�
		m_position = charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	}
	else
	{
		//�͈͓��ɂ���̂ňړ����Ȃ�
		m_moveSpeed = Vector3::Zero;
	}
}

Vector3 MobMonster::SetRamdomDirection(int range)
{
	Vector3 randomPos = g_vec3Zero;
	randomPos.y = 0.0f;
	float X = (rand() % (range - (-range) + 1)) + (-range);
	float Z = (rand() % (range - (-range) + 1)) + (-range);
	randomPos.x += X;
	randomPos.z += Z;
	randomPos.Normalize();

	return randomPos;
}

bool MobMonster::IsBumpedForest(float pos2Length)
{
	//todo
	Vector3 forward = m_forward;
	forward.Normalize();
	/*float t = forward.Dot(g_vec3Zero);

	Vector3 diff1 = m_position - forward;
	Vector3 diff2 = m_position - g_vec3Zero;
	diff1.Normalize();
	diff2.Normalize();*/

	/*if (diff1.Dot(diff2) >= 1.0f)
	{
		return false;
	}*/

	//���̏�Œ�~���Ă���̂ŏ������Ȃ�
	if (forward.x == 0.0f && forward.y == 0.0f && forward.z == 0.0f)
	{
		return false;
	}

	Vector3 endPos;
	endPos = m_direction;
	endPos.Normalize();
	endPos *= pos2Length;
	endPos += m_position;

	SphereCollider m_sphereCollider;
	m_sphereCollider.Create(1.0f);

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//�n�_�̓G�l�~�[�̍��W�B
	start.setOrigin(btVector3(m_position.x, 0.0f, m_position.z));
	//�I�_�̓v���C���[�̍��W�B
	end.setOrigin(btVector3(
		endPos.x, 0.0f, endPos.z));

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

bool MobMonster::IsBumpedMonster()
{
	//�t�B�[���h��̓G�̃��X�g���擾
	//std::vector<AIActor*> monsters = m_lich->GetMonsters();

	//for (auto monster : monsters)
	//{
	//	//���X�g�̃����X�^�[�̍��W���擾
	//	Vector3 mosterPos = monster->GetPosition();
	//	Vector3 diff = mosterPos - m_position;
	//	//
	//	if (diff.Length() <= 30.0f)
	//	{
	//		diff.Normalize();
	//		m_passPower += diff * 12.0f;
	//	}

	//}

	////���b�`�Ƃ̋������v�Z
	//Vector3 lichPos = m_lich->GetPosition();
	//Vector3 diff = lichPos - m_position;
	//if (diff.Length() <= 80.0f)
	//{
	//	Vector3 direction;
	//	//todo ���������W���������炷
	//	direction = SetRamdomDirection(m_angleRange);

	//	diff.Normalize();
	//	m_passPower += direction * 400.0f;
	//	m_passPower += diff * 30.0f;
	//}

	//m_passPower.y = 0.0f;
	return false;
}

void MobMonster::SubPassPower()
{
	if (m_passPower.Length() < 10.0f) {
		m_passPower *= 0.99f;
	}
	else {
		m_passPower = Vector3::Zero;
	}
}

void MobMonster::ProcessDead()
{
	g_soundManager->InitAndPlaySoundSource(enSoundName_Mob_Die, g_soundManager->GetSEVolume());

	EffectEmitter* deadEffect = NewGO<EffectEmitter>(0);
	deadEffect->Init(InitEffect::enEffect_Mob_Dead);
	deadEffect->Play();
	deadEffect->SetPosition(m_position);
	deadEffect->SetScale(g_vec3One * DEAD_EFFECT_SIZE);
	deadEffect->Update();
}

//Quaternion MobMonster::Rotation(float rotSpeed, float rotOnlySpeed)
//{
//	if (RotationOnly() == true)
//	{
//		//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
//		if (fabsf(m_SaveMoveSpeed.x) >= 0.001f || fabsf(m_SaveMoveSpeed.z) >= 0.001f)
//		{
//			m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * rotOnlySpeed, m_rotMove, m_SaveMoveSpeed);
//			m_rotation.SetRotationYFromDirectionXZ(m_rotMove);
//
//			//�O������ݒ�
//			//m_forward = m_rotMove;
//			
//		}
//		//�O������ݒ�
//		m_forward = m_rotMove;
//		m_forward.Normalize();
//		//m_rotation.Apply(m_forward);
//
//		//Vector3 diff = m_direction-
//
//		return m_rotation;
//	}
//
//	//x��z�̈ړ����x����������
//	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
//	{
//		//�ɂ₩�ɉ�]������
//		m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * rotSpeed, m_rotMove, m_moveSpeed);
//		m_rotation.SetRotationYFromDirectionXZ(m_rotMove);
//
//		
//		//m_forward.Normalize();
//	}
//	//�O������ݒ�
//	m_forward = m_rotMove;
//	m_forward.Normalize();
//	/*m_forward = Vector3::AxisZ;
//	m_rotation.Apply(m_forward);*/
//
//	return m_rotation;
//}

void MobMonster::CreateHitEffect()
{
	//todo exe���Ɖ��̂��摜���Ȃ��Ȃ�
	EffectEmitter* hitEffect = NewGO<EffectEmitter>(0);
	hitEffect->Init(InitEffect::enEffect_Hit);
	hitEffect->Play();
	hitEffect->SetPosition(m_position);
	hitEffect->SetScale(g_vec3One * HIT_EFFECT_SIZE);
	hitEffect->Update();
}


