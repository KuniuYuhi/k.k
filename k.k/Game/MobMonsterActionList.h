#pragma once

namespace MobMonsterInfo {

	/// <summary>
	/// �A�N�V�����̖��O
	/// </summary>
	enum EnActionName
	{
		enActionName_Idle,		//�ҋ@���
		enActionName_Patrol,	//����
		enActionName_Chase,		//�ǂ�������
		enActionName_Attack,	//�U��
		enActionName_Skill,		//�X�L��
		enActionName_Hit,		//��_���[�W
		enActionName_Die,		//���S
		enActionName_Victory,	//����
		enActionName_Appear		//�o�����̃A�j���[�V����
	};

	/// <summary>
	/// �A�j���[�V�����X�e�[�g
	/// </summary>
	enum EnAnimationState
	{
		enAninationState_Idle,		//�ҋ@���
		enAninationState_Patrol,	//����
		enAninationState_Chase,		//�ǂ�������
		enAnimationState_Attack,	//�U��
		enAnimationState_Skill,		//�X�L��
		enAnimationState_Hit,		//��_���[�W
		enAnimationState_Die,		//���S
		enAnimationState_Victory,	//����
		enAnimationState_Appear,	//�o�����̃A�j���[�V����
		enAnimationState_Num
	};

	/// <summary>
	/// �A�j���[�V�����N���b�v
	/// </summary>
	enum EnAnimationClip
	{
		enAnimationClip_Idle,		//�ҋ@���
		enAnimationClip_Patrol,		//����
		enAnimationClip_Chase,		//�ǂ�������
		enAnimationClip_Attack,		//�U��
		enAnimationClip_Skill,		//�X�L��
		enAnimationClip_Hit,		//��_���[�W
		enAnimationClip_Die,		//���S
		enAnimationClip_Victory,	//����
		enAnimationClip_Appear,		//�o�����̃A�j���[�V����
		enAnimationClip_Num
	};

	/// <summary>
	/// �X�e�[�g�}�V���X�e�[�g
	/// </summary>
	enum EnStateMachineState
	{
		enStateMachineState_Patrol,		//����X�e�[�g�}�V��
		enStateMachineState_Chase		//�ǐՃX�e�[�g�}�V��
	};
}





