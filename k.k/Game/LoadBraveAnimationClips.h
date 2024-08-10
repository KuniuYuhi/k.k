#pragma once

#include "BraveAnimClipAndStateInfo.h"


using namespace BraveAnimationClip;

/// <summary>
/// ���킻�ꂼ��̃A�j���[�V�����N���b�v�O���[�v
/// </summary>
enum AnimationClipGroup {
	AnimationClipGroup_OneHandedSword,	// �Ў茕�𑕔����̃A�j���[�V�����N���b�v�O���[�v
	AnimationClipGroup_TwoHandedSword,	// ���茕�𑕔����̃A�j���[�V�����N���b�v�O���[�v
	AnimationClipGroup_Bow,				// �|�𑕔����̃A�j���[�V�����N���b�v�O���[�v
	AnimationClipGroup_Num,
};


class LoadBraveAnimationClips
{
public:

	/// <summary>
	/// �A�j���[�V�����N���b�v��Ԃ�
	/// </summary>
	/// <returns></returns>
	AnimationClip* GetBraveAnimationClip()
	{
		return m_animationClip;
	}

	/// <summary>
	/// �A�j���[�V�����N���b�v�̑�����Ԃ�
	/// </summary>
	/// <returns></returns>
	const int GetNumAnimationClips()
	{
		return enBraveAnimClip_Num * AnimationClipGroup_Num;
	}

	/// <summary>
	/// �A�j���[�V�����N���b�v��S�ă��[�h����
	/// </summary>
	void RoadWeaponsAnimClips();

	/// <summary>
	/// ���݂̃A�j���[�V�����̍ŏ��̔ԍ����擾
	/// </summary>
	/// <returns></returns>
	const int GetCurrentAnimationStartIndexNo()
	{
		return m_currentAnimationStartIndexNo;
	}


private:

	/// <summary>
	/// �Ў茕�Ɋւ���A�j���[�V�����N���b�v�����[�h����
	/// </summary>
	void RoadOneHandSwordAnimationClip();

	/// <summary>
	/// ���茕�Ɋւ���A�j���[�V�����N���b�v�����[�h����
	/// </summary>
	void RoadTwoHandSwordAnimationClip();

	/// <summary>
	/// �|��Ɋւ���A�j���[�V�����N���b�v�����[�h����
	/// </summary>
	void RoadBowAndArrowAnimationClip();

private:

	//�A�j���[�V�����N���b�v
	//�A�j���[�V�����N���b�v�~���퐔
	AnimationClip m_animationClip[enBraveAnimClip_Num * AnimationClipGroup_Num];

	//���C������̃A�j���[�V�����N���b�v�̍ŏ��̔ԍ�
	const int m_swordShieldAnimationStartIndexNo = 0;
	//�T�u����̃A�j���[�V�����N���b�v�̍ŏ��̔ԍ�
	const int m_greateSwordAnimationStartIndexNo =
		enBraveAnimClip_Num * AnimationClipGroup_TwoHandedSword;
	//�T�u�Q����̃A�j���[�V�����N���b�v�̍ŏ��̔ԍ�
	const int m_bowArrowAnimationStartIndexNo = 
		enBraveAnimClip_Num * AnimationClipGroup_Bow;

	//���݂̕���̃A�j���[�V�����̍ŏ��̔ԍ�
	int m_currentAnimationStartIndexNo = m_swordShieldAnimationStartIndexNo;

};

