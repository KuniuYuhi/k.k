#include "stdafx.h"
#include "MobMonster.h"
#include "InitEffect.h"
#include "CharactersInfoManager.h"
#include "Boss.h"

//todo�@�V�����e������

namespace {
	const float DEAD_EFFECT_SIZE = 2.0f;
	const float HIT_EFFECT_SIZE = 15.0f;

	const float FIND_DISTANCE = 200.0f;

	const float KNOCKBACK_SIFFNESS_TIMER_LIMMIT = 0.2f;

	const int MONSTER_NEAR_PLAYER_COUNT_LIMMIT = 2;

	const float MOB_MONSTER_DISTANCE = 40.0f;
	const float BOSS_DISTANCE = 150.0f;
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
	if (isRotationEnable() != true)
	{
		return true;
	}
	return false;
}

void MobMonster::MoveMonster(CharacterController& charaCon)
{
	//�ړ����x�̌v�Z
	m_moveSpeed = CalcVelocity(m_status, m_direction, m_chasePlayerFlag);
	//�͂����p���[������������
	//SubPassPower();
	//�͂����͂����킹��
	//m_moveSpeed += m_passPower;
	
}

void MobMonster::MovePatrol(CharacterController& charaCon)
{
	//�����_���ȕ����Ɉړ�
	m_chasePlayerFlag = false;
	//���b�Ԋu�Ō������x�N�g����ς���
	if (m_angleChangeTimeFlag == false)
	{
		m_direction = SetRamdomDirection(m_angleRange);
		//���x���|����
		m_direction *= m_status.GetDefaultSpeed();
		//�A���O����؂�ւ����̂Ńt���O���Z�b�g
		m_angleChangeTimeFlag = true;
	}

	//�ǂɂԂ������甽�]
	if (IsBumpedForest(m_pos2Length) == true)
	{
		//�ړ���������𔽓]����
		m_direction *= -1.0f;
		return;
	}

	//�e������
	if (IsBumpedMonster() == true)
	{
		//���]
		m_direction *= -1.0f;
	}

	//���߂������ɍ��W���ړ�
	m_position = charaCon.Execute(m_direction, 1.0f / 60.0f);

	m_moveSpeed = m_direction;
	m_SaveMoveSpeed = m_moveSpeed;
	//�O�����̐ݒ�
	m_forward = m_direction;
	m_forward.Normalize();
}

void MobMonster::MoveChasePlayer(CharacterController& charaCon)
{
	Vector3 toPlayerDir = m_toTarget;

	m_direction = m_targetPosition;

	//�ړ����x�̌v�Z
	MoveMonster(charaCon);

	//�v���C���[��ǂ��������Ȃ��Ȃ�{����艓�����Ŏ~�܂�
	if (GetEnableChasePlayerFag() == false)
	{
		//�{����m_stayRange��艓���Ŏ~�܂�
		if (IsFindPlayer(250.0f) == true)
		{
			//�͈͓��ɂ���̂ňړ����Ȃ�
			m_moveSpeed = Vector3::Zero;
		}
	}
	//�v���C���[�Ƃ̋������߂��A�~�܂鋗���Ȃ�45
	else if (IsFindPlayer(m_stayRange) == true)
	{
		//�͈͓��ɂ���̂ňړ����Ȃ�
		m_moveSpeed = Vector3::Zero;
	}

	//�v���C���[�̎���Ɋ��Ƀ����X�^�[���������񂢂���
	//if (CharactersInfoManager::GetInstance()->GetMonstersNearPlayerCount() >
	//	MONSTER_NEAR_PLAYER_COUNT_LIMMIT)
	//{
	//	//�{����m_stayRange��艓���Ŏ~�܂�
	//	if (IsFindPlayer(250.0f) == true)
	//	{
	//		//�͈͓��ɂ���̂ňړ����Ȃ�
	//		m_moveSpeed = Vector3::Zero;
	//	}
	//}
	////�v���C���[�Ƃ̋������߂��A�~�܂鋗���Ȃ�45
	//else if (IsFindPlayer(m_stayRange) == true)
	//{
	//	//�͈͓��ɂ���̂ňړ����Ȃ�
	//	m_moveSpeed = Vector3::Zero;
	//}


	SetEnableChasePlayerFag(false);

	//���W���ړ�
	m_position = charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
}

