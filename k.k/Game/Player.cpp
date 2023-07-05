#include "stdafx.h"
#include "Player.h"
#include "Hero.h"
#include "Wizard.h"
#include "Actor.h"

Player::Player()
{
}

Player::~Player()
{
}

bool Player::Start()
{
	m_hero = NewGO<Hero>(0, "hero");
	m_wizard = NewGO<Wizard>(0, "wizard");


	actor[enHero] = m_hero;
	actor[enWizard] = m_wizard;

	//�����L�����N�^�[���q�[���[�ɐݒ�
	m_enActiveCharacter = enHero;

	//���W�̐ݒ�
	m_position = actor[m_enActiveCharacter]->GetPosition();

	//�L�����R���̐ݒ�
	m_charaCon.Init(12.0f, 33.0f, m_position);

	

	return true;
}

void Player::Update()
{
	//�L�����N�^�[�̐؂�ւ�
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		switch (m_enActiveCharacter)
		{
		case enHero:
			ChangeFromHeroToWizard();
			break;
		case enWizard:
			ChangeFromWizardToHero();
			break;

		default:
			break;
		}
	}

	//�L�����R��

	//m_position = actor[m_enActiveCharacter]->GetPosition();

	m_moveSpeed = actor[m_enActiveCharacter]->calcVelocity(actor[m_enActiveCharacter]->GetStatus());

	m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	
}

void Player::ChangeFromHeroToWizard()
{
	//���@�g�����A�N�e�B�u�ɂ���
	actor[enWizard]->Activate();
	//���W�A��]�A�T�C�Y���q�[���[�Ɠ����ɂ���
	actor[enWizard]->SetTransForm(
		actor[m_enActiveCharacter]->GetPosition(),
		actor[m_enActiveCharacter]->GetRotation()
	);
	//m_charaCon.SetRadius(50.0f);
	//�L�����R���̍��W
	actor[enWizard]->SetCharaConPosition(actor[enWizard]->GetPosition());

	//�q�[���[���A�N�e�B�u�ɂ���
	actor[m_enActiveCharacter]->Deactivate();


	//���݂̃L�����N�^�[�𖂖@�g���ɕύX����
	m_enActiveCharacter = enWizard;
}

void Player::ChangeFromWizardToHero()
{
	//�q�[���[���A�N�e�B�u�ɂ���
	actor[enHero]->Activate();
	//���W�A��]�A�T�C�Y�𖂖@�g���Ɠ����ɂ���
	actor[enHero]->SetTransForm(
		actor[m_enActiveCharacter]->GetPosition(),
		actor[m_enActiveCharacter]->GetRotation(),
		actor[enHero]->GetScale()
	);
	//m_charaCon.SetRadius(33.0f);
	//�L�����R���̍��W
	actor[enHero]->SetCharaConPosition(actor[enHero]->GetPosition());

	//���@�g�����A�N�e�B�u�ɂ���
	actor[m_enActiveCharacter]->Deactivate();

	//���݂̃L�����N�^�[���q�[���[�ɕύX����
	m_enActiveCharacter = enHero;
}
