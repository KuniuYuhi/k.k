#pragma once

/// <summary>
/// �T�{�e���̃X�e�[�g
/// </summary>
namespace CactusStates {

	/// <summary>
	///  �T�{�e���̃X�e�[�g
	/// </summary>
	enum EnCactusState
	{
		enCactusState_Idle,					//�ҋ@
		enCactusState_Run,					//����
		enCactusState_Attack,				//�U��
		enCactusState_Hit,					//��_���[�W
		enCactusState_Die,					//���S
		enCactusState_Victory,				//����
		enCactusState_Appear,				//�o��
	};

}

/// <summary>
///  �T�{�e���̃A�j���[�V�����N���b�v
/// </summary>
namespace CactusAnimationClips {

	enum EnCactusAnimClip
	{
		enCactusAnimClip_Idle,					//�ҋ@
		enCactusAnimClip_Run,					//����
		enCactusAnimClip_Attack,				//�U��
		enCactusAnimClip_Hit,					//��_���[�W
		enCactusAnimClip_Die,					//���S
		enCactusAnimClip_Victory,				//����
		enCactusAnimClip_Appear,				//�o��
		enCactusAnimClip_num
	};


}