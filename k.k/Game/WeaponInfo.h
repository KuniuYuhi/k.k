#pragma once

/// <summary>
/// ����̑����̃l�[���X�y�[�X
/// </summary>
namespace WeaponAttribute {

	/// <summary>
	/// ����̑���
	/// </summary>
	enum EnWeaponAttribute
	{
		enWeaponAttribute_None,
		enWeaponAttribute_Slash,	//�a�^�C�v
		enWeaponAttribute_Pierce,	//�˃^�C�v
	};

}

/// <summary>
/// ����̎�ނ̃l�[���X�y�[�X
/// </summary>
namespace WeaponType {

	/// <summary>
	/// ����̎��
	/// </summary>
	enum EnWeaponType
	{
		enSwordShield,			//�\�[�h���V�[���h
		enGreateSword,			//�O���C�g�\�[�h
		enBowArrow				//�{�E���A���[
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