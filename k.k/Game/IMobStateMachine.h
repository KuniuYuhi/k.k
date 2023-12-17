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
	/// �C���X�^���X�𐶐�
	/// </summary>
	/// <param name="bossInstance"></param>
	static void CreateInstance(MobMonster* mobInstance)
	{
		m_stateMachineInstance = new IMobStateMachine(mobInstance);
	}
	/// <summary>
	/// �C���X�^���X�̍폜
	/// </summary>
	static void DeleteInstance()
	{
		delete m_stateMachineInstance;
		m_stateMachineInstance = nullptr;
	}

	/// <summary>
	/// �C���X�^���X�̎擾
	/// </summary>
	/// <returns></returns>
	static IMobStateMachine* GetInstance()
	{
		return m_stateMachineInstance;
	}

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
	static IMobStateMachine* m_stateMachineInstance;

	MobMonster* m_mobMonster = nullptr;

};

