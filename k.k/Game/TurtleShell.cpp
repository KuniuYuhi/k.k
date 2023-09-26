#include "stdafx.h"
#include "TurtleShell.h"
#include "ITurtleShellState.h"
#include "TurtleShellStateIdle.h"
#include "TurtleShellStateWalk.h"
#include "TurtleShellStateAttack_1.h"
#include "TurtleShellStateAttack_2.h"
#include "TurtleShellStateDifence.h"
#include "TurtleShellStateDifenceDamage.h"
#include "TurtleShellStateDamage.h"
#include "TurtleShellStateDie.h"
#include "TurtleShellStateVictory.h"
#include "TurtleShellStateAppear.h"

#include "Lich.h"

namespace {
	const float ANGLE = 45.0f;				//����p
	const float DISTANCE_TO_PLAYER = 500.0f;			//�v���C���[�Ƃ̋���
	const float ATTACK_RANGE = 50.0f;					//�U���ł��鋗��
	const float STAY_RANGR = 45.0f;						//��~���鋗��
	const float ATTACK_INTAERVALE_TIME = 2.5f;			//�U������Ԋu
	const float ANGLE_RANGE = 2.0f;						//�ړ�����A���O���͈̔�
	const float POS2_LENGTH = 30.0f;
	const float ROT_SPEED = 3.8f;

	//�X�e�[�^�X
	int MAXHP = 200;
	int MAXMP = 500;
	int ATK = 15;
	float SPEED = 80.0f;
	const char* NAME = "TurtleShell";
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
}

TurtleShell::~TurtleShell()
{
	//if (m_lich != nullptr)
	//{
	//	//���X�g���玩�g������
	//	m_lich->RemoveAIActorFromList(this);
	//}
}

//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
//struct IsForestResult :public btCollisionWorld::ConvexResultCallback
//{
//	bool isHit = false;						//�Փ˃t���O�B
//	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
//	{
//		//�n�ʂƂԂ����ĂȂ�������B
//		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall) {
//			//�Փ˂����͕̂ǂł͂Ȃ��B
//			isHit = false;
//			return 0.0f;
//		}
//		else
//		{
//			//�n�ʂƂԂ�������B
//		//�t���O��true�ɁB
//			isHit = true;
//			return 0.0f;
//		}
//
//	}
//};

bool TurtleShell::Start()
{
	//�����̃A�j���[�V�����X�e�[�g��ҋ@��Ԃɂ���B
	SetNextAnimationState(enAninationState_Idle);

	m_status.InitStatus(
		MAXHP,
		MAXMP,
		ATK,
		SPEED,
		NAME
	);

	InitModel();

	//�܂������A�j���[�V�����B���̌�s��
	SetNextAnimationState(enAnimationState_Appear);

	//�@�������������B
	srand((unsigned)time(NULL));

	//6����8�͈̔͂̃C���^�[�o��
	m_angleChangeTime = rand() % 3 + 6;
	return true;
}

void TurtleShell::InitModel()
{
	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/TurtleShell/Idle_Normal.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Walk].Load("Assets/animData/character/TurtleShell/Walk.tka");
	m_animationClip[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClip[enAnimClip_Walk].Load("Assets/animData/character/TurtleShell/Run.tka");
	m_animationClip[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClip[enAnimClip_Attack_1].Load("Assets/animData/character/TurtleShell/Attack1.tka");
	m_animationClip[enAnimClip_Attack_1].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_2].Load("Assets/animData/character/TurtleShell/Attack2.tka");
	m_animationClip[enAnimClip_Attack_2].SetLoopFlag(false);
	m_animationClip[enAnimClip_Difence].Load("Assets/animData/character/TurtleShell/Defence.tka");
	m_animationClip[enAnimClip_Difence].SetLoopFlag(false);
	m_animationClip[enAnimClip_DefenceDamage].Load("Assets/animData/character/TurtleShell/DefenceDamage.tka");
	m_animationClip[enAnimClip_DefenceDamage].SetLoopFlag(false);
	m_animationClip[enAnimClip_Damage].Load("Assets/animData/character/TurtleShell/Damage.tka");
	m_animationClip[enAnimClip_Damage].SetLoopFlag(false);
	m_animationClip[enAnimClip_Die].Load("Assets/animData/character/TurtleShell/Die.tka");
	m_animationClip[enAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enAnimClip_Victory].Load("Assets/animData/character/TurtleShell/Victory.tka");
	m_animationClip[enAnimClip_Victory].SetLoopFlag(true);
	m_animationClip[enAnimClip_Appear].Load("Assets/animData/character/TurtleShell/Appear.tka");
	m_animationClip[enAnimClip_Appear].SetLoopFlag(false);

	m_modelRender.Init(
		"Assets/modelData/character/TurtleShell/TurtleShell.tkm",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);
	//���f���̐ÓI�I�u�W�F�N�g�쐬
	//m_monsterStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	////�R���W����������t����
	//m_monsterStaticObject.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Monster);

	m_charaCon.Init(
		22.0f,
		4.0f,
		m_position
	);

	//���W�̐ݒ�
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	m_attackBoonId = m_modelRender.FindBoneID(L"Head");
}

