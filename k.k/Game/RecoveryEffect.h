#pragma once

class UseEffect;
class Brave;

/// <summary>
/// HP�񕜃o�t�I�u�W�F�N�g
/// </summary>
class RecoveryEffect:public IGameObject
{
public:
	RecoveryEffect();
	~RecoveryEffect();

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
	/// �v���C���[�Ƀq�b�g������
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

