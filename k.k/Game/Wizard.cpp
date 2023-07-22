#include "stdafx.h"
#include "Wizard.h"
#include "Player.h"
#include "WizardStateIdle.h"
#include "WizardStateWalk.h"
#include "WizardStateRun.h"
#include "WizardStateAttack_1.h"
//�X�L������ւ��Ďg����
//�A���U���t�@�C���[�{�[��
#include "WizardStateAttack_2_Start.h"
#include "WizardStateAttack_2_main.h"
#include "FireBall.h"
//�t���C���[�s���[
#include "WizardStateAttack_3_Start.h"
#include "WizardStateAttack_3_main.h"
#include "WizardState_Attack_4.h"
#include "FlamePillar.h"
#include "WizardStateDie.h"

namespace {
	int MAXHP = 150;
	int MAXMP = 200;
	int ATK = 30;
	float SPEED = 80.0f;
	const char* NAME = "Wizard";

	
}

Wizard::Wizard()
{
}

Wizard::~Wizard()
{
}

bool Wizard::Start()
{
	m_player = FindGO<Player>("player");

	// �����̃A�j���[�V�����X�e�[�g��ҋ@��Ԃɂ���B
	SetNextAnimationState(enAninationState_Idle);

	//�X�e�[�^�X�̏�����
	m_status.InitStatus(
		MAXHP,
		MAXMP,
		ATK,
		SPEED,
		NAME
	);

	InitModel();

	//��A�N�e�B�u��
	Deactivate();


	m_fireBallTimer = m_createFireBallTime;

	m_wandRotation.AddRotationDegX(50.0f);


	return true;
}

void Wizard::InitModel()
{
	//Attack3_maintain
	//�A�j���[�V�����N���b�v�����[�h����B
	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/Wizard/Idle.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Walk].Load("Assets/animData/character/Wizard/Walk_Normal.tka");
	m_animationClip[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClip[enAnimClip_Run].Load("Assets/animData/character/Wizard/Run.tka");
	m_animationClip[enAnimClip_Run].SetLoopFlag(true);
	m_animationClip[enAnimClip_Attack_1].Load("Assets/animData/character/Wizard/Attack1.tka");
	m_animationClip[enAnimClip_Attack_1].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_2_start].Load("Assets/animData/character/Wizard/Attack2_start.tka");
	m_animationClip[enAnimClip_Attack_2_start].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_2_main].Load("Assets/animData/character/Wizard/Attack2_maintain.tka");
	m_animationClip[enAnimClip_Attack_2_main].SetLoopFlag(true);
	m_animationClip[enAnimClip_Attack_3_start].Load("Assets/animData/character/Wizard/Attack3_start.tka");
	m_animationClip[enAnimClip_Attack_3_start].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_3_main].Load("Assets/animData/character/Wizard/Attack3_maintain.tka");
	m_animationClip[enAnimClip_Attack_3_main].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_4].Load("Assets/animData/character/Wizard/Attack4.tka");
	m_animationClip[enAnimClip_Attack_4].SetLoopFlag(false);
	m_animationClip[enAnimClip_Die].Load("Assets/animData/character/Wizard/Die.tka");
	m_animationClip[enAnimClip_Die].SetLoopFlag(false);


	m_modelRender.Init(
		"Assets/modelData/character/Wizard/Wizard.tkm",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);
	//m_position = { 70.0f, 0.0f, 0.0f };
	//m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	//�{�[��ID�̎擾
	m_magicWandBoonId = m_modelRender.FindBoneID(L"Weapon");


	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//m_charaCon.Init(12.0f, 33.0f, m_position);
}

void Wizard::Update()
{
	//���ꂽ�Ȃ瑼�̏��������s���Ȃ�
	if (GetDieFlag() == true)
	{
		ManageState();
		PlayAnimation();
		m_modelRender.Update();
		return;
	}


	RecoveryMP();

	Move();
	Attack();
	ManageState();
	PlayAnimation();

	if (m_createAttackCollisionFlag == true)
	{
		CreateCollision();
	}
	
	Damage(10);

	SetTransFormModel(m_modelRender);
	m_modelRender.Update();
}

