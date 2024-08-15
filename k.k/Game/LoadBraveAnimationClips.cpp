#include "stdafx.h"
#include "LoadBraveAnimationClips.h"

void LoadBraveAnimationClips::RoadWeaponsAnimClips()
{
	//武器に応じたアニメーションをロード
	RoadOneHandSwordAnimationClip();
	RoadTwoHandSwordAnimationClip();
	RoadBowAndArrowAnimationClip();

	//現在のアニメーションの番号を設定
	//m_currentAnimationStartIndexNo = m_swordShieldAnimationStartIndexNo;

}

void LoadBraveAnimationClips::SetCurrentWeaponAnimationStartIndexNo(
	WeaponType::EnWeaponType startWeaponType)
{
	switch (startWeaponType)
	{
	case WeaponType::enSwordShield:
		m_currentAnimationStartIndexNo = m_swordShieldAnimationStartIndexNo;
		break;
	case WeaponType::enGreateSword:
		m_currentAnimationStartIndexNo = m_greateSwordAnimationStartIndexNo;
		break;
	case WeaponType::enBowArrow:
		m_currentAnimationStartIndexNo = m_bowArrowAnimationStartIndexNo;
		break;
	default:
		break;
	}
}

void LoadBraveAnimationClips::RoadOneHandSwordAnimationClip()
{
	// 片手剣のアニメーションクリップをロードする
	const std::pair<const char*, bool> oneHandedSwordAnimClipFilePaths[] = {
		{"Assets/animData/character/Player/OneHandSword/Idle.tka",true},
		{"Assets/animData/character/Player/OneHandSword/Sprint.tka",true},
		{"Assets/animData/character/Player/OneHandSword/KnockBack.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Hit.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Defend.tka", true },
		{"Assets/animData/character/Player/OneHandSword/DefendHit.tka", false },
		{"Assets/animData/character/Player/OneHandSword/Die.tka", false },
		{"Assets/animData/character/Player/OneHandSword/ChangeSwordShield.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Win_start.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Win_main.tka",true},
		{"Assets/animData/character/Player/OneHandSword/Attack_1.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Attack_2.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Attack_3.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Skill_Start.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Skill_Main.tka",false},
		{"None",false}
	};

	for (int i = 0; i < enBraveAnimClip_Num; i++) {
		m_animationClip[m_swordShieldAnimationStartIndexNo + i].Load(oneHandedSwordAnimClipFilePaths[i].first);
		m_animationClip[m_swordShieldAnimationStartIndexNo + i].SetLoopFlag(oneHandedSwordAnimClipFilePaths[i].second);
	}
}

void LoadBraveAnimationClips::RoadTwoHandSwordAnimationClip()
{
	// 両手剣のアニメーションクリップをロードする
	const std::pair<const char*, bool> twoHandedSwordAnimClipFilePaths[] = {
		{"Assets/animData/character/Player/TwoHandSword/idle_BigSword.tka",true},
		{"Assets/animData/character/Player/TwoHandSword/Sprint.tka",true},
		{"Assets/animData/character/Player/TwoHandSword/KnockBack.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Hit.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Rool.tka", false },
		{"Assets/animData/character/Player/TwoHandSword/Rool.tka", false },
		{"Assets/animData/character/Player/TwoHandSword/Die.tka", false },
		{"Assets/animData/character/Player/TwoHandSword/ChangeTwoHandSword.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Win_Start.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Win_Main.tka",true},
		{"Assets/animData/character/Player/TwoHandSword/Attack_1.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Attack_2.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Attack_3.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Skill_Start2.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Skill_Main2.tka",false},
		{"None",false}
	};

	for (int i = 0; i < enBraveAnimClip_Num; i++) {
		m_animationClip[m_greateSwordAnimationStartIndexNo + i].Load(twoHandedSwordAnimClipFilePaths[i].first);
		m_animationClip[m_greateSwordAnimationStartIndexNo + i].SetLoopFlag(twoHandedSwordAnimClipFilePaths[i].second);
	}

}

void LoadBraveAnimationClips::RoadBowAndArrowAnimationClip()
{
	// 弓のアニメーションクリップをロードする
	const std::pair<const char*, bool> bowAnimClipFilePaths[] = {
		{"Assets/animData/character/Player/Bow/Idle.tka",true},
		{"Assets/animData/character/Player/Bow/Sprint.tka",true},
		{"Assets/animData/character/Player/Bow/KnockBack.tka",false},
		{"Assets/animData/character/Player/Bow/Hit.tka",false},
		{"Assets/animData/character/Player/Bow/Rool.tka", false },
		{"Assets/animData/character/Player/Bow/Rool.tka", false },
		{"Assets/animData/character/Player/Bow/Die.tka", false },
		{"Assets/animData/character/Player/Bow/ChangeBow.tka",false},
		{"Assets/animData/character/Player/Bow/Win_Start.tka",false},
		{"Assets/animData/character/Player/Bow/Win_Main.tka",true},
		{"Assets/animData/character/Player/Bow/Attack_1.tka",false},
		{"Assets/animData/character/Player/Bow/Attack_2.tka",false},
		{"Assets/animData/character/Player/Bow/Attack_3.tka",false},
		{"Assets/animData/character/Player/Bow/Skill_Start.tka",true},
		{"Assets/animData/character/Player/Bow/Skill_Main.tka",false},
		{"None",false}
	};

	for (int i = 0; i < enBraveAnimClip_Num; i++) {
		m_animationClip[m_bowArrowAnimationStartIndexNo + i].Load(bowAnimClipFilePaths[i].first);
		m_animationClip[m_bowArrowAnimationStartIndexNo + i].SetLoopFlag(bowAnimClipFilePaths[i].second);
	}
}
