#include "stdafx.h"
#include "BraveStateContext.h"


#include "BraveAllStateFiles.h"



BraveStateContext::BraveStateContext()
{
}

BraveStateContext::~BraveStateContext()
{
	m_braveStateMap.clear();
}

void BraveStateContext::Init(Brave* brave, EnBraveState startState)
{
	//IBraveState* setState = new BraveState_Idle(brave);
	//�X�e�[�g���}�b�v�Ɋi�[����
	m_braveStateMap.insert(std::make_pair(enBraveState_Idle, new BraveState_Idle(brave,this)));
	m_braveStateMap.insert(std::make_pair(enBraveState_Sprint, new BraveState_Sprint(brave, this)));
	m_braveStateMap.insert(std::make_pair(enBraveState_FirstAttack, new BraveState_FirstAttack(brave, this)));
	m_braveStateMap.insert(std::make_pair(enBraveState_SecondAttack, new BraveState_SecondAttack(brave, this)));
	m_braveStateMap.insert(std::make_pair(enBraveState_ThirdAttack, new BraveState_ThirdAttack(brave, this)));
	m_braveStateMap.insert(std::make_pair(enBraveState_SkillStart, new BraveState_SkillStart(brave, this)));
	m_braveStateMap.insert(std::make_pair(enBraveState_SkillMain, new BraveState_SkillMain(brave, this)));
	m_braveStateMap.insert(std::make_pair(enBraveState_Hit, new BraveState_Hit(brave, this)));
	m_braveStateMap.insert(std::make_pair(enBraveState_DefensiveActions, new BraveState_DefensiveAction(brave, this)));
	m_braveStateMap.insert(std::make_pair(enBraveState_DefensiveHit, new BraveState_DefensiveHit(brave, this)));
	m_braveStateMap.insert(std::make_pair(enBraveState_Die, new BraveState_Die(brave, this)));
	m_braveStateMap.insert(std::make_pair(enBraveState_ChangeWeapon, new BraveState_ChangeWeapon(brave, this)));
	m_braveStateMap.insert(std::make_pair(enBraveState_WinStart, new BraveState_WinStart(brave, this)));
	m_braveStateMap.insert(std::make_pair(enBraveState_WinMain, new BraveState_WinMain(brave, this)));


	//�X�e�[�g��؂�ւ���
	ChangeBraveState(startState);

}

void BraveStateContext::ChangeBraveState(EnBraveState changeState)
{
	//�O�̃X�e�[�g��ۑ����Ă���
	m_EnPreviousBraveState = m_EnCurrentBraveState;

	if (m_currentBraveState != nullptr)
	{
		//���݂̃X�e�[�g�̍Ō�̏������s��
		m_currentBraveState->Exit();
	}

	//null�ɂ���
	m_currentBraveState = nullptr;

	//�}�b�v����L�[�ɑΉ�����X�e�[�g�������Ă���
	//�V�����X�e�[�g�ɓ���ւ���
	m_currentBraveState = m_braveStateMap.at(changeState);
	//�L�[���Ή����ĂȂ�������
	if (m_currentBraveState == nullptr)
	{
		std::abort();
		return;
	}

	//�ŏ��̏��������s
	m_currentBraveState->Entry();

	//���݂̃X�e�[�g�̒l��ۑ�
	m_EnCurrentBraveState = changeState;

}

void BraveStateContext::ChangeBraveState(int changeStateNum)
{
	EnBraveState tempState = (EnBraveState)changeStateNum;


	//�O�̃X�e�[�g��ۑ����Ă���
	m_EnPreviousBraveState = m_EnCurrentBraveState;

	if (m_currentBraveState != nullptr)
	{
		//���݂̃X�e�[�g�̍Ō�̏������s��
		m_currentBraveState->Exit();
	}

	//null�ɂ���
	m_currentBraveState = nullptr;

	//�}�b�v����L�[�ɑΉ�����X�e�[�g�������Ă���
	//�V�����X�e�[�g�ɓ���ւ���
	m_currentBraveState = m_braveStateMap.at(tempState);
	//�L�[���Ή����ĂȂ�������
	if (m_currentBraveState == nullptr)
	{
		std::abort();
		return;
	}

	//�ŏ��̏��������s
	m_currentBraveState->Entry();

	//���݂̃X�e�[�g�̒l��ۑ�
	m_EnCurrentBraveState = tempState;
}

void BraveStateContext::UpdateCurrentState()
{
	if (m_currentBraveState == nullptr)
	{
		return;
	}
	//���݂̃X�e�[�g�̍X�V����
	m_currentBraveState->Ubdate();

}

void BraveStateContext::PlayAnimationCurrentState()
{
	if (m_currentBraveState == nullptr)
	{
		return;
	}
	//�A�j���[�V�������Đ�
	m_currentBraveState->PlayAnimation();
}
