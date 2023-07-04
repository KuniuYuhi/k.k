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
	m_enCharacters = enHero;

	return true;
}

void Player::Update()
{
	//�L�����N�^�[�̐؂�ւ�
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		switch (m_enCharacters)
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

	
}

void Player::ChangeFromHeroToWizard()
{
	//���@�g�����A�N�e�B�u�ɂ���
	actor[enWizard]->Activate();
	//���W�A��]�A�T�C�Y���q�[���[�Ɠ����ɂ���
	actor[enWizard]->SetTransForm(
		actor[m_enCharacters]->GetPosition(),
		actor[m_enCharacters]->GetRotation(),
		actor[m_enCharacters]->GetScale()
	);
	//�L�����R���̍��W
	actor[enWizard]->SetCharaConPosition(actor[enWizard]->GetPosition());

	//�q�[���[���A�N�e�B�u�ɂ���
	actor[m_enCharacters]->Deactivate();

	//���݂̃L�����N�^�[�𖂖@�g���ɕύX����
	m_enCharacters = enWizard;
}

void Player::ChangeFromWizardToHero()
{
	//�q�[���[���A�N�e�B�u�ɂ���
	actor[enHero]->Activate();
	//���W�A��]�A�T�C�Y�𖂖@�g���Ɠ����ɂ���
	actor[enHero]->SetTransForm(
		actor[m_enCharacters]->GetPosition(),
		actor[m_enCharacters]->GetRotation(),
		actor[m_enCharacters]->GetScale()
	);
	//�L�����R���̍��W
	actor[enHero]->SetCharaConPosition(actor[enHero]->GetPosition());

	//���@�g�����A�N�e�B�u�ɂ���
	actor[m_enCharacters]->Deactivate();

	//���݂̃L�����N�^�[���q�[���[�ɕύX����
	m_enCharacters = enHero;
}
