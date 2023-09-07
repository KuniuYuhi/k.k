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

	void CalcDamageTime();

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

	/// <summary>
	/// çUåÇóÕÇÃéÊìæ
	/// </summary>
	/// <returns></returns>
	const int& GetAtk() const
	{
		return m_AttackPower;
	}

	const bool& GetCanDamageFlag() const
	{
		return m_canDamageFlag;
	}

	void SetCanDamageFlag(bool flag)
	{
		m_canDamageFlag = flag;
	}

private:
	Wizard* m_wizard = nullptr;

	ModelRender m_model;

	Vector3 m_position = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;

	CollisionObject* m_pillarCollision;

	const float m_limitDistance=0;

	const int m_AttackPower = 40;

	const float m_pillarLimitTime = 5.0f;
	float m_pillarTimer = 0;

	const float m_distance = 300.0f;

	bool m_canDamageFlag = false;			//É_ÉÅÅ[ÉWÇó^Ç¶ÇÁÇÍÇÈÇ©	
	const float m_canDamageTime = 1.0f;
	float m_canDamageTimer = 0.0f;

};

