#pragma once

#include "WeaponAttribute.h"

using namespace WeaponType;

/// <summary>
/// 武器のステータス
/// </summary>
class WeaponStatus
{
private:


	/// <summary>
	/// ステータス
	/// </summary>
	struct CommonStatus
	{
		int defaultPower = 0;				//基礎攻撃力
		int currentPower = 0;				//現在の攻撃力
		int skillPower = 0;
		EnWeaponAttribute weaponAttribute;	//武器の属性

	};



public:


};

