#include "stdafx.h"
#include "TurtleShell.h"
#include "ITurtleShellState.h"
#include "TurtleShellStateIdle.h"
#include "TurtleShellStateAttack_1.h"
#include "TurtleShellStateDamage.h"
#include "TurtleShellStateDie.h"
#include "TurtleShellStateVictory.h"
#include "TurtleShellStateAppear.h"
#include "TurtleShellStateChase.h"
#include "TurtleShellStatePatrol.h"
#include "TurtleShellStateSkill.h"

#include "CharactersInfoManager.h"
#include "GameManager.h"

#include "MobMonsterSM_Patrol.h"
#include "MobMonsterSM_Chase.h"

namespace {
	const float ANGLE = 65.0f;				//����p
	const float DISTANCE_TO_PLAYER = 300.0f;			//�v���C���[�Ƃ̋���
	const float ATTACK_RANGE = 50.0f;					//�U���ł��鋗��
	const float SKILL_ATTACK_RANGE = 60.0f;				//�X�L���U���ł��鋗��
	const float STAY_RANGR = 45.0f;						//��~���鋗��
	const float ATTACK_INTAERVALE_TIME = 2.5f;			//�U������Ԋu
	const float PLAYER_NEARBY_RANGE = 150.0f;			//�U��������̃v���C���[�����G�ł���͈�
	const int ANGLE_RANGE = 2;						//�ړ�����A���O���͈̔�
	const float POS2_LENGTH = 30.0f;
	const float ROT_SPEED = 7.0f;
	const float SKILL_TIMER_LIMMIT = 10.0f;
	const float DEFENCE_RANGE = 1200.0f;

	const float MUL_SCALE = 1.3f;

	const float MUL_COLLISION_SIZE = 17.0f;
}

TurtleShell::TurtleShell()
{
	m_angle = ANGLE;

	m_distanceToPlayer = DISTANCE_TO_PLAYER;
	m_attackRange = ATTACK_RANGE;
	m_stayRange = STAY_RANGR;

	m_attackIntervalTime = ATTACK_INTAERVALE_TIME;

	m_angleRange = ANGLE_RANGE;

	m_pos2Length = POS2_LENGTH;

	m_scale *= MUL_SCALE;

	m_skillUsableLimmit = SKILL_TIMER_LIMMIT;
	m_skillAttackRange = SKILL_ATTACK_RANGE;
}

TurtleShell::~TurtleShell()
{
	DeleteGO(m_headCollision);
}

bool TurtleShell::Start()
{
	//�@�������������B
	srand((unsigned)time(NULL));

	//�v���C���[�̃C���X�^���X��T��
	m_player = FindGO<Player>("player");

	//�X�e�[�^�X�̏�����
	m_status.InitCharacterStatus(GetName());

	//���f���̏�����
	InitModel();
	//�X�e�[�g�}�V���̐���
	SetNextStateMachine(enStateMachineState_Patrol);

	//�܂������A�j���[�V�����B���̌�s��
	SetNextAnimationState(enAnimationState_Appear);

	//6����8�͈̔͂̃C���^�[�o��
	m_angleChangeTime = rand() % 3 + 6;
	return true;
}

void TurtleShell::InitModel()
{
	m_animationClip[enAnimationClip_Idle].Load("Assets/animData/character/TurtleShell/Idle_Normal.tka");
	m_animationClip[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Patrol].Load("Assets/animData/character/TurtleShell/Walk.tka");
	m_animationClip[enAnimationClip_Patrol].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Chase].Load("Assets/animData/character/TurtleShell/Run.tka");
	m_animationClip[enAnimationClip_Chase].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Attack].Load("Assets/animData/character/TurtleShell/Attack1.tka");
	m_animationClip[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Skill].Load("Assets/animData/character/TurtleShell/Defence.tka");
	m_animationClip[enAnimationClip_Skill].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Hit].Load("Assets/animData/character/TurtleShell/Damage.tka");
	m_animationClip[enAnimationClip_Hit].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Die].Load("Assets/animData/character/TurtleShell/Die.tka");
	m_animationClip[enAnimationClip_Die].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Victory].Load("Assets/animData/character/TurtleShell/Victory.tka");
	m_animationClip[enAnimationClip_Victory].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Appear].Load("Assets/animData/character/TurtleShell/Appear.tka");
	m_animationClip[enAnimationClip_Appear].SetLoopFlag(false);
	//���f����������
	m_modelRender.Init(
		"Assets/modelData/character/TurtleShell/TurtleShell.tkm",
		L"Assets/shader/ToonTextrue/lamp_TurltleShell.DDS",
		m_animationClip,
		enAnimationClip_Num,
		enModelUpAxisZ
	);
	//�L�����R��������
	m_charaCon.Init(
		23.6f,
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
	m_attackBoonId = m_modelRender.FindBoneID(L"Head");
}

