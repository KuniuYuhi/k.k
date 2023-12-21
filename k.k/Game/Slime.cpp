#include "stdafx.h"
#include "Slime.h"
#include "ISlimeState.h"
#include "SlimeStateIdle.h"
#include "SlimeStateWalk.h"
#include "SlimeStateAttack.h"
#include "SlimeStateDamage.h"
#include "SlimeStateDie.h"
#include "SlimeStateVictory.h"
#include "SlimeStateAppear.h"
#include "SlimeStateChase.h"
#include "SlimeStatePatrol.h"
#include "SlimeStateSkill.h"


#include "CharactersInfoManager.h"
#include "GameManager.h"
#include "InitEffect.h"

#include "IMobStateMachine.h"

namespace {
	const float ANGLE = 70.0f;				//����p
	const float DISTANCE_TO_PLAYER = 300.0f;			//�v���C���[�Ƃ̋���
	const float ATTACK_RANGE = 50.0f;					//�U���ł��鋗��
	const float SKILL_ATTACK_RANGE = 70.0f;				//�X�L���U���ł��鋗��
	const float STAY_RANGR = 45.0f;						//��~���鋗��
	const float ATTACK_INTAERVALE_TIME = 2.0f;			//�U������Ԋu
	const float PLAYER_NEARBY_RANGE = 140.0f;			//�U��������̃v���C���[�����G�ł���͈�
	const float ANGLE_RANGE = 2.0f;						//�ړ�����A���O���͈̔�
	const float POS2_LENGTH = 30.0f;
	const float ROT_SPEED = 7.5f;
	const float SKILL_TIMER_LIMMIT = 8.0f;

	//�X�e�[�^�X
	int MAXHP = 150;
	int MAXMP = 500;
	int ATK = 10;
	float SPEED = 110.0f;
	const char* NAME = "Slime";
}

Slime::Slime()
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

Slime::~Slime()
{
	delete m_stateMachine;
	DeleteGO(m_headCollision);
}

bool Slime::Start()
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


	//4����U�͈̔͂̃C���^�[�o��
	m_angleChangeTime = rand() % 3 + 4;

	
	return true;
}

void Slime::InitModel()
{
	m_animationClip[enAnimationClip_Idle].Load("Assets/animData/character/Slime/Idle_Normal.tka");
	m_animationClip[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Patrol].Load("Assets/animData/character/Slime/Walk.tka");
	m_animationClip[enAnimationClip_Patrol].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Chase].Load("Assets/animData/character/Slime/Run.tka");
	m_animationClip[enAnimationClip_Chase].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Attack].Load("Assets/animData/character/Slime/Attack1.tka");
	m_animationClip[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Skill].Load("Assets/animData/character/Slime/Skill.tka");
	m_animationClip[enAnimationClip_Skill].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Hit].Load("Assets/animData/character/Slime/Damege.tka");
	m_animationClip[enAnimationClip_Hit].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Die].Load("Assets/animData/character/Slime/Die.tka");
	m_animationClip[enAnimationClip_Die].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Victory].Load("Assets/animData/character/Slime/Victory.tka");
	m_animationClip[enAnimationClip_Victory].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Appear].Load("Assets/animData/character/Slime/Appear.tka");
	m_animationClip[enAnimationClip_Appear].SetLoopFlag(false);

	m_modelRender.Init(
		"Assets/modelData/character/Slime/slime.tkm",
		L"Assets/shader/ToonTextrue/lamp_Slime.DDS",
		m_animationClip, 
		enAnimationClip_Num,
		enModelUpAxisZ
	);
	
	m_charaCon.Init(
		22.0f,
		9.0f,
		m_position
	);

	//���W�̐ݒ�
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//�U���p�{�[���̎擾
	m_attackBoonId = m_modelRender.FindBoneID(L"Head");

}

void Slime::Update()
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

bool Slime::IsStopProcessing()
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
	if (GetKnockBackFlag() == true)
	{
		//�m�b�N�o�b�N�̏���������Ȃ�
		if (IsProcessKnockBack(
			m_moveSpeed, m_knockBackTimer) == true)
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

void Slime::CreateCollision()
{
	m_headCollision = NewGO<CollisionObject>(0, "monsterattack");
	m_headCollision->SetCreatorName(GetName());
	m_headCollision->CreateSphere(
		m_position,
		m_rotation,
		17.0f
	);
	//���[���h���W�擾
	Matrix HeadMatrix = m_modelRender.GetBone(m_attackBoonId)->GetWorldMatrix();
	m_headCollision->SetWorldMatrix(HeadMatrix);
}

void Slime::Damage(int attack)
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

void Slime::ManageState()
{
	m_state->ManageState();
}

void Slime::PlayAttackSound()
{
	g_soundManager->InitAndPlaySoundSource(enSoundName_Slime_Attack, g_soundManager->GetSEVolume());
}

void Slime::PlayAnimation()
{
	m_state->PlayAnimation();
}

void Slime::SetNextAnimationState(EnAnimationState nextState)
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
		m_state = new SlimeStateIdle(this);
		break;
	case enAninationState_Patrol:
		m_state = new SlimeStatePatrol(this);
		//�v���C���[��ǂ������Ă��Ȃ��̂Ńt���O�����Z�b�g
		m_chasePlayerFlag = false;
		break;
	case enAninationState_Chase:
		m_state = new SlimeStateChase(this);
		//�v���C���[��ǂ������Ă���̂ŁA�t���O���Z�b�g
		m_chasePlayerFlag = true;
		break;
	case enAnimationState_Attack:
		m_state = new SlimeStateAttack(this);
		//�U�������̂ŁA�t���O�����Z�b�g
		m_attackEnableFlag = false;
		break;
	case enAnimationState_Skill:
		m_state = new SlimeStateSkill(this);
		//�X�L���U�������̂ŁA�X�L���U���g�p�\�t���O�����Z�b�g
		m_skillUsableFlag = false;
		m_attackEnableFlag = false;
		break;
	case enAnimationState_Hit:
		m_state = new SlimeStateDamage(this);
		break;
	case enAnimationState_Die:
		m_state = new SlimeStateDie(this);
		break;
	case enAnimationState_Victory:
		m_state = new SlimeStateVictory(this);
		break;
	case enAnimationState_Appear:
		m_state = new SlimeStateAppear(this);
		break;
	default:
		// �����ɗ�����X�e�[�g�̃C���X�^���X�쐬�����̒ǉ��Y��B
		std::abort();
		break;
	}
}

void Slime::ProcessCommonStateTransition()
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

void Slime::OnProcessAttack_1StateTransition()
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

void Slime::OnProcessDamageStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{

		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Slime::OnProcessDieStateTransition()
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

void Slime::OnProcessVictoryStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{

		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Slime::OnProcessAppearStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Slime::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
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

void Slime::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}