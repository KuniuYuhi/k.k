#include "stdafx.h"
#include "Cactus.h"
#include "ICactusState.h"
#include "CactusStateIdle.h"
#include "CactusStateRun.h"
#include "CactusStateAttack_1.h"
#include "CactusStateDamage.h"
#include "CactusStateDie.h"
#include "CactusStateVictory.h"
#include "CactusStateAppear.h"
#include "CactusStateSkill.h"
#include "CactusStatePatrol.h"
#include "CactusStateChase.h"

#include "CharactersInfoManager.h"
#include "GameManager.h"

#include "IMobStateMachine.h"

namespace {
	const float ANGLE = 90.0f;				//����p
	const float DISTANCE_TO_PLAYER = 280.0f;			//�v���C���[�Ƃ̋���
	const float ATTACK_RANGE = 50.0f;					//�U���ł��鋗��
	const float SKILL_ATTACK_RANGE = 60.0f;				//�X�L���U���ł��鋗��
	const float STAY_RANGR = 50.0f;						//��~���鋗��
	const float ATTACK_INTAERVALE_TIME = 1.7f;			//�U������Ԋu
	const float ANGLE_RANGE = 2.0f;						//�ړ�����A���O���͈̔�
	const float POS2_LENGTH = 30.0f;
	const float ROT_SPEED = 7.5f;						//��]���x
	const float PLAYER_NEARBY_RANGE = 120.0f;			//�U��������̃v���C���[�����G�ł���͈�
	const float SKILL_TIMER_LIMMIT = 6.0f;

	//�X�e�[�^�X
	int MAXHP = 120;
	int MAXMP = 500;
	int ATK = 15;
	float SPEED = 120.0f;
	const char* NAME = "Cactus";
}

Cactus::Cactus()
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

Cactus::~Cactus()
{
	DeleteGO(m_headCollision);
}

bool Cactus::Start()
{
	//�X�e�[�^�X�̏�����
	m_status.InitCharacterStatus(
		MAXHP,
		MAXMP,
		ATK,
		SPEED,
		NAME
	);
	//���f���̏�����
	InitModel();
	//�X�e�[�g�}�V���̐���
	m_stateMachine = new IMobStateMachine(this);
	//�܂������A�j���[�V�����B���̌�s��
	SetNextAnimationState(enAnimationState_Appear);
	
	//�@�������������B
	srand((unsigned)time(NULL));

	//4����5�͈̔͂̃C���^�[�o��
	m_angleChangeTime = rand() % 2 + 4;

	return true;
}

