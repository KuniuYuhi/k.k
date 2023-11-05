#include "stdafx.h"
#include "Player.h"
#include "Wizard.h"
#include "Actor.h"
#include "Game.h"

#include "Brave.h"

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

	//���݂̃L�����N�^�[��E�҂ɐݒ肷��
	m_nowActor = m_brave;

	//�L�����N�^�[�̍��W��ݒ�
	m_nowActor->SetPosition(START_POSITION);

	//���W�̐ݒ�
	m_position = m_nowActor->GetPosition();

	return true;
}

void Player::Update()
{
	//�Q�[�����n�܂�܂ł͈ړ����Ȃ�
	if (m_game->GetNowGameState() != Game::enGameState_Game)
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
	//���������� 
	if (m_game->GetEnOutCome() == Game::enOutCome_Player_Win)
	{
		//�������̏������s
		m_nowActor->ProcessWin();
		//
		m_decisionOutComeFlag = true;
		return true;
	}
	//��������
	if (m_game->GetEnOutCome() == Game::enOutCome_Player_Lose)
	{
		m_decisionOutComeFlag = true;
		return true;
	}
	//�Q�[�����łȂ��Ȃ�
	if (m_game->IsMatchGameState(Game::enGameState_Game) != true)
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
	if (m_nowActor->GetDieFlag() == true)
	{
		return true;
	}
	return false;
}


int Player::GetAtk()
{
	return m_nowActor->GetStatus().atk;
}

bool Player::IsComboStateSame()
{
	return m_nowActor->IsComboStateSame();
}

void Player::SetDamagedComboState(Actor::EnComboState damagedcombostate)
{
	m_nowActor->SetDamagedComboState(damagedcombostate);
}

Actor::EnComboState Player::GetNowComboState() const
{
	return m_nowActor->GetNowComboState();
}



