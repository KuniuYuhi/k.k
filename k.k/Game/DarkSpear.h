#pragma once

namespace {
	const int SPEAR_ROWS = 3;		//���̗�̐�
	const int SPEAR_CREATE_PLACE = 8;	//8���𐶐�����ꏊ�̐�

	const int SPEARS = SPEAR_ROWS * SPEAR_CREATE_PLACE;

	const float FIRST_CREATE_POS_LENGTH = 200.0f;

	const float ADD_CREATE_POS_LENGTH = 250.0f;

	const int CREATE_TIMER_LIMMIT = 1.0f;
}

class DarkSpearObj;

class DarkSpear: public IGameObject
{
public:
	DarkSpear();
	~DarkSpear();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector3 position)
	{
		m_position = position;
	}
	/// <summary>
	/// ��]��ݒ�
	/// </summary>
	/// <param name="rotation"></param>
	void SetRotation(Quaternion rotation)
	{
		m_rotation = rotation;
	}

	/// <summary>
	/// �O������ݒ�
	/// </summary>
	/// <param name="forward"></param>
	void SetForward(Vector3 forward)
	{
		m_forward = forward;
	}

	/// <summary>
	/// �������I��������̃t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetCreateEndFlag() const
	{
		return m_createEndFlag;
	}

	/// <summary>
	/// �����J�n�t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetCreateStartFlag(bool flag)
	{
		m_createStartFlag = flag;
	}

private:

	/// <summary>
	/// ���𐶐�������W�����߂�
	/// </summary>
	void DecideCreatePosition();

	/// <summary>
	/// ��������^�C�~���O��
	/// </summary>
	/// <returns></returns>
	bool IsCreateTiming();



private:
	ModelRender m_model[SPEAR_CREATE_PLACE][SPEAR_ROWS];

	DarkSpearObj* m_darkSpearObj[SPEAR_CREATE_PLACE][SPEAR_ROWS] = { nullptr };


	Quaternion m_rotation = g_quatIdentity;

	Vector3 m_position = g_vec3Zero;
	Vector3 m_forward = g_vec3Zero;

	Vector3 m_createPosition[SPEAR_CREATE_PLACE][SPEAR_ROWS];



	bool m_createEndFlag = false;

	bool m_createStartFlag = false;

	float m_timer = 0.0f;

	int m_createCount = 0;

	float m_createTimer = CREATE_TIMER_LIMMIT;

};

