#pragma once

class Wizard;

class FlamePillar:public IGameObject
{
public:
	FlamePillar();
	~FlamePillar();

	bool Start();

	void Update();

	void Timer();

	void Render(RenderContext& rc);

	void SetPosition(Vector3 postion)
	{
		m_position = postion;
	}

	void SetRotation(Quaternion rotation)
	{
		m_rotation = rotation;
	}

	void SetWizard(Wizard* wizard)
	{
		m_wizard = wizard;
	}

private:
	Wizard* m_wizard = nullptr;

	ModelRender m_model;

	Vector3 m_position = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;

	CollisionObject* m_pillarCollision;

	const float m_limitDistance=0;

	const int m_AttackPower = 50;

	const float m_pillarLimitTime = 5.0f;
	float m_pillarTimer = 0;

	const float m_distance = 300.0f;
};

