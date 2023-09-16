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
	//ボスの魔法陣のエフェクト読み込む
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Boss_Summon_Circle, u"Assets/effect/sprite/Boss_Summon_circle.efk");
	//モブモンスターの魔法陣のライトのエフェクト読み込む
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Boss_Summon_FogRemoval, u"Assets/effect/sprite/Boss_Summon_wind.efk");
	//モブモンスターの魔法陣のエフェクト読み込む
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Mob_Summon_Circle, u"Assets/effect/sprite/Summon_circle.efk");
	//モブモンスターの魔法陣のライトのエフェクト読み込む
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Mob_Summon_Right, u"Assets/effect/sprite/Summon_right.efk");
	//ダークボールのエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_DarkBall, u"Assets/effect/sprite/DarkBall2.efk");
	//ダークボールの爆発のエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_DarkBall_Explosion, u"Assets/effect/sprite/DarkBall_Explosion.efk");
	//ダークメテオのエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_DarkMeteorite, u"Assets/effect/sprite/DarkMeteorite.efk");
	//ダークメテオの風のエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_DarkMeteorite_Wind, u"Assets/effect/sprite/DarkMeteorite_wind.efk");
	//メテオのエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Meteo, u"Assets/effect/sprite/Meteo.efk");
	//メテオの爆発エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Meteo_Explosion, u"Assets/effect/sprite/Meteo_Explosion.efk");
	//モブモンスターのやられエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Mob_Dead, u"Assets/effect/sprite/Mob_Dead.efk");
	//ダメージ無効エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Invalibation, u"Assets/effect/sprite/Invalibation.efk");
	//ダッシュエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Dash, u"Assets/effect/sprite/Dash.efk");
	//ヒットエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Hit, u"Assets/effect/sprite/hit.efk");
	//ファイヤーボールエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_FireBall, u"Assets/effect/sprite/FireBall.efk");
	//ファイヤーボールの爆発エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_FireBall_Explosion, u"Assets/effect/sprite/FireBall_Explosion.efk");
	//フレイムピラーエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_FlamePillar, u"Assets/effect/sprite/FlamePillar.efk");
	//フレイムピラーの魔法陣のエフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_FlamePillar_Circle, u"Assets/effect/sprite/FlamePillar_Circle.efk");


}
