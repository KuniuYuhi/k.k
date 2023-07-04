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
	m_enCharacters = enHero;

	return true;
}

void Player::Update()
{
	//キャラクターの切り替え
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
	//魔法使いをアクティブにする
	actor[enWizard]->Activate();
	//座標、回転、サイズをヒーローと同じにする
	actor[enWizard]->SetTransForm(
		actor[m_enCharacters]->GetPosition(),
		actor[m_enCharacters]->GetRotation(),
		actor[m_enCharacters]->GetScale()
	);
	//キャラコンの座標
	actor[enWizard]->SetCharaConPosition(actor[enWizard]->GetPosition());

	//ヒーローを非アクティブにする
	actor[m_enCharacters]->Deactivate();

	//現在のキャラクターを魔法使いに変更する
	m_enCharacters = enWizard;
}

void Player::ChangeFromWizardToHero()
{
	//ヒーローをアクティブにする
	actor[enHero]->Activate();
	//座標、回転、サイズを魔法使いと同じにする
	actor[enHero]->SetTransForm(
		actor[m_enCharacters]->GetPosition(),
		actor[m_enCharacters]->GetRotation(),
		actor[m_enCharacters]->GetScale()
	);
	//キャラコンの座標
	actor[enHero]->SetCharaConPosition(actor[enHero]->GetPosition());

	//魔法使いを非アクティブにする
	actor[m_enCharacters]->Deactivate();

	//現在のキャラクターをヒーローに変更する
	m_enCharacters = enHero;
}
