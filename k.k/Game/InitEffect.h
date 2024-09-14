#pragma once

#include "EffectNumbers.h"

using namespace EffectNumbers;

class InitEffect
{
public:
	InitEffect();
	~InitEffect();

	/// <summary>
	/// 各種エフェクトの初期化
	/// </summary>
	void InitEFK();



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



