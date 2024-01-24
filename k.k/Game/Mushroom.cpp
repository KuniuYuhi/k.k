#include "stdafx.h"
#include "Mushroom.h"
#include "IMushroomState.h"
#include "MushroomStateIdle.h"
#include "MushroomStateWalk.h"
#include "MushroomStateAttack_1.h"
#include "MushroomStateDamage.h"
#include "MushroomStateDie.h"
#include "MushroomStateVictory.h"
#include "MushroomStateAppear.h"
#include "MushroomStateChase.h"
#include "MushroomStatePatrol.h"
#include "MushroomStateSkill.h"

#include "CharactersInfoManager.h"
#include "GameManager.h"

#include "MobMonsterSM_Patrol.h"
#include "MobMonsterSM_Chase.h"

namespace {
	const float ANGLE = 60.0f;				//����p
	const float DISTANCE_TO_PLAYER = 350.0f;			//�v���C���[�Ƃ̋���
	const float ATTACK_RANGE = 45.0f;					//�U���ł��鋗��
	const float SKILL_ATTACK_RANGE = 50.0f;				//�X�L���U���ł��鋗��
	const float STAY_RANGR = 45.0f;						//��~���鋗��
	const float ATTACK_INTAERVALE_TIME = 1.5f;			//�U������Ԋu
	const float PLAYER_NEARBY_RANGE = 100.0f;			//�U��������̃v���C���[�����G�ł���͈�
	const float ANGLE_RANGE = 2.0f;						//�ړ�����A���O���͈̔�
	const float POS2_LENGTH = 30.0f;
	const float ROT_SPEED = 8.0f;
	const float SKILL_TIMER_LIMMIT = 5.0f;
}

Mushroom::Mushroom()
{
	m_angle = ANGLE;

	m_distanceToPlayer = DISTANCE_TO_PLAYER;
	m_attackRange = ATTACK_RANGE;
	m_stayRange = STAY_RANGR;

	m_attackIntervalTime = ATTACK_INTAERVALE_TIME;

	m_angleRange = ANGLE_RANGE;

	m_pos2Length = POS2_LENGTH;

	m_scale *= 1.3f;

	m_skillUsableLimmit = SKILL_TIMER_LIMMIT;
	m_skillAttackRange = SKILL_ATTACK_RANGE;
}

Mushroom::~Mushroom()
{
	DeleteGO(m_headCollision);
}

bool Mushroom::Start()
{
	//�@�������������B
	srand((unsigned)time(NULL));

	//�v���C���[�̃C���X�^���X��T��
	m_player = FindGO<Player>("player");

	//�X�e�[�^�X�̏�����
	m_status.Init(GetName());
	//���f���̏�����
	InitModel();
	//�X�e�[�g�}�V���̐���
	SetNextStateMachine(enStateMachineState_Patrol);
	//�܂������A�j���[�V�����B���̌�s��
	SetNextAnimationState(enAnimationState_Appear);

	//3����5�͈̔͂̃C���^�[�o��
	m_angleChangeTime = rand() % 3 + 3;

	return true;
}

void Mushroom::InitModel()
{
	m_animationClip[enAnimationClip_Idle].Load("Assets/animData/character/Mushroom/Idle_normal.tka");
	m_animationClip[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Patrol].Load("Assets/animData/character/Mushroom/Walk.tka");
	m_animationClip[enAnimationClip_Patrol].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Chase].Load("Assets/animData/character/Mushroom/Run.tka");
	m_animationClip[enAnimationClip_Chase].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Attack].Load("Assets/animData/character/Mushroom/Attack1.tka");
	m_animationClip[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Skill].Load("Assets/animData/character/Mushroom/Attack2.tka");
	m_animationClip[enAnimationClip_Skill].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Hit].Load("Assets/animData/character/Mushroom/Damage.tka");
	m_animationClip[enAnimationClip_Hit].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Die].Load("Assets/animData/character/Mushroom/Die.tka");
	m_animationClip[enAnimationClip_Die].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Victory].Load("Assets/animData/character/Mushroom/Victory.tka");
	m_animationClip[enAnimationClip_Victory].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Appear].Load("Assets/animData/character/Mushroom/Appear.tka");
	m_animationClip[enAnimationClip_Appear].SetLoopFlag(false);
	//���f����������
	m_modelRender.Init(
		"Assets/modelData/character/Mushroom/Mushroom.tkm",
		L"Assets/shader/ToonTextrue/lamp_Mushroom.DDS",
		m_animationClip,
		enAnimationClip_Num,
		enModelUpAxisZ
	);
	//�L�����R��������
	m_charaCon.Init(
		22.0f,
		9.0f,
		m_position
	);

	//�o�ꎞ�̑O�����̐ݒ�
	m_direction = SetRamdomDirection(m_angleRange,true);
	m_forward = m_direction;
	m_forward.Normalize();
	//��]�̐ݒ�
	Rotation(ROT_SPEED, ROT_SPEED);

	//���W�̐ݒ�
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//�U���Ɏg���{�[��Id�̎擾
	m_attackBoonId = m_modelRender.FindBoneID(L"mushroom_spine03");
}

