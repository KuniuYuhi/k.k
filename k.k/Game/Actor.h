#pragma once
#include "Status.h"

class Actor:public IGameObject
{
public:
	Actor();
	virtual ~Actor();

	const Vector3 GetPosition()
	{
		return m_position;
	}

protected:

	void SetTransFormModel(ModelRender& modelRender);

	virtual void InitModel()=0;

	Vector3 Move(Status status);

	Quaternion Rotation();



protected:
	Status m_status;

	Vector3 m_position = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;

	bool m_dashFlag = false;		//ダッシュするかのフラグ

};

