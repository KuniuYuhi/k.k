#pragma once


namespace BraveAnimationClip {

	/// <summary>
	/// 勇者(プレイヤー)のアニメーションクリップ
	/// </summary>
	enum EnBraveAnimationClip
	{
		enBraveAnimClip_Idle,
		enBraveAnimClip_Sprint,
		enBraveAnimClip_KnockBack,
		enBraveAnimClip_Hit,
		enBraveAnimClip_Defend,
		enBraveAnimClip_DefendHit,
		enBraveAnimClip_Die,
		enBraveAnimClip_ChangeWeapon,
		enBraveAnimClip_Win_Start,
		enBraveAnimClip_Win_Main,
		enBraveAnimClip_attack1,
		enBraveAnimClip_attack2,
		enBraveAnimClip_attack3,
		enBraveAnimClip_Skill_Start,
		enBraveAnimClip_Skill_Main,
		enBraveAnimClip_Num
	};
}


namespace BraveState {

	/// <summary>
	/// 勇者のステート
	/// </summary>
	enum EnBraveState {
		enBraveState_Idle,
		enBraveState_Sprint,
		enBraveState_FirstAttack,
		enBraveState_SecondAttack,
		enBraveState_ThirdAttack,
		enBraveState_SkillStart,
		enBraveState_SkillMain,
		enBraveState_KnockBack,
		enBraveState_Hit,
		enBraveState_DefensiveActions,
		enBraveState_DefensiveHit,
		enBraveState_Die,
		enBraveState_ChangeWeapon,
		enBraveState_WinStart,
		enBraveState_WinMain,
	};

}

