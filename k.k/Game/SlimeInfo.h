#pragma once

/// <summary>
/// �X���C���̃X�e�[�g
/// </summary>
namespace SlimeStates {

	/// <summary>
	/// �X���C���̃X�e�[�g
	/// </summary>
	enum EnSlimeState
	{
		enSlimeState_Idle,					//�ҋ@
		enSlimeState_Run,					//����
		enSlimeState_Attack,				//�U��
		enSlimeState_Hit,					//��_���[�W
		enSlimeState_Die,					//���S
		enSlimeState_Victory,				//����
		enSlimeState_Appear,				//�o��
	};

}

/// <summary>
/// �X���C���̃A�j���[�V�����N���b�v
/// </summary>
namespace SlimeAnimationClips {

	enum EnSlimeAnimClip
	{
		enSlimeAnimClip_Idle,					//�ҋ@
		enSlimeAnimClip_Run,					//����
		enSlimeAnimClip_Attack,				//�U��
		enSlimeAnimClip_Hit,					//��_���[�W
		enSlimeAnimClip_Die,					//���S
		enSlimeAnimClip_Victory,				//����
		enSlimeAnimClip_Appear,				//�o��
		enSlimeAnimClip_num
	};


}