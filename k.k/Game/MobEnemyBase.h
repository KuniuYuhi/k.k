#pragma once
#include "EnemyBase.h"



/// <summary>
/// ���u�G�l�~�[�p�̃x�[�X�N���X
/// ���u�G�l�~�[�ŋ��ʂ��鏈������������
/// </summary>
class MobEnemyBase : public EnemyBase
{
public:

	virtual ~MobEnemyBase() override = default;


	/// <summary>
	/// �X�V�����B�g���ĂȂ��I�I�I�I
	/// </summary>
	virtual void UpdateCharacter() override {}

protected:

	/// <summary>
	/// ��{�I�ȃR���|�[�l���g���Z�b�e�B���O
	/// </summary>
	virtual void SettingDefaultComponent() override {}


protected:



};

