#include "stdafx.h"
#include "Hero.h"
#include "Player.h"
#include "HeroStateIdle.h"
#include "HeroStateWalk.h"
#include "HeroStateDash.h"
#include "HeroStateAttack_1.h"
#include "HeroStateAttack_2.h"
#include "HeroStateAttack_3.h"
#include "HeroStateAttack_4.h"
#include "HeroStateAttack_Skill_Main.h"
#include "HeroStateDie.h"
#include "HeroStateDamage.h"
#include "HeroStatePowerUp.h"
#include "HeroStateVictory.h"

#include "InitEffect.h"

//todo �_�b�V�����U���A�j���[�V�����ύX

namespace {
	const float ROT_SPEED = 2.0f;
	const float ROT_ONLY_SPEED = 2.2f;

	const float SWORD_EFFECT_SIZE = 10.0f;
	const Vector3 DASH_EFFECT_SIZE = { 10.0f,20.0f,8.0f };

	int MAXHP = 200;
	int MAXMP = 100;
	int ATK = 50;
	float SPEED = 200.0f;
	const char* NAME = "Hero";

	const int SKILL_ATTACK_POWER = 30;
}

Hero::Hero()
{
	m_skillAttackPower = SKILL_ATTACK_POWER;
}

Hero::~Hero()
{
}

bool Hero::Start()
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




	return true;
}

void Hero::InitModel()
{
	//�A�j���[�V�����N���b�v�����[�h����B
	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/Player/Idle_Normal.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Walk].Load("Assets/animData/character/Player/MoveFWD_Battle.tka");
	m_animationClip[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClip[enAnimClip_Dash].Load("Assets/animData/character/Player/Dash.tka");
	m_animationClip[enAnimClip_Dash].SetLoopFlag(true);
	m_animationClip[enAnimClip_Attack_1].Load("Assets/animData/character/Player/Attack1_1.tka");
	m_animationClip[enAnimClip_Attack_1].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_2].Load("Assets/animData/character/Player/Attack2.tka");
	m_animationClip[enAnimClip_Attack_2].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_3].Load("Assets/animData/character/Player/Attack3.tka");
	m_animationClip[enAnimClip_Attack_3].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_Skill_Charge].Load("Assets/animData/character/Player/Attack_Skill_Charge.tka");
	m_animationClip[enAnimClip_Attack_Skill_Charge].SetLoopFlag(true);
	m_animationClip[enAnimClip_Attack_Skill_Main].Load("Assets/animData/character/Player/Attack_Skill_Main.tka");
	m_animationClip[enAnimClip_Attack_Skill_Main].SetLoopFlag(true);
	m_animationClip[enAnimClip_Die].Load("Assets/animData/character/Player/Dead.tka");
	m_animationClip[enAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enAnimClip_Damage].Load("Assets/animData/character/Player/Damage.tka");
	m_animationClip[enAnimClip_Damage].SetLoopFlag(false);
	m_animationClip[enAnimClip_PowerUp].Load("Assets/animData/character/Player/PowerUp.tka");
	m_animationClip[enAnimClip_PowerUp].SetLoopFlag(false);
	m_animationClip[enAnimClip_Victory].Load("Assets/animData/character/Player/Victory.tka");
	m_animationClip[enAnimClip_Victory].SetLoopFlag(true);



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

