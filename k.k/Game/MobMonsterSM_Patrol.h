#pragma once
#include "IMobStateMachine.h"


class MobMonsterSM_Patrol:public IMobStateMachine
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="mobmonster"></param>
	MobMonsterSM_Patrol(MobMonster* mobmonster)
		:IMobStateMachine(mobmonster)
	{
	}

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~MobMonsterSM_Patrol() {}

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
	/// ����X�e�[�g�����߂�
	/// </summary>
	void ProcessPatrol();

private:


};

