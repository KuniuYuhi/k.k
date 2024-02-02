#pragma once
#include "SummonerActionList.h"

class Summoner;
class ISummonerState;

using namespace SummonerActions;

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
	virtual ~IBossStateMachine();

	/// <summary>
	/// ���t���[���s������
	/// </summary>
	virtual void Execute(){}


	/// <summary>
	/// �A���U���񐔂̐ݒ�
	/// </summary>
	/// <param name="count"></param>
	void SetContinuousAttackCount(int count)
	{
		m_continuousAttackCount = count;
	}

	/// <summary>
	/// �A���U���񐔂̎擾
	/// </summary>
	/// <returns></returns>
	const int& GetContinuousAttackCount() const
	{
		return m_continuousAttackCount;
	}


	/// <summary>
	/// �X�e�C�v���C���[�^�C�}�[��ݒ�
	/// </summary>
	/// <param name="timer"></param>
	void SetSaveStayPlayerTimer(int timer)
	{
		m_stayPlayerTimer = timer;
	}



protected:
	



protected:

	Summoner* m_summoner = nullptr;

	int m_continuousAttackCount = 0;	//�A���U���J�E���g

	static float m_stayPlayerTimer;     //�v���C���[���߂��ɂ���Ԃ��v��^�C�}�[



};

