#include "stdafx.h"
#include "Lich.h"
#include "LichStateIdle.h"
#include "LichStateWalk.h"
#include "LichStateAttack_1.h"
#include "LichStateAttack_2.h"
#include "LichStateDie.h"

namespace {
	const float SCALE_UP = 3.0f;									//�L�����N�^�[�̃T�C�Y
	const Vector3 FIRST_POSITION = Vector3(0.0f, 0.0f, -250.0f);	//�ŏ��̍��W
	const float DISTANCE = 1500.0f;									//�v���C���[�𔭌��ł��鋗��


	//�X�e�[�^�X
	int MAXHP = 500;
	int MAXMP = 500;
	int ATK = 20;
	float SPEED = 50.0f;
	const char* NAME = "Lich";
}

Lich::Lich()
{
	//�ŏ��̍��W��ݒ肷��
	m_position = FIRST_POSITION;
	//�v���C���[�𔭌��ł��鋗����ݒ肷��
	m_distance = DISTANCE;
	//���b�`�̃T�C�Y��ݒ�
	m_scale *= SCALE_UP;
}

Lich::~Lich()
{
}

bool Lich::Start()
{
	//�����̃A�j���[�V�����X�e�[�g��ҋ@��Ԃɂ���B
	SetNextAnimationState(enAninationState_Idle);


	//�X�e�[�^�X�̏�����
	m_status.InitStatus(
		MAXHP,
		MAXMP,
		ATK,
		SPEED,
		NAME
	);

	InitModel();

	return true;
}

void Lich::InitModel()
{
	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/Lich/Idle.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Walk].Load("Assets/animData/character/Lich/Walk.tka");
	m_animationClip[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClip[enAnimClip_Attack_1].Load("Assets/animData/character/Lich/Attack1.tka");
	m_animationClip[enAnimClip_Attack_1].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_2].Load("Assets/animData/character/Lich/Attack2.tka");
	m_animationClip[enAnimClip_Attack_2].SetLoopFlag(false);
	m_animationClip[enAnimClip_Die].Load("Assets/animData/character/Lich/Die.tka");
	m_animationClip[enAnimClip_Die].SetLoopFlag(false);

	m_modelRender.Init("Assets/modelData/character/Lich/Lich.tkm",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);

	//���f����TRS�̐ݒ�
	m_modelRender.SetTransform(
		m_position,
		m_rotation,
		m_scale
	);
	m_modelRender.Update();

	//�L�����R���̐ݒ�
	m_charaCon.Init(
		70.0f,
		50.0f,
		m_position
	);

}

void Lich::Update()
{
	//�|���ꂽ�瑼�̏��������s���Ȃ��悤�ɂ���
	if (m_dieFlag==true)
	{
		ManageState();
		PlayAnimation();
		m_modelRender.Update();
		return;
	}

	AttackInterval();

	DecideNextAction();

	
	

	ManageState();
	PlayAnimation();

	DamageCollision();

	SetTransFormModel(m_modelRender);
	m_modelRender.Update();

	m_oldMoveSpeed = m_moveSpeed;
}

void Lich::Move()
{
	SetTargetPosition();

	if (m_attackRangeFlag == true)
	{
		//�ړ����Ȃ��悤�ɂ���
		m_moveSpeed = calcVelocity(m_status);
		m_moveSpeed = Vector3::Zero;
	}
	//�v���C���[����������
	else if (IsFindPlayer(m_distance) == true)
	{
		//�ړ�
		m_moveSpeed = calcVelocity(m_status);
		m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	}
	else
	{
		//�ړ����Ȃ��悤�ɂ���
		m_moveSpeed = Vector3::Zero;
	}

	
	Rotation();

}

void Lich::Damage()
{
	if (m_status.hp > 0)
	{
		m_status.hp -= m_player->GetAtk();
	}
	else if(m_status.hp <= 0)
	{
		//Die�t���O��true�ɂ���
		m_dieFlag = true;
		m_status.hp = 0;
		SetNextAnimationState(enAnimationState_Die);
	}

}

bool Lich::RotationOnly()
{
	//����̃A�j���[�V�������Đ����̂Ƃ�
	if (isRotationEntable() != true)
	{
		//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
		if (fabsf(m_SaveMoveSpeed.x) >= 0.001f || fabsf(m_SaveMoveSpeed.z) >= 0.001f)
		{
			m_rotation.SetRotationYFromDirectionXZ(m_SaveMoveSpeed);
		}
		return true;
	}

	return false;
}


bool Lich::Attack()
{
	//�A�^�b�N�P�̍U���͈͂Ƀ^�[�Q�b�g��������
	if (IsFindPlayer(m_Attack_1Distance) == true)
	{
		//�C���^�[�o�����Ȃ��Ȃ�U���\
		if (m_attackFlag == false)
		{
			SetNextAnimationState(enAnimationState_Attack_1);
			m_attackFlag = true;
			//�U���͈͂Ƀ^�[�Q�b�g������
			m_attackRangeFlag = true;

			return true;
		}
	}
	else
	{
		//�U���͈͂Ƀ^�[�Q�b�g�����Ȃ�
		m_attackRangeFlag = false;
	}

	return false;
}