void TurtleShell::Update()
{
	//�|�[�Y��ʂȂ珈�������Ȃ�
	if (GameManager::GetInstance()->GetGameSeenState() ==
		GameManager::enGameSeenState_Pause)
	{
		return;
	}

	if (IsStopProcessing() != true)
	{
		//�X�L���̃C���^�[�o���̌v�Z
		CalcSkillAttackIntarval();
		//�U���Ԋu�C���^�[�o��
		AttackInterval(m_attackIntervalTime);
		//�A���O���؂�ւ��C���^�[�o��
		AngleChangeTimeIntarval(m_angleChangeTime);
		
		//�m�b�N�o�b�N���łȂ��Ȃ��]����
		if (GetKnockBackFlag() != true)
		{
			//��]�����ƑO�����̐ݒ�
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

void TurtleShell::CreateCollision()
{
	m_headCollision = NewGO<CollisionObject>(0, "monsterattack");
	m_headCollision->SetCreatorName(GetName());
	m_headCollision->CreateSphere(
		m_position,
		m_rotation,
		MUL_COLLISION_SIZE
	);
	//���[���h���W�擾
	Matrix HeadMatrix = m_modelRender.GetBone(m_attackBoonId)->GetWorldMatrix();
	m_headCollision->SetWorldMatrix(HeadMatrix);
}

bool TurtleShell::IsStopProcessing()
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

	//����ȊO�Ȃ�
	return false;
}

bool TurtleShell::IsDifenceEnd()
{
	if (m_difenceTime < m_difenceTimer)
	{
		//�h��\�t���O�����Z�b�g
		m_difenceEnableFlag = false;
		m_difenceTimer = 0.0f;
		return true;
	}
	else
	{
		//�^�C�}�[�����Z
		m_difenceTimer += g_gameTime->GetFrameDeltaTime();
		return false;
	}
}

void TurtleShell::HitNormalAttack()
{
	m_damage = m_player->GetAtk();
	//�_���[�W���󂯂鏈��
	Damage(m_damage);
	//�X�L�����g���Ă��Ȃ�������_���[�W�ƃG�t�F�N�g�\��
	if (m_enAnimationState != enAnimationState_Skill)
	{
		//�_���[�W��\��
		CreateDamageFont(m_damage);
		//�G�t�F�N�g���Đ�
		CreateHitEffect();
		//�q�b�g���̍Đ�
		PlayHitSound();
	}
	//�_���[�W���󂯂��̂Ńt���O��true�ɂȂ�Ȃ��ƃ_���[�W���󂯂Ȃ��悤�ɂ���
	SetDamageHitEnableFlag(false);
}

void TurtleShell::HitSkillAttack()
{
	m_damage = m_player->GetSkillAtk();
	//�_���[�W���󂯂鏈��
	Damage(m_damage);
	//�X�L�����g���Ă��Ȃ�������_���[�W�ƃG�t�F�N�g�\��
	if (m_enAnimationState != enAnimationState_Skill)
	{
		//�_���[�W��\��
		CreateDamageFont(m_damage);
		//�G�t�F�N�g���Đ�
		CreateHitEffect();
	}
	//�_���[�W���󂯂��̂Ńt���O��true�ɂȂ�Ȃ��ƃ_���[�W���󂯂Ȃ��悤�ɂ���
	SetDamageHitEnableFlag(false);
}

bool TurtleShell::IsSkillUsable()
{
	//�X�L���U���\�t���O���Z�b�g����Ă�����X�L���U���\
	//�܂��v���C���[���߂��ɂ���Ȃ�
	if (m_skillUsableFlag == true&& 
		m_difenceEnableFlag == true)
	{
		//�X�L���U���\
		return true;
	}
	//�X�L���U���s�\
	return false;
}

void TurtleShell::Damage(int attack)
{
	//�h�䒆�͏������Ȃ�
	if (m_enAnimationState == enAnimationState_Skill)
	{
		//SE�Đ�

		return;
	}

	//�U������������Ȃ��̂œ����蔻��𐶐����Ȃ��悤�ɂ���
	m_createAttackCollisionFlag = false;

	//HP�����炷
	m_status.CalcHp(attack, false);

	//�m�b�N�o�b�N�t���O���Z�b�g
	SetKnockBackFlag(true);
	m_moveSpeed = SetKnockBackDirection(
		m_position,
		m_player->GetPosition(),
		m_player->GetKnockBackPower()
	);

	//HP��0�ȉ��Ȃ�
	if (m_status.GetHp() <= 0)
	{
		//���ꂽ�̂Ńt���O�𗧂Ă�
		m_deadFlag = true;
		//����A�j���[�V�����X�e�[�g
		m_status.SetHp(0);
		SetNextAnimationState(enAnimationState_Die);
		ProcessDead();
		return;
	}

	//��_���[�W�A�j���[�V�����X�e�[�g
	SetNextAnimationState(enAnimationState_Hit);
	
}

bool TurtleShell::RotationOnly()
{
	if (m_enAnimationState == enAnimationState_Skill)
	{
		return true;
	}
	return false;
}

void TurtleShell::ManageState()
{
	m_state->ManageState();
}

void TurtleShell::PlayAnimation()
{
	m_state->PlayAnimation();
}

void TurtleShell::SetNextAnimationState(EnAnimationState nextState)
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
		m_state = new TurtleShellStateIdle(this);
		break;
	case enAninationState_Patrol:
		m_state = new TurtleShellStatePatrol(this);
		//�v���C���[��ǂ������Ă��Ȃ��̂Ńt���O�����Z�b�g
		m_chasePlayerFlag = false;
		break;
	case enAninationState_Chase:
		m_state = new TurtleShellStateChase(this);
		//�v���C���[��ǂ������Ă���̂ŁA�t���O���Z�b�g
		m_chasePlayerFlag = true;
		break;
	case enAnimationState_Attack:
		m_state = new TurtleShellStateAttack_1(this);
		//�U�������̂ŁA�t���O�����Z�b�g
		m_attackEnableFlag = false;
		break;
	case enAnimationState_Skill:
		m_state = new TurtleShellStateSkill(this);
		//�X�L���U�������̂ŁA�X�L���U���g�p�\�t���O�����Z�b�g
		m_skillUsableFlag = false;
		m_attackEnableFlag = false;
		break;
	case enAnimationState_Hit:
		m_state = new TurtleShellStateDamage(this);
		break;
	case enAnimationState_Die:
		m_state = new TurtleShellStateDie(this);
		break;
	case enAnimationState_Victory:
		m_state = new TurtleShellStateVictory(this);
		break;
	case enAnimationState_Appear:
		m_state = new TurtleShellStateAppear(this);
		break;
	default:
		// �����ɗ�����X�e�[�g�̃C���X�^���X�쐬�����̒ǉ��Y��B
		std::abort();
		break;
	}
}

