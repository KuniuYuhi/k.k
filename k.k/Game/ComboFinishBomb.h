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
	/// 座標を設定
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector3 position)
	{
		m_position = position;
	}
	/// <summary>
	/// 座標に行列を適応
	/// </summary>
	/// <param name="matrix"></param>
	void ApplyPositionToMatrix(Matrix matrix)
	{
		matrix.Apply(m_position);
	}



private:
	/// <summary>
	/// エフェクト再生
	/// </summary>
	void PlayBombEffect();
	/// <summary>
	/// 当たり判定生成
	/// </summary>
	void CreatCollision();

private:

	Vector3 m_position = g_vec3Zero;

	CollisionObject* m_collision = nullptr;

};

