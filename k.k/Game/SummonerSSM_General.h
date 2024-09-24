#pragma once
#include "ISummonerStateMachine.h"

class Summoner;
class SummonerSMContext;

/// <summary>
/// ��{�s���T�u�X�e�[�g�}�V��
/// </summary>
class SummonerSSM_General : public ISummonerStateMachine
{
public:

	SummonerSSM_General(Summoner* summoner, SummonerSMContext* context)
		:ISummonerStateMachine(summoner,context)
	{}


	void Entry() override;

	void Update() override;

	void Exit() override;

private:
	/// <summary>
	/// �X�e�[�g�}�V���؂�ւ��^�C�}�[�̌v�Z
	/// </summary>
	void CalcStateMachineChangeTimer();

	/// <summary>
	/// �ړ��p�^�[�������߂�
	/// </summary>
	void CheckMovePattern();
	/// <summary>
	/// �ړ��p�^�[���^�C�}�[�̌v�Z
	/// </summary>
	void CalcMovePatternCheckTime();

	/// <summary>
	/// �A�^�b�N�X�e�[�g�}�V�����؂�ւ��邩�`�F�b�N����
	/// </summary>
	void CheckEnableChangeAttackStateMachine();

	/// <summary>
	/// �󋵂𔻒f����B�󋵂ɂ���Ă̓X�e�[�g��؂�ւ���
	/// </summary>
	bool EvaluateSituation();


	/// <summary>
	/// ���[�v�X�e�[�g�ɐ؂�ւ��邩�`�F�b�N����
	/// </summary>
	/// <returns>�؂�ւ���Ȃ�true</returns>
	bool CheckChangeWarpState();


	/// <summary>
	/// �����̃^�C�}�[�����Z����
	/// </summary>
	/// <param name="timer"></param>
	void AddTimer(float& timer);

	/// <summary>
	/// �v���C���[���ߋ����ɂ���ԃ^�C�}�[���X�V����v
	/// </summary>
	void UpdateCloseDistanceTimer();


private:

	float m_stateMachineChangeTimeLimit = 5.0f;
	float m_stateMachineChangeTimer = 0.0f;
	bool m_isEnableChangeStateMachine = false;

	float m_movePatternCheckTimer = 0.0f;
	float m_movePatternCheckTimeLimit = 5.0f;
	bool m_isMovePatternCheck = false;


	float m_warpCoolTimer = 0.0f;
	float m_warpCoolTimeLimit = 18.0f;


	float m_closeDistanceTimer = 0.0f;


	int m_summonerHp = 0;

};

