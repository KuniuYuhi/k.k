#pragma once

namespace {
	const int LENGTH = 800.0f;
}

//���e�I�̏I�_�̌v�Z

/// <summary>
/// ���e�I�̏I�_�̎擾�B�v���C���[�̎���ɗ��Ƃ�
/// </summary>
/// <param name="targetCenterPosition">���e�I�𗎂Ƃ����S�̍��W</param>
/// <returns></returns>
Vector3 GetMeteoTargetPosition(Vector3 targetCenterPosition)
{
	while (true)
	{
		Vector3 finalPos = targetCenterPosition;
		//���S����300�͈͓̔��Ń����_���Ȓl�����߂�
		float X = rand() % (LENGTH * 2 + 1) - (LENGTH);
		float Z = rand() % (LENGTH * 2 + 1) - (LENGTH);

		finalPos.x += X;
		finalPos.z += Z;
		finalPos.y = 0.0f;
		return finalPos;
	}
}