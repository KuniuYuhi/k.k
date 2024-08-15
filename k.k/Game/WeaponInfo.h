#pragma once

/// <summary>
/// 武器の属性のネームスペース
/// </summary>
namespace WeaponAttribute {

	/// <summary>
	/// 武器の属性
	/// </summary>
	enum EnWeaponAttribute
	{
		enWeaponAttribute_None,
		enWeaponAttribute_Slash,	//斬タイプ
		enWeaponAttribute_Pierce,	//突タイプ
	};

}

/// <summary>
/// 武器の種類のネームスペース
/// </summary>
namespace WeaponType {

	/// <summary>
	/// 武器の種類
	/// </summary>
	enum EnWeaponType
	{
		enSwordShield,			//ソード＆シールド
		enGreateSword,			//グレイトソード
		enBowArrow				//ボウ＆アロー
	};

}