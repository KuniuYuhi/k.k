#pragma once

/// <summary>
///サモナーのステート
/// </summary>
namespace SummonerStates {

	/// <summary>
	/// サモナーのステート
	/// </summary>
	enum EnSummonerState
	{
		enSummonerState_Appear,
		enSummonerState_Idle,
		enSummonerState_Run,
		enSummonerState_DarkBall,
		enSummonerState_DarkWall,
		enSummonerState_ShockWave,
		enSummonerState_FirstAttack,
		enSummonerState_SecondAttack,
		enSummonerState_ThirdAttack,
		enSummonerState_Attack_DarkMeteorite,
		enSummonerState_Summon,
		enSummonerState_Hit,
		enSummonerState_Die,
		enSummonerState_Victory,
		enSummonerState_Angry,
		enSummonerState_Warp,
		enSummonerState_KnockBackAttack,
		enSummonerState_None
	};

}

/// <summary>
/// サモナーのサブステートマシン
/// </summary>
namespace SummonerSubStateMachine {

	enum EnSubStateMachine
	{
		enSubStateMachine_General,		//普段
		enSubStateMachine_Offensive,		//攻撃
	};
}

/// <summary>
/// サモナーのアニメーションクリップ
/// </summary>
namespace SummonerAnimationClips {

	enum EnSummonerAnimClip
	{
		enSummonerAnimClip_Appear,
		enSummonerAnimClip_Idle,
		enSummonerAnimClip_Run,
		enSummonerAnimClip_DarkBall,
		enSummonerAnimClip_DarkWall,
		enSummonerAnimClip_ShockWave,
		enSummonerAnimClip_FirstAttack,
		enSummonerAnimClip_SecondAttack,
		enSummonerAnimClip_ThirdAttack,
		enSummonerAnimClip_Attack_DarkMeteorite_start,
		enSummonerAnimClip_Attack_DarkMeteorite_main,
		enSummonerAnimClip_Summon,
		enSummonerAnimClip_Hit,
		enSummonerAnimClip_Die,
		enSummonerAnimClip_Victory,
		enSummonerAnimClip_Angry,
		enSummonerAnimClip_Warp,
		enSummonerAnimClip_KnockBackAttack,
		enSummonerAnimClip_Num,				// 7 :アニメーションクリップの数
	};
}

/// <summary>
/// サモナーの攻撃の種類
/// </summary>
namespace SummonerSkillType
{

	enum EnSkillSType
	{
		enSkillType_ComboFirst,
		enSkillType_ComboSecond,
		enSkillType_ComboThird,
		enSkillType_ShockWave,
		enSkillType_Num,
	};
}