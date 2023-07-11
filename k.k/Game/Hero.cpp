#include "stdafx.h"
#include "Hero.h"
#include "HeroStateIdle.h"
#include "HeroStateWalk.h"
#include "HeroStateRun.h"
#include "HeroStateAttack_1.h"
#include "HeroStateAttack_2.h"
#include "HeroStateAttack_3.h"
#include "HeroStateAttack_4.h"
#include "HeroStateAttack_Skill_Main.h"


namespace {
	int MAXHP = 200;
	int ATK = 50;
	float SPEED = 100.0f;
	const char* NAME = "Hero";
}

Hero::Hero()
{
}

Hero::~Hero()
{
}

bool Hero::Start()
{
	// �����̃A�j���[�V�����X�e�[�g��ҋ@��Ԃɂ���B
	SetNextAnimationState(enAninationState_Idle);

	//�X�e�[�^�X�̏�����
	m_status.InitStatus(
	MAXHP,
	ATK,
	SPEED,
	NAME
	);

	InitModel();

	

	return true;
}

void Hero::InitModel()
{
	//�A�j���[�V�����N���b�v�����[�h����B
	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/Player/Idle_Normal.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Walk].Load("Assets/animData/character/Player/MoveFWD_Battle.tka");
	m_animationClip[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClip[enAnimClip_Run].Load("Assets/animData/character/Player/SprintFWD_Battle.tka");
	m_animationClip[enAnimClip_Run].SetLoopFlag(true);
	m_animationClip[enAnimClip_Attack_1].Load("Assets/animData/character/Player/Attack1.tka");
	m_animationClip[enAnimClip_Attack_1].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_2].Load("Assets/animData/character/Player/Attack2.tka");
	m_animationClip[enAnimClip_Attack_2].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_3].Load("Assets/animData/character/Player/Attack3.tka");
	m_animationClip[enAnimClip_Attack_3].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_Skill_Charge].Load("Assets/animData/character/Player/Attack_Skill_Charge.tka");
	m_animationClip[enAnimClip_Attack_Skill_Charge].SetLoopFlag(true);
	m_animationClip[enAnimClip_Attack_Skill_Main].Load("Assets/animData/character/Player/Attack_Skill_Main.tka");
	m_animationClip[enAnimClip_Attack_Skill_Main].SetLoopFlag(true);


	m_modelRender.Init(
		"Assets/modelData/character/Player/Hero.tkm",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);

	//m_modelRender.SetPosition(50.0f, 0.0f, 50.0f);
	m_scale = { 1.2f,1.2f,1.2f };
	m_modelRender.SetScale(m_scale);
	m_modelRender.Update();

	//�{�[��ID�̎擾
	m_swordBoonId = m_modelRender.FindBoneID(L"hand_r");
	m_skillBoonId = m_modelRender.FindBoneID(L"spine_03");

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

}

/// <summary>
/// ���t���[��SetNextAnimationState()�����s
/// </summary>

void Hero::Update()
{
	
	Move();
	Attack();
	ManageState();
	PlayAnimation();

	if (m_createAttackCollisionFlag == true)
	{
		CreateCollision();
	}

	if (m_createSkillCollisionFlag == true)
	{
		CreateSkillCollision();
	}

	SetTransFormModel(m_modelRender);
	m_modelRender.Update();
}

void Hero::Move()
{
	//����̃A�j���[�V�������Đ����̂Ƃ�
	//�����ς���
	if (isAnimationSwappable() != true)
	{
		if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
		{
			if (g_pad[0]->IsPress(enButtonA))
			{
				m_dashFlag = true;
			}
			else
			{
				m_dashFlag = false;
			}
		}
	}

	
	
	m_moveSpeed = m_player->GetMoveSpeed();
	m_position = m_player->GetPosition();
	
	Rotation();
}