void Cactus::InitModel()
{
	m_animationClip[enAninationClip_Idle].Load("Assets/animData/character/Cactus/Idle.tka");
	m_animationClip[enAninationClip_Idle].SetLoopFlag(true);
	m_animationClip[enAninationClip_Patrol].Load("Assets/animData/character/Cactus/Walk.tka");
	m_animationClip[enAninationClip_Patrol].SetLoopFlag(true);
	m_animationClip[enAninationClip_Chase].Load("Assets/animData/character/Cactus/Run.tka");
	m_animationClip[enAninationClip_Chase].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Attack].Load("Assets/animData/character/Cactus/Attack1.tka");
	m_animationClip[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Skill].Load("Assets/animData/character/Cactus/Attack2.tka");
	m_animationClip[enAnimationClip_Skill].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Hit].Load("Assets/animData/character/Cactus/Damage.tka");
	m_animationClip[enAnimationClip_Hit].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Die].Load("Assets/animData/character/Cactus/Die.tka");
	m_animationClip[enAnimationClip_Die].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Victory].Load("Assets/animData/character/Cactus/Victory.tka");
	m_animationClip[enAnimationClip_Victory].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Appear].Load("Assets/animData/character/Cactus/Appear.tka");
	m_animationClip[enAnimationClip_Appear].SetLoopFlag(false);

	m_modelRender.Init(
		"Assets/modelData/character/Cactus/Cactus.tkm",
		L"Assets/shader/ToonTextrue/lamp_Cactus.DDS",
		m_animationClip,
		enAnimationClip_Num,
		enModelUpAxisZ
	);
	
	m_charaCon.Init(
		22.0f,
		6.5f,
		m_position
	);

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	m_attackBoonId = m_modelRender.FindBoneID(L"cactus_head");

	//���W�̐ݒ�
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void Cactus::Update()
{
	if (IsStopProcessing() != true)
	{
		//�X�L���U���̃C���^�[�o���̌v�Z
		CalcSkillAttackIntarval();
		//�U���Ԋu�C���^�[�o��
		AttackInterval(m_attackIntervalTime);
		//�A���O���؂�ւ��C���^�[�o��
		AngleChangeTimeIntarval(m_angleChangeTime);
		//���t���[���s������
		m_stateMachine->Execute();
		//��]����
		Rotation(ROT_SPEED, ROT_SPEED);

		//�����蔻��
		DamageCollision(m_charaCon);
	}

	ManageState();
	PlayAnimation();

	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

//void Cactus::Attack()
//{
//	//��_���[�W�A�f�X���͏��������Ȃ�
//	if (isAnimationEnable() != true)
//	{
//		return;
//	}
//	//�U�����͏������Ȃ�
//	if (IsAttackEnable() != true)
//	{
//		return;
//	}
//
//	//�U��������̃C���^�[�o���Ȃ甲���o��
//	if (m_attackEnableFlag == true)
//	{
//		return;
//	}
//
//	//���̋����Ƀ^�[�Q�b�g��������
//	if (IsFindPlayer(m_attackRange) == true)
//	{
//		Vector3 toPlayerDir = m_toTarget;
//		//����p���Ƀ^�[�Q�b�g��������
//		if (IsInFieldOfView(toPlayerDir, m_forward, m_angle) == true)
//		{
//			int i = rand() % 2;
//			switch (i)
//			{
//			case enAttackName_1:
//				//�U��1
//				SetNextAnimationState(enAnimationState_Attack_1);
//				//�U�������̂Ńt���O��true�ɂ��ăC���^�[�o���ɓ���
//				m_attackEnableFlag = true;
//				m_enAttackName = enAttackName_1;
//				break;
//			case enAttackName_2:
//				//�U��2
//				SetNextAnimationState(enAnimationState_Attack_2);
//				//�U�������̂Ńt���O��true�ɂ��ăC���^�[�o���ɓ���
//				m_attackEnableFlag = true;
//				m_enAttackName = enAttackName_2;
//				break;
//			default:
//				break;
//			}
//		}
//	}
//}

bool Cactus::IsStopProcessing()
{
	//�Q�[���X�e�[�g�ȊO�Ȃ�
	if (GameManager::GetInstance()->GetGameSeenState() !=
		GameManager::enGameSeenState_Game)
	{
		return true;
	}

	//���s�����܂�����
	if (m_enOutCome != enOutCome_None)
	{
		return true;
	}

	//����������
	if (GameManager::GetInstance()->GetOutComeState()
		== GameManager::enOutComeState_PlayerLose)
	{
		//���s�X�e�[�g�̐ݒ�
		SetEnOutCome(enOutCome_Win);
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
	if (GetKnockBackFlag() == true)
	{
		//�m�b�N�o�b�N�̏���������Ȃ�
		if (IsProcessKnockBack(
			m_knockBackTimer, m_moveSpeed) == true)
		{
			//���W���ړ�
			m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
			return true;
		}
		else
		{
			SetKnockBackFlag(false);
		}
	}

	//����ȊO�Ȃ�
	return false;
}

void Cactus::CreateCollision()
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

void Cactus::Damage(int attack)
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
		150.0f
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
	
	SetNextAnimationState(enAnimationState_Hit);
}

void Cactus::ManageState()
{
	m_state->ManageState();
}

void Cactus::PlayAttackSound()
{
	switch (m_enAttackName)
	{
	case Cactus::enAttackName_1:
		g_soundManager->InitAndPlaySoundSource(enSoundName_Cactus_Attack1, g_soundManager->GetSEVolume());
		break;
	case Cactus::enAttackName_2:
		g_soundManager->InitAndPlaySoundSource(enSoundName_Cactus_Attack1, g_soundManager->GetSEVolume());
		break;
	default:
		break;
	}
}

void Cactus::PlayAnimation()
{
	m_state->PlayAnimation();
}

void Cactus::SetNextAnimationState(EnAnimationState nextState)
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
		m_state = new CactusStateIdle(this);
		break;
	case enAninationState_Patrol:
		m_state = new CactusStatePatrol(this);
		//�v���C���[��ǂ������Ă��Ȃ��̂Ńt���O�����Z�b�g
		m_chasePlayerFlag = false;
		break;
	case enAninationState_Chase:
		m_state = new CactusStateChase(this);
		//�v���C���[��ǂ������Ă���̂ŁA�t���O���Z�b�g
		m_chasePlayerFlag = true;
		break;
	case enAnimationState_Attack:
		m_state = new CactusStateAttack_1(this);
		//�U�������̂ŁA�t���O�����Z�b�g
		m_attackEnableFlag = false;
		break;
	case enAnimationState_Skill:
		m_state = new CactusStateSkill(this);
		//�X�L���U�������̂ŁA�X�L���U���g�p�\�t���O�����Z�b�g
		m_skillUsableFlag = false;
		m_attackEnableFlag = false;
		break;
	case enAnimationState_Hit:
		m_state = new CactusStateDamage(this);
		break;
	case enAnimationState_Die:
		m_state = new CactusStateDie(this);
		break;
	case enAnimationState_Victory:
		m_state = new CactusStateVictory(this);
		break;
	case enAnimationState_Appear:
		m_state = new CactusStateAppear(this);
		break;
	default:
		//�����I�ɃN���b�V��������
		std::abort();
		break;
	}
}


void Cactus::ProcessCommonStateTransition()
{
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		SetNextAnimationState(enAninationState_Patrol);
	}
	else
	{
		SetNextAnimationState(enAninationState_Idle);
	}
}

void Cactus::OnProcessAttack_1StateTransition()
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

void Cactus::OnProcessAttack_2StateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{

		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Cactus::OnProcessPlantStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{

		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Cactus::OnProcessPlantToBattleStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{

		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Cactus::OnProcessDamageStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{

		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}


void Cactus::OnProcessDieStateTransition()
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

void Cactus::OnProcessVictoryStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Cactus::OnProcessAppearStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Cactus::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
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

void Cactus::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}