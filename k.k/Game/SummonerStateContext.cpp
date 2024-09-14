#include "stdafx.h"
#include "SummonerStateContext.h"

#include "ISummonerState.h"

#include "SummonerState_Appear.h"
#include "SummonerState_Idle.h"
#include "SummonerState_Run.h"
#include "SummonerState_DarkBall.h"
#include "SummonerState_DarkMeteorite.h"
#include "SummonerState_FirstAttack.h"
#include "SummonerState_SecondAttack.h"
#include "SummonerState_ThirdAttack.h"
#include "SummonerState_ShockWave.h"
#include "SummonerState_Hit.h"
#include "SummonerState_Die.h"
#include "SummonerState_Victory.h"
#include "SummonerState_Warp.h"


SummonerStateContext::SummonerStateContext()
{
}

SummonerStateContext::~SummonerStateContext()
{
	m_summonerStateMap.clear();
	m_currentState = nullptr;
}

void SummonerStateContext::Init(Summoner* summoner, EnSummonerState startState)
{
	
	m_summonerStateMap.insert(std::make_pair(enSummonerState_Appear, new SummonerState_Appear(summoner, this)));
	m_summonerStateMap.insert(std::make_pair(enSummonerState_Idle, new SummonerState_Idle(summoner, this)));
	m_summonerStateMap.insert(std::make_pair(enSummonerState_Run, new SummonerState_Run(summoner, this)));
	m_summonerStateMap.insert(std::make_pair(enSummonerState_DarkBall, new SummonerState_DarkBall(summoner, this)));
	m_summonerStateMap.insert(std::make_pair(enSummonerState_Attack_DarkMeteorite, new SummonerState_DarkMeteorite(summoner, this)));
	m_summonerStateMap.insert(std::make_pair(enSummonerState_FirstAttack, new SummonerState_FirstAttack(summoner, this)));
	m_summonerStateMap.insert(std::make_pair(enSummonerState_SecondAttack, new SummonerState_SecondAttack(summoner, this)));
	m_summonerStateMap.insert(std::make_pair(enSummonerState_ThirdAttack, new SummonerState_ThirdAttack(summoner, this)));
	m_summonerStateMap.insert(std::make_pair(enSummonerState_ShockWave, new SummonerState_ShockWave(summoner, this)));
	m_summonerStateMap.insert(std::make_pair(enSummonerState_Hit, new SummonerState_Hit(summoner, this)));
	m_summonerStateMap.insert(std::make_pair(enSummonerState_Die, new SummonerState_Die(summoner, this)));
	m_summonerStateMap.insert(std::make_pair(enSummonerState_Victory, new SummonerState_Victory(summoner, this)));
	m_summonerStateMap.insert(std::make_pair(enSummonerState_Warp, new SummonerState_Warp(summoner, this)));

	ChangeState(startState);
}

void SummonerStateContext::ChangeState(EnSummonerState changeState)
{
	//�O�̃X�e�[�g��ۑ����Ă���
	m_EnPreviousState = m_EnCurrentState;

	if (m_currentState != nullptr)
	{
		//���݂̃X�e�[�g�̍Ō�̏������s��
		m_currentState->Exit();
	}

	//null�ɂ���
	m_currentState = nullptr;

	//�}�b�v����L�[�ɑΉ�����X�e�[�g�������Ă���
	//�V�����X�e�[�g�ɓ���ւ���
	m_currentState = m_summonerStateMap.at(changeState);
	//�L�[���Ή����ĂȂ�������
	if (m_currentState == nullptr)
	{
		std::abort();
		return;
	}

	//�ŏ��̏��������s
	m_currentState->Entry();

	//���݂̃X�e�[�g�̒l��ۑ�
	m_EnCurrentState = changeState;

}

void SummonerStateContext::UpdateCurrentState()
{
	if (m_currentState == nullptr) return;

	m_currentState->Ubdate();
}

void SummonerStateContext::PlayAnimationCurrentState()
{
	if (m_currentState == nullptr) return;

	m_currentState->PlayAnimation();
}