void Wizard::Move()
{
	if (g_pad[0]->IsPress(enButtonA))
	{
		m_dashFlag = true;
	}
	else
	{
		m_dashFlag = false;
	}
	
	m_moveSpeed = m_player->GetMoveSpeed();
	m_position = m_player->GetPosition();
	
	Rotation();
}

bool Wizard::RotationOnly()
{
	//�t�@�C���[�{�[����ł��Ă���Ƃ�
	if (m_enAttackPatternState == enAttackPattern_2_main)
	{
		//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
		if (fabsf(m_SaveMoveSpeed.x) >= 0.001f || fabsf(m_SaveMoveSpeed.z) >= 0.001f)
		{
			m_rotation.SetRotationYFromDirectionXZ(m_SaveMoveSpeed);
		}
		return true;
	}

	return false;
}

void Wizard::Attack()
{
	//�X�L���̐؂�ւ�
	if (g_pad[0]->IsTrigger(enButtonRB3))
	{
		switch (m_enSkillPatternState)
		{
			//�t���C���s���[����t�@�C���[�{�[���ɐ؂�ւ�
		case Wizard::enSkillPattern_FlamePillar:
			m_enSkillPatternState = enSkillPattern_FireBall;
			break;
			//�t�@�C���[�{�[������t���C���s���[�ɐ؂�ւ�
		case Wizard::enSkillPattern_FireBall:
			m_enSkillPatternState = enSkillPattern_FlamePillar;
			break;
		default:
			break;
		}
	}


	////////////////////////////////////////////////////////////////////////////////////////
	// �ʏ�U��
	////////////////////////////////////////////////////////////////////////////////////////
	//1�R���{
	if (g_pad[0]->IsTrigger(enButtonY)&& m_enAttackPatternState==enAttackPattern_None)
	{
		m_enAttackPatternState = enAttackPattern_1;
		SetNowComboState(enNowCombo_1);
		SetNextAnimationState(enAnimationState_Attack_1);
		return;
	}
	//2�R���{��t�^�C��
	if (g_pad[0]->IsTrigger(enButtonY) && m_enAttackPatternState == enAttackPattern_1)
	{
		m_enAttackPatternState = enAttackPattern_1to4;
		return;
	}



	////////////////////////////////////////////////////////////////////////////////////////
	// �X�L��
	////////////////////////////////////////////////////////////////////////////////////////

	

	
	
	if (g_pad[0]->IsTrigger(enButtonX) && m_enAttackPatternState == enAttackPattern_None)
	{
		//�t���C���s���[
		if (m_enSkillPatternState == enSkillPattern_FlamePillar)
		{
			//�X�L����MP������Ȃ��Ȃ甲���o��
			if (m_status.mp < m_flamePillar_skillMp) {
				return;
			}
			m_enAttackPatternState = enAttackPattern_3_start;
			SetNextAnimationState(enAnimationState_Attack_3_start);
			//MP�񕜏�Ԃ��~�߂�
			SetRecoveryMpFlag(false);
			return;
		}
		//�t�@�C���[�{�[��
		else
		{
			//�X�L����MP������Ȃ��Ȃ甲���o��
			if (m_status.mp < m_fireBall_SkillMp) {
				return;
			}
			m_enAttackPatternState = enAttackPattern_2_start;
			SetNextAnimationState(enAnimationState_Attack_2_start);
			//MP�񕜏�Ԃ��~�߂�
			SetRecoveryMpFlag(false);
			return;
		}
	}
	

}

void Wizard::CreateCollision()
{
	auto AtkCollision = NewGO<CollisionObject>(0, "Attack");

	//�{�b�N�X�̓����蔻��쐬
	AtkCollision->CreateBox(
		m_position,
		m_wandRotation,
		Vector3(10.0f, 100.0f, 10.0f)
	);

	//��̃{�[���̃��[���h���W���擾
	Matrix WandBoonMatrix = m_modelRender.GetBone(m_magicWandBoonId)->GetWorldMatrix();
	/*Matrix WandRotMatrix;
	WandRotMatrix.MakeRotationFromQuaternion(m_wandRotation);

	Matrix finalWandBoonMatrix = WandBoonMatrix * WandRotMatrix;*/

	AtkCollision->SetWorldMatrix(WandBoonMatrix);
}

