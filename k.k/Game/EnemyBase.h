#pragma once
#include "CharacterBase.h"


/// <summary>
/// �G�l�~�[�p�̃x�[�X�N���X
/// �G�l�~�[�ŋ��ʂ��鏈������������
/// </summary>
class EnemyBase : public CharacterBase
{
public:

	virtual ~EnemyBase() override = default;

	/// <summary>
	/// �X�V�����B�g���ĂȂ��I�I�I�I
	/// </summary>
	virtual void UpdateCharacter() override {}


protected:


	/// <summary>
	/// ��{�I�ȃR���|�[�l���g���Z�b�e�B���O
	/// </summary>
	//virtual void SettingDefaultComponent() {}


protected:

	//��{�I�ȃX�e�[�^�X

};

