#pragma once

/// <summary>
/// �L�m�R�̃X�e�[�g
/// </summary>
namespace MushroomStates {

	/// <summary>
	/// �L�m�R�̃X�e�[�g
	/// </summary>
	enum EnMushroomState
	{
		enMushroomState_Idle,					//�ҋ@
		enMushroomState_Run,					//����
		enMushroomState_Attack,				//�U��
		enMushroomState_Hit,					//��_���[�W
		enMushroomState_Die,					//���S
		enMushroomState_Victory,				//����
		enMushroomState_Appear,				//�o��
	};

}

/// <summary>
/// �L�m�R�̃A�j���[�V�����N���b�v
/// </summary>
namespace MushroomAnimationClips {

	enum EnMushroomAnimClip
	{
		enMushroomAnimClip_Idle,					//�ҋ@
		enMushroomAnimClip_Run,					//����
		enMushroomAnimClip_Attack,				//�U��
		enMushroomAnimClip_Hit,					//��_���[�W
		enMushroomAnimClip_Die,					//���S
		enMushroomAnimClip_Victory,				//����
		enMushroomAnimClip_Appear,				//�o��
		enMushroomAnimClip_num
	};


}