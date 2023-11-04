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
/// �m�b�N�o�b�N�̏����B
/// </summary>
/// <param name="knockBackPower"></param>
/// <param name="timer"></param>
/// <param name="moveSpeed"></param>
/// <returns>��������true�Atimer������ɒB�����false</returns>
bool IsProcessKnockBack(
	float &timer,
	Vector3& moveSpeed
);




