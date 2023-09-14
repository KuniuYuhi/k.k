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
		enEffect_DarkMeteorite,									//ダークメテオ
		enEffect_DarkMeteorite_Wind,							//ダークメテオの風
		enEffect_Meteo,											//メテオ
		enEffect_Meteo_Explosion,								//メテオの爆発

	};
};



