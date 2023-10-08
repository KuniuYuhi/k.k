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

#include "Lich.h"
#include "InitEffect.h"

namespace {
	const float ANGLE = 70.0f;				//����p
	const float DISTANCE_TO_PLAYER = 200.0f;			//�v���C���[�Ƃ̋���
	const float ATTACK_RANGE = 50.0f;					//�U���ł��鋗��
	const float STAY_RANGR = 45.0f;						//��~���鋗��
	const float ATTACK_INTAERVALE_TIME = 2.0f;			//�U������Ԋu
	const float ANGLE_RANGE = 2.0f;						//�ړ�����A���O���͈̔�
	const float POS2_LENGTH = 30.0f;
	const float ROT_SPEED = 7.5f;

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
}

Slime::~Slime()
{
	//if (m_lich != nullptr)
	//{
	//	//���X�g���玩�g������
	//	m_lich->RemoveAIActorFromList(this);
	//}
}

bool Slime::Start()
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


	//4����U�͈̔͂̃C���^�[�o��
	m_angleChangeTime = rand() % 3 + 4;

	return true;
}

void Slime::InitModel()
{
	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/Slime/Idle_Normal.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Walk].Load("Assets/animData/character/Slime/Walk.tka");
	m_animationClip[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClip[enAnimClip_Walk].Load("Assets/animData/character/Slime/Run.tka");
	m_animationClip[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClip[enAnimClip_Attack_1].Load("Assets/animData/character/Slime/Attack1.tka");
	m_animationClip[enAnimClip_Attack_1].SetLoopFlag(false);
	m_animationClip[enAnimClip_Damage].Load("Assets/animData/character/Slime/Damege.tka");
	m_animationClip[enAnimClip_Damage].SetLoopFlag(false);
	m_animationClip[enAnimClip_Die].Load("Assets/animData/character/Slime/Die.tka");
	m_animationClip[enAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enAnimClip_Victory].Load("Assets/animData/character/Slime/Victory.tka");
	m_animationClip[enAnimClip_Victory].SetLoopFlag(true);
	m_animationClip[enAnimClip_Appear].Load("Assets/animData/character/Slime/Appear.tka");
	m_animationClip[enAnimClip_Appear].SetLoopFlag(false);

	m_modelRender.Init(
		"Assets/modelData/character/Slime/slime.tkm",
		L"Assets/shader/ToonTextrue/lamp_Slime.DDS",
		m_animationClip, 
		enAnimClip_Num,
		enModelUpAxisZ
	);
	//���f���̐ÓI�I�u�W�F�N�g�쐬
	//m_monsterStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	////�R���W����������t����
	//m_monsterStaticObject.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Monster);

	m_charaCon.Init(
		16.0f,
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

	//�U���p�{�[���̎擾
	m_attackBoonId = m_modelRender.FindBoneID(L"Head");

}

void Slime::Update()
{
	if (m_lich != nullptr)
	{
		//����������
		if (m_lich->GetWinFlag() == true)
		{
			SetWinFlag(true);
			//�U�����łȂ����
			SetNextAnimationState(enAnimationState_Victory);
		}
		//�^�C���A�b�v�t���O��true�Ȃ�
		if (m_lich->GetTimeUpEndFlag() == true)
		{
			//�G�̏���
			SetWinFlag(true);
			SetNextAnimationState(enAninationState_Idle);
		}
	}
	
	if (IsStopProcessing()==true)
	{
		//�����艺�̏��������Ȃ�
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

void Slime::Attack()
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
			//�U��
			SetNextAnimationState(enAnimationState_Attack_1);
			//�U�������̂Ńt���O��true�ɂ��ăC���^�[�o���ɓ���
			m_attackFlag = true;
		}
	}


}

bool Slime::IsStopProcessing()
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

void Slime::CreateCollision()
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

void Slime::Damage(int attack)
{
	//�U������������Ȃ��̂œ����蔻��𐶐����Ȃ��悤�ɂ���
	m_createAttackCollisionFlag = false;

	//HP�����炷
	m_status.hp -= attack;

	//HP��0�ȉ��Ȃ�
	if (m_status.hp <= 0)
	{
		//����A�j���[�V�����X�e�[�g
		m_status.hp = 0;
		SetNextAnimationState(enAnimationState_Die);
		Dead();
		return;
	}
	//��_���[�W�A�j���[�V�����X�e�[�g
	SetNextAnimationState(enAnimationState_Damage);
	
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
	case Slime::enAninationState_Idle:
		m_state = new SlimeStateIdle(this);
		break;
	case Slime::enAninationState_Walk:
		m_state = new SlimeStateWalk(this);
		break;
	case Slime::enAninationState_Run:
		m_state = new SlimeStateWalk(this);
		break;
	case Slime::enAnimationState_Attack_1:
		m_state = new SlimeStateAttack(this);
		break;
	case Slime::enAnimationState_Damage:
		m_state = new SlimeStateDamage(this);
		break;
	case Slime::enAnimationState_Die:
		m_state = new SlimeStateDie(this);
		break;
	case Slime::enAnimationState_Victory:
		m_state = new SlimeStateVictory(this);
		break;
	case Slime::enAnimationState_Appear:
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
		SetNextAnimationState(enAninationState_Walk);
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