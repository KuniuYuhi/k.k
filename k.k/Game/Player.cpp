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

	//初期キャラクターをヒーローに設定
	m_enActiveCharacter = enHero;

	//座標の設定
	m_position = actor[m_enActiveCharacter]->GetPosition();

	//キャラコンの設定
	m_charaCon.Init(12.0f, 33.0f, m_position);

	

	return true;
}

void Player::Update()
{
	//キャラクターの切り替え
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

	//キャラコン

	//m_position = actor[m_enActiveCharacter]->GetPosition();

	m_moveSpeed = actor[m_enActiveCharacter]->calcVelocity(actor[m_enActiveCharacter]->GetStatus());

	m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	
}

void Player::ChangeFromHeroToWizard()
{
	//魔法使いをアクティブにする
	actor[enWizard]->Activate();
	//座標、回転、サイズをヒーローと同じにする
	actor[enWizard]->SetTransForm(
		actor[m_enActiveCharacter]->GetPosition(),
		actor[m_enActiveCharacter]->GetRotation()
	);
	//m_charaCon.SetRadius(50.0f);
	//キャラコンの座標
	actor[enWizard]->SetCharaConPosition(actor[enWizard]->GetPosition());

	//ヒーローを非アクティブにする
	actor[m_enActiveCharacter]->Deactivate();


	//現在のキャラクターを魔法使いに変更する
	m_enActiveCharacter = enWizard;
}

void Player::ChangeFromWizardToHero()
{
	//ヒーローをアクティブにする
	actor[enHero]->Activate();
	//座標、回転、サイズを魔法使いと同じにする
	actor[enHero]->SetTransForm(
		actor[m_enActiveCharacter]->GetPosition(),
		actor[m_enActiveCharacter]->GetRotation(),
		actor[enHero]->GetScale()
	);
	//m_charaCon.SetRadius(33.0f);
	//キャラコンの座標
	actor[enHero]->SetCharaConPosition(actor[enHero]->GetPosition());

	//魔法使いを非アクティブにする
	actor[m_enActiveCharacter]->Deactivate();

	//現在のキャラクターをヒーローに変更する
	m_enActiveCharacter = enHero;
}
