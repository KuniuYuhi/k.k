#pragma once
#include "ICharacter.h"

/// <summary>
/// �L�����N�^�[�̃x�[�X�N���X
/// �L�����N�^�[�ŋ��ʂ��鏈������������
/// </summary>
class CharacterBase : public ICharacter
{
public:
	virtual ~CharacterBase() = default;

	/// <summary>
	/// �X�V�����B�g���ĂȂ��I�I�I�I
	/// </summary>
	virtual void UpdateCharacter() = 0;



	//�e���v���[�g�ŃX�e�[�^�X���擾����


protected:

	/// <summary>
	/// ��{�I�ȃR���|�[�l���g���Z�b�e�B���O
	/// </summary>
	virtual void SettingDefaultComponent(){}

	


protected:
	//�X�e�[�^�X

	//�f�t�H���g�R���|�[�l���g

};

