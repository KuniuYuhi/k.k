#include "stdafx.h"
#include "MobMonster.h"
#include "InitEffect.h"

#include "MobMonsterSM_Patrol.h"
#include "MobMonsterSM_Chase.h"

//todo　新しい弾き処理

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

bool MobMonster::RotationOnly()
{
	if (isRotationEnable() != true)
	{
		return true;
	}
	return false;
}

void MobMonster::Move(CharacterController& charaCon)
{
	//chaseかpatrolのときだけ移動処理


	if (isAnimationEnable() != true)
	{
		return;
	}
	//攻撃中は処理しない
	if (IsAttackEnable() != true)
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
			m_chasePlayerFlag = true;
			m_direction = m_targetPosition;
			//toPlayerDir.Normalize();
			////追いかける
			//m_direction = toPlayerDir;
			/*m_moveSpeed = CalcVelocity(m_status, m_direction*(-1.0f),true);
			m_SaveMoveSpeed = m_moveSpeed;*/
		}
		else
		{
			//視野角内にはいないが攻撃可能距離にいるなら
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
		//ランダムな方向に移動
		m_chasePlayerFlag = false;
		//数秒間隔で向かうベクトルを変える
		if (m_angleChangeTimeFlag == false)
		{
			m_direction = SetRamdomDirection(m_angleRange);

			m_direction = m_position + (m_direction * 1000.0f);

			m_angleChangeTimeFlag = true;
		}
		
		/*m_moveSpeed = m_direction * m_status.defaultSpeed;
		m_SaveMoveSpeed = m_moveSpeed;*/
	}

	//壁にぶつかったら反転
	if (IsBumpedForest(m_pos2Length) == true)
	{
		//移動する方向を反転する
		m_direction *= -1.0f;
		return;
	}


	//プレイヤーとの距離が近くないなら移動する
	if (IsFindPlayer(m_stayRange) != true)
	{
		//移動速度の計算
		m_moveSpeed = CalcVelocity(m_status, m_direction, m_chasePlayerFlag);
		m_SaveMoveSpeed = m_moveSpeed;
		//弾き処理
		//IsBumpedMonster();
		//はじきパワーを小さくする
		SubPassPower();
		//はじく力を合わせる
		//m_moveSpeed += m_passPower;
		//座標を移動
		m_position = charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	}
	else
	{
		//範囲内にいるので移動しない
		m_moveSpeed = Vector3::Zero;
	}
}

void MobMonster::MovePatrol(CharacterController& charaCon)
{
	//ランダムな方向に移動
	m_chasePlayerFlag = false;
	//数秒間隔で向かうベクトルを変える
	if (m_angleChangeTimeFlag == false)
	{
		m_direction = SetRamdomDirection(m_angleRange);

		m_direction = m_position + (m_direction * 1000.0f);

		m_angleChangeTimeFlag = true;
	}

	//壁にぶつかったら反転
	if (IsBumpedForest(m_pos2Length) == true)
	{
		//移動する方向を反転する
		m_direction *= -1.0f;
		return;
	}

	//移動速度の計算
	m_moveSpeed = CalcVelocity(m_status, m_direction, m_chasePlayerFlag);
	m_SaveMoveSpeed = m_moveSpeed;
	//弾き処理
	//IsBumpedMonster();
	//はじきパワーを小さくする
	SubPassPower();
	//はじく力を合わせる
	//m_moveSpeed += m_passPower;
	//座標を移動
	m_position = charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
}

