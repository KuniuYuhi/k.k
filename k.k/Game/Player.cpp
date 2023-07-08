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
	//現在のキャラクターをヒーローに設定する
	m_nowActor = actor[m_enActiveCharacter];

	//座標の設定
	m_position = m_nowActor->GetPosition();

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
			ChangeCharacter(enWizard);
			break;
		case enWizard:
			ChangeCharacter(enHero);
			break;

		default:
			break;
		}
	}

	//キャラコン

	//m_position = actor[m_enActiveCharacter]->GetPosition();

	m_moveSpeed = m_nowActor->calcVelocity(m_nowActor->GetStatus());

	m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	
}

void Player::ChangeCharacter(EnCharacters nextCharacter)
{
	// まずは現在のアクターを非アクティブにする。
	m_nowActor->Deactivate();

	// アクターを切り替える
	m_nowActor = actor[nextCharacter];

	//アクターのアニメーションを待機ステートにする
	m_nowActor->SetAnimationState();

	m_nowActor->Activate();
	//座標、回転、サイズをヒーローと同じにする
	m_nowActor->SetTransForm(
		actor[m_enActiveCharacter]->GetPosition(),
		actor[m_enActiveCharacter]->GetRotation()
	);
	//m_charaCon.SetRadius(50.0f);
	//キャラコンの座標
	m_nowActor->SetCharaConPosition(m_nowActor->GetPosition());
	

	//現在のキャラクターを魔法使いに変更する
	m_enActiveCharacter = nextCharacter;

}