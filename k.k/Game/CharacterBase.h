#pragma once
#include "ICharacter.h"
#include "Damageable.h"

/// <summary>
/// �L�����N�^�[�̃x�[�X�N���X
/// �L�����N�^�[�ŋ��ʂ��鏈������������
/// </summary>
class CharacterBase : public ICharacter,Damageable
{
public:
	//virtual ~CharacterBase() = default;

	/// <summary>
	/// ��_���[�W����
	/// </summary>
	virtual void Damage() override {}

	/// <summary>
	/// ���S������
	/// </summary>
	virtual void Dead() override {}
	
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

};

