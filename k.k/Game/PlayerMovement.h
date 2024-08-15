#pragma once
//#include "Component.h"

#include "IComponent.h"


class Brave;
class Status_Player;

/// <summary>
/// �L�����N�^�[�ړ��p�R���|�[�l���g
/// </summary>
class PlayerMovement : public IComponent
{
public:
	PlayerMovement();
	~PlayerMovement();

	void Start() override;

	void UpdateComponent() override;


	int GetSpeed()
	{
		return m_speed;
	}

	/// <summary>
	/// ���i�̈ړ����x��
	/// �v�Z
	/// </summary>
	/// <param name="playerStatus">�X�e�[�^�X</param>
	/// <param name="moveSpeed">���݂̈ړ����x</param>
	/// <param name="StickInput">�R���g���[���[�̈ړ��X�e�B�b�N�̓��͗�</param>
	/// <returns>�ړ����x</returns>
	Vector3 CalcSimpleMovementVerocity(
		Status_Player playerStatus,
		Vector3 moveSpeed,
		Vector3 StickInput
	);



private:

	Brave* m_brave = nullptr;

	int m_speed = 10;


};