void MobMonster::MoveChasePlayer(CharacterController& charaCon)
{
	Vector3 toPlayerDir = m_toTarget;

	m_direction = m_targetPosition;

	//プレイヤーとの距離が近くないなら移動する
	if (IsFindPlayer(m_stayRange) != true)
	{
		//移動速度の計算
		m_moveSpeed = CalcVelocity(m_status, m_direction, m_chasePlayerFlag);
		m_SaveMoveSpeed = m_moveSpeed;
		//弾き処理
		//IsBumpedMonster();
		//はじきパワーを小さくする
		SubPassPower();
		//はじく力を合わせる
		//m_moveSpeed += m_passPower;
		//座標を移動
		m_position = charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	}
	else
	{
		//範囲内にいるので移動しない
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

	if ((randomPos.x == 0.0f) && (randomPos.z==0.0f))
	{
		int a = 0;
	}

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

	//その場で停止しているので処理しない
	if (forward.x == 0.0f && forward.y == 0.0f && forward.z == 0.0f)
	{
		return false;
	}
	//ここが0だとレイの長さが0になる
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
	//始点はエネミーの座標。
	start.setOrigin(btVector3(m_position.x, 0.0f, m_position.z));
	//終点はプレイヤーの座標。
	end.setOrigin(btVector3(
		endPos.x, 0.0f, endPos.z));

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
	//フィールド上の敵のリストを取得
	//std::vector<AIActor*> monsters = m_lich->GetMonsters();

	//for (auto monster : monsters)
	//{
	//	//リストのモンスターの座標を取得
	//	Vector3 mosterPos = monster->GetPosition();
	//	Vector3 diff = mosterPos - m_position;
	//	//
	//	if (diff.Length() <= 30.0f)
	//	{
	//		diff.Normalize();
	//		m_passPower += diff * 12.0f;
	//	}

	//}

	////リッチとの距離を計算
	//Vector3 lichPos = m_lich->GetPosition();
	//Vector3 diff = lichPos - m_position;
	//if (diff.Length() <= 80.0f)
	//{
	//	Vector3 direction;
	//	//todo 向かう座標を少しずらす
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

void MobMonster::ProcessDead(bool seFlag)
{
	//効果音フラグが立っているなら、効果音の再生
	if (seFlag)
	{
		g_soundManager->InitAndPlaySoundSource(enSoundName_Mob_Die, g_soundManager->GetSEVolume());
	}

	//死亡時エフェクトの再生
	EffectEmitter* deadEffect = NewGO<EffectEmitter>(0);
	deadEffect->Init(InitEffect::enEffect_Mob_Dead);
	deadEffect->Play();
	deadEffect->SetPosition(m_position);
	deadEffect->SetScale(g_vec3One * DEAD_EFFECT_SIZE);
	deadEffect->Update();
}

void MobMonster::CreateHitEffect()
{
	//todo exeだと何故か画像がなくなる
	EffectEmitter* hitEffect = NewGO<EffectEmitter>(0);
	hitEffect->Init(InitEffect::enEffect_Hit);
	hitEffect->Play();
	hitEffect->SetPosition(m_position);
	hitEffect->SetScale(g_vec3One * HIT_EFFECT_SIZE);
	hitEffect->Update();
}

bool MobMonster::IsFoundPlayerFlag()
{
	//まずプレイヤーとの距離が近いか
	if (IsFindPlayer(m_distanceToPlayer) == true)
	{
		//追いかけている時は視野角判定なし
		if (m_chasePlayerFlag == true)
		{
			return true;
		}

		//攻撃後で、まだ近くにプレイヤーがいるなら
		if (GetPlayerNearbyFlag() == true)
		{
			SetPlayerNearbyFlag(false);
			return true;
		}

		Vector3 toPlayerDir = m_toTarget;
		//視野角内にプレイヤーがいるなら
		if (IsInFieldOfView(toPlayerDir, m_forward, m_angle) == true)
		{
			return true;
		}
	}
	

	return false;
}

bool MobMonster::IsPlayerInAttackRange()
{
	//自身からプレイヤーに向かうベクトルの計算
	Vector3 diff = m_player->GetPosition() - m_position;

	//接近しながら攻撃するスキルもあるため
	//スキル用の判定をする
	if (m_skillUsableFlag == true)
	{
		//スキル攻撃可能な範囲内なら
		if (diff.Length() < m_skillAttackRange)
		{
			return true;
		}
	}
	//ベクトルが通常攻撃範囲内なら
	else if (diff.Length() < m_attackRange)
	{
		return true;
	}
	return false;

	
}

bool MobMonster::IsProcessAttackEnable()
{
	//アタックフラグがセットされていたら攻撃可能
	if (m_attackEnableFlag == true)
	{
		return true;
	}
	//攻撃不可能
	return false;
}

bool MobMonster::IsSkillUsable()
{
	//スキル攻撃可能フラグがセットされていたらスキル攻撃可能
	if (m_skillUsableFlag == true)
	{
		//スキル攻撃可能
		return true;
	}
	//スキル攻撃不可能
	return false;
}

void MobMonster::CalcSkillAttackIntarval()
{
	//スキル攻撃可能なら処理しない
	if (m_skillUsableFlag == true)
	{
		return;
	}

	if (m_skillUsableTimer > m_skillUsableLimmit)
	{
		m_skillUsableTimer = 0.0f;
		m_skillUsableFlag = true;
	}
	else
	{
		m_skillUsableTimer += g_gameTime->GetFrameDeltaTime();
	}
}


