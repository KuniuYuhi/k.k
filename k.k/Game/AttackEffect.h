#pragma once

class UseEffect;
class Brave;

/// <summary>
/// �U���̓o�t�I�u�W�F�N�g
/// </summary>
class AttackEffect:public IGameObject
{
public:
	AttackEffect();
	~AttackEffect();

	bool Start() override;

	void Update() override;


	void SetStartPosition(Vector3 position)
	{
		m_position = position;
	}

private:
	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// 
	/// </summary>
	void HitPlayer();


	/// <summary>
	/// �������邩�H
	/// </summary>
	/// <returns></returns>
	bool IsDelete();

	void DeleteProcess();

private:
	Brave* m_brave = nullptr;

	UseEffect* m_effect = nullptr;

	Vector3 m_position = g_vec3Zero;


	bool m_isHit = false;

	float m_deleteTimer = 0.0f;

};

