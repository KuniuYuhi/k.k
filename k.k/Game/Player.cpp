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

	
	//�����L�����N�^�[���q�[���[�ɐݒ�
	m_enActiveCharacter = enHero;
	//���݂̃L�����N�^�[���q�[���[�ɐݒ肷��
	m_nowActor = actor[m_enActiveCharacter];

	//���W�̐ݒ�
	m_position = m_nowActor->GetPosition();

	//�L�����R���̐ݒ�
	m_charaCon.Init(12.0f, 33.0f, m_position);

	return true;
}

void Player::Update()
{
	//MP�̕\��
	int NowActorMP = m_nowActor->GetStatus().mp;
	int NowActorMaxMP = m_nowActor->GetStatus().maxMp;
	wchar_t MP[255];
	swprintf_s(MP, 255, L"MP %3d/%d", NowActorMP, NowActorMaxMP);
	m_MpFont.SetText(MP);

	//����̃A�j���[�V�������Đ����̎��͓���ւ��\
	if (m_nowActor->isAnimationSwappable() != true)
	{
		//�L�����N�^�[�̐؂�ւ�
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

	

	//�L�����R��

	//m_position = actor[m_enActiveCharacter]->GetPosition();
	//�ړ�����
	m_moveSpeed = m_nowActor->calcVelocity(m_nowActor->GetStatus());

	m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	
}

void Player::ChangeCharacter(EnCharacters nextCharacter)
{
	// �܂��͌��݂̃A�N�^�[���A�N�e�B�u�ɂ���B
	m_nowActor->Deactivate();

	// �A�N�^�[��؂�ւ���
	m_nowActor = actor[nextCharacter];

	//�A�N�^�[�̃A�j���[�V������ҋ@�X�e�[�g�ɂ���
	//m_nowActor->SetAnimationState();

	m_nowActor->Activate();
	//���W�A��]�A�T�C�Y���q�[���[�Ɠ����ɂ���
	m_nowActor->SetTransForm(
		actor[m_enActiveCharacter]->GetPosition(),
		actor[m_enActiveCharacter]->GetRotation()
	);
	//m_charaCon.SetRadius(50.0f);
	//�L�����R���̍��W
	m_nowActor->SetCharaConPosition(m_nowActor->GetPosition());
	

	//���݂̃L�����N�^�[�𖂖@�g���ɕύX����
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

