#include "stdafx.h"
#include "InitEffect.h"

InitEffect::InitEffect()
{
}

InitEffect::~InitEffect()
{
}

void InitEffect::InitEFK()
{
	//プレイヤーに関するエフェクトを初期化
	InitPlayerEffect();
	//モンスターに関するエフェクトを初期化
	InitMonsterEffect();
}

void InitEffect::InitPlayerEffect()
{
	//勇者の着地エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_BraveLanding,
		u"Assets/effect/sprite/BraveLanding.efk"
	);
	//ダッシュエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Dash, 
		u"Assets/effect/sprite/Dash.efk"
	);
	//	弓のチャージエフェクト１を読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_ArrowCharge1, 
		u"Assets/effect/sprite/SwordStorm_Charge.efk"
	);
	//	弓のチャージエフェクト２を読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_ArrowCharge2,
		u"Assets/effect/sprite/Charge2.efk"
	);

	//剣＆盾の通常攻撃１、２エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_SwordShieldCombo12,
		u"Assets/effect/sprite/SwordShieldComboSlash.efk"
	);
	//剣＆盾の通常攻撃３エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_SwordShieldCombo3,
		u"Assets/effect/sprite/SwordShieldComboThrust.efk"
	);
	//剣＆盾の盾バリアエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_SwordShieldDefendBarrier,
		u"Assets/effect/sprite/ShieldBarrier.efk"
	);
	//剣＆盾のスキル上昇エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_SwordShieldSkillRising,
		u"Assets/effect/sprite/SwordShieldSkillRising.efk"
	);
	//剣＆盾のスキル攻撃エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_SwordShieldSkillAttack,
		u"Assets/effect/sprite/SwordShieldSkillAttack.efk"
	);


	//両手剣の通常攻撃１、２エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_GreatSwordCombo1,
		u"Assets/effect/sprite/GreatSwordComboSlash.efk"
	);
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_GreatSwordCombo2,
		u"Assets/effect/sprite/GreatSwordComboSlashSlow.efk"
	);
	//両手剣の通常攻撃３エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_GreatSwordCombo3,
		u"Assets/effect/sprite/GreatSwordComboThrust.efk"
	);
	//両手剣のスキル上昇エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_GreatSwordSkillRising,
		u"Assets/effect/sprite/GreatSwordSkillRising.efk"
	);
	//両手剣のスキル攻撃エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_GreatSwordSkillAttack,
		u"Assets/effect/sprite/GreatSwordSkillAttack.efk"
	);

	//弓＆矢の通常攻撃エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_BowArrowCombo,
		u"Assets/effect/sprite/BowArrowCombo.efk"
	);
	//矢の攻撃エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Arrow,
		u"Assets/effect/sprite/Arrow.efk"
	);
	//弓＆矢のスキル攻撃エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_BowArrowSkillShot,
		u"Assets/effect/sprite/BowArrowSkillShot.efk"
	);
	


	//攻撃力アップ開始エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_AttackUpStart,
		u"Assets/effect/sprite/AttackUpStart.efk"
	);

	//攻撃力アップエフェクトシンボルエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_AttackEffectSynbol,
		u"Assets/effect/sprite/AttackEffectSymbol.efk"
	);

	//回復開始エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_RecoveryStart,
		u"Assets/effect/sprite/RecoveryEffect.efk"
	);

	//回復エフェクトシンボルエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_RecoveryEffectSynbol,
		u"Assets/effect/sprite/RecoveryEffectSymbol.efk"
	);
}

void InitEffect::InitMonsterEffect()
{
	//モンスターのやられエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Mob_Dead, 
		u"Assets/effect/sprite/Mob_Dead.efk"
	);

	//ヒットエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Hit,
		u"Assets/effect/sprite/hit.efk"
	);

	//モブモンスターに関するエフェクトを初期化
	InitMobMonsterEffect();
	//ボスに関するエフェクトを初期化
	InitBossMonsterEffect();
}

void InitEffect::InitBossMonsterEffect()
{
	//ボスの魔法陣のエフェクト読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Boss_Summon_Circle, 
		u"Assets/effect/sprite/Boss_Summon_circle.efk"
	);
	//ダークボールのエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_DarkBall, 
		u"Assets/effect/sprite/DarkBall2.efk"
	);
	//ダークボールの爆発のエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_DarkBall_Explosion, 
		u"Assets/effect/sprite/DarkBall_Explosion.efk"
	);
	//ダークメテオのエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_DarkMeteorite, 
		u"Assets/effect/sprite/DarkMeteorite.efk"
	);
	//ダークメテオの風のエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_DarkMeteorite_Wind, 
		u"Assets/effect/sprite/DarkMeteorite_wind.efk"
	);
	//メテオのエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Meteo, 
		u"Assets/effect/sprite/Meteo.efk"
	);
	//メテオの爆発エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Meteo_Explosion, 
		u"Assets/effect/sprite/Meteo_Explosion.efk"
	);
	//メテオの範囲エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Meteo_Range, 
		u"Assets/effect/sprite/Range.efk"
	);
	//ダークスピアのエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_DarkSpear,
		u"Assets/effect/sprite/DarkSpear.efk"
	);
	//通常攻撃１のエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Boss_Combo_1,
		u"Assets/effect/sprite/Summoner_Slash.efk"
	);
	//通常攻撃2のエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Boss_Combo_2,
		u"Assets/effect/sprite/Boss_Combo_2.efk"
	);
	//通常攻撃３の爆発エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_ComboFinishExplosion,
		u"Assets/effect/sprite/ComboFinishExplosion.efk"
	);
	//ノックバックエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Boss_KnockBack,
		u"Assets/effect/sprite/Boss_KnockBack.efk"
	);

	//ダメージ無効エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Invalibation, 
		u"Assets/effect/sprite/Invalibation.efk"
	);

	//ワープ魔法陣エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Boss_WarpSircle,
		u"Assets/effect/sprite/Summoner_WarpSircle.efk"
	);

	//ワープの風エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Boss_WarpWind,
		u"Assets/effect/sprite/Summoner_Warp.efk"
	);
}

void InitEffect::InitMobMonsterEffect()
{
	//モブモンスターの魔法陣のライトのエフェクト読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Boss_Summon_FogRemoval, 
		u"Assets/effect/sprite/Boss_Summon_wind.efk"
	);
	//モブモンスターの魔法陣のエフェクト読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Mob_Summon_Circle, 
		u"Assets/effect/sprite/Summon_circle.efk"
	);
	//モブモンスターの魔法陣のライトのエフェクト読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Mob_Summon_Right, 
		u"Assets/effect/sprite/Summon_right.efk"
	);

	//モブエネミーのボールエフェクト読み込む
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_EyeBall,
		u"Assets/effect/sprite/miniBall.efk"
	);
	
}