void Hero::Update()
{
	//�������Ƃ��ɏ������Ȃ�
	//���ꂽ�Ȃ瑼�̏��������s���Ȃ�
	if (GetDieFlag() == true|| m_player->GetGameEndFlag() == true)
	{
		m_invincibleTimeFlag = false;
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

	PowerUpTimer();

	if (m_createAttackCollisionFlag == true)
	{
		CreateCollision();
	}

	if (m_createSkillCollisionFlag == true)
	{
		CreateSkillCollision();
	}

	CalcInvincibleTime();

	//���G���ԂłȂ��Ȃ瓖���蔻��̏������s��
	if (CalcInvicibleDash() == false)
	{
		DamageCollision(m_player->GetCharacterController());
	}
	
	SetTransFormModel(m_modelRender);
	m_modelRender.Update();
}

void Hero::Move()
{
	//����̃A�j���[�V�������Đ����̂Ƃ�
	//����ւ��\�A�j���[�V�����̂Ƃ�����
	if (isAnimationSwappable() != true)
	{
		if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
		{
			//���W�b�g�{�f�B�Ń_�b�V��
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				//�_�b�V���p�̑O�������擾
				m_dashForward = m_forward;
				//�_�b�V�������u�Ԃ������G��Ԃɂ���
				SetInvicibleDashState(enDashInvicibleState_On);
				m_invincbledDashTimer = 0.0f;
				//�_�b�V���̓r���ōU��������_���[�W�󂯂���������Ȃ��̂Ń^�C�}�[���Z�b�g
				m_dashTimer = 1.0f;
				SetNextAnimationState(enAninationState_Dash);
				//�_�b�V���G�t�F�N�g�Đ�
				//�_�b�V���G�t�F�N�g�̓ǂݍ���
				m_dashEffect = NewGO<EffectEmitter>(0);
				m_dashEffect->Init(InitEffect::enEffect_Dash);
				m_dashEffect->Play();
				m_dashEffect->SetScale(DASH_EFFECT_SIZE);
				m_dashEffect->SetPosition(m_position);
				m_dashEffect->SetRotation(m_rotation);
				m_dashEffect->Update();
				m_dashEffectFlag = true;
			}	
		}
	}

	m_moveSpeed = m_player->GetMoveSpeed();
	m_position = m_player->GetPosition();
	
	Rotation(ROT_SPEED, ROT_ONLY_SPEED);

	if (m_dashEffectFlag ==true)
	{
		m_dashEffect->SetPosition(m_position);
		m_dashEffect->SetRotation(m_rotation);
		m_dashEffect->Update();
	}
}

bool Hero::RotationOnly()
{
	//�X�L���̃`���[�W���Ԃ̊�
	if (isRotationEntable()!=true)
	{
		return true;
	}

	return false;
}

void Hero::Attack()
{
	//��_���[�W���A�f�X���ɍU�����Ȃ�
	if (isCollisionEntable() != true)
	{
		return;
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	// �ʏ�U��
	///////////////////////////////////////////////////////////////////////////////////////////
	//1�R���{
	if (g_pad[0]->IsTrigger(enButtonB)&& m_enAttackPatternState==enAttackPattern_None)
	{
		
		
		m_enAttackPatternState = enAttackPattern_1;
		SetNowComboState(enNowCombo_1);
		SetNextAnimationState(enAnimationState_Attack_1);
		return;
	}
	//2�R���{�ڎ�t
	if (m_enAttackPatternState == enAttackPattern_1)
	{
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			m_enAttackPatternState = enAttackPattern_1to2;
			return;
		}
	}
	//3�R���{�ڎ�t
	if (m_enAttackPatternState == enAttackPattern_2)
	{
		if (g_pad[0]->IsTrigger(enButtonB))
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
		//MP���X�L����MP��葽��������
		if (g_pad[0]->IsPress(enButtonX)&&m_status.mp>=m_skillMp)
		{
			
			m_enAttackPatternState = enAttackPattern_Skill_Charge;
			SetNextAnimationState(enAnimationState_Attack_Skill_Charge);
			//MP�񕜏�Ԃ��~�߂�
			SetRecoveryMpFlag(false);
			return;
		}
		
	}

	//�p���[�A�b�v
	if (m_enAttackPatternState == enAttackPattern_None)
	{
		//MP���p���[�A�b�v��MP��葽��������
		if (g_pad[0]->IsPress(enButtonY) && m_status.mp >= m_skillPowerUpMp)
		{
			m_enAttackPatternState = enAttackPattern_Skill_PowerUp;
			SetNextAnimationState(enAnimationState_PowerUp);
			//MP�񕜏�Ԃ��~�߂�
			SetRecoveryMpFlag(false);
		}	
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
		Vector3(11.0f, 5.0f, 140.0f)
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
	//���̃{�[���̃��[���h���W���擾
	Matrix SkillBoonMatrix = m_modelRender.GetBone(m_skillBoonId)->GetWorldMatrix();

	SkillCollision->SetWorldMatrix(SkillBoonMatrix);
}

void Hero::Damage(int attack)
{
	//�X�L���g�p���Ȃ�
	{
		m_dashFlag = false;
		//�X�L����ł��I������̂�MP�񕜃t���O��true�ɂ���
		SetRecoveryMpFlag(true);
		//�X�L���g�p���Ƀ_���[�W���󂯂���������Ȃ�
		m_createSkillCollisionFlag = false;
		//�G�t�F�N�g�̒�~
		if (m_swordStormEffect != nullptr)
		{
			m_swordStormEffect->Stop();
		}
	}
	//HP��0���傫���Ȃ�
	if (m_status.hp > 0)
	{
		m_status.hp -= attack;
		SetInvicibleTimeFlag(true);
		SetNextAnimationState(enAnimationState_Damage);
	}
	//0��0�ȉ��Ȃ�
	if (m_status.hp <= 0)
	{
		//���ꂽ�̂�dieFlag��true�ɂ���
		SetDieFlag(true);
		m_status.hp = 0;

		//�t���[�����[�g�𗎂Ƃ�
		g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Variable, 30);

		SetNextAnimationState(enAnimationState_Die);
	}
}

