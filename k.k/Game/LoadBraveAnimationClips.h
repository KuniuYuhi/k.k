#pragma once

#include "BraveAnimClipAndStateInfo.h"


using namespace BraveAnimationClip;

/// <summary>
/// 武器それぞれのアニメーションクリップグループ
/// </summary>
enum AnimationClipGroup {
	AnimationClipGroup_OneHandedSword,	// 片手剣を装備中のアニメーションクリップグループ
	AnimationClipGroup_TwoHandedSword,	// 両手剣を装備中のアニメーションクリップグループ
	AnimationClipGroup_Bow,				// 弓を装備中のアニメーションクリップグループ
	AnimationClipGroup_Num,
};


class LoadBraveAnimationClips
{
public:

	/// <summary>
	/// アニメーションクリップを返す
	/// </summary>
	/// <returns></returns>
	AnimationClip* GetBraveAnimationClip()
	{
		return m_animationClip;
	}

	/// <summary>
	/// アニメーションクリップの総数を返す
	/// </summary>
	/// <returns></returns>
	const int GetNumAnimationClips()
	{
		return enBraveAnimClip_Num * AnimationClipGroup_Num;
	}

	/// <summary>
	/// アニメーションクリップを全てロードする
	/// </summary>
	void RoadWeaponsAnimClips();

	/// <summary>
	/// 現在のアニメーションの最初の番号を取得
	/// </summary>
	/// <returns></returns>
	const int GetCurrentAnimationStartIndexNo()
	{
		return m_currentAnimationStartIndexNo;
	}


private:

	/// <summary>
	/// 片手剣に関するアニメーションクリップをロードする
	/// </summary>
	void RoadOneHandSwordAnimationClip();

	/// <summary>
	/// 両手剣に関するアニメーションクリップをロードする
	/// </summary>
	void RoadTwoHandSwordAnimationClip();

	/// <summary>
	/// 弓矢に関するアニメーションクリップをロードする
	/// </summary>
	void RoadBowAndArrowAnimationClip();

private:

	//アニメーションクリップ
	//アニメーションクリップ×武器数
	AnimationClip m_animationClip[enBraveAnimClip_Num * AnimationClipGroup_Num];

	//メイン武器のアニメーションクリップの最初の番号
	const int m_swordShieldAnimationStartIndexNo = 0;
	//サブ武器のアニメーションクリップの最初の番号
	const int m_greateSwordAnimationStartIndexNo =
		enBraveAnimClip_Num * AnimationClipGroup_TwoHandedSword;
	//サブ２武器のアニメーションクリップの最初の番号
	const int m_bowArrowAnimationStartIndexNo = 
		enBraveAnimClip_Num * AnimationClipGroup_Bow;

	//現在の武器のアニメーションの最初の番号
	int m_currentAnimationStartIndexNo = m_swordShieldAnimationStartIndexNo;

};

