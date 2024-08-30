#pragma once
#include "ICharacter.h"
#include "DamageFont.h"
#include "DamageProvider.h"

/// <summary>
/// �L�����N�^�[�̃x�[�X�N���X
/// �L�����N�^�[�ŋ��ʂ��鏈������������
/// </summary>
class CharacterBase : public ICharacter//,DamageProvider
{
public:
	
	/// <summary>
	/// ���݂̍U���͂��擾
	/// </summary>
	/// <returns></returns>
	virtual int GetCurrentPower() { return 0; }
	
	

	/// <summary>
	/// �L�����R���̍쐬
	/// </summary>
	void CreateCharacterController();

	/// <summary>
	/// �ړ��ʂ��擾
	/// </summary>
	/// <returns></returns>
	Vector3 GetMoveSpeed()
	{


		return m_moveSpeed;
	}

	/// <summary>
	/// �L�����R�����擾
	/// </summary>
	/// <returns></returns>
	CharacterController* GetCharaCon()
	{
		return m_charaCon.get();
	}

	/// <summary>
	/// ��]������ݒ�
	/// </summary>
	/// <param name="direction"></param>
	void SetRotateDirection(Vector3 direction);

	/// <summary>
	/// �_���[�W�t�H���g���쐬
	/// </summary>
	/// <param name="hitDamage">�\������_���[�W</param>
	/// <param name="hitActor">�N���_���[�W���󂯂�̂�</param>
	void CreateDamageFont(int hitDamage, DamageFont::EnDamageActor hitActor);


	/// <summary>
	/// ���S�t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetDieFlag(bool flag)
	{
		m_isDieFlag = flag;
	}
	/// <summary>
	/// ���S�������H
	/// </summary>
	/// <returns></returns>
	bool IsDie()
	{
		return m_isDieFlag;
	}





protected:

	/// <summary>
	/// ��{�I�ȃR���|�[�l���g���Z�b�e�B���O
	/// </summary>
	virtual void SettingDefaultComponent(){}

	
	

protected:
	
	//CharacterController m_charaCon;		//�L�����N�^�[�R���g���[���[
	
	std::unique_ptr<CharacterController> m_charaCon = nullptr;


	Vector3 m_moveSpeed = Vector3::Zero;		//�ړ���

	Vector3 m_rotateDirection = Vector3::Zero;	//��]��������x�N�g��


	bool m_isDieFlag = false;				//���S�t���O


};

