#pragma once

/// <summary>
/// �m�b�N�o�b�N�p�^�[��
/// </summary>
namespace KnockBackInfo {

	/// <summary>
	/// �m�b�N�o�b�N�̃p�^�[��
	/// </summary>
	enum EnKnockBackPattern
	{
		enKBPattern_ShortGroundedRetreat,  // �����ジ����
		enKBPattern_LongGroundedRetreat,   // ���߂Ɍジ����
		enKBPattern_SlightAirborneRetreat, // ����������тȂ���ジ����
		enKBPattern_FullAirborneRetreat,   // ���߂ɐ�����тȂ���ジ����
		enKBPattern_VerticalLaunch,        // ���̏ꂩ�瓮�����ɋ󒆂ɐ������(�󒆂ɕ���)

		enKnockBackPattern_Num,			//����

	};


	/// <summary>
	/// �J�[�u�f�[�^�̃|�C���g
	/// </summary>
	struct CurvePoint {
		float time = 0.0f;
		Vector3 curvePosition = g_vec3Zero;
	};

}
