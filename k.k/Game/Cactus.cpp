#include "stdafx.h"
#include "Cactus.h"
#include "ICactusState.h"
#include "CactusStateIdle.h"
#include "CactusStateRun.h"
#include "CactusStateAttack_1.h"
#include "CactusStateAttack_2.h"
#include "CactusStatePlant.h"
#include "CactusStatePlantToBattle.h"
#include "CactusStateDamage.h"
#include "CactusStateDie.h"
#include "CactusStateVictory.h"
#include "Lich.h"

namespace {
	const float ANGLE = 70.0f;				//����p

	//�X�e�[�^�X
	int MAXHP = 120;
	int MAXMP = 500;
	int ATK = 15;
	float SPEED = 100.0f;
	const char* NAME = "Cactus";
}

Cactus::Cactus()
{
	m_angle = ANGLE;
}

Cactus::~Cactus()
{
	//if (m_lich != nullptr)
	//{
	//	//���X�g���玩�g������
	//	m_lich->RemoveAIActorFromList(this);
	//}
}

bool Cactus::Start()
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

	//4����5�͈̔͂̃C���^�[�o��
	m_angleChangeTime = rand() % 2 + 4;

	return true;
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

void Cactus::InitModel()
{
	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/Cactus/Idle.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Run].Load("Assets/animData/character/Cactus/Run.tka");
	m_animationClip[enAnimClip_Run].SetLoopFlag(true);
	m_animationClip[enAnimClip_Attack_1].Load("Assets/animData/character/Cactus/Attack1.tka");
	m_animationClip[enAnimClip_Attack_1].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_2].Load("Assets/animData/character/Cactus/Attack2.tka");
	m_animationClip[enAnimClip_Attack_2].SetLoopFlag(false);
	m_animationClip[enAnimClip_Plant].Load("Assets/animData/character/Cactus/Plant.tka");
	m_animationClip[enAnimClip_Plant].SetLoopFlag(false);
	m_animationClip[enAnimClip_PlantToBattle].Load("Assets/animData/character/Cactus/PlantToBattle.tka");
	m_animationClip[enAnimClip_PlantToBattle].SetLoopFlag(false);
	m_animationClip[enAnimClip_Damage].Load("Assets/animData/character/Cactus/Damage.tka");
	m_animationClip[enAnimClip_Damage].SetLoopFlag(false);
	m_animationClip[enAnimClip_Die].Load("Assets/animData/character/Cactus/Die.tka");
	m_animationClip[enAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enAnimClip_Victory].Load("Assets/animData/character/Cactus/Victory.tka");
	m_animationClip[enAnimClip_Victory].SetLoopFlag(true);

	m_modelRender.Init(
		"Assets/modelData/character/Cactus/Cactus.tkm",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);
	//���f���̐ÓI�I�u�W�F�N�g�쐬
	//m_monsterStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	////�R���W����������t����
	//m_monsterStaticObject.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Monster);

	m_charaCon.Init(
		20.0f,
		5.0f,
		m_position
	);

	//���W�̐ݒ�
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void Cactus::Update()
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

	Attack();

	ManageState();
	PlayAnimation();

	if (m_createAttackCollisionFlag == true)
	{
		CreateCollision();
	}

	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	m_attackBoonId = m_modelRender.FindBoneID(L"cactus_head");
}

void Cactus::Move()
{
	//��_���[�W�A�f�X���͏��������Ȃ�
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

void Cactus::Attack()
{
	//��_���[�W�A�f�X���͏��������Ȃ�
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

void Cactus::CreateCollision()
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

Vector3 Cactus::SetDirection()
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

bool Cactus::IsBumpedForest()
{
	Vector3 pos1 = m_position;
	Vector3 pos2 = m_position;
	pos1.Normalize();
	pos2 += pos1 * 20.0f;

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

void Cactus::Damage(int attack)
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

bool Cactus::RotationOnly()
{
	if (isRotationEntable() != true)
	{
		//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
		if (fabsf(m_SaveMoveSpeed.x) >= 0.001f || fabsf(m_SaveMoveSpeed.z) >= 0.001f)
		{
			m_rotation.SetRotationYFromDirectionXZ(m_SaveMoveSpeed);
			return true;
		}
	}
	return false;
}

void Cactus::ManageState()
{
	m_state->ManageState();
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
	case Cactus::enAninationState_Idle:
		m_state = new CactusStateIdle(this);
		break;
	case Cactus::enAninationState_Walk:
		m_state = new CactusStateRun(this);
		break;
	case Cactus::enAninationState_Run:
		m_state = new CactusStateRun(this);
		break;
	case Cactus::enAnimationState_Attack_1:
		m_state = new CactusStateAttack_1(this);
		break;
	case Cactus::enAnimationState_Attack_2:
		m_state = new CactusStateAttack_2(this);
		break;
	case Cactus::enAnimationState_Plant:
		m_state = new CactusStatePlant(this);
		break;
	case Cactus::enAnimationState_PlantToBattle:
		m_state = new CactusStatePlantToBattle(this);
		break;
	case Cactus::enAnimationState_Damage:
		m_state = new CactusStateDamage(this);
		break;
	case Cactus::enAnimationState_Die:
		m_state = new CactusStateDie(this);
		break;
	case Cactus::enAnimationState_Victory:
		m_state = new CactusStateVictory(this);
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
		SetNextAnimationState(enAninationState_Run);
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