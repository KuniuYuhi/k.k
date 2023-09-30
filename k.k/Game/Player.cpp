#include "stdafx.h"
#include "Player.h"
#include "Hero.h"
#include "Wizard.h"
#include "Actor.h"
#include "Game.h"

//todo �����œ����蔻�肷��̂����肩��
//todo �d��
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
	m_game = FindGO<Game>("game");


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

	//�L�����N�^�[�̍��W��ݒ�
	m_nowActor->SetPosition(m_position);

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
	//���s�����܂�����
	if (IsWinnerDecision() == true)
	{
		return;
	}

	//�Q�[�����n�܂�܂ł͈ړ����Ȃ�
	if (m_game->GetNowGameState() != Game::enGameState_Game)
	{
		return;
	}
	//�s���\�ɂ���
	if (m_dontActionFlag != false)
	{
		SetDontActionFlag(false);
	}

	//���݂̃L�����N�^�[�����ꂽ�狭���I�ɐ؂�ւ�
	if (ForcedChange() == false)
	{
		//�t���O��false�Ȃ�
		if (m_ChangCharacterFlag == false)
		{
			//�L�����؂�ւ�
			Change();
		}
		
	}

	//�L�����؂�ւ���̃N�[���^�C���̌v�Z
	ChangeCharacterTime();

	//���݂̃L�����N�^�[������Ă��Ȃ��Ȃ��̏������Ȃ�
	if (m_nowActor->GetDieFlag() != true)
	{
		//�ړ�����
		m_moveSpeed = m_nowActor->calcVelocity(m_nowActor->GetStatus());
		m_moveSpeed.y = 0.0f;
		
		CalcPosition(m_moveSpeed,1.0f/60.0f);
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
		if (g_pad[0]->IsTrigger(enButtonLB1))
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
			//�L�����؂�ւ��t���O
			SetChangCharacterFlag(true);
			//gameUI�ɒm�点��p�t���O�BgameUI��false�ɖ߂�
			SetChangCharacterFlagForGameUI(true);
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

	//�L������؂�ւ����̂Ŗ��G���ԃt���O��true�ɂ���
	m_nowActor->SetChangeCharacterInvincbleFlag(true);

	//m_charaCon.SetRadius(50.0f);
	//�L�����R���̍��W
	//m_nowActor->SetCharaConPosition(m_nowActor->GetPosition());
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
			//gameUI�ɒm�点��p�t���O�BgameUI��false�ɖ߂�
			SetChangCharacterFlagForGameUI(true);
		}
		else
		{
			//�^�C�}�[���Z
			m_dieToChangeTimer += g_gameTime->GetFrameDeltaTime();
		}

		
	}

	return true;
}

bool Player::ChangeCharacterTime()
{
	//true�łȂ��Ȃ珈�����Ȃ�
	if (m_ChangCharacterFlag == false)
	{
		return true;
	}

	if (0.0f >= m_ChangeCharacterTimer)
	{
		m_ChangCharacterFlag = false;
		m_ChangeCharacterTimer = m_ChangeCharacterTime;
		return true;
	}
	else
	{
		m_ChangeCharacterTimer -= g_gameTime->GetFrameDeltaTime();
	}

	return false;
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

void Player::CalcPosition(Vector3 moveSpeed,float deltaTime)
{
	m_position = m_charaCon.Execute(moveSpeed, deltaTime);
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

bool Player::IsWinnerDecision()
{
	//����
	//�{�X�����񂾂珈�����Ȃ�
	if (GameClear() == true)
	{
		return true;
	}
	//�s�k
	//�L�����N�^�[���S�ł��Ă����珈�����Ȃ�
	if (GameOver() == true)
	{
		return true;
	}
	//�^�C���A�b�v
	if (TimeUp() == true)
	{
		//��������
		return true;
	}
	//����ȊO�Ȃ�	
	return false;
}

bool Player::GameClear()
{
	if (m_game->GetDeathBossFlag() == true)
	{
		m_gameEndFlag = true;

		if (m_VictoryAnimFlag == false)
		{
			//�A�j���[�V�������Ă��Ȃ�������
			if (m_nowActor->isAnimationEntable() == true)
			{
				//���݂̃A�N�^�[�̃X�e�[�g�������X�e�[�g�ɂ���
				m_nowActor->SetVictoryAnimationState();
				m_VictoryAnimFlag = true;
			}
		}
		return true;
	}

	return false;
}

bool Player::GameOver()
{
	if (IsAnnihilation() == true)
	{
		m_gameEndFlag = true;
		//�Q�[���ɑS�ł������Ƃ���x�����`����
		if (m_informGameFlag != true)
		{
			Game* game = FindGO<Game>("game");
			game->SetPlayerAnnihilationFlag(true);
			m_informGameFlag = true;
		}

		return true;
	}

	return false;
}

bool Player::TimeUp()
{
	if (m_game->IsTimeUp() == true)
	{
		m_gameEndFlag = true;

		if (m_idleAnimFlag == false)
		{
			//���݂̃A�N�^�[�̃X�e�[�g���A�C�h���X�e�[�g�ɂ���
			m_nowActor->SetIdleAnimationState();
			m_idleAnimFlag = true;
			
		}
		return true;
	}

	return false;
}



