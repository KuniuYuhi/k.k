#include "stdafx.h"
#include "Mushroom.h"
#include "IMushroomState.h"
#include "MushroomStateIdle.h"
#include "MushroomStateWalk.h"
#include "MushroomStateAttack_1.h"
#include "MushroomStateAttack_2.h"
#include "MushroomStateDamage.h"
#include "MushroomStateDie.h"
#include "MushroomStateVictory.h"
#include "Lich.h"

namespace {
	const float ANGLE = 50.0f;				//����p

	//�X�e�[�^�X
	int MAXHP = 100;
	int MAXMP = 500;
	int ATK = 7;
	float SPEED = 110.0f;
	const char* NAME = "Mushroom";
}

Mushroom::Mushroom()
{
	m_angle = ANGLE;
}

Mushroom::~Mushroom()
{
	//if (m_lich != nullptr)
	//{
	//	//���X�g���玩�g������
	//	m_lich->RemoveAIActorFromList(this);
	//}
}

//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
struct IsForestResult :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//�Փ˃t���O�B
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//�n�ʂƂԂ����ĂȂ�������B
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall) {
			//�Փ˂����͕̂ǂł͂Ȃ��B
			isHit = false;
			return 0.0f;
		}
		else
		{
			//�n�ʂƂԂ�������B
		//�t���O��true�ɁB
			isHit = true;
			return 0.0f;
		}

	}
};

bool Mushroom::Start()
{
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



	//�@�������������B
	srand((unsigned)time(NULL));

	//3����5�͈̔͂̃C���^�[�o��
	m_angleChangeTime = rand() % 3 + 3;

	return true;
}

void Mushroom::InitModel()
{
	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/Mushroom/Idle_normal.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Walk].Load("Assets/animData/character/Mushroom/Walk.tka");
	m_animationClip[enAnimClip_Walk].SetLoopFlag(true);
	/*m_animationClip[enAnimClip_Walk].Load("Assets/animData/character/Mushroom/Run.tka");
	m_animationClip[enAnimClip_Walk].SetLoopFlag(true);*/
	m_animationClip[enAnimClip_Attack_1].Load("Assets/animData/character/Mushroom/Attack1.tka");
	m_animationClip[enAnimClip_Attack_1].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_2].Load("Assets/animData/character/Mushroom/Attack2.tka");
	m_animationClip[enAnimClip_Attack_2].SetLoopFlag(false);
	m_animationClip[enAnimClip_Damage].Load("Assets/animData/character/Mushroom/Damage.tka");
	m_animationClip[enAnimClip_Damage].SetLoopFlag(false);
	m_animationClip[enAnimClip_Die].Load("Assets/animData/character/Mushroom/Die.tka");
	m_animationClip[enAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enAnimClip_Victory].Load("Assets/animData/character/Mushroom/Victory.tka");
	m_animationClip[enAnimClip_Victory].SetLoopFlag(true);

	m_modelRender.Init(
		"Assets/modelData/character/Mushroom/Mushroom.tkm",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);


	m_charaCon.Init(
		20.0f,
		5.0f,
		m_position
	);

	//���W�̐ݒ�
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	m_attackBoonId = m_modelRender.FindBoneID(L"mushroom_spine03");
}

void Mushroom::Update()
{
	if (m_lich != nullptr)
	{
		//�v���C���[���{�X�����ꂽ�������
		if (m_lich->GetWinFlag() == true)
		{
			SetWinFlag(true);
			//�U�����łȂ����
			SetNextAnimationState(enAnimationState_Victory);
		}
	}

	if (GetWinFlag() == true)
	{
		ManageState();
		PlayAnimation();
		m_modelRender.Update();
		return;
	}

	AttackInterval(m_attackIntervalTime);

	DamageCollision(m_charaCon);

	AngleChangeTimeIntarval(m_angleChangeTime);

	Move();
	Rotation();

	//�U��
	Attack();

	ManageState();
	PlayAnimation();

	if (m_createAttackCollisionFlag == true)
	{
		CreateCollision();
	}

	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void Mushroom::Move()
{
	//����̃A�j���[�V�������Đ����Ȃ甲���o��
	if (isAnimationEntable() != true)
	{
		return;
	}
	//�U�����͏������Ȃ�
	if (IsAttackEntable() != true)
	{
		return;
	}

	//���E�Ƀ^�[�Q�b�g����������
	if (IsFindPlayer(m_distanceToPlayer) == true)
	{
		Vector3 toPlayerDir = m_toTarget;
		//����p���Ƀ^�[�Q�b�g��������
		if (IsInFieldOfView(toPlayerDir, m_forward, m_angle) == true)
		{
			toPlayerDir.Normalize();
			//�ǂ�������
			m_direction = toPlayerDir;
			//m_moveSpeed = CalcVelocity(m_status, m_direction);
			m_moveSpeed = m_direction * m_status.defaultSpeed;
			m_SaveMoveSpeed = m_moveSpeed;
		}
		else
		{
			//����p���ɂ͂��Ȃ����U���\�����ɂ���Ȃ�
			if (IsFindPlayer(100.0f) == true)
			{
				m_moveSpeed = CalcVelocity(m_status, m_targetPosition);
				m_SaveMoveSpeed = m_moveSpeed;
			}
		}
	}
	else
	{
		//���b�Ԋu�Ō������x�N�g����ς���
		if (m_angleChangeTimeFlag == false)
		{
			m_direction = SetDirection();
			m_angleChangeTimeFlag = true;
		}
		//�����_���ȕ����Ɉړ�
		m_moveSpeed = m_direction * m_status.defaultSpeed;
		m_SaveMoveSpeed = m_moveSpeed;
	}

	//�ǂɂԂ������甽�]
	if (IsBumpedForest() == true)
	{
		m_direction *= -1.0f;
		m_moveSpeed = m_direction * m_status.defaultSpeed;
		m_SaveMoveSpeed = m_moveSpeed;
		m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
		return;
	}


	//�v���C���[�Ƃ̋������߂��Ȃ��Ȃ�ړ�����
	if (IsFindPlayer(m_stayDistance) != true)
	{
		m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	}
	else
	{
		//�͈͓��ɂ���̂ňړ����Ȃ�
		m_moveSpeed = Vector3::Zero;
	}
}

void Mushroom::Attack()
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

	//�U��������̃C���^�[�o���Ȃ甲���o��
	if (m_attackFlag == true)
	{
		return;
	}

	//���̋����Ƀ^�[�Q�b�g��������
	if (IsFindPlayer(m_attackRange) == true)
	{
		Vector3 toPlayerDir = m_toTarget;
		//����p���Ƀ^�[�Q�b�g��������
		if (IsInFieldOfView(toPlayerDir, m_forward, m_angle) == true)
		{
			int i = rand() % 2;
			switch (i)
			{
			case enAttackName_1:
				//�U��
				SetNextAnimationState(enAnimationState_Attack_1);
				//�U�������̂Ńt���O��true�ɂ��ăC���^�[�o���ɓ���
				m_attackFlag = true;
				break;
			case enAttackName_2:
				//�U��
				SetNextAnimationState(enAnimationState_Attack_2);
				//�U�������̂Ńt���O��true�ɂ��ăC���^�[�o���ɓ���
				m_attackFlag = true;
				break;
			default:
				break;
			}
		}
	}
}

