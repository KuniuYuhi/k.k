#include "stdafx.h"
#include "Player.h"
#include "Hero.h"
#include "Wizard.h"
#include "Actor.h"
#include "Game.h"

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
	m_mpFont.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_mpFont.SetPosition(-800.0f, -300.0f);
	m_mpFont.SetScale(1.5f);

	m_hpFont.SetColor(0.0f, 1.0f, 0.0f, 1.0f);
	m_hpFont.SetPosition(-800.0f, -200.0f);
	m_hpFont.SetScale(1.5f);


	m_hero = NewGO<Hero>(0, "hero");
	m_wizard = NewGO<Wizard>(0, "wizard");


	actor[enHero] = m_hero;
	actor[enWizard] = m_wizard;

	
	//初期キャラクターをヒーローに設定
	m_enActiveCharacter = enHero;
	//サブのキャラクターをウィザードに設定
	m_subActor = actor[enWizard];

	//現在のキャラクターをヒーローに設定する
	m_nowActor = actor[m_enActiveCharacter];

	//座標の設定
	m_position = m_nowActor->GetPosition();

	//キャラコンの設定
	m_charaCon.Init(12.0f, 33.0f, m_position);


	/*RigidBodyInitData rbid;
	rbid.collider = m_charaCon.GetCollider();
	rbid.mass = 20.0f;
	rbid.pos = m_position;
	rbid.restitution = 200.0f;
	rbid.rot = m_nowActor->GetRotation();

	rigitBody.Init(rbid);
	rigitBody.SetLinearFactor(1.0f, 0.0f, 1.0f);
	rigitBody.SetAngularFactor(0.0f, 1.0f, 0.0f);*/


	return true;
}

void Player::Update()
{
	//MPの表示
	int NowActorMP = m_nowActor->GetStatus().mp;
	int NowActorMaxMP = m_nowActor->GetStatus().maxMp;
	wchar_t MP[255];
	swprintf_s(MP, 255, L"MP %3d/%d", NowActorMP, NowActorMaxMP);
	m_mpFont.SetText(MP);

	int NowActorHP = m_nowActor->GetStatus().hp;
	int NowActorMaxHP = m_nowActor->GetStatus().maxHp;
	wchar_t HP[255];
	swprintf_s(HP, 255, L"HP %3d/%d", NowActorHP, NowActorMaxHP);
	m_hpFont.SetText(HP);

	//キャラクターが全滅していたら処理しない
	if (IsAnnihilation() == true)
	{
		//ゲームに全滅したことを一度だけ伝える
		if (m_informGameFlag != true)
		{
			Game* game = FindGO<Game>("game");
			game->SetPlayerAnnihilationFlag(true);
			m_informGameFlag = true;
		}
		
		return;
	}

	//現在のキャラクターがやられたら強制的に切り替え
	if (ForcedChange() == false)
	{
		Change();
	}

	//現在のキャラクターがやられていないなら先の処理しない
	if (m_nowActor->GetDieFlag() != true)
	{
		/*if (g_pad[0]->IsTrigger(enButtonStart))
		{
			Vector3 a = { 50.0f,0.0f,20.0f };
			rigitBody.AddForce(a, m_position);
		}*/



		//移動処理
		m_moveSpeed = m_nowActor->calcVelocity(m_nowActor->GetStatus());
		//キャラコンの移動処理
		m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);

		/*rigitBody.SetPositionAndRotation(
			m_position,
			m_nowActor->GetRotation()
		);*/
	}
}

void Player::Change()
{
	//サブのアクターがやられていたら先処理をしない
	if (m_subActor->GetDieFlag() == true)
	{
		return;
	}

	//特定のアニメーションが再生中の時は入れ替え可能
	if (m_nowActor->isAnimationSwappable() != true)
	{
		//キャラクターの切り替え
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			switch (m_enActiveCharacter)
			{
			case enHero:
				//切り替え先がやられていたら交代しない
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
}

void Player::ChangeCharacter(EnCharacters nextCharacter)
{
	//サブのアクターを入れ替える
	m_subActor = m_nowActor;

	// 現在のアクターを非アクティブにする。
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

bool Player::ForcedChange()
{
	//現在のキャラクターがやられていないなら先の処理しない
	if (m_nowActor->GetDieFlag() != true)
	{
		return false;
	}

	//現在のキャラクターのDieアニメーションが終わり切り替え可能になったら
	if (m_nowActor->GetDieToChangeFlag() == true)
	{
		//数秒経ってから切り替え
		if (m_dieToChangeTime < m_dieToChangeTimer)
		{
			//タイマーリセット
			m_dieToChangeTimer = 0.0f;

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
		else
		{
			//タイマー加算
			m_dieToChangeTimer += g_gameTime->GetFrameDeltaTime();
		}

		
	}

	return true;
}

bool Player::IsAnnihilation()
{
	//現在のキャラクターとサブのキャラクターがやられていたら
	if (m_nowActor->GetDieToChangeFlag() == true && m_subActor->GetDieToChangeFlag() == true)
	{
		//全滅している
		return true;
	}
	//生存しているキャラクターがいる
	return false;
}

int Player::GetAtk()
{
	return m_nowActor->GetStatus().atk;
}

int Player::GetSkillAtk()
{
	return m_nowActor->GetSkillAttackPower();
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
	//m_mpFont.Draw(rc);
	//m_hpFont.Draw(rc);
}



