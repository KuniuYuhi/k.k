#pragma once

class Lich;

class DarkWall:public IGameObject
{
public:
	DarkWall();
	~DarkWall();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void SetLich(Lich* lich)
	{
		m_lich = lich;
	}

	const int GetAtk() const
	{
		return m_attak;
	}

private:
	Lich* m_lich = nullptr;

	ModelRender m_model;

	Vector3 m_position = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;

	CollisionObject* m_collision;

	const int m_attak = 20;

	const float m_speed = 50.0f;

	const float m_deleteTime = 2.0f;
	float m_deleteTimer = 0.0f;

};