void Mushroom::Update()
{
	//�|�[�Y��ʂȂ珈�������Ȃ�
	if (GameManager::GetInstance()->GetGameSeenState() ==
		GameManager::enGameSeenState_Pause)
	{
		return;
	}

	if (IsStopProcessing() != true)
	{
		//�X�L���U���̃C���^�[�o���̌v�Z
		CalcSkillAttackIntarval();
		//�U���Ԋu�C���^�[�o��
		AttackInterval(m_attackIntervalTime);
		//�A���O���؂�ւ��C���^�[�o��
		AngleChangeTimeIntarval(m_angleChangeTime);

		//�m�b�N�o�b�N���łȂ��Ȃ��]����
		if (GetKnockBackFlag() != true)
		{
			//��]����
			Rotation(ROT_SPEED, ROT_SPEED);
		}

		//�����蔻��
		DamageCollision(m_charaCon);
	}

	//���t���[���s������
	m_mobStateMachine->Execute();
	//��ԊǗ�
	ManageState();
	//�A�j���[�V����
	PlayAnimation();

	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void Mushroom::CreateCollision()
{
	m_headCollision = NewGO<CollisionObject>(0, "monsterattack");
	m_headCollision->SetCreatorName(GetName());
	m_headCollision->CreateSphere(
		m_position,
		m_rotation,
		16.0f
	);
	//���[���h���W�擾
	Matrix HeadMatrix = m_modelRender.GetBone(m_attackBoonId)->GetWorldMatrix();
	m_headCollision->SetWorldMatrix(HeadMatrix);
}

bool Mushroom::IsStopProcessing()
{
	//�Q�[���X�e�[�g�ȊO�Ȃ�
	if (GameManager::GetInstance()->GetGameSeenState() !=
		GameManager::enGameSeenState_Game)
	{
		return true;
	}
	
	//����������
	if (GameManager::GetInstance()->GetOutComeState()
		== GameManager::enOutComeState_PlayerLose)
	{
		SetWinFlag(true);
		//�U�����łȂ����
		SetNextAnimationState(enAnimationState_Victory);
		return true;
	}
	//��������
	if (GameManager::GetInstance()->GetOutComeState()
		== GameManager::enOutComeState_PlayerWin)
	{
		SetNextAnimationState(enAninationState_Idle);
		return true;
	}

	//�������ꂽ���̃A�j���[�V�����X�e�[�g�Ȃ�	
	if (m_enAnimationState == enAnimationState_Appear)
	{
		return true;
	}

	//�m�b�N�o�b�N���Ȃ�
	//if (GetKnockBackFlag() == true)
	//{
	//	//�m�b�N�o�b�N�̏���������Ȃ�
	//	if (IsKnockingBack(
	//		m_moveSpeed, m_knockBackTimer) == true)
	//	{
	//		//���W���ړ�
	//		m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	//		return true;
	//	}
	//	else
	//	{
	//		SetKnockBackFlag(false);
	//	}
	//}

	//����ȊO�Ȃ�
	return false;
}

void Mushroom::Damage(int attack)
{
	//�U������������Ȃ��̂œ����蔻��𐶐����Ȃ��悤�ɂ���
	m_createAttackCollisionFlag = false;
	//HP�����炷
	m_status.CalcHp(attack, false);

	//�m�b�N�o�b�N�t���O���Z�b�g
	//todo ����������ڂ�X�L���ɂ���ĕς���
	SetKnockBackFlag(true);
	m_moveSpeed = SetKnockBackDirection(
		m_position,
		m_player->GetPosition(),
		m_player->GetKnockBackPower()
	);

	//HP��0�ȉ��Ȃ�
	if (m_status.GetHp() <= 0)
	{
		//����A�j���[�V�����X�e�[�g
		m_status.SetHp(0);
		SetNextAnimationState(enAnimationState_Die);
		ProcessDead();
		return;
	}

	//��_���[�W�A�j���[�V�����X�e�[�g
	SetNextAnimationState(enAnimationState_Hit);
}

void Mushroom::ManageState()
{
	m_state->ManageState();
}

void Mushroom::PlayAnimation()
{
	m_state->PlayAnimation();
}

void Mushroom::SetNextAnimationState(EnAnimationState nextState)
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
	case enAninationState_Idle:
		m_state = new MushroomStateIdle(this);
		break;
	case enAninationState_Patrol:
		m_state = new MushroomStatePatrol(this);
		//�v���C���[��ǂ������Ă��Ȃ��̂Ńt���O�����Z�b�g
		m_chasePlayerFlag = false;
		break;
	case enAninationState_Chase:
		m_state = new MushroomStateChase(this);
		//�v���C���[��ǂ������Ă���̂ŁA�t���O���Z�b�g
		m_chasePlayerFlag = true;
		break;
	case enAnimationState_Attack:
		m_state = new MushroomStateAttack_1(this);
		//�U�������̂ŁA�t���O�����Z�b�g
		m_attackEnableFlag = false;
		break;
	case enAnimationState_Skill:
		m_state = new MushroomStateSkill(this);
		//�X�L���U�������̂ŁA�X�L���U���g�p�\�t���O�����Z�b�g
		m_skillUsableFlag = false;
		m_attackEnableFlag = false;
		break;
	case enAnimationState_Hit:
		m_state = new MushroomStateDamage(this);
		break;
	case enAnimationState_Die:
		m_state = new MushroomStateDie(this);
		break;
	case enAnimationState_Victory:
		m_state = new MushroomStateVictory(this);
		break;
	case enAnimationState_Appear:
		m_state = new MashroomStateAppear(this);
		break;
	default:
		// �����ɗ�����X�e�[�g�̃C���X�^���X�쐬�����̒ǉ��Y��B
		std::abort();
		break;
	}
}

