#pragma once
#include "SummonerActionList.h"

class Summoner;
class ISummonerState;

class IBossStateMachine
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="bossInstance"></param>
	IBossStateMachine(Summoner* bossInstance);
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~IBossStateMachine();

	/// <summary>
	/// �C���X�^���X�𐶐�
	/// </summary>
	/// <param name="bossInstance"></param>
	static void CreateInstance(Summoner* bossInstance)
	{
		m_stateMachineInstance = new IBossStateMachine(bossInstance);
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
	static IBossStateMachine* GetInstance()
	{
		return m_stateMachineInstance;
	}

	/// <summary>
	/// �����_���Ȓl��ݒ�
	/// </summary>
	void SetRandomActionValue()
	{
		m_randomValue = rand() % randomMaxValue;
	}



	/// <summary>
	/// ���t���[���s������
	/// </summary>
	void Execute();

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

private:
	/// <summary>
	/// �U���X�e�[�g�����߂�
	/// </summary>
	void ProcessAttackState();
	/// <summary>
	///	�ҋ@�X�e�[�g�����߂�
	/// </summary>
	void ProcessWaitState();

	/// <summary>
	/// �������U���̒�����X�e�[�g�����߂�
	/// </summary>
	void ProcessLongRangeAttack();
	/// <summary>
	/// �ߋ����U���̒�����X�e�[�g�����߂�
	/// </summary>
	void ProcessMeleeAttack();



	/// <summary>
	/// �v���C���[���߂��ɂƂǂ܂��Ă���^�C�}�[�����Z����
	/// </summary>
	void AddStayPlayerTimer();

private:
	static IBossStateMachine* m_stateMachineInstance;

	EnActionName m_oldActionName;


	Summoner* m_summoner = nullptr;

	Vector3 m_toPlayer = g_vec3Zero;

	float m_waitTimer = 0.0f;		//�ҋ@���Ԃ��v�Z����^�C�}�[

	int attackRetryProbability = 3;	//������x�U������m��
	const int randomMaxValue = 10;	//���I���鎞�̍ő�l
	bool m_lotteryChanceFlag = false;		//������x�U�����邩���I����`�����X
	int m_randomValue = 0;



	int m_meleeCount = 0;			//�ߋ����U��������x�ɃJ�E���g
	float m_stayPlayerTimer = 0.0f;	//�v���C���[���߂��ɂ���Ԃ��v��^�C�}�[
	float m_accelerateStayPlayerTimer = 1.0f;	//�v���C���[���߂��ɂ���Ԃ��v��^�C�}�[������������
	
};

