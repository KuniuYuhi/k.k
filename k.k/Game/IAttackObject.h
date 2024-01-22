#pragma once


/// <summary>
/// �U����^����I�u�W�F�N�g�̊��N���X�B
/// �{�X����������_�[�N�{�[���Ȃ�
/// </summary>
class IAttackObject:public IGameObject
{
public:
	IAttackObject(){}
	virtual ~IAttackObject(){}

	/// <summary>
	/// �U���͂̐ݒ�
	/// </summary>
	/// <param name="attack"></param>
	void SetAttack(int attack)
	{
		m_attack = attack;
	}
	/// <summary>
	/// �U���͂̎擾
	/// </summary>
	/// <returns></returns>
	const int& GetAttack() const
	{
		return m_attack;
	}

	/// <summary>
	/// �ϓ�����U���͂̎擾
	/// </summary>
	/// <returns></returns>
	virtual const int& GetVariableAttack()
	{
		return 0;
	}

protected:

	/// <summary>
	/// �����蔻��̖��O���擾
	/// </summary>
	/// <returns></returns>
	const char* GetCollisionName() const
	{
		return m_collisionName;
	}

protected:
	int m_attack = 0;	//�U����

	const char* m_collisionName = "attackobject";	//�����蔻��𐶐����鎞�̖��O

};
