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
	DeleteGO(m_hero);
	DeleteGO(m_wizard);
}

bool Player::Start()
{
	m_MpFont.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_MpFont.SetPosition(-800.0f, -300.0f);
	m_MpFont.SetScale(2.0f);


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
	//MPの表示
	int NowActorMP = m_nowActor->GetStatus().mp;
	int NowActorMaxMP = m_nowActor->GetStatus().maxMp;
	wchar_t MP[255];
	swprintf_s(MP, 255, L"MP %3d/%d", NowActorMP, NowActorMaxMP);
	m_MpFont.SetText(MP);

	//特定のアニメーションが再生中の時は入れ替え可能
	if (m_nowActor->isAnimationSwappable() != true)
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
	}

	

	//キャラコン

	//m_position = actor[m_enActiveCharacter]->GetPosition();
	//移動処理
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
	//m_nowActor->SetAnimationState();

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

void Player::Render(RenderContext& rc)
{
	m_MpFont.Draw(rc);
}

