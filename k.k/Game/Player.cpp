#include "stdafx.h"
#include "Player.h"
#include "Game.h"

#include "GameManager.h"
#include "CharactersInfoManager.h"

namespace {
	const Vector3 START_POSITION = { 0.0f, 0.0f, -400.0f };
}

//todo �d��
Player::Player()
{
}

Player::~Player()
{
	DeleteGO(m_brave);
}

bool Player::Start()
{
	//�Q�[���N���X�ƗE�҃N���X�̃C���X�^���X������
	m_game = FindGO<Game>("game");


	m_brave = NewGO<Brave>(0, "brave");
	m_brave->SetPosition(m_position);

	//�L�����N�^�[�̍��W��ݒ�
	m_brave->SetPosition(START_POSITION);

	return true;
}

void Player::Update()
{
	//�Q�[�����n�܂�܂ł͈ړ����Ȃ�
	/*if (GameManager::GetInstance()->GetGameSeenState()!=
		GameManager::enGameSeenState_Game)
	{
		return;
	}*/

	if (IsInaction() == true)
	{
		return;
	}

	//�s���\�ɂ���
	if (m_dontActionFlag != false)
	{
		SetDontActionFlag(false);
	}
}

bool Player::IsInaction()
{
	if (m_decisionOutComeFlag == true)
	{
		return true;
	}

	//////////////////////////////////////////////////
	// �s���o���Ȃ��Ȃ����
	////////////////////////////////////////////////// 
	//�����������Q�[���I����̏������I������Ȃ�
	if (GameManager::GetInstance()->GetPlayerWinFlag()==true&&
		GameManager::GetInstance()->GetGameFinishProcessEndFlag())
	{
		//�������̏������s
		m_brave->ProcessWin();
		//���s����
		m_decisionOutComeFlag = true;
		return true;
	}
	//��������
	if (GameManager::GetInstance()->GetPlayerLoseFlag() == true)
	{
		//���s����
		m_decisionOutComeFlag = true;
		//�A�C�h����ԂɑJ��
		m_brave->ForciblyIdleAnim();
		//���f�����_�ł��Ȃ��悤�ɂ���
		m_brave->SetInvicibleTimeFlag(false);
		return true;
	}
	//�Q�[�����łȂ��Ȃ�
	if (GameManager::GetInstance()->GetGameSeenState() !=
		GameManager::enGameSeenState_Game)
	{
		return true;
	}
	//////////////////////////////////////////////////
	// �s���ł���
	//////////////////////////////////////////////////
	return false;
}

bool Player::IsDeadPlayer()
{
	//�v���C���[�����ꂽ��
	if (m_brave->GetDieFlag() == true)
	{
		return true;
	}
	return false;
}

int Player::GetAtk()
{
	return m_brave->GetStatus().GetAtk();
}

bool Player::IsComboStateSame()
{
	return m_brave->IsComboStateSame();
}

void Player::SetDamagedComboState(Actor::EnComboState damagedcombostate)
{
	m_brave->SetDamagedComboState(damagedcombostate);
}

Actor::EnComboState Player::GetNowComboState() const
{
	return m_brave->GetNowComboState();
}

void Player::ProcessPlayerDead()
{
	//����Ă��Ȃ��Ȃ珈�����Ȃ�
	if (m_isPlayerDeadFlag != true)
	{
		return;
	}




}