void Hero::PowerUpTimer()
{
	//�p���[�A�b�v�t���O�������Ă��Ȃ��Ə������Ȃ�
	if (m_powerUpTimeFlag != true)
	{
		return;
	}
	//�������ԂɒB������
	if (m_powerUpTime < m_powerUpTimer)
	{
		//�p���[�A�b�v�I��
		m_powerUpTimer = 0.0f;
		m_powerUpTimeFlag = false;
		//�U���͂�߂�
		m_status.atk = m_status.defaultAtk;
		m_skillAttackPower = SKILL_ATTACK_POWER;
	}
	else
	{
		m_powerUpTimer += g_gameTime->GetFrameDeltaTime();
	}
}

bool Hero::CalcDash()
{
	if (m_dashTimer < 0.2f)
	{
		//�_�b�V���I���
		m_dashTimer = 1.0f;

		return true;
	}
	//���`���
	float speed;
	speed = Math::Lerp(m_dashTimer, 0.0f, 8.0f);

	m_dashTimer -= g_gameTime->GetFrameDeltaTime()*(1.0f);

	Vector3 forward = m_dashForward;
	forward *= speed;

	//�V�������W�ɂ���
	m_player->CalcPosition(forward, 1.0f);
	m_position = m_player->GetPosition();

	forward.Normalize();
	m_forward = forward;

	//�X�V
	SetTransFormModel(m_modelRender);
	m_modelRender.Update();

	return false;
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
	case Hero::enAninationState_Dash:
		// �_�b�V���X�e�[�g���쐬����B
		m_state = new HeroStateDash(this);
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
	case Hero::enAnimationState_Die:
		//Die���C���X�e�[�g���쐬����B
		m_state = new HeroStateDie(this);
		break;
	case Hero::enAnimationState_Damage:
		//��_���[�W���C���X�e�[�g���쐬����B
		m_state = new HeroStateDamage(this);
		break;
	case Hero::enAnimationState_PowerUp:
		//�p���[�A�b�v���C���X�e�[�g���쐬����B
		m_state = new HeroStatePowerUp(this);
		break;
	case Hero::enAnimationState_Victory:
		//�������C���X�e�[�g���쐬����B
		m_state = new HeroStateVictory(this);
		break;
	default:
		// �����ɗ�����X�e�[�g�̃C���X�^���X�쐬�����̒ǉ��Y��B
		// �N���b�V������
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
		SetNextAnimationState(enAninationState_Walk);
	}
	else
	{
		m_dashEffectFlag = false;
		SetNextAnimationState(enAninationState_Idle);
	}
}

