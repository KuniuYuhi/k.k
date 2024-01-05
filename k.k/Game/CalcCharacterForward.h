#pragma once

/// <summary>
/// �L�����N�^�[�̑O�������v�Z����N���X
/// </summary>
class CalcCharacterForward
{
public:


	/// <summary>
	/// ��ԋ߂������X�^�[�Ɍ������x�N�g����O�����ɂ��Ď擾
	/// </summary>
	/// <param name="charPosition">�L�����N�^�[�̍��W</param>
	/// <param name="forward">�O����</param>
	/// <param name="moveSpeed">�ړ�����</param>
	/// <param name="chaseDistance">�ǂ������鋗��(�͈�)</param>
	void CalcForwardOfNearMonster(
		Vector3 charPosition,
		Vector3& forward,
		Vector3& moveSpeed,
		float chaseDistance
	);


private:

	std::vector<Vector3> m_monstersPosition;

	Vector3 monsterPos;

};

