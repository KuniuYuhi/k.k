#include "stdafx.h"
#include "Player.h"
#include "Game.h"

#include "GameManager.h"
#include "CharactersInfoManager.h"

#include "GameSceneManager.h"
#include "AllGameSceneState.h"



namespace {
	const Vector3 START_POSITION = { 0.0f, 0.0f, -400.0f };
}

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
	//���s�����܂��Ă�����
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


	//�Q�[���V�[���X�e�[�g�ł��
	if (GameSceneManager::GetInstance()->GetCurrentGameSceneState() != enGameSceneState_Game)
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

int Player::GetSkillAtk()
{
	return m_brave->GetSkillAtk();
}

void Player::SetKnockBackInfo(
	bool flag, Vector3 endPos, float KnockBackPower, float knockBackLimmit)
{
	m_brave->SetHitKnockBackFlag(flag);
	m_brave->SettingKnockBackInfo(endPos, KnockBackPower, knockBackLimmit);

}

void Player::ProcessPlayerDead()
{
	//����Ă��Ȃ��Ȃ珈�����Ȃ�
	if (m_isPlayerDeadFlag != true)
	{
		return;
	}
}



