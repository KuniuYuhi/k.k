#pragma once
#include "IAttackObject.h"

class ComboFinishBomb:public IAttackObject
{
public:
	ComboFinishBomb();
	~ComboFinishBomb();

	bool Start() override;
	void Update() override;
	
	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector3 position)
	{
		m_position = position;
	}
	/// <summary>
	/// ���W�ɍs���K��
	/// </summary>
	/// <param name="matrix"></param>
	void ApplyPositionToMatrix(Matrix matrix)
	{
		matrix.Apply(m_position);
	}



private:
	/// <summary>
	/// �G�t�F�N�g�Đ�
	/// </summary>
	void PlayBombEffect();
	/// <summary>
	/// �����蔻�萶��
	/// </summary>
	void CreatCollision();

private:

	Vector3 m_position = g_vec3Zero;

	CollisionObject* m_collision = nullptr;

};

