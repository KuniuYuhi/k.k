#pragma once
#include "IBossStateMachine.h"

class SummonerSM_Vigilance :public IBossStateMachine
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="summoner"></param>
	SummonerSM_Vigilance(Summoner* summoner,bool saveTimerlesetFlag = false)
		:IBossStateMachine(summoner)
	{
		Init(saveTimerlesetFlag);
	}
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~SummonerSM_Vigilance();

	/// <summary>
	/// ���t���[���s������
	/// </summary>
	virtual void Execute() override;



private:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="saveTimerlesetFlag">�X�e�C�v���C���[�^�C�}�[�����Z�b�g���邩</param>
	void Init(bool saveTimerlesetFlag);

	/// <summary>
	/// �A�N�V���������߂鏈��
	/// </summary>
	void ProcessDecideAction();

	/// <summary>
	///	�ҋ@�X�e�[�g�����߂�
	/// </summary>
	void ProcessWaitState();

	/// <summary>
	/// �{�X���A�N�V��������
	/// </summary>
	/// <returns></returns>
	bool IsInActionBoss()
	{
		return false;
	}

	/// <summary>
	/// �m�b�N�o�b�N���邩���߂鏈��
	/// </summary>
	/// <returns>�m�b�N�o�b�N����Ȃ�true</returns>
	bool IsKnockBack();
	/// <summary>
	/// �m�b�N�o�b�N�^�C�}�[�����������邩
	/// </summary>
	void IsKnockBackTimerAccelerate();

	/// <summary>
	/// �v���C���[���߂��ɂƂǂ܂��Ă���^�C�}�[�����Z����
	/// </summary>
	void AddStayPlayerTimer();

	/// <summary>
	/// �v���C���[��ǂ������邩
	/// </summary>
	/// <returns>�ǂ�������Ȃ�true,�ҋ@�Ȃ�false</returns>
	bool IsChasePlayer();

	


private:
	Vector3 m_toPlayer = g_vec3Zero;

	bool m_idleStateFlag = true;

	static float m_decideActionTimer;


	float m_waitTimer = 0.0f;		//�ҋ@���Ԃ��v�Z����^�C�}�[

	
	float m_accelerateStayPlayerTimer = 1.0f;	//�v���C���[���߂��ɂ���Ԃ��v��^�C�}�[������������


};

