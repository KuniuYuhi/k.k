#pragma once


/// <summary>
/// �A�N�V�����̖��O
/// </summary>
enum EnActionName
{
	enActionName_Idle,		//�ҋ@���
	enActionName_Walk,		//����
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
	enAninationState_Walk,		//����
	enAnimationState_Attack,	//�U��
	enAnimationState_Skill,		//�X�L��
	enAnimationState_Hit,		//��_���[�W
	enAnimationState_Die,		//���S
	enAnimationState_Victory,	//����
	enAnimationState_Appear		//�o�����̃A�j���[�V����
};

/// <summary>
/// �A�j���[�V�����N���b�v
/// </summary>
enum EnAnimationClip
{
	enAninationClip_Idle,		//�ҋ@���
	enAninationClip_Walk,		//����
	enAnimationClip_Attack,		//�U��
	enAnimationClip_Skill,		//�X�L��
	enAnimationClip_Hit,		//��_���[�W
	enAnimationClip_Die,		//���S
	enAnimationClip_Victory,	//����
	enAnimationClip_Appear		//�o�����̃A�j���[�V����
};




