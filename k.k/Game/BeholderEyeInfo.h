#pragma once

/// <summary>
/// �ڋʂ̃X�e�[�g
/// </summary>
namespace BeholderEyeStates {

	/// <summary>
	/// �ڋʂ̃X�e�[�g
	/// </summary>
	enum EnBeholderEyeState
	{
		enBeholderEyeState_Idle,				//�ҋ@
		enBeholderEyeState_Run,					//����
		enBeholderEyeState_Attack,				//�U��
		enBeholderEyeState_Hit,					//��_���[�W
		enBeholderEyeState_Die,					//���S
		enBeholderEyeState_Victory,				//����
		enBeholderEyeState_Appear,				//�o��
	};

}

/// <summary>
/// �ڋʂ̃A�j���[�V�����N���b�v
/// </summary>
namespace BeholderEyeAnimationClips {

	enum EnBeholderEyeAnimClip
	{
		enBeholderEyeAnimClip_Idle,					//�ҋ@
		enBeholderEyeAnimClip_Run,					//����
		enBeholderEyeAnimClip_Attack,				//�U��
		enBeholderEyeAnimClip_Hit,					//��_���[�W
		enBeholderEyeAnimClip_Die,					//���S
		enBeholderEyeAnimClip_Victory,				//����
		enBeholderEyeAnimClip_Appear,				//�o��
		enBeholderEyeAnimClip_num
	};


}