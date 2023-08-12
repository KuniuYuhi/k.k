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

	
	//�����L�����N�^�[���q�[���[�ɐݒ�
	m_enActiveCharacter = enHero;
	//�T�u�̃L�����N�^�[���E�B�U�[�h�ɐݒ�
	m_subActor = actor[enWizard];

	//���݂̃L�����N�^�[���q�[���[�ɐݒ肷��
	m_nowActor = actor[m_enActiveCharacter];

	//���W�̐ݒ�
	m_position = m_nowActor->GetPosition();

	//�L�����R���̐ݒ�
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
	//MP�̕\��
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

	//�L�����N�^�[���S�ł��Ă����珈�����Ȃ�
	if (IsAnnihilation() == true)
	{
		//�Q�[���ɑS�ł������Ƃ���x�����`����
		if (m_informGameFlag != true)
		{
			Game* game = FindGO<Game>("game");
			game->SetPlayerAnnihilationFlag(true);
			m_informGameFlag = true;
		}
		
		return;
	}

	//���݂̃L�����N�^�[�����ꂽ�狭���I�ɐ؂�ւ�
	if (ForcedChange() == false)
	{
		Change();
	}

	//���݂̃L�����N�^�[������Ă��Ȃ��Ȃ��̏������Ȃ�
	if (m_nowActor->GetDieFlag() != true)
	{
		/*if (g_pad[0]->IsTrigger(enButtonStart))
		{
			Vector3 a = { 50.0f,0.0f,20.0f };
			rigitBody.AddForce(a, m_position);
		}*/



		//�ړ�����
		m_moveSpeed = m_nowActor->calcVelocity(m_nowActor->GetStatus());
		//�L�����R���̈ړ�����
		m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);

		/*rigitBody.SetPositionAndRotation(
			m_position,
			m_nowActor->GetRotation()
		);*/
	}
}

void Player::Change()
{
	//�T�u�̃A�N�^�[������Ă�����揈�������Ȃ�
	if (m_subActor->GetDieFlag() == true)
	{
		return;
	}

	//����̃A�j���[�V�������Đ����̎��͓���ւ��\
	if (m_nowActor->isAnimationSwappable() != true)
	{
		//�L�����N�^�[�̐؂�ւ�
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			switch (m_enActiveCharacter)
			{
			case enHero:
				//�؂�ւ��悪����Ă������サ�Ȃ�
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
	//�T�u�̃A�N�^�[�����ւ���
	m_subActor = m_nowActor;

	// ���݂̃A�N�^�[���A�N�e�B�u�ɂ���B
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

bool Player::ForcedChange()
{
	//���݂̃L�����N�^�[������Ă��Ȃ��Ȃ��̏������Ȃ�
	if (m_nowActor->GetDieFlag() != true)
	{
		return false;
	}

	//���݂̃L�����N�^�[��Die�A�j���[�V�������I���؂�ւ��\�ɂȂ�����
	if (m_nowActor->GetDieToChangeFlag() == true)
	{
		//���b�o���Ă���؂�ւ�
		if (m_dieToChangeTime < m_dieToChangeTimer)
		{
			//�^�C�}�[���Z�b�g
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
			//�^�C�}�[���Z
			m_dieToChangeTimer += g_gameTime->GetFrameDeltaTime();
		}

		
	}

	return true;
}

bool Player::IsAnnihilation()
{
	//���݂̃L�����N�^�[�ƃT�u�̃L�����N�^�[������Ă�����
	if (m_nowActor->GetDieToChangeFlag() == true && m_subActor->GetDieToChangeFlag() == true)
	{
		//�S�ł��Ă���
		return true;
	}
	//�������Ă���L�����N�^�[������
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