void Hero::Attack()
{
	

	///////////////////////////////////////////////////////////////////////////////////////////
	// �ʏ�U��
	///////////////////////////////////////////////////////////////////////////////////////////
	//�A�^�b�N�p�^�[�����Ȃ��̎�
	//1�R���{
	if (g_pad[0]->IsTrigger(enButtonY)&& m_enAttackPatternState==enAttackPattern_None)
	{
		m_createAttackCollisionFlag = true;
		//�U���p�^�[���P
		m_enAttackPatternState = enAttackPattern_1;

		SetNextAnimationState(enAnimationState_Attack_1);
		return;
	}
	//2�R���{�ڎ�t
	if (m_enAttackPatternState == enAttackPattern_1)
	{
		if (g_pad[0]->IsTrigger(enButtonY))
		{
			m_enAttackPatternState = enAttackPattern_1to2;
			return;
		}
	}
	//3�R���{�ڎ�t
	if (m_enAttackPatternState == enAttackPattern_2)
	{
		if (g_pad[0]->IsTrigger(enButtonY))
		{
			m_enAttackPatternState = enAttackPattern_2to3;
			return;
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	// �X�L��
	/////////////////////////////////////////////////////////////////////////////////////////

	//�܂��̓`���[�W
	if (m_enAttackPatternState == enAttackPattern_None ||
		m_enAttackPatternState == enAnimationState_Attack_Skill_Charge)
	{
		if (g_pad[0]->IsPress(enButtonX))
		{
			m_enAttackPatternState = enAttackPattern_Skill_Charge;
			SetNextAnimationState(enAnimationState_Attack_Skill_Charge);
		}
		return;
	}
	
 
}

void Hero::PlayAnimation()
{
	m_state->PlayAnimation();
}

void Hero::CreateCollision()
{
	auto AtkCollision = NewGO<CollisionObject>(0,"Attack");
	
	//�{�b�N�X�̓����蔻��쐬
	AtkCollision->CreateBox(
		m_position,
		Quaternion(90.0f,0.0f,0.0f,1.0f),
		Vector3(10.0f, 5.0f, 100.0f)
	);

	//���̃{�[���̃��[���h���W���擾
	Matrix SwordBoonMatrix = m_modelRender.GetBone(m_swordBoonId)->GetWorldMatrix();
	
	AtkCollision->SetWorldMatrix(SwordBoonMatrix);
}

void Hero::CreateSkillCollision()
{
	auto SkillCollision = NewGO<CollisionObject>(0, "SkillAttack");

	//�J�v�Z���̓����蔻��쐬
	SkillCollision->CreateSphere(
		m_position,
		Quaternion::Identity,
		70.0f
	);

	//�J�v�Z���̓����蔻��쐬
	/*SkillCollision->CreateBox(
		m_position,
		Quaternion::Identity,
		Vector3(60.0f,40.0f,30.0f)
	);*/

	//���̃{�[���̃��[���h���W���擾
	Matrix SkillBoonMatrix = m_modelRender.GetBone(m_skillBoonId)->GetWorldMatrix();

	SkillCollision->SetWorldMatrix(SkillBoonMatrix);
}

void Hero::SetNextAnimationState(EnAnimationState nextState)
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
	case Hero::enAninationState_Idle:
		// �ҋ@�X�e�[�g���쐬����B
		m_state = new HeroStateIdle(this);
		break;
	case Hero::enAninationState_Walk:
		// �����X�e�[�g���쐬����B
		m_state = new HeroStateWalk(this);
		break;
	case Hero::enAninationState_Run:
		// ����X�e�[�g���쐬����B
		m_state = new HeroStateRun(this);
		break;
	case Hero::enAnimationState_Attack_1:
		//�A�^�b�N�P�X�e�[�g���쐬����B
		m_state = new HeroStateAttack_1(this);
		break;
	case Hero::enAnimationState_Attack_2:
		//�A�^�b�N�Q�X�e�[�g���쐬����B
		m_state = new HeroStateAttack_2(this);
		break;
	case Hero::enAnimationState_Attack_3:
		//�A�^�b�N�R�X�e�[�g���쐬����B
		m_state = new HeroStateAttack_3(this);
		break;
	case Hero::enAnimationState_Attack_Skill_Charge:
		//�A�^�b�N�X�L���`���[�W�X�e�[�g���쐬����B
		m_state = new HeroStateAttack_4(this);
		break;
	case Hero::enAnimationState_Attack_Skill_Main:
		//�A�^�b�N�X�L�����C���X�e�[�g���쐬����B
		m_state = new HeroStateAttack_Skill_Main(this);
		break;

	default:
		// �����ɗ�����X�e�[�g�̃C���X�^���X�쐬�����̒ǉ��Y��B
		std::abort();
		break;
	}

	
	
}

//��ԑJ�ڊǗ�
void Hero::ManageState()
{
	m_state->ManageState();
}

