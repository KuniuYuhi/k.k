#pragma once

namespace EffectNumbers {

	/// <summary>
	/// エフェクト番号
	/// </summary>
	enum EnEFK
	{
		enEffect_Boss_Summon_Circle,							//ボスの召喚陣
		enEffect_Boss_Summon_FogRemoval,						//ボスの霧払い
		enEffect_Mob_Summon_Circle,								//モブモンスターの召喚陣
		enEffect_Mob_Summon_Right,								//モンスターの召喚された瞬間のライト
		enEffect_DarkBall,										//ダークボール
		enEffect_DarkBall_Explosion,							//ダークボールの爆発
		enEffect_DarkSpear,										//ダークスピア
		enEffect_DarkMeteorite,									//ダークメテオ
		enEffect_DarkMeteorite_Wind,							//ダークメテオの風
		enEffect_Meteo,											//メテオ
		enEffect_Meteo_Explosion,								//メテオの爆発
		enEffect_Meteo_Range,									//メテオの範囲
		enEffect_Mob_Dead,										//モブモンスターがやられたときのエフェクト
		enEffect_Invalibation,									//ダメージが無効のときのエフェクト
		enEffect_Boss_Combo_1,									//ボスの通常攻撃１
		enEffect_Boss_Combo_2,									//ボスの通常攻撃２
		enEffect_ComboFinishExplosion,							//ボスの通常攻撃３の爆発
		enEffect_Boss_KnockBack,								//ボスのノックバック攻撃
		enEffect_Boss_WarpSircle,								//ボスのワープ魔法陣
		enEffect_Boss_WarpWind,									//ボスのワープの風

		enEffect_EyeBall,

		enEffect_Dash,											//ダッシュ
		enEffect_Hit,											//ヒット
		enEffect_ArrowCharge1,
		enEffect_ArrowCharge2,

		enEffect_BraveLanding,									//勇者登場時の着地

		enEffect_SwordShieldCombo12,							//剣＆盾の通常攻撃１と２
		enEffect_SwordShieldCombo3,								//剣＆盾の通常攻撃３
		enEffect_SwordShieldSkillRising,						//剣＆盾のスキルの上昇
		enEffect_SwordShieldSkillAttack,						//剣＆盾のスキル攻撃
		enEffect_SwordShieldDefendBarrier,						//剣＆盾の盾バリア
		enEffect_GreatSwordCombo1,								//両手剣の通常攻撃１
		enEffect_GreatSwordCombo2,								//両手剣の通常攻撃２
		enEffect_GreatSwordCombo3,								//両手剣の通常攻撃３
		enEffect_GreatSwordSkillRising,							//両手剣のスキルの上昇
		enEffect_GreatSwordSkillAttack,							//両手剣のスキル攻撃
		enEffect_BowArrowCombo,									//弓＆矢の通常攻撃
		enEffect_Arrow,											//矢のエフェクト
		enEffect_BowArrowSkillShot,								//弓＆矢のスキル攻撃

		enEffect_AttackEffectSynbol,							//攻撃エフェクトシンボル
		enEffect_AttackUpStart,									//攻撃力Upスタート
	
		enEffect_RecoveryEffectSynbol,							//回復エフェクトシンボル
		enEffect_RecoveryStart,									//回復スタート
	
		enEffect_StaminaRecoveryEffectSynbol,					//スタミナ回復エフェクトシンボル
		enEffect_StaminaRecoveryStart,							//スタミナ回復エフェクトスタート

	};
}