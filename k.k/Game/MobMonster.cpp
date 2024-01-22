#include "stdafx.h"
#include "MobMonster.h"
#include "InitEffect.h"
#include "CharactersInfoManager.h"
#include "Boss.h"

//todo　新しい弾き処理

namespace {
	const float DEAD_EFFECT_SIZE = 2.0f;
	const float HIT_EFFECT_SIZE = 15.0f;

	const float FIND_DISTANCE = 200.0f;

	const float KNOCKBACK_SIFFNESS_TIMER_LIMMIT = 0.2f;

	const int MONSTER_NEAR_PLAYER_COUNT_LIMMIT = 2;

	const float MOB_MONSTER_DISTANCE = 40.0f;
	const float BOSS_DISTANCE = 150.0f;
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

void MobMonster::MoveMonster(CharacterController& charaCon)
{
	//移動速度の計算
	m_moveSpeed = CalcVelocity(m_status, m_direction, m_chasePlayerFlag);
	//はじきパワーを小さくする
	//SubPassPower();
	//はじく力を合わせる
	//m_moveSpeed += m_passPower;
	
}

void MobMonster::MovePatrol(CharacterController& charaCon)
{
	//ランダムな方向に移動
	m_chasePlayerFlag = false;
	//数秒間隔で向かうベクトルを変える
	if (m_angleChangeTimeFlag == false)
	{
		m_direction = SetRamdomDirection(m_angleRange);
		//速度を掛ける
		m_direction *= m_status.GetDefaultSpeed();
		//アングルを切り替えたのでフラグをセット
		m_angleChangeTimeFlag = true;
	}

	//壁にぶつかったら反転
	if (IsBumpedForest(m_pos2Length) == true)
	{
		//移動する方向を反転する
		m_direction *= -1.0f;
		return;
	}

	//弾き処理
	if (IsBumpedMonster() == true)
	{
		//反転
		m_direction *= -1.0f;
	}

	//決めた方向に座標を移動
	m_position = charaCon.Execute(m_direction, 1.0f / 60.0f);

	m_moveSpeed = m_direction;
	m_SaveMoveSpeed = m_moveSpeed;
	//前方向の設定
	m_forward = m_direction;
	m_forward.Normalize();
}

void MobMonster::MoveChasePlayer(CharacterController& charaCon)
{
	Vector3 toPlayerDir = m_toTarget;

	m_direction = m_targetPosition;

	//移動速度の計算
	MoveMonster(charaCon);

	//プレイヤーを追いかけられないなら本来より遠い所で止まる
	if (GetEnableChasePlayerFag() == false)
	{
		//本来のm_stayRangeより遠くで止まる
		if (IsFindPlayer(250.0f) == true)
		{
			//範囲内にいるので移動しない
			m_moveSpeed = Vector3::Zero;
		}
	}
	//プレイヤーとの距離が近く、止まる距離なら45
	else if (IsFindPlayer(m_stayRange) == true)
	{
		//範囲内にいるので移動しない
		m_moveSpeed = Vector3::Zero;
	}

	//プレイヤーの周りに既にモンスターがたくさんいたら
	//if (CharactersInfoManager::GetInstance()->GetMonstersNearPlayerCount() >
	//	MONSTER_NEAR_PLAYER_COUNT_LIMMIT)
	//{
	//	//本来のm_stayRangeより遠くで止まる
	//	if (IsFindPlayer(250.0f) == true)
	//	{
	//		//範囲内にいるので移動しない
	//		m_moveSpeed = Vector3::Zero;
	//	}
	//}
	////プレイヤーとの距離が近く、止まる距離なら45
	//else if (IsFindPlayer(m_stayRange) == true)
	//{
	//	//範囲内にいるので移動しない
	//	m_moveSpeed = Vector3::Zero;
	//}


	SetEnableChasePlayerFag(false);

	//座標を移動
	m_position = charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
}

void MobMonster::ProcessKnockBack(CharacterController& charaCon)
{
	//ノックバック中なら
	if (GetKnockBackFlag() == true)
	{
		//ノックバックの処理をするなら
		if (IsKnockingBack(
			m_moveSpeed, m_knockBackTimer) == true)
		{
			//座標を移動
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
	//ランダムな方向の設定
	Vector3 randomPos = g_vec3Zero;

	while (true)
	{
		randomPos.y = 0.0f;
		float X = (rand() % (range - (-range) + 1)) + (-range);
		float Z = (rand() % (range - (-range) + 1)) + (-range);
		randomPos.x += X;
		randomPos.z += Z;
		//正規化
		randomPos.Normalize();

		//0ループフラグがセットされているなら
		if (zeroLoopFlag == true)
		{
			//移動方向が0(g_vec3Zero)の時
			if (randomPos.Length() <= 0.0f)
			{
				//やり直し
				continue;
			}
		}
		//フラグがセットされていないならループから抜ける
		break;
	}

	//移動方向を返す
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
	Vector3 diff = g_vec3Zero;

	////フィールド上の敵のリストを取得
	//for (auto monster : CharactersInfoManager::GetInstance()->GetMobMonsters())
	//{
	//	//リストの中身が自身と同じなら処理しない
	//	if (this == monster)
	//	{
	//		continue;
	//	}

	//	//自身の座標から他のモンスターに向かうベクトルを計算
	//	diff = monster->GetPosition() - m_position;
	//	//ベクトルの長さが一定値以下なら
	//	if (diff.Length() < MOB_MONSTER_DISTANCE)
	//	{
	//		return true;
	//	}
	//}
	////ボスに向かうベクトルを計算
	//diff = CharactersInfoManager::GetInstance()->GetBossInstance()->GetPosition()
	//	- m_position;
	////ベクトルの長さが一定値以下なら
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
	//効果音フラグが立っているなら、効果音の再生
	if (seFlag)
	{
		g_soundManager->InitAndPlaySoundSource(enSoundName_Mob_Die, g_soundManager->GetSEVolume());
	}

	//死亡時エフェクトの再生
	/*EffectEmitter* deadEffect = NewGO<EffectEmitter>(0);
	deadEffect->Init(InitEffect::enEffect_Mob_Dead);
	deadEffect->Play();
	deadEffect->SetPosition(m_position);
	deadEffect->SetScale(g_vec3One * DEAD_EFFECT_SIZE);
	deadEffect->Update();*/
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

		m_forward.Normalize();
		//視野角内にプレイヤーがいるなら
		//前方向のせいで被ダメ後に追いかけない
		if (IsInFieldOfView(m_toTarget, m_forward, m_angle) == true)
		{
			return true;
		}
	}
	

	return false;
}

bool MobMonster::IsPlayerInAttackRange()
{
	//攻撃できない状態でも範囲外とする
	/*if (IsProcessAttackEnable() != true)
	{
		return false;
	}*/

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
	/*if (CharactersInfoManager::GetInstance()->SearchMonsterNearPlayer(this) == true)
	{
		return false;
	}*/

	//if (CharactersInfoManager::GetInstance()->GetMonstersNearPlayerCount() >=
	//	MONSTER_NEAR_PLAYER_COUNT_LIMMIT)
	//{
	//	//プレイヤーとの距離が近く、止まる距離なら
	//	if (IsFindPlayer(120.0f) == true)
	//	{
	//		//
	//		return false;
	//	}
	//}

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

	//制限時間に達したら
	if (m_skillUsableTimer > m_skillUsableLimmit)
	{
		//スキル使用可能にする
		m_skillUsableTimer = 0.0f;
		m_skillUsableFlag = true;
	}
	else
	{
		//タイマーを加算
		m_skillUsableTimer += g_gameTime->GetFrameDeltaTime();
	}
}