void TurtleShell::Update()
{
	if (m_lich != nullptr)
	{
		if (m_lich->GetWinFlag() == true)
		{
			SetWinFlag(true);
			//�U�����łȂ����
			SetNextAnimationState(enAnimationState_Victory);
		}
		if (m_lich->GetTimeUpEndFlag() == true)
		{
			SetWinFlag(true);
			SetNextAnimationState(enAninationState_Idle);
		}
	}

	if (IsStopProcessing() == true)
	{
		ManageState();
		PlayAnimation();
		m_modelRender.Update();
		return;
	}

	AttackInterval(m_attackIntervalTime);

	DamageCollision(m_charaCon);

	AngleChangeTimeIntarval(m_angleChangeTime);

	Move(m_charaCon);
	Rotation(ROT_SPEED, ROT_SPEED);

	ManageState();
	PlayAnimation();

	DecideNextAction();

	if (m_createAttackCollisionFlag == true)
	{
		CreateCollision();
	}

	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void TurtleShell::DecideNextAction()
{
	//��_���[�W�A�f�X���͏������Ȃ�
	if (isAnimationEntable() != true)
	{
		return;
	}
	//�U�����͏������Ȃ�
	if (IsAttackEntable() != true)
	{
		return;
	}
	if (m_enAnimationState == enAnimationState_Difence)
	{
		return;
	}

	//���̋����Ƀ^�[�Q�b�g��������
	if (IsFindPlayer(m_distanceToPlayer) == true)
	{
		Vector3 toPlayerDir = m_toTarget;
		//����p���Ƀ^�[�Q�b�g��������
		if (IsInFieldOfView(toPlayerDir, m_forward, m_angle) == true)
		{
			//�U�����邩�K�[�h���邩���߂�
			//�_���[�W���󂯂���
			if (m_defenceState == enDefenceState_damaged)
			{
				if (Difence() == true)
				{
					//�h�䂷��̂ōU�����������Ȃ�
					return;
				}
			}
			//�h�䂵�Ă��Ȃ��Ȃ�
			if (m_defenceState != enDefenceState_Defence)
			{
				if (m_attackFlag == false)
				{
					Attack();
				}
				
			}
		}
	}
}

void TurtleShell::Attack()
{
	//���̋����Ƀ^�[�Q�b�g��������
	if (IsFindPlayer(m_attackRange) == true)
	{
		Vector3 toPlayerDir = m_toTarget;
		//����p���Ƀ^�[�Q�b�g��������
		if (IsInFieldOfView(toPlayerDir, m_forward, m_angle) == true)
		{
			//�U��
			SetNextAnimationState(enAnimationState_Attack_1);
			//�U�������̂Ńt���O��true�ɂ��ăC���^�[�o���ɓ���
			m_attackFlag = true;
		}
	}
}

bool TurtleShell::Difence()
{
	//�U���\�����Ƀ^�[�Q�b�g��������
	if (IsFindPlayer(m_distanceToPlayer) == true)
	{
		//�m���Ŗh��
		int i = rand() % 4;
		if (i >= 2)
		{
			//�h�䂷��
			m_defenceState = enDefenceState_Defence;
			SetNextAnimationState(enAnimationState_Difence);

			return true;
		}
	}
	return false;
}

void TurtleShell::CreateCollision()
{
	auto HeadCollision = NewGO<CollisionObject>(0, "monsterattack");
	HeadCollision->SetCreatorName(GetName());
	HeadCollision->CreateSphere(
		m_position,
		m_rotation,
		17.0f
	);
	//���[���h���W�擾
	Matrix HeadMatrix = m_modelRender.GetBone(m_attackBoonId)->GetWorldMatrix();
	HeadCollision->SetWorldMatrix(HeadMatrix);
}

bool TurtleShell::IsStopProcessing()
{
	//����������
	if (GetWinFlag() == true)
	{
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

bool TurtleShell::IsDifenceTime()
{
	if (m_difenceTime < m_difenceTimer)
	{
		//�h���Ԃ���߂�
		m_difenceFlag = false;
		m_difenceTimer = 0.0f;
		return false;
	}
	else
	{
		//�h�䒆
		m_difenceTimer += g_gameTime->GetFrameDeltaTime();
		return true;
	}
}

void TurtleShell::Damage(int attack)
{
	//�U������������Ȃ��̂œ����蔻�������
	m_createAttackCollisionFlag = false;

	//�h���ԂȂ�_���[�W�����炷
	if (m_defenceState == enDefenceState_Defence)
	{
		m_damage = 0;
		return;
	}
	//�h���Ԃł͂Ȃ��Ȃ�
	else 
	{
		//�U�����󂯂�
		m_defenceState = enDefenceState_damaged;
	}
	
	//HP�����炷
	m_status.hp -= m_damage;

	//HP��0�ȉ��Ȃ�
	if (m_status.hp <= 0)
	{
		m_status.hp = 0;
		SetNextAnimationState(enAnimationState_Die);
		Dead();
		return;
	}

	SetNextAnimationState(enAnimationState_Damage);

	//�����h�䒆�Ȃ�
	/*if (m_defenceState == enDefenceState_Defence)
	{
		m_defenceState = enDefenceState_DefenceDamaged;
		SetNextAnimationState(enAnimationState_DifenceDamage);
	}
	else
	{
		SetNextAnimationState(enAnimationState_Damage);
	}*/
	
	
}

bool TurtleShell::RotationOnly()
{
	if (m_enAnimationState == enAnimationState_Difence)
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
	case TurtleShell::enAninationState_Idle:
		m_state = new TurtleShellStateIdle(this);
		break;
	case TurtleShell::enAninationState_Walk:
		m_state = new TurtleShellStateWalk(this);
		break;
	case TurtleShell::enAninationState_Run:
		m_state = new TurtleShellStateWalk(this);
		break;
	case TurtleShell::enAnimationState_Attack_1:
		m_state = new TurtleShellStateAttack_1(this);
		break;
	case TurtleShell::enAnimationState_Attack_2:
		m_state = new TurtleShellStateAttack_2(this);
		break;
	case TurtleShell::enAnimationState_Difence:
		m_state = new TurtleShellStateDifence(this);
		break;
	case TurtleShell::enAnimationState_DifenceDamage:
		m_state = new TurtleShellStateDifenceDamage(this);
		break;
	case TurtleShell::enAnimationState_Damage:
		m_state = new TurtleShellStateDamage(this);
		break;
	case TurtleShell::enAnimationState_Die:
		m_state = new TurtleShellStateDie(this);
		break;
	case TurtleShell::enAnimationState_Victory:
		m_state = new TurtleShellStateVictory(this);
		break;
	case TurtleShell::enAnimationState_Appear:
		m_state = new TurtleShellStateAppear(this);
		break;
	default:
		// �����ɗ�����X�e�[�g�̃C���X�^���X�쐬�����̒ǉ��Y��B
		std::abort();
		break;
	}
}

void TurtleShell::ProcessCommonStateTransition()
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

void TurtleShell::OnProcessAttack_1StateTransition()
{
	//�A�j���[�V�������I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���ʂ̃X�e�[�g�J�ڏ������s
		ProcessCommonStateTransition();
	}
}

void TurtleShell::OnProcessAttack_2StateTransition()
{
	//�A�j���[�V�������I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���ʂ̃X�e�[�g�J�ڏ������s
		ProcessCommonStateTransition();
	}
}

