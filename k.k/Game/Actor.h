#pragma once
#include "Status.h"

class Actor:public IGameObject
{
public:
	Actor();
	virtual ~Actor();

protected:
	void Move();


private:
	Status m_status;


	Vector3 m_position = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;

};

