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