void TurtleShell::SetNextStateMachine(EnStateMachineState nextStateMachine)
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

void TurtleShell::ProcessCommonStateTransition()
{
	SetNextAnimationState(enAninationState_Patrol);
}

void TurtleShell::OnProcessAttack_1StateTransition()
{
	//�A�j���[�V�������I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�U���I����܂��߂��Ƀv���C���[������Ȃ�
		if (IsFindPlayer(PLAYER_NEARBY_RANGE) == true)
		{
			//�v���C���[���߂��ɂ��邩�t���O���Z�b�g
			SetPlayerNearbyFlag(true);
		}
		//���ʂ̃X�e�[�g�J�ڏ������s
		ProcessCommonStateTransition();
	}
}

void TurtleShell::OnProcessDamageStateTransition()
{
	if (GetKnockBackFlag() == false)
	{
		//���t���[�����d�������Ă���
		//�d�����I�������
		if (IsKnockBackStiffness() == false)
		{
			//�_���[�W���󂯂��̂ŃX�L�����g����悤�ɂ���
			m_difenceEnableFlag = true;
			//���ʂ̏�ԑJ�ڏ����Ɉڍs
			ProcessCommonStateTransition();
		}
		return;
	}
	//�m�b�N�o�b�N����
	ProcessKnockBack(m_charaCon);
}

void TurtleShell::OnProcessDieStateTransition()
{
	//�A�j���[�V�������I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���X�g���玩�g������
		CharactersInfoManager::GetInstance()->RemoveMobMonsterFormList(this);
		m_elaseListFlag = true;
		//���g���폜����
		DeleteGO(this);
	}
}

void TurtleShell::OnProcessVictoryStateTransition()
{
	//�A�j���[�V�������I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���ʂ̃X�e�[�g�J�ڏ������s
		ProcessCommonStateTransition();
	}
}

void TurtleShell::OnProcessAppearStateTransition()
{
	//�A�j���[�V�������I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���ʂ̃X�e�[�g�J�ڏ������s
		ProcessCommonStateTransition();
	}
}

void TurtleShell::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
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

void TurtleShell::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
