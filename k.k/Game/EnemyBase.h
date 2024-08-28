#pragma once
#include "CharacterBase.h"
#include "CommonEnemyStatus.h"			//�G�l�~�[�̃X�e�[�^�X


class CommonEnemyStatus;

/// <summary>
/// �G�l�~�[�p�̃x�[�X�N���X
/// �G�l�~�[�ŋ��ʂ��鏈������������
/// </summary>
class EnemyBase : public CharacterBase
{
public:

	//virtual ~EnemyBase() = default;


	/// <summary>
	/// ��{�I�X�e�[�^�X���擾
	/// </summary>
	/// <returns></returns>
	CommonEnemyStatus& GetCommonStatus()
	{
		return m_status;
	}

	/// <summary>
	/// �A�N�V��������
	/// </summary>
	/// <returns></returns>
	const bool IsAction()
	{
		return isAction;
	}
	/// <summary>
	/// �A�N�V�������ɂ���
	/// </summary>
	void ActionActive()
	{
		isAction = true;
	}
	/// <summary>
	/// �A�N�V�������I���
	/// </summary>
	void ActionDeactive()
	{
		isAction = false;
	}



protected:


	/// <summary>
	/// ��{�I�ȃR���|�[�l���g���Z�b�e�B���O
	/// </summary>
	virtual void SettingDefaultComponent() override {}


protected:

	//��{�I�ȃX�e�[�^�X
	CommonEnemyStatus m_status;

	


	bool isAction = false;		//�A�N�V���������̃t���O

};