bool Lich::Attack2()
{
	//�A�^�b�N�P�̍U���͈͂Ƀ^�[�Q�b�g��������
	if (IsFindPlayer(m_Attack_2Distance) == true)
	{
		//�C���^�[�o�����Ȃ��Ȃ�U���\
		if (m_attackFlag == false)
		{
			SetNextAnimationState(enAnimationState_Attack_2);
			m_attackFlag = true;
			//�U���͈͂Ƀ^�[�Q�b�g������
			m_attackRangeFlag = true;

			return true;
		}
	}
	else
	{
		//�U���͈͂Ƀ^�[�Q�b�g�����Ȃ�
		m_attackRangeFlag = false;
	}

	return false;
}

void Lich::DecideNextAction()
{
	Move();

	if (m_attackFlag == false)
	{
		//�͈͂�������
		if (Attack2() == true)
		{
			return;
		}

		if (Attack() == true)
		{
			return;
		}
	}
}

bool Lich::AttackInterval()
{
	//�U��������
	if (m_attackFlag == true)
	{
		//�^�C�}�[���C���^�[�o���𒴂�����
		if (m_attackIntervalTime < m_attackIntervalTimer)
		{
			//�U���\�ɂ���
			m_attackFlag = false;
			//�^�C�}�[�����Z�b�g
			m_attackIntervalTimer = 0.0f;
		}
		else
		{
			m_attackIntervalTimer += g_gameTime->GetFrameDeltaTime();
			//�U���s�\
			return false;

		}
	}
	//�U���\
	return true;
}


void Lich::PlayAnimation()
{
	m_state->ManageState();
}

void Lich::ManageState()
{
	m_state->PlayAnimation();
}

void Lich::SetNextAnimationState(EnAnimationState nextState)
{
	if (m_state != nullptr) {
		// �Â��X�e�[�g���폜����B
		delete m_state;
		m_state = nullptr;
	}

	//�A�j���[�V�����X�e�[�g�����̃X�e�[�g�ɕς���
	m_enAnimationState = nextState;

	switch (m_enAnimationState)
	{
	case Lich::enAninationState_Idle:
		// �ҋ@�X�e�[�g���쐬����B
		m_state = new LichStateIdle(this);
		break;
	case Lich::enAninationState_Walk:
		// �ҋ@�X�e�[�g���쐬����B
		m_state = new LichStateWalk(this);
		break;
	case Lich::enAninationState_Run:
		break;
	case Lich::enAnimationState_Attack_1:
		// �A�^�b�N�P�X�e�[�g���쐬����B
		m_state = new LichStateAttack_1(this);
		break;
	case Lich::enAnimationState_Attack_2:
		// �A�^�b�N�Q�X�e�[�g���쐬����B
		m_state = new LichStateAttack_2(this);
		break;
	case Lich::enAnimationState_Attack_3_start:
		break;
	case Lich::enAnimationState_Attack_3_main:
		break;
	case Lich::enAnimationState_Attack_4:
		break;
	case Lich::enAnimationState_Die:
		//Die�X�e�[�g���쐬����
		m_state = new LichStateDie(this);
		break;
	default:
		// �����ɗ�����X�e�[�g�̃C���X�^���X�쐬�����̒ǉ��Y��B
		std::abort();
		break;
		break;
	}


		
	
}

void Lich::ProcessCommonStateTransition()
{
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		SetNextAnimationState(enAninationState_Walk);
	}
	else
	{
		SetNextAnimationState(enAninationState_Idle);
	}
	
}

void Lich::OnProcessAttack_1StateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�U���p�^�[�����Ȃ���Ԃɂ���
		//m_enAttackPatternState = enAttackPattern_None;
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Lich::OnProcessAttack_2StateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�U���p�^�[�����Ȃ���Ԃɂ���
		//m_enAttackPatternState = enAttackPattern_None;
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Lich::OnProcessDieStateTransition()
{

}

void Lich::DamageCollision()
{
	//�G�̍U���p�̃R���W�������擾���閼�O�ꏏ�ɂ���
	const auto& Attack_1Collisions = g_collisionObjectManager->FindCollisionObjects("Attack");
	//�R���W�����̔z���for���ŉ�
	for (auto collision : Attack_1Collisions)
	{
		//���g�̃L�����R���ƏՓ˂�����
		if (collision->IsHit(m_charaCon) == true)
		{
			//�P�R���{�̊Ԃ�1�񂾂�����
			//�_���[�W���󂯂����̃R���{�X�e�[�g�ƌ��݂̃R���{�X�e�[�g���Ⴄ�Ȃ�
			if (m_player->IsComboStateSame()==true)
			{
				Damage();

				//�_���[�W���󂯂����̃R���{�X�e�[�g�Ɍ��݂̃R���{�X�e�[�g��������
				m_player->SetDamagedComboState(m_player->GetNowComboState());
			}
			else
			{

			}

			
			
		}

		
	}

	//�G�̍U���p�̃R���W�������擾���閼�O�ꏏ�ɂ���
	const auto& SkillCollisions = g_collisionObjectManager->FindCollisionObjects("SkillAttack");
	//�R���W�����̔z���for���ŉ�
	for (auto collision : SkillCollisions)
	{
		//���g�̃L�����R���ƏՓ˂�����
		if (collision->IsHit(m_charaCon) == true)
		{
			
			Damage();
		}
	}

}

void Lich::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}