void MobMonster::ProcessKnockBack(CharacterController& charaCon)
{
	//�m�b�N�o�b�N���Ȃ�
	if (GetKnockBackFlag() == true)
	{
		//�m�b�N�o�b�N�̏���������Ȃ�
		if (IsKnockingBack(
			m_moveSpeed, m_knockBackTimer) == true)
		{
			//���W���ړ�
			m_position = charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
		}
		else
		{
			SetKnockBackFlag(false);
		}
	}
}

Vector3 MobMonster::SetRamdomDirection(int range, bool zeroLoopFlag)
{
	//�����_���ȕ����̐ݒ�
	Vector3 randomPos = g_vec3Zero;

	while (true)
	{
		randomPos.y = 0.0f;
		float X = (rand() % (range - (-range) + 1)) + (-range);
		float Z = (rand() % (range - (-range) + 1)) + (-range);
		randomPos.x += X;
		randomPos.z += Z;
		//���K��
		randomPos.Normalize();

		//0���[�v�t���O���Z�b�g����Ă���Ȃ�
		if (zeroLoopFlag == true)
		{
			//�ړ�������0(g_vec3Zero)�̎�
			if (randomPos.Length() <= 0.0f)
			{
				//��蒼��
				continue;
			}
		}
		//�t���O���Z�b�g����Ă��Ȃ��Ȃ烋�[�v���甲����
		break;
	}

	//�ړ�������Ԃ�
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

	if (m_direction.LengthSq() == 0.0f)
	{
		return false;
	}

	//���̏�Œ�~���Ă���̂ŏ������Ȃ�
	if (forward.x == 0.0f && forward.y == 0.0f && forward.z == 0.0f)
	{
		return false;
	}
	//������0���ƃ��C�̒�����0�ɂȂ�
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
	Vector3 diff = g_vec3Zero;

	////�t�B�[���h��̓G�̃��X�g���擾
	//for (auto monster : CharactersInfoManager::GetInstance()->GetMobMonsters())
	//{
	//	//���X�g�̒��g�����g�Ɠ����Ȃ珈�����Ȃ�
	//	if (this == monster)
	//	{
	//		continue;
	//	}

	//	//���g�̍��W���瑼�̃����X�^�[�Ɍ������x�N�g�����v�Z
	//	diff = monster->GetPosition() - m_position;
	//	//�x�N�g���̒��������l�ȉ��Ȃ�
	//	if (diff.Length() < MOB_MONSTER_DISTANCE)
	//	{
	//		return true;
	//	}
	//}
	////�{�X�Ɍ������x�N�g�����v�Z
	//diff = CharactersInfoManager::GetInstance()->GetBossInstance()->GetPosition()
	//	- m_position;
	////�x�N�g���̒��������l�ȉ��Ȃ�
	//if (diff.Length() < BOSS_DISTANCE)
	//{
	//	return true;
	//}

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

void MobMonster::ProcessDead(bool seFlag)
{
	//���ʉ��t���O�������Ă���Ȃ�A���ʉ��̍Đ�
	if (seFlag)
	{
		g_soundManager->InitAndPlaySoundSource(enSoundName_Mob_Die, g_soundManager->GetSEVolume());
	}

	//���S���G�t�F�N�g�̍Đ�
	/*EffectEmitter* deadEffect = NewGO<EffectEmitter>(0);
	deadEffect->Init(InitEffect::enEffect_Mob_Dead);
	deadEffect->Play();
	deadEffect->SetPosition(m_position);
	deadEffect->SetScale(g_vec3One * DEAD_EFFECT_SIZE);
	deadEffect->Update();*/
}

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

const bool& MobMonster::IsKnockBackStiffness()
{
	if (m_knockBackStiffnessTimer > KNOCKBACK_SIFFNESS_TIMER_LIMMIT)
	{
		m_knockBackStiffnessTimer = 0.0f;
		return false;
	}
	else
	{
		m_knockBackStiffnessTimer += g_gameTime->GetFrameDeltaTime();
		return true;
	}
}

bool MobMonster::IsFoundPlayerFlag()
{
	//�܂��v���C���[�Ƃ̋������߂���
	if (IsFindPlayer(m_distanceToPlayer) == true)
	{
		//�ǂ������Ă��鎞�͎���p����Ȃ�
		if (m_chasePlayerFlag == true)
		{
			return true;
		}

		//�U����ŁA�܂��߂��Ƀv���C���[������Ȃ�
		if (GetPlayerNearbyFlag() == true)
		{
			SetPlayerNearbyFlag(false);
			return true;
		}

		m_forward.Normalize();
		//����p���Ƀv���C���[������Ȃ�
		//�O�����̂����Ŕ�_����ɒǂ������Ȃ�
		if (IsInFieldOfView(m_toTarget, m_forward, m_angle) == true)
		{
			return true;
		}
	}
	

	return false;
}

bool MobMonster::IsPlayerInAttackRange()
{
	//�U���ł��Ȃ���Ԃł��͈͊O�Ƃ���
	/*if (IsProcessAttackEnable() != true)
	{
		return false;
	}*/

	//���g����v���C���[�Ɍ������x�N�g���̌v�Z
	Vector3 diff = m_player->GetPosition() - m_position;

	//�ڋ߂��Ȃ���U������X�L�������邽��
	//�X�L���p�̔��������
	if (m_skillUsableFlag == true)
	{
		//�X�L���U���\�Ȕ͈͓��Ȃ�
		if (diff.Length() < m_skillAttackRange)
		{
			return true;
		}
	}
	//�x�N�g�����ʏ�U���͈͓��Ȃ�
	else if (diff.Length() < m_attackRange)
	{
		return true;
	}


	return false;
}

bool MobMonster::IsProcessAttackEnable()
{
	/*if (CharactersInfoManager::GetInstance()->SearchMonsterNearPlayer(this) == true)
	{
		return false;
	}*/

	//if (CharactersInfoManager::GetInstance()->GetMonstersNearPlayerCount() >=
	//	MONSTER_NEAR_PLAYER_COUNT_LIMMIT)
	//{
	//	//�v���C���[�Ƃ̋������߂��A�~�܂鋗���Ȃ�
	//	if (IsFindPlayer(120.0f) == true)
	//	{
	//		//
	//		return false;
	//	}
	//}

	//�A�^�b�N�t���O���Z�b�g����Ă�����U���\
	if (m_attackEnableFlag == true)
	{
		return true;
	}
	//�U���s�\
	return false;
}

bool MobMonster::IsSkillUsable()
{
	//�X�L���U���\�t���O���Z�b�g����Ă�����X�L���U���\
	if (m_skillUsableFlag == true)
	{
		//�X�L���U���\
		return true;
	}
	//�X�L���U���s�\
	return false;
}

void MobMonster::CalcSkillAttackIntarval()
{
	//�X�L���U���\�Ȃ珈�����Ȃ�
	if (m_skillUsableFlag == true)
	{
		return;
	}

	//�������ԂɒB������
	if (m_skillUsableTimer > m_skillUsableLimmit)
	{
		//�X�L���g�p�\�ɂ���
		m_skillUsableTimer = 0.0f;
		m_skillUsableFlag = true;
	}
	else
	{
		//�^�C�}�[�����Z
		m_skillUsableTimer += g_gameTime->GetFrameDeltaTime();
	}
}