void Hero::OnProcessDashStateTransition()
{
	if (CalcDash() == true)
	{
		m_dashEffect->Stop();
		
		m_dashEffectFlag = false;

		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
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
		//�R���{���I�������
		SetNowComboState(enNowCombo_None);
		SetDamagedComboState(enDamageCombo_None);
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
		//�R���{���I�������
		SetNowComboState(enNowCombo_None);
		SetDamagedComboState(enDamageCombo_None);
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Hero::OnProcessAttack_3StateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�R���{���I���Ȃ̂ŃR���{��Ԃ��Ȃ��ɖ߂�
		SetNowComboState(enNowCombo_None);
		SetDamagedComboState(enDamageCombo_None);
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
		//�`���[�W
		m_ChargeTimer += g_gameTime->GetFrameDeltaTime()*2.0f;
		//�ő�܂ŗ��܂�����ő�l�ɂ���
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

		//MP�����炷
		m_status.mp -= m_skillMp;

		//�X�L�������_�b�V���������Ƃɂ���
		m_dashFlag = true;
		//�����蔻��쐬
		m_createSkillCollisionFlag = true;
		//�X�e�[�g�����C���ɑJ��
		m_enAttackPatternState = enAttackPattern_Skill_Main;
		SetNextAnimationState(enAnimationState_Attack_Skill_Main);
		//�G�t�F�N�g�Đ�
		m_swordStormEffect = NewGO<EffectEmitter>(0);
		m_swordStormEffect->Init(InitEffect::enEffect_SwordStorm);
		m_swordStormEffect->Play();
		m_swordStormEffect->SetPosition(m_position);
		m_swordStormEffect->SetScale(g_vec3One * SWORD_EFFECT_SIZE);
		m_swordStormEffect->Update();

	}

}

void Hero::OnProcessAttack_Skill_MainStateTransition()
{
	//�`���[�W�������Ԃ��؂ꂽ��
	if (m_ChargeTimer < 0.0f)
	{
		//�_�b�V���t���O��false�ɂ���
		m_dashFlag = false;
		//�����蔻��̐�������߂�
		m_createSkillCollisionFlag = false;
		//�G�t�F�N�g�̒�~
		m_swordStormEffect->Stop();
		//�`���[�W���Ԃ����Z�b�g
		m_ChargeTimer = 0.0f;
		//�X�L����ł��I������̂�MP�񕜃t���O��true�ɂ���
		SetRecoveryMpFlag(true);
		//�U���p�^�[�����Ȃ���Ԃɂ���
		m_enAttackPatternState = enAttackPattern_None;
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
	else
	{
		//�G�t�F�N�g�̍��W���X�V
		m_swordStormEffect->SetPosition(m_position);
		m_swordStormEffect->Update();
		//�`���[�W�������Ԃ����炷
		m_ChargeTimer -= g_gameTime->GetFrameDeltaTime();
	}
}

void Hero::OnProcessDieStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�t���[�����[�g��߂�
		g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Variable, 60);
		//�A�j���[�V�������I������̂ŃL�����N�^�[��؂�ւ���t���O��true�ɂ���
		SetDieToChangeFlag(true);
	}
}

void Hero::OnProcessDamageStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�s���̓r����������Ȃ��̂ŃX�e�[�g��t���O�������ɖ߂�
		SetInvicibleDashState(enDashInvicibleState_None);
		//�U���p�^�[�����Ȃ���Ԃɂ���
		m_enAttackPatternState = enAttackPattern_None;
		m_createAttackCollisionFlag = false;
		//�R���{���I���Ȃ̂ŃR���{��Ԃ��Ȃ��ɖ߂�
		SetNowComboState(enNowCombo_None);
		SetDamagedComboState(enDamageCombo_None);

		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Hero::OnProcessPowerUpStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//��莞�ԃp���[�A�b�v���J�n
		m_powerUpTimeFlag = true;
		//�U���͂��グ��
		m_status.atk += m_powerUpPower;
		m_skillAttackPower += m_powerUpPower;
		//�X�L����ł��I������̂�MP�񕜃t���O��true�ɂ���
		SetRecoveryMpFlag(true);
		//�U���p�^�[�����Ȃ���Ԃɂ���
		m_enAttackPatternState = enAttackPattern_None;
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Hero::OnProcessVictoryStateTransition()
{
}

void Hero::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�A�^�b�N1�̃R���{��t�^�C�����n�܂�����
	if (wcscmp(eventName, L"Attack1_ComboStart") == 0)
	{
		m_createAttackCollisionFlag = true;
	}
	
	if (wcscmp(eventName, L"Attack1_ClollisionEnd") == 0)
	{
		m_createAttackCollisionFlag = false;
	}

	//�A�^�b�N1�̃R���{��t�^�C�����I�������
	if (wcscmp(eventName, L"Attack1_ComboEnd") == 0)
	{
		//2�R���{
		if (m_enAttackPatternState == enAttackPattern_1to2)
		{
			m_enAttackPatternState = enAttackPattern_2;
			SetNowComboState(enNowCombo_2);
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
			SetNowComboState(enNowCombo_3);
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

	//�p���[�A�b�v�I�I
	if (wcscmp(eventName, L"PowerUp") == 0)
	{
		//MP�����炷
		m_status.mp -= m_skillPowerUpMp;
	}

}

void Hero::Render(RenderContext& rc)
{
	//���G���Ԃ̊Ԃ͓_��
	if (m_invincibleTimeFlag == true)
	{
		if (m_modelDrawFlag == true)
		{
			m_modelRender.Draw(rc);
			m_modelDrawFlag = false;
		}
		else
		{
			m_modelDrawFlag =! m_modelDrawFlag;
		}
	}
	else
	m_modelRender.Draw(rc);

	
}
	




