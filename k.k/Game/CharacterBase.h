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
	/// �X�V����
	/// </summary>
	virtual void UpdateCharacter() = 0;

protected:


	void SettingDefaultComponent();

	


protected:
	//�X�e�[�^�X

	//�f�t�H���g�R���|�[�l���g

};

