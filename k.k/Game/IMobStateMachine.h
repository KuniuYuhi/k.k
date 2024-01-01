#pragma once

class MobMonster;

class IMobStateMachine
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="mobMonsterInstance"></param>
	IMobStateMachine(MobMonster* mobMonsterInstance);
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~IMobStateMachine();

	/// <summary>
	/// ���t���[���s������
	/// </summary>
	virtual void Execute(){}

protected:

	MobMonster* m_mobMonster = nullptr;

};

