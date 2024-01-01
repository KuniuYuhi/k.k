#pragma once
#include "IBossStateMachine.h"

class SummonerSM_Attack:public IBossStateMachine
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="summoner"></param>
	SummonerSM_Attack(Summoner* summoner)
		:IBossStateMachine(summoner)
	{
	}
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~SummonerSM_Attack(){}

	/// <summary>
	/// ���t���[���s������
	/// </summary>
	virtual void Execute() override;

private:

	

	/// <summary>
	/// �����_���Ȓl��ݒ�
	/// </summary>
	void SetRandomActionValue()
	{
		m_randomValue = rand() % randomMaxValue;
	}

	/// <summary>
	/// �A�N�V���������߂鏈��
	/// </summary>
	void ProcessDecideAction();

	/// <summary>
	/// ���̃A�N�V���������߂�
	/// </summary>
	void DecideNextAction();

	/// <summary>
	/// �{�X���A�N�V��������
	/// </summary>
	/// <returns></returns>
	bool IsInActionBoss()
	{
		return false;
	}

	/// <summary>
	/// �������U���̒�����X�e�[�g�����߂�
	/// </summary>
	void ProcessLongRangeAttack();
	/// <summary>
	/// �ߋ����U���̒�����X�e�[�g�����߂�
	/// </summary>
	void ProcessMeleeAttack();

	/// <summary>
	///	�_�[�N�{�[���̃A�j���[�V�����Ɉڂ邽�߂̏���
	/// </summary>
	void IsNextActionDarkBall();

	/// <summary>
	/// ���̍s�����_�[�N���e�I�ɂ��邩���߂�
	/// </summary>
	/// <returns>�_�[�N���e�I�Ȃ�true</returns>
	bool IsNextActionDarkMeteo();

private:
	Vector3 m_toPlayer = g_vec3Zero;

	float m_continuousAttackTimer = 0.0f;		//�ҋ@���Ԃ��v�Z����^�C�}�[

	int attackRetryProbability = 3;	//������x�U������m��
	const int randomMaxValue = 10;	//���I���鎞�̍ő�l
	bool m_lotteryChanceFlag = false;		//������x�U�����邩���I����`�����X
	int m_randomValue = 0;

	int m_meleeCount = 0;			//�ߋ����U��������x�ɃJ�E���g


};

