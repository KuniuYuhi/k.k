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

	/// <summary>
	/// �����X�^�[�̍��W�����X�g�ɒǉ��B
	/// </summary>
	void PushBackMonsterPositionList();
	
	/// �x�N�g�����쐬
	/// </summary>
	/// <param name="startPosition">�x�N�g���̎n�_</param>
	/// <param name="endPosition">�x�N�g���̏I�_</param>
	/// <param name="isNormalizeFlag">���K�����邩�̃t���O�B�f�t�H���g��true</param>
	/// <returns></returns>
	Vector3 GetCreateVector(
		Vector3 startPosition, 
		Vector3 endPosition,
		bool isNormalizeFlag = true
	);


private:

	std::vector<Vector3> m_monstersPosition;

	Vector3 monsterPos;

};

