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


	

	/// <summary>
	/// ���i�̈ړ����x�̌v�Z�B�v���C���[�̑O�������ݒ肷���I
	/// </summary>
	/// <param name="playerStatus">�X�e�[�^�X</param>
	/// <param name="moveSpeed">���݂̈ړ����x</param>
	/// <param name="StickInput">�R���g���[���[�̈ړ��X�e�B�b�N�̓��͗�</param>
	/// <returns>�ړ����x</returns>
	Vector3 CalcSimpleMovementVerocity(
		float speed,
		Vector3 moveSpeed,
		Vector3 StickInput
	);


	
	/// <summary>
	/// �ړ������݂̂̌v�Z
	/// </summary>
	/// <param name="forward">�O����</param>
	/// <param name="moveSpeed">���݂̈ړ����x</param>
	/// <returns></returns>
	Vector3 CalcForwardDirection(Vector3 forward, Vector3 moveSpeed = g_vec3Zero);


	/// <summary>
	/// �ړ������݂̂̌v�Z�B���͕������l��
	/// </summary>
	/// <param name="forward">�O����</param>
	/// <param name="stick">���͕���</param>
	/// <param name="moveSpeed">���݂̈ړ����x</param>
	/// <returns></returns>
	Vector3 CalcMoveDirection(Vector3 forward, Vector3 stick, Vector3 moveSpeed = g_vec3Zero);


private:

	/// <summary>
	/// �J�����̑O�������擾�B���K���ς�
	/// </summary>
	/// <returns></returns>
	Vector3 GetCameraForward();
	/// <summary>
	/// �J�����̉E�������擾�B���K���ς�
	/// </summary>
	/// <returns></returns>
	Vector3 GetCameraRight();



private:

	Brave* m_brave = nullptr;


	Vector3 m_moveDirection = g_vec3Zero;		//�ړ������̂݁B���K�����邱�ƁI�I


};