//���ʂ̏�ԑJ�ڏ���
void Hero::ProcessCommonStateTransition()
{
	

	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		if (m_dashFlag == true)
		{
			SetNextAnimationState(enAninationState_Run);
			return;
		}

		SetNextAnimationState(enAninationState_Walk);
	}
	else
	{
		m_dashFlag = false;
		SetNextAnimationState(enAninationState_Idle);
	}

	
	
}
//�A�^�b�N�P�̏�ԑJ�ڏ���
void Hero::OnProcessAttack_1StateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�U���p�^�[�����Ȃ���Ԃɂ���
		m_enAttackPatternState = enAttackPattern_None;
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}

}

void Hero::OnProcessAttack_2StateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�U���p�^�[�����Ȃ���Ԃɂ���
		m_enAttackPatternState = enAttackPattern_None;
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Hero::OnProcessAttack_3StateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�U���p�^�[�����Ȃ���Ԃɂ���
		m_enAttackPatternState = enAttackPattern_None;
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Hero::OnProcessAttack_Skill_ChargeStateTransition()
{
	//�`���[�W���Ԃ��v�Z
	if (m_MaxChargeTime > m_ChargeTimer)
	{
		m_ChargeTimer += g_gameTime->GetFrameDeltaTime();
		//
		if (m_ChargeTimer > m_MaxChargeTime)
		{
			m_ChargeTimer = m_MaxChargeTime;
		}
	}
	
	//�{�^���𗣂�����X�L�����C���Ɉڂ�
	if (g_pad[0]->IsPress(enButtonX) == false)
	{
		//�Œ�ł�1�b�̓X�L������������悤�ɂ���
		if (m_ChargeTimer < 1.0f) {
			m_ChargeTimer = 1.0f;
		}

		//�X�L�������_�b�V���������Ƃɂ���
		m_dashFlag = true;
		//�����蔻��쐬
		m_createSkillCollisionFlag = true;

		m_enAttackPatternState = enAttackPattern_Skill_Main;
		SetNextAnimationState(enAnimationState_Attack_Skill_Main);
	}

}

void Hero::OnProcessAttack_Skill_MainStateTransition()
{
	//�`���[�W�������Ԃ��؂ꂽ��
	if (m_ChargeTimer < 0.0f)
	{
		//�_�b�V���t���O��false�ɂ���
		m_dashFlag = false;

		m_createSkillCollisionFlag = false;

		//�`���[�W���Ԃ����Z�b�g
		m_ChargeTimer = 0.0f;

		//�U���p�^�[�����Ȃ���Ԃɂ���
		m_enAttackPatternState = enAttackPattern_None;
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
	else
	{
		m_ChargeTimer -= g_gameTime->GetFrameDeltaTime();
	}
}

void Hero::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�A�^�b�N1�̃R���{��t�^�C�����I�������
	if (wcscmp(eventName, L"Attack1_ComboEnd") == 0)
	{
		m_createAttackCollisionFlag = false;

		//2�R���{
		if (m_enAttackPatternState == enAttackPattern_1to2)
		{
			m_enAttackPatternState = enAttackPattern_2;
			SetNextAnimationState(enAnimationState_Attack_2);
		}
	}

	//�A�^�b�N�Q�̃R���{��t�^�C�����n�܂�����
	if (wcscmp(eventName, L"Attack2_ComboStart") == 0)
	{
		m_createAttackCollisionFlag = true;
		
	}
	//�A�^�b�N�Q�̂��I�������
	if (wcscmp(eventName, L"Attack2_CollisionDelete") == 0)
	{
		m_createAttackCollisionFlag = false;
	}
	//�A�^�b�N�Q�̃R���{��t�^�C�����I�������
	if (wcscmp(eventName, L"Attack2_ComboEnd") == 0)
	{
		//3�R���{
		if (m_enAttackPatternState == enAttackPattern_2to3)
		{
			m_enAttackPatternState = enAttackPattern_3;
			SetNextAnimationState(enAnimationState_Attack_3);
		}
	}

	//�A�^�b�N�R�̃R���{��t�^�C�����n�܂�����
	if (wcscmp(eventName, L"Attack3_ComboStart") == 0)
	{
		m_createAttackCollisionFlag = true;

	}
	//�A�^�b�N�R�̃R���{��t�^�C�����I�������
	if (wcscmp(eventName, L"Attack3_ComboEnd") == 0)
	{
		m_createAttackCollisionFlag = false;
	}


}

void Hero::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}