void Wizard::Damage(int attack)
{
	if (m_status.hp > 0)
	{
		m_status.hp -= attack;
	}

	if (m_status.hp <= 0)
	{
		//Die�t���O��true�ɂ���
		SetDieFlag(true);
		m_status.hp = 0;
		//�t���[�����[�g�𗎂Ƃ�
		g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Variable, 30);

		SetNextAnimationState(enAnimationState_Die);
	}
}

void Wizard::CreateFlamePillar()
{
	//�t���C���s���[����
	FlamePillar* flamePillar = NewGO<FlamePillar>(0, "flamepillar");
	flamePillar->SetWizard(this);
	//MP�����炷
	m_status.mp -= m_flamePillar_skillMp;
}

bool Wizard::CreateFireBall()
{
	//�X�L����MP������Ȃ��Ȃ甲���o��
	if (m_status.mp < m_fireBall_SkillMp) {
		return false;
	}


	if (m_createFireBallTime < m_fireBallTimer)
	{
		//�t�@�C���[�{�[������
		FireBall* fireBall = NewGO<FireBall>(0, "fireball");
		fireBall->SetWizard(this);
		//MP�����炷
		m_status.mp -= m_fireBall_SkillMp;

		m_fireBallTimer = 0.0f;
	}
	else
	{
		m_fireBallTimer += g_gameTime->GetFrameDeltaTime();
	}

	return true;
}

void Wizard::PlayAnimation()
{
	m_animationState->PlayAnimation();
}

void Wizard::ManageState()
{
	m_animationState->ManageState();
}

bool Wizard::IsComboStateSame()
{
	return false;
}

void Wizard::SetNextAnimationState(EnAnimationState nextState)
{
	if (m_animationState != nullptr) {
		// �Â��X�e�[�g���폜����B
		delete m_animationState;
		m_animationState = nullptr;
	}

	m_enAnimationState = nextState;

	switch (m_enAnimationState)
	{
	case Wizard::enAninationState_Idle:
		//�ҋ@�X�e�[�g���쐬����B
		m_animationState = new WizardStateIdle(this);
		break;
	case Wizard::enAninationState_Walk:
		//�����X�e�[�g���쐬����B
		m_animationState = new WizardStateWalk(this);
		break;
	case Wizard::enAninationState_Run:
		//����X�e�[�g���쐬����B
		m_animationState = new WizardStateRun(this);
		break;
	case Wizard::enAnimationState_Attack_1:
		//�A�^�b�N�P�X�e�[�g���쐬����B
		m_animationState = new WizardStateAttack_1(this);
		break;
	case Wizard::enAnimationState_Attack_2_start:
		//�A�^�b�N�Q�X�^�[�g�X�e�[�g���쐬����B
		m_animationState = new WizardStateAttack_2_Start(this);
		break;
	case Wizard::enAnimationState_Attack_2_main:
		//�A�^�b�N�Q���C���X�e�[�g���쐬����B
		m_animationState = new WizardStateAttack_2_main(this);
		break;
	case Wizard::enAnimationState_Attack_3_start:
		//�A�^�b�N�R�X�^�[�g�X�e�[�g���쐬����B
		m_animationState = new WizardStateAttack_3_Start(this);
		break;
	case Wizard::enAnimationState_Attack_3_main:
		//�A�^�b�N�R���C���X�e�[�g���쐬����B
		m_animationState = new WizardStateAttack_3_Main(this);
		break;
	case Wizard::enAnimationState_Attack_4:
		//�A�^�b�N�S�X�e�[�g���쐬����B
		m_animationState = new WizardState_Attack_4(this);
		break;
	case Wizard::enAnimationState_Die:
		//�A�^�b�N�S�X�e�[�g���쐬����B
		m_animationState = new WizardStateDie(this);
		break;

	default:
		// �����ɗ�����X�e�[�g�̃C���X�^���X�쐬�����̒ǉ��Y��B
		std::abort();
		break;
	}
	
	
}