void Mushroom::SetNextStateMachine(EnStateMachineState nextStateMachine)
{
	if (m_mobStateMachine != nullptr)
	{
		delete m_mobStateMachine;
		m_mobStateMachine = nullptr;
	}

	m_enStateMachineState = nextStateMachine;

	switch (m_enStateMachineState)
	{
	case MobMonsterInfo::enStateMachineState_Patrol:
		m_mobStateMachine = new MobMonsterSM_Patrol(this);
		break;
	case MobMonsterInfo::enStateMachineState_Chase:
		m_mobStateMachine = new MobMonsterSM_Chase(this);
		break;
	default:
		std::abort();
		break;
	}
}

void Mushroom::ProcessCommonStateTransition()
{
	SetNextAnimationState(enAninationState_Patrol);
}

void Mushroom::OnProcessAttack_1StateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�U���I����܂��߂��Ƀv���C���[������Ȃ�
		if (IsFindPlayer(PLAYER_NEARBY_RANGE) == true)
		{
			//�v���C���[���߂��ɂ��邩�t���O���Z�b�g
			SetPlayerNearbyFlag(true);
		}
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Mushroom::OnProcessAttack_2StateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{

		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Mushroom::OnProcessDamageStateTransition()
{
	if (GetKnockBackFlag() == false)
	{
		//���t���[�����d�������Ă���
		//�d�����I�������
		if (IsKnockBackStiffness() == false)
		{
			//���ʂ̏�ԑJ�ڏ����Ɉڍs
			ProcessCommonStateTransition();
		}
		return;
	}
	//�m�b�N�o�b�N����
	ProcessKnockBack(m_charaCon);

	//�A�j���[�V�����̍Đ����I�������
	//if (m_modelRender.IsPlayingAnimation() == false)
	//{

	//	//���ʂ̏�ԑJ�ڏ����Ɉڍs
	//	ProcessCommonStateTransition();
	//}
}

void Mushroom::OnProcessDieStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���X�g���玩�g������
		CharactersInfoManager::GetInstance()->RemoveMobMonsterFormList(this);
		m_elaseListFlag = true;
		//���g���폜����
		DeleteGO(this);
	}
}

void Mushroom::OnProcessVictoryStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Mushroom::OnProcessAppearStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Mushroom::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�����蔻�萶���^�C�~���O
	if (wcscmp(eventName, L"Collision_Start") == 0)
	{
		m_createAttackCollisionFlag = true;
	}
	//�����蔻�萶���I���
	if (wcscmp(eventName, L"Collision_End") == 0)
	{
		m_createAttackCollisionFlag = false;
	}
}

void Mushroom::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}