void TurtleShell::OnProcessDifenceStateTransition()
{
	//�^�C�}�[��false�ɂȂ����甲���o��
	if (IsDifenceTime() == false)
	{
		//�_���[�W���󂯂��t���O��false�ɂ���
		m_defenceState = enDefenceState_None;
		//���ʂ̃X�e�[�g�J�ڏ������s
		ProcessCommonStateTransition();
	}
}

void TurtleShell::OnProcessDifenceDamageStateTransition()
{
	//�A�j���[�V�������I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		////�܂��h�䒆�Ȃ�
		//if (m_difenceFlag == true)
		//{
		//	//�_���[�W���󂯂��t���O��false�ɂ���
		//	m_damagedFlag = false;
		//	//�h��X�e�[�g�ɂ���
		//	SetNextAnimationState(enAnimationState_Difence);
		//	return;
		//}
		//�h���Ԃ�����
		m_defenceState = enDefenceState_None;
		//�^�C�}�[���Z�b�g
		m_difenceTimer = 0.0f;
		////�_���[�W���󂯂��t���O��false�ɂ���
		//m_damagedFlag = false;
		//���ʂ̃X�e�[�g�J�ڏ������s
		ProcessCommonStateTransition();
	}
}

void TurtleShell::OnProcessDamageStateTransition()
{
	//�A�j���[�V�������I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�_���[�W���󂯂��t���O��false�ɂ���
		//m_damagedFlag = false;
		//���ʂ̃X�e�[�g�J�ڏ������s
		ProcessCommonStateTransition();
	}
}

void TurtleShell::OnProcessDieStateTransition()
{
	//�A�j���[�V�������I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		if (m_lich != nullptr)
		{
			//���X�g���玩�g������
			m_lich->RemoveAIActorFromList(this);
			m_elaseListFlag = true;
		}
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