void Wizard::ProcessCommonStateTransition()
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

void Wizard::OnProcessAttack_1StateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�R���{���I�������
		SetNowComboState(enNowCombo_None);
		SetDamagedComboState(enDamageCombo_None);
		//�U���p�^�[�����Ȃ���Ԃɂ���
		m_enAttackPatternState = enAttackPattern_None;
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}
void Wizard::OnProcessAttack_2StateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_enAttackPatternState = enAttackPattern_2_main;



		//���C���A�j���[�V�������Đ�
		SetNextAnimationState(enAnimationState_Attack_2_main);
		//�t�@�C���[�{�[���^�C�}�[���Z�b�g
		m_fireBallTimer = m_createFireBallTime;
	}
}

void Wizard::OnProcessAttack_2MainStateTransition()
{
	//X�{�^���������Ă����
	if (g_pad[0]->IsPress(enButtonX))
	{
		//MP������Ȃ��Ȃ����狭���I�ɃX�L�����I���
		if (!CreateFireBall())
		{
			//�U���p�^�[�����Ȃ���Ԃɂ���
			m_enAttackPatternState = enAttackPattern_None;
			//���ʂ̏�ԑJ�ڏ����Ɉڍs
			ProcessCommonStateTransition();
			//�X�L����ł��I������̂�MP�񕜃t���O��true�ɂ���
			SetRecoveryMpFlag(true);
		}
	}
	else
	{
		//�U���p�^�[�����Ȃ���Ԃɂ���
		m_enAttackPatternState = enAttackPattern_None;
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
		//�X�L����ł��I������̂�MP�񕜃t���O��true�ɂ���
		SetRecoveryMpFlag(true);
	}
}

void Wizard::OnProcessAttack_3StateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�X�^�[�g��������
		if (m_enAttackPatternState == enAttackPattern_3_start)
		{
			m_enAttackPatternState = enAttackPattern_3_main;
			//���C���A�j���[�V�������Đ�
			SetNextAnimationState(enAnimationState_Attack_3_main);

			//�t���C���s���[����
			CreateFlamePillar();
			
			return;
		}

		//�U���p�^�[�����Ȃ���Ԃɂ���
		m_enAttackPatternState = enAttackPattern_None;
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
		//�X�L����ł��I������̂�MP�񕜃t���O��true�ɂ���
		SetRecoveryMpFlag(true);
	}
}
void Wizard::OnProcessAttack_4StateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�R���{���I�������
		SetNowComboState(enNowCombo_None);
		SetDamagedComboState(enDamageCombo_None);
		//�U���p�^�[�����Ȃ���Ԃɂ���
		m_enAttackPatternState = enAttackPattern_None;
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Wizard::OnProcessDieStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�t���[�����[�g��߂�
		g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Fix, 60);
		//�A�j���[�V�������I������̂ŃL�����N�^�[��؂�ւ���t���O��true�ɂ���
		SetDieToChangeFlag(true);
	}
}

void Wizard::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (wcscmp(eventName, L"Attack1_Collision_Start") == 0)
	{
		m_createAttackCollisionFlag = true;
	}
	if (wcscmp(eventName, L"Attack1_Collision_End") == 0)
	{
		m_createAttackCollisionFlag = false;
	}
	//�A�^�b�N�P�̃R���{��t�^�C�����I�������
	if (wcscmp(eventName, L"Attack1_ComboEnd") == 0)
	{
		if (m_enAttackPatternState == enAttackPattern_1to4)
		{
			m_enAttackPatternState = enAttackPattern_4;
			SetNowComboState(enNowCombo_2);
			SetNextAnimationState(enAnimationState_Attack_4);
		}
	}

	//�������̓_���[�W���󂯂Ȃ�
	if (wcscmp(eventName, L"Damageless") == 0)
	{

	}
	if (wcscmp(eventName, L"Attack4_CollisionStart") == 0)
	{
		m_createAttackCollisionFlag = true;
	}
	if (wcscmp(eventName, L"Attack4_CollisionEnd") == 0)
	{
		m_createAttackCollisionFlag = false;
	}
}

void Wizard::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}


