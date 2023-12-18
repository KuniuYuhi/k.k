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
	void Execute();

private:
	/// <summary>
	/// �A�N�V���������߂鏈��
	/// </summary>
	void ProcessDecideAction();

	/// <summary>
	/// ���̃A�N�V���������߂�
	/// </summary>
	void DecideNextAction();


	/// <summary>
	/// �U���X�e�[�g�����߂�
	/// </summary>
	void ProcessAttackState();
	/// <summary>
	/// ����X�e�[�g�����߂�
	/// </summary>
	void ProcessPatrol();

	/// <summary>
	/// �ǂ�������X�e�[�g
	/// </summary>
	void ProcessChase();



private:

	MobMonster* m_mobMonster = nullptr;

};

