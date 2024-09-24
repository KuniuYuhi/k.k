#include "stdafx.h"
#include "SummonerSMContext.h"

#include "ISummonerStateMachine.h"

#include "SummonerSSM_General.h"
#include "SummonerSSM_Offensive.h"

#include "Summoner.h"


namespace {
	const int QUOTIENT_VALUE = 4;
}

SummonerSMContext::SummonerSMContext()
{
}

SummonerSMContext::~SummonerSMContext()
{
}

void SummonerSMContext::Init(
	Summoner* summoner, EnSubStateMachine startSubStateMchine)
{
	//�ő�HP�������������v�Z
	CalcMaxHpQuotient(summoner->GetCommonStatus().GetMaxHp());

	//�X�e�[�g�}�V�����}�b�v�ɒǉ�����
	m_subStateMachineMap.insert(std::make_pair(enSubStateMachine_General, new SummonerSSM_General(summoner,this)));
	m_subStateMachineMap.insert(std::make_pair(enSubStateMachine_Offensive, new SummonerSSM_Offensive(summoner,this)));

	//�X�e�[�g�}�V����؂�ւ���
	ChangeSubStateMachine(startSubStateMchine);
}

void SummonerSMContext::ChangeSubStateMachine(EnSubStateMachine changeSubStateMachine)
{
	if (m_currentStateMachine != nullptr)
	{
		m_currentStateMachine->Exit();
	}

	m_currentStateMachine = nullptr;

	//�}�b�v����L�[���g���đΉ�����T�u�X�e�[�g�}�V���������Ă���
	m_currentStateMachine = m_subStateMachineMap.at(changeSubStateMachine);

	if (m_currentStateMachine == nullptr) std::abort();

	//���݂̃T�u�X�e�[�g�}�V���̍ŏ��̏��������s
	m_currentStateMachine->Entry();

	m_enCurrentSubStateMachine = changeSubStateMachine;
}

void SummonerSMContext::CurrentSubStateMachineUpdate()
{
	if (m_currentStateMachine == nullptr) return;

	//�T�u�X�e�[�g�}�V���̍X�V����
	m_currentStateMachine->Update();
}

bool SummonerSMContext::CheckAndTransitionStateMachine()
{
	return m_currentStateMachine->IsEnableChangeStateMachine();
}

void SummonerSMContext::CalcMaxHpQuotient(int maxHp)
{
	m_maxHpQuotient = maxHp / QUOTIENT_VALUE;
}
