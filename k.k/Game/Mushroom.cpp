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
#include "MushroomStateAppear.h"

#include "CharactersInfoManager.h"

#include "Lich.h"

namespace {
	const float ANGLE = 60.0f;				//����p
	const float DISTANCE_TO_PLAYER = 300.0f;			//�v���C���[�Ƃ̋���
	const float ATTACK_RANGE = 45.0f;					//�U���ł��鋗��
	const float STAY_RANGR = 45.0f;						//��~���鋗��
	const float ATTACK_INTAERVALE_TIME = 1.5f;			//�U������Ԋu
	const float ANGLE_RANGE = 2.0f;						//�ړ�����A���O���͈̔�
	const float POS2_LENGTH = 27.0f;
	const float ROT_SPEED = 8.0f;

	//�X�e�[�^�X
	int MAXHP = 100;
	int MAXMP = 500;
	int ATK = 7;
	float SPEED = 130.0f;
	const char* NAME = "Mushroom";
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
}

Mushroom::~Mushroom()
{
	//if (m_lich != nullptr)
	//{
	//	//���X�g���玩�g������
	//	m_lich->RemoveAIActorFromList(this);
	//}
}

bool Mushroom::Start()
{
	SetNextAnimationState(enAninationState_Idle);

	m_status.InitCharacterStatus(
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
	m_animationClip[enAnimClip_Appear].Load("Assets/animData/character/Mushroom/Appear.tka");
	m_animationClip[enAnimClip_Appear].SetLoopFlag(false);

	m_modelRender.Init(
		"Assets/modelData/character/Mushroom/Mushroom.tkm",
		L"Assets/shader/ToonTextrue/lamp_Mushroom.DDS",
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
		6.0f,
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
	if (IsStopProcessing() != true)
	{
		//�U���Ԋu�C���^�[�o��
		AttackInterval(m_attackIntervalTime);
		//�����蔻��
		DamageCollision(m_charaCon);
		//�A���O���؂�ւ��C���^�[�o��
		AngleChangeTimeIntarval(m_angleChangeTime);
		//�ړ�����
		Move(m_charaCon);
		//��]����
		Rotation(ROT_SPEED, ROT_SPEED);
		//�U������
		Attack();
		//�����蔻��̐���
		if (m_createAttackCollisionFlag == true)
		{
			CreateCollision();
		}
	}

	ManageState();
	PlayAnimation();

	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void Mushroom::Attack()
{
	//��_���[�W�A�f�X���͏������Ȃ�
	if (isAnimationEnable() != true)
	{
		return;
	}

	//�U�����͏������Ȃ�
	if (IsAttackEnable() != true)
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

bool Mushroom::IsStopProcessing()
{
	//���s�����܂�����
	if (m_enOutCome != enOutCome_None)
	{
		return true;
	}

	//����������
	if (m_lich != nullptr)
	{
		if (m_lich->GetEnOutCome() == enOutCome_Win)
		{
			//���s�X�e�[�g�̐ݒ�
			SetEnOutCome(enOutCome_Win);
			SetWinFlag(true);
			//�U�����łȂ����
			SetNextAnimationState(enAnimationState_Victory);
			return true;
		}
		//��������
		if (m_lich->GetEnOutCome() == enOutCome_Lose)
		{
			SetNextAnimationState(enAninationState_Idle);
			return true;
		}
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

void Mushroom::Damage(int attack)
{
	//�U������������Ȃ��̂œ����蔻��𐶐����Ȃ��悤�ɂ���
	m_createAttackCollisionFlag = false;
	//HP�����炷
	m_status.CalcHp(attack, false);

	//HP��0�ȉ��Ȃ�
	if (m_status.hp <= 0)
	{
		//����A�j���[�V�����X�e�[�g
		m_status.SetHp(0);
		SetNextAnimationState(enAnimationState_Die);
		ProcessDead();
		return;
	}
	//���̍U���Ƀm�b�N�o�b�N���ʂ�����Ȃ�
	if (m_player->GetKnockBackAttackFlag() == true)
	{
		//�m�b�N�o�b�N�t���O���Z�b�g
		SetKnockBackFlag(true);
		//�m�b�N�o�b�N������������߂�
		m_moveSpeed = SetKnockBackDirection(
			m_player->GetAttackPosition(),
			m_position,
			m_player->GetKnockBackPower()
		);
	}
	SetNextAnimationState(enAnimationState_Damage);
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
	case Mushroom::enAnimationState_Appear:
		m_state = new MashroomStateAppear(this);
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
			CharactersInfoManager::GetInstance()->RemoveMobMonsterFormList(this);
			//m_lich->RemoveAIActorFromList(this);
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


