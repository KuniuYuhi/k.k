#include "stdafx.h"
#include "Actor.h"

Actor::Actor()
{
}

Actor::~Actor()
{
}

void Actor::SetTransFormModel(ModelRender& modelRender)
{
	modelRender.SetTransform(m_position, m_rotation, m_scale);
}

void Actor::SetTransForm(Vector3 position, Quaternion rotation,Vector3 scale)
{
	m_position = position;
	m_rotation = rotation;
	m_scale = scale;
}

Vector3 Actor::calcVelocity(Status status)
{
	Vector3 moveSpeed = Vector3::Zero;

	//カメラの前方向と右方向のベクトルを持ってくる。
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//y方向には移動させない。
	forward.y = 0.0f;
	right.y = 0.0f;
	forward.Normalize();

	//移動の入力
	Vector3 stickL = Vector3::Zero;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//ダッシュフラグがfalseなら
	if (m_dashFlag == false)
	{
		//移動の入力量とstatusのスピードを乗算。
		right *= stickL.x * status.defaultSpeed;
		forward *= stickL.y * status.defaultSpeed;
	}
	else
	{
		//移動の入力量とstatusのスピードとダッシュの値を乗算。
		right *= stickL.x * (status.defaultSpeed + status.dashSpeed);
		forward *= stickL.y * (status.defaultSpeed + status.dashSpeed);
	}

	moveSpeed += right + forward;

	return moveSpeed;
	
}

Quaternion Actor::Rotation()
{
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
	}

	return m_rotation;
}
