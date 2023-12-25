#pragma once
class InitEffect
{
public:
	InitEffect();
	~InitEffect();

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
		enEffect_Dash,											//ダッシュ
		enEffect_Hit,											//ヒット
		enEffect_FireBall,										//ファイヤーボール
		enEffect_FireBall_Explosion,							//ファイヤーボールの爆発
		enEffect_FlamePillar,									//フレイムピラー
		enEffect_FlamePillar_Circle,							//フレイムピラーの魔法陣
		enEffect_SwordStorm,									//ソードストーム
		enEffect_SwordStorm_Charge,								//ソードストームのチャージ
		

	};
};



