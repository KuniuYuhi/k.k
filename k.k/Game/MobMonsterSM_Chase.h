#pragma once
#include "IMobStateMachine.h"

class MobMonsterSM_Chase:public IMobStateMachine
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="mobmonster"></param>
	MobMonsterSM_Chase(MobMonster* mobmonster)
		:IMobStateMachine(mobmonster)
	{
	}

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~MobMonsterSM_Chase(){}

	/// <summary>
	/// ���t���[���s������
	/// </summary>
	void Execute() override;

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
	/// �ǂ�������X�e�[�g
	/// </summary>
	void ProcessChase();

private:

};

