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

/// <summary>
/// 
/// </summary>
namespace WeaponUIPoint {

	const Vector2 MAIN_WEAPON_CENTER_POS = { 800.0f,-330.0f };
	const Vector2 SUB_WEAPON_CENTER_POS = { 850.0f,50.0f };
	const Vector2 SUB_WEAPON2_COENTER_POS = { 850.0f,-110.0f };


	const Vector3 MAIN_WEAPON_SCALE = g_vec3One * 0.9f;
	const Vector3 SUB_WEAPON_SCALE = g_vec3One * 0.5f;
}