void Mushroom::CreateCollision()
{
	auto HeadCollision = NewGO<CollisionObject>(0, "monsterattack");
	HeadCollision->SetCreatorName(GetName());
	HeadCollision->CreateSphere(
		m_position,
		m_rotation,
		16.0f
	);
	//���[���h���W�擾
	Matrix HeadMatrix = m_modelRender.GetBone(m_attackBoonId)->GetWorldMatrix();
	HeadCollision->SetWorldMatrix(HeadMatrix);
}

Vector3 Mushroom::SetDirection()
{
	Vector3 randomPos = g_vec3Zero;
	randomPos.y = 0.0f;
	float X = (rand() % (2 - (-2) + 1)) + (-2);
	float Z = (rand() % (2 - (-2) + 1)) + (-2);
	randomPos.x += X;
	randomPos.z += Z;
	randomPos.Normalize();

	return randomPos;
}

bool Mushroom::IsBumpedForest()
{
	Vector3 pos1 = m_position;
	Vector3 pos2 = m_position;
	pos1.Normalize();
	pos2 += pos1 * 30.0f;

	SphereCollider m_sphereCollider;
	m_sphereCollider.Create(1.0f);

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//�n�_�̓G�l�~�[�̍��W�B
	start.setOrigin(btVector3(m_position.x, m_position.y, m_position.z));
	//�I�_�̓v���C���[�̍��W�B
	end.setOrigin(btVector3(
		pos2.x, pos2.y, pos2.z));

	//�ǂ̔����Ԃ�
	IsForestResult callback_Forest;
	//�R���C�_�[���n�_����I�_�܂œ������āB
	//�ǂƏՓ˂��邩�ǂ����𒲂ׂ�B
	PhysicsWorld::GetInstance()->ConvexSweepTest(
		(const btConvexShape*)m_sphereCollider.GetBody(),
		start, end, callback_Forest);
	//�X�ɏՓ˂���
	if (callback_Forest.isHit == true)
	{
		return true;
	}
	else
	{
		//�Փ˂��Ȃ�����
		return false;
	}
}

void Mushroom::Damage(int attack)
{
	//�U������������Ȃ��̂œ����蔻��𐶐����Ȃ��悤�ɂ���
	m_createAttackCollisionFlag = false;
	//HP�����炷
	m_status.hp -= attack;

	//HP��0�ȉ��Ȃ�
	if (m_status.hp <= 0)
	{
		m_status.hp = 0;
		SetNextAnimationState(enAnimationState_Die);
		return;
	}

	//�����h�䒆�Ȃ�

	SetNextAnimationState(enAnimationState_Damage);
}

bool Mushroom::RotationOnly()
{
	return false;
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
	case Mushroom::enAninationState_Idle:
		m_state = new MushroomStateIdle(this);
		break;
	case Mushroom::enAninationState_Walk:
		m_state = new MushroomStateWalk(this);
		break;
	case Mushroom::enAninationState_Run:
		m_state = new MushroomStateWalk(this);
		break;
	case Mushroom::enAnimationState_Attack_1:
		m_state = new MushroomStateAttack_1(this);
		break;
	case Mushroom::enAnimationState_Attack_2:
		m_state = new MushroomStateAttack_2(this);
		break;
	case Mushroom::enAnimationState_Damage:
		m_state = new MushroomStateDamage(this);
		break;
	case Mushroom::enAnimationState_Die:
		m_state = new MushroomStateDie(this);
		break;
	case Mushroom::enAnimationState_Victory:
		m_state = new MushroomStateVictory(this);
		break;
	default:
		// �����ɗ�����X�e�[�g�̃C���X�^���X�쐬�����̒ǉ��Y��B
		std::abort();
		break;
	}
}

void Mushroom::ProcessCommonStateTransition()
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

void Mushroom::OnProcessAttack_1StateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{

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
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{

		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Mushroom::OnProcessDieStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
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

void Mushroom::OnProcessVictoryStateTransition()
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


