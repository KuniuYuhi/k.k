#pragma once
#include "IAttackObject.h"

class ComboFinishBomb:public IAttackObject
{
public:
	ComboFinishBomb();
	~ComboFinishBomb();

	bool Start() override;
	void Update() override;
	
	void SetPosition(Vector3 position)
	{
		m_position = position;
	}

	void ApplyPositionToMatrix(Matrix matrix)
	{
		matrix.Apply(m_position);
	}



private:

	void PlayBombEffect();

	void CreatCollision();

private:

	Vector3 m_position = g_vec3Zero;



};

