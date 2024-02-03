#pragma once
class InitEffect
{
public:
	InitEffect();
	~InitEffect();

	/// <summary>
	/// 各種エフェクトの初期化
	/// </summary>
	void InitEFK();

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
		enEffect_DarkWall,										//ダークウォール
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

		enEffect_Dash,											//ダッシュ
		enEffect_Hit,											//ヒット
		enEffect_SwordStorm_Charge,								//ソードストームのチャージ
		
		enEffect_BraveLanding,									//勇者登場時の着地

		enEffect_SwordShieldCombo12,							//剣＆盾の通常攻撃１と２
		enEffect_SwordShieldCombo3,								//剣＆盾の通常攻撃３
		enEffect_SwordShieldSkillRising,						//剣＆盾のスキルの上昇
		enEffect_SwordShieldSkillAttack,						//剣＆盾のスキル攻撃
		enEffect_SwordShieldDefendHit,							//剣＆盾の盾防御ヒット
		enEffect_GreatSwordCombo12,								//両手剣の通常攻撃１と２
		enEffect_GreatSwordCombo3,								//両手剣の通常攻撃３
		enEffect_GreatSwordSkillRising,							//両手剣のスキルの上昇
		enEffect_GreatSwordSkillAttack,							//両手剣のスキル攻撃
		enEffect_BowArrowCombo,									//弓＆矢の通常攻撃
		enEffect_Arrow,											//矢のエフェクト
		enEffect_BowArrowSkillShot,								//弓＆矢のスキル攻撃




	};


private:
	/// <summary>
	/// プレイヤーに関するエフェクト初期化
	/// </summary>
	void InitPlayerEffect();
	/// <summary>
	/// モンスターに関するエフェクト初期化
	/// </summary>
	void InitMonsterEffect();
	/// <summary>
	/// ボスモンスターに関するエフェクト初期化
	/// </summary>
	void InitBossMonsterEffect();
	/// <summary>
	/// モブモンスターに関するエフェクト初期化
	/// </summary>
	void InitMobMonsterEffect();

};



