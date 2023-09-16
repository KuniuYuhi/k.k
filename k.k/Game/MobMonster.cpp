#include "stdafx.h"
#include "MobMonster.h"
#include "InitEffect.h"

namespace {
	const float DEAD_EFFECT_SIZE = 2.0f;
	const float HIT_EFFECT_SIZE = 15.0f;

	const float FIND_DISTANCE = 200.0f;
}

//衝突したときに呼ばれる関数オブジェクト(壁用)
struct IsForestResult :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//衝突フラグ。
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//地面とぶつかってなかったら。
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall) {
			//衝突したのは壁ではない。
			isHit = false;
			return 0.0f;
		}
		else
		{
			//地面とぶつかったら。
		//フラグをtrueに。
			isHit = true;
			return 0.0f;
		}
	}
};

//衝突したときに呼ばれる関数オブジェクト(壁用)
//struct IsMonsterResult :public btCollisionWorld::ConvexResultCallback
//{
//	bool isHit = false;						//衝突フラグ。
//	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
//	{
//		//地面とぶつかってなかったら。
//		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Monster) {
//			//衝突したのは壁ではない。
//			isHit = false;
//			return 0.0f;
//		}
//		else
//		{
//			//地面とぶつかったら。
//		//フラグをtrueに。
//			isHit = true;
//			return 0.0f;
//		}
//	}
//};

bool MobMonster::RotationOnly()
{
	if (isRotationEntable() != true)
	{
		//xかzの移動速度があったら(スティックの入力があったら)。
		if (fabsf(m_SaveMoveSpeed.x) >= 0.001f || fabsf(m_SaveMoveSpeed.z) >= 0.001f)
		{
			m_rotation.SetRotationYFromDirectionXZ(m_SaveMoveSpeed);
			return true;
		}
	}
	return false;
}

void MobMonster::Move(CharacterController& charaCon)
{
	if (isAnimationEntable() != true)
	{
		return;
	}
	//攻撃中は処理しない
	if (IsAttackEntable() != true)
	{
		return;
	}

	//視界にターゲットを見つけたら
	if (IsFindPlayer(m_distanceToPlayer) == true)
	{
		Vector3 toPlayerDir = m_toTarget;
		//視野角内にターゲットがいたら
		if (IsInFieldOfView(toPlayerDir, m_forward, m_angle) == true)
		{
			toPlayerDir.Normalize();
			//追いかける
			m_direction = toPlayerDir;
			//m_moveSpeed = CalcVelocity(m_status, m_direction);
			m_moveSpeed = m_direction * m_status.defaultSpeed;
			m_SaveMoveSpeed = m_moveSpeed;
		}
		else
		{
			//視野角内にはいないが攻撃可能距離にいるなら
			if (IsFindPlayer(FIND_DISTANCE) == true)
			{
				m_moveSpeed = CalcVelocity(m_status, m_targetPosition);
				m_SaveMoveSpeed = m_moveSpeed;
			}
		}
	}
	else
	{
		//数秒間隔で向かうベクトルを変える
		if (m_angleChangeTimeFlag == false)
		{
			m_direction = SetDirection(m_angleRange);
			m_angleChangeTimeFlag = true;
		}
		//ランダムな方向に移動
		m_moveSpeed = m_direction * m_status.defaultSpeed;
		m_SaveMoveSpeed = m_moveSpeed;
	}

	//壁にぶつかったら反転
	if (IsBumpedForest(m_pos2Length) == true)
	{
		m_direction *= -1.0f;
		m_moveSpeed = m_direction * m_status.defaultSpeed;
		m_SaveMoveSpeed = m_moveSpeed;
		m_position = charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
		return;
	}


	//プレイヤーとの距離が近くないなら移動する
	if (IsFindPlayer(m_stayRange) != true)
	{
		//弾き処理
		//モンスターが近くにいるなら
		if (IsBumpedMonster() == true)
		{
			Pass();
		}

		m_position = charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	}
	else
	{
		//範囲内にいるので移動しない
		m_moveSpeed = Vector3::Zero;
	}
}

Vector3 MobMonster::SetDirection(int range)
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
	Vector3 pos1 = m_position;
	Vector3 pos2 = m_position;
	pos1.Normalize();
	pos2 += pos1 * pos2Length;
	SphereCollider m_sphereCollider;
	m_sphereCollider.Create(1.0f);

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//始点はエネミーの座標。
	start.setOrigin(btVector3(m_position.x, m_position.y, m_position.z));
	//終点はプレイヤーの座標。
	end.setOrigin(btVector3(
		pos2.x, pos2.y, pos2.z));

	//壁の判定を返す
	IsForestResult callback_Forest;
	//コライダーを始点から終点まで動かして。
	//壁と衝突するかどうかを調べる。
	PhysicsWorld::GetInstance()->ConvexSweepTest(
		(const btConvexShape*)m_sphereCollider.GetBody(),
		start, end, callback_Forest);
	//森に衝突した
	if (callback_Forest.isHit == true)
	{
		return true;
	}
	else
	{
		//衝突しなかった
		return false;
	}
}

bool MobMonster::IsBumpedMonster()
{
	return false;
}

void MobMonster::Pass()
{
	m_moveSpeed += m_passPower;
	m_SaveMoveSpeed = m_moveSpeed;
}

void MobMonster::Dead()
{
	EffectEmitter* deadEffect = NewGO<EffectEmitter>(0);
	deadEffect->Init(InitEffect::enEffect_Mob_Dead);
	deadEffect->Play();
	deadEffect->SetPosition(m_position);
	deadEffect->SetScale(g_vec3One * DEAD_EFFECT_SIZE);
	deadEffect->Update();
}

void MobMonster::CreateHitEffect()
{
	EffectEmitter* hitEffect = NewGO<EffectEmitter>(0);
	hitEffect->Init(InitEffect::enEffect_Hit);
	hitEffect->Play();
	hitEffect->SetPosition(m_position);
	hitEffect->SetScale(g_vec3One * HIT_EFFECT_SIZE);
	hitEffect->Update();
}
