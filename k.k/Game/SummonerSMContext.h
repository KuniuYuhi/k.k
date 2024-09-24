#pragma once

#include "SummonerInfo.h"

class Summoner;
class ISummonerStateMachine;
class SummonerStateContext; 

using namespace SummonerStates;
using namespace SummonerSubStateMachine;

/// <summary>
/// �T���i�[�̃T�u�X�e�[�g�}�V���Ǘ��N���X
/// </summary>
class SummonerSMContext
{
public:

	SummonerSMContext();
	~SummonerSMContext();


	/// <summary>
	/// ������
	/// </summary>
	/// <param name="summoner">�T���i�[�̃C���X�^���X</param>
	/// <param name="startSubStateMchine">�ŏ��̃T�u�X�e�[�g�}�V��</param>
	void Init(
		Summoner* summoner,
		EnSubStateMachine startSubStateMchine
	);

	/// <summary>
	/// �T�u�X�e�[�g�}�V����؂�ւ���
	/// </summary>
	/// <param name="changeSubStateMachine"></param>
	void ChangeSubStateMachine(EnSubStateMachine changeSubStateMachine);


	/// <summary>
	/// ���݂̃T�u�X�e�[�g�}�V���̍X�V���������s
	/// </summary>
	void CurrentSubStateMachineUpdate();


	/// <summary>
	/// 
	/// </summary>
	bool CheckAndTransitionStateMachine();


	/// <summary>
	/// �\��X�e�[�g���擾
	/// </summary>
	/// <returns></returns>
	EnSummonerState GetReservationState()
	{
		return m_reservationState;
	}
	/// <summary>
	/// �\��X�e�[�g��ݒ�
	/// </summary>
	/// <param name="reservationState"></param>
	void SetReservationState(EnSummonerState reservationState)
	{
		m_reservationState = reservationState;
	}
	/// <summary>
	/// �\��X�e�[�g�͂��邩�H
	/// </summary>
	/// <returns>����Ȃ�true</returns>
	bool IsReservationState()
	{
		return m_reservationState != enSummonerState_None;
	}

	/// <summary>
	/// �O��̃X�e�[�g���擾
	/// </summary>
	/// <returns></returns>
	EnSummonerState GetPreviousState()
	{
		return m_previousState;
	}
	/// <summary>
	/// �O��̃X�e�[�g��ݒ�
	/// </summary>
	/// <param name="reservationState"></param>
	void SetPreviousState(EnSummonerState reservationState)
	{
		m_previousState = reservationState;
	}


	const int GetMaxHpQuotient()
	{
		return m_maxHpQuotient;
	}

private:

	/// <summary>
	/// 
	/// </summary>
	void CalcMaxHpQuotient(int maxHp);

private:


	//�T�u�X�e�[�g�}�V���}�b�v
	std::map<EnSubStateMachine, ISummonerStateMachine*> m_subStateMachineMap;

	ISummonerStateMachine* m_currentStateMachine = nullptr;		//���݂̃T�u�X�e�[�g�}�V��

	EnSubStateMachine m_enCurrentSubStateMachine;

	//�X�e�[�g�R���e�L�X�g

	EnSummonerState m_reservationState = enSummonerState_None;		//�\��X�e�[�g

	EnSummonerState m_previousState = enSummonerState_None;		//�O��̃X�e�[�g



	int m_maxHpQuotient = 0;			//�ő�HP����������


};

