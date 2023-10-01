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

	/// <summary>
	/// �t���C���s���[���_���[�W��^������悤�ɂȂ����Ƃ��ɌĂ�
	/// </summary>
	void StartFlamePillar();

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
	/// �U���͂̎擾
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

	/// <summary>
	/// �t���C���s���[�̉����o���n�߂邩�̃t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetStartFlamePllarFlag(bool flag)
	{
		m_startFlamePllarFlag = flag;
	}

private:
	Wizard* m_wizard = nullptr;

	EffectEmitter* m_circleEffect;
	EffectEmitter* m_flamePillarEffect;

	Vector3 m_position = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;

	CollisionObject* m_pillarCollision;

	const float m_limitDistance=0;

	const int m_AttackPower = 40;

	const float m_pillarLimitTime = 5.0f;
	float m_pillarTimer = 0;

	const float m_distance = 300.0f;

	bool m_canDamageFlag = false;			//�_���[�W��^�����邩	
	const float m_canDamageTime = 1.0f;
	float m_canDamageTimer = 0.0f;

	bool m_startFlamePllarFlag = false;

};

