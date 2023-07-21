#include "stdafx.h"
#include "AIActor.h"

AIActor::AIActor()
{
	m_player = FindGO<Player>("player");
}

AIActor::~AIActor()
{
}

void AIActor::SetTransFormModel(ModelRender& modelRender)
{
	modelRender.SetTransform(m_position, m_rotation, m_scale);
}

void AIActor::SetTransForm(Vector3 position, Quaternion rotation, Vector3 scale)
{
	m_position = position;
	m_rotation = rotation;
	m_scale = scale;
}

Vector3 AIActor::calcVelocity(Status status)
{
	Vector3 moveSpeed = Vector3::Zero;

	//自身からターゲットに向かうベクトルを計算する
	Vector3 diff = m_targetPosition - m_position;
	//正規化
	diff.Normalize();
	diff.y = 0.0f;
	//速度を設定
	moveSpeed = diff * status.defaultSpeed;

	//値をセーブしておく
	m_SaveMoveSpeed = moveSpeed;

	//特定のアニメーションが再生中のとき
	if (isAnimationEntable() != true)
	{
		return moveSpeed = Vector3::Zero;
	}
	else
	{
		return moveSpeed;
	}

	
	
}

void AIActor::SetTargetPosition()
{
	//ターゲット(プレイヤー)の座標を取得
	m_targetPosition = m_player->GetPosition();
}

bool AIActor::AttackInterval(const float attackintarvaltime)
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

bool AIActor::DamageInterval(const float damageintarvaltime)
{
	//ダメージを受けたら
	if (m_damageFlag == true)
	{
		//タイマーがインターバルを超えたら
		if (damageintarvaltime < m_damageIntervalTimer)
		{
			//ダメージを受けられるようにする
			m_damageFlag = false;
			//タイマーをリセット
			m_damageIntervalTimer = 0.0f;
		}
		else
		{
			m_damageIntervalTimer += g_gameTime->GetFrameDeltaTime();
			//ダメージを受けない
			return false;
		}
	}
	//ダメージを受けられるようにする
	return true;
}

bool AIActor::IsFindPlayer(float distance)
{
	//ターゲット(プレイヤー)の座標を取得
	m_targetPosition = m_player->GetPosition();
	//自身からターゲットに向かうベクトルを計算する
	Vector3 diff = m_targetPosition - m_position;
	diff.y = 0.0f;
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


	return false;
}

Quaternion AIActor::Rotation()
{
	if (RotationOnly() == true)
	{
		return m_rotation;
	}

	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
	}

	return m_rotation;
}
