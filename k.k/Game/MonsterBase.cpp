#include "stdafx.h"
#include "MonsterBase.h"

MonsterBase::MonsterBase()
{
}

MonsterBase::~MonsterBase()
{
}

Vector3 MonsterBase::CalcVelocity(Status status, Vector3 targetposition, bool dashFlag)
{
	Vector3 moveSpeed = Vector3::Zero;
	//自身からターゲットに向かうベクトルを計算する
	Vector3 diff = targetposition - m_position;
	diff.y = 0.0f;
	//正規化
	diff.Normalize();
	//ダッシュフラグがtrueなら
	if (dashFlag == true)
	{
		//速度を設定
		moveSpeed = diff * (status.defaultSpeed * 1.5f);
	}
	else
	{
		//速度を設定
		moveSpeed = diff * status.defaultSpeed;
	}
	//飛ばないようにする
	moveSpeed.y = 0.0f;
	//前方向を設定
	m_forward = diff;
	//値をセーブしておく
	m_SaveMoveSpeed = moveSpeed;
	return moveSpeed;
}

bool MonsterBase::IsInFieldOfView(Vector3 toPlayerDir, Vector3 forward, float angle)
{
	//ベクトル正規化
	toPlayerDir.Normalize();
	//ターゲットに向かうベクトルと前方向の内積を計算する
	float t = toPlayerDir.Dot(forward);
	//内積の結果をacos関数に渡して、m_enemyFowradとtoPlayerDirのなす角度を求める。
	float calcangle = acos(t);
	//視野角判定。指定した角度の範囲内の値か
	if (fabsf(calcangle) < Math::DegToRad(angle))
	{
		//範囲内なら
		return true;
	}
	//範囲外なら
	return false;
}

bool MonsterBase::IsFindPlayer(float distance)
{
	//ターゲット(プレイヤー)の座標を取得
	m_targetPosition = m_player->GetPosition();
	//自身からターゲットに向かうベクトルを計算する
	Vector3 diff = m_targetPosition - m_position;
	diff.y = 0.0f;
	//ベクトルをセーブ
	m_toTarget = diff;
	//計算したベクトルが上限の距離より小さかったら
	if (diff.Length() < distance)
	{
		//見つけた
		return true;
	}
	else
	{
		//見つからなかった
		m_targetPosition = Vector3::Zero;
		return false;
	}
}

bool MonsterBase::AttackInterval(const float attackintarvaltime)
{
	//攻撃したら
	if (m_attackFlag == true)
	{
		//タイマーがインターバルを超えたら
		if (attackintarvaltime < m_attackIntervalTimer)
		{
			//攻撃可能にする
			m_attackFlag = false;
			//タイマーをリセット
			m_attackIntervalTimer = 0.0f;
		}
		else
		{
			m_attackIntervalTimer += g_gameTime->GetFrameDeltaTime();
			//攻撃不可能
			return false;
		}
	}
	//攻撃可能
	return true;
}

bool MonsterBase::AngleChangeTimeIntarval(float LimitTime)
{
	return false;
}

void MonsterBase::CreateDamageFont(int damage)
{
}

bool MonsterBase::IsCollisionDetection()
{
	//特定のアニメーションが再生中なら
	if (isAnimationEntable() != true)
	{
		//判定を取らない
		return false;
	}
	//判定をとる
	return true;
}

Quaternion MonsterBase::Rotation(float rotSpeed, float rotOnlySpeed)
{
	if (RotationOnly() == true)
	{
		//xかzの移動速度があったら(スティックの入力があったら)。
		if (fabsf(m_SaveMoveSpeed.x) >= 0.001f || fabsf(m_SaveMoveSpeed.z) >= 0.001f)
		{
			m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * rotOnlySpeed, m_rotMove, m_SaveMoveSpeed);
			m_rotation.SetRotationYFromDirectionXZ(m_rotMove);
		}
		//前方向を設定
		/*m_forward = Vector3::AxisZ;
		m_rotation.Apply(m_forward);*/
		m_forward = m_rotMove;
		m_forward.Normalize();
		return m_rotation;
	}

	//xかzの移動速度があったら
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//緩やかに回転させる
		m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * rotSpeed, m_rotMove, m_moveSpeed);
		m_rotation.SetRotationYFromDirectionXZ(m_rotMove);
	}
	//前方向を設定
	/*m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);*/
	m_forward = m_rotMove;
	m_forward.Normalize();
	return m_rotation;
}
