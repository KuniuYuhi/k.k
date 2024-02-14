#pragma once

/// <summary>
/// �m�b�N�o�b�N���鎞�̕���
/// </summary>
/// <param name="startPosition">�n�_�B�U���̒��S</param>
/// <param name="endPosition">�I�_�B���g�̍��W</param>
/// <param name="knockBackPower">�m�b�N�o�b�N�p���[</param>
/// <returns></returns>
const Vector3 SetKnockBackDirection(
	Vector3 startPosition,
	Vector3 endPosition,
	float knockBackPower
);

/// <summary>
/// �m�b�N�o�b�N�̈ړ������̏����B
/// </summary>
/// <param name="moveSpeed">�ړ�����</param>
/// <param name="timer">�i�s����</param>
/// <param name="timerLimmit">�m�b�N�o�b�N���鐧������</param>
/// <param name=""></param>
/// <returns>��������true�Atimer������ɒB�����false</returns>
bool IsKnockingBack(
	Vector3& moveSpeed,
	float &timer,
	const float timerLimmit=1.0f
);




