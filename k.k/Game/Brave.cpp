#include "stdafx.h"
#include "Brave.h"
#include "BraveStateIdle.h"
#include "BraveStateSprint.h"
#include "BraveStateAttack_1.h"
#include "BraveStateAttack_2.h"
#include "BraveStateAttack_3.h"
#include "BraveStateSkill_Start.h"
#include "BraveStateSkill_Main.h"
#include "BraveStateDefend.h"
#include "BraveStateDefendHit.h"
#include "BraveStateHit.h"
#include "BraveStateDie.h"
#include "BraveStateChangeSwordShield.h"
#include "BraveStateWin_Start.h"
#include "BraveStateWin_Main.h"

#include "Player.h"

#include "SwordShield.h"
#include "BigSword.h"



//todo
//�U���͕͂���
//���茕�̃A�j���[�V�����x��

namespace {
	const float ADD_SCALE = 1.2f;

	const float ROT_SPEED = 20.0f;
	const float ROT_ONLY_SPEED = 20.0f;

	const float SWORD_EFFECT_SIZE = 10.0f;
	const Vector3 DASH_EFFECT_SIZE = { 10.0f,20.0f,8.0f };

	int MAXHP = 200;
	int MAXMP = 100;
	int ATK = 40;
	float SPEED = 200.0f;
	const char* NAME = "Brave";

	const int SKILL_ATTACK_POWER = 30;
}

Brave::Brave()
{
}

Brave::~Brave()
{
	DeleteGO(m_subWeapon);
	DeleteGO(m_mainWeapon);
}

bool Brave::Start()
{
	m_player = FindGO<Player>("player");

	//�X�e�[�^�X�̏�����
	m_status.InitStatus(
		MAXHP,
		MAXMP,
		ATK,
		SPEED,
		NAME
	);

	InitModel();

	//m_modelRender.SetAnimationSpeed(0.5f);

	//�L�����R���̐ݒ�
	m_charaCon.Init(12.0f, 33.0f, m_position);


	//����̐���
	m_subWeapon = NewGO<BigSword>(0, "bigsword");
	m_mainWeapon = NewGO<SwordShield>(0,"swordshield");

	m_useWeapon[enWeapon_Main].weapon = m_mainWeapon;
	m_useWeapon[enWeapon_Main].weaponAnimationStartIndexNo
		= OneHandSwordAnimationStartIndexNo;

	m_useWeapon[enWeapon_Sub].weapon = m_subWeapon;
	m_useWeapon[enWeapon_Sub].weaponAnimationStartIndexNo
		= TwoHandSwordAnimationStartIndexNo;

	//���݂̕���̃A�j���[�V�����N���b�v�̍ŏ��̔ԍ�
	m_currentAnimationStartIndexNo 
		= m_useWeapon[enWeapon_Main].weaponAnimationStartIndexNo;

	SetNextAnimationState(enAninationState_Idle);

	return true;
}

void Brave::Update()
{
	//�s���s�\�ȏ�ԂłȂ��Ȃ�
	if (IsInaction() != true)
	{
		//MP�̉�
		RecoveryMP();
		//����̐؂�ւ�����
		ChangeWeapon();
		//�ړ�����
		Move();
		//��]����
		ProcessRotation();
		//�U������
		ProcessAttack();
		//�h�䏈��
		ProcessDefend();

		//�����蔻��
		DamageCollision(m_charaCon);
	}
	

	ManageState();
	PlayAnimation();
	
	SetTransFormModel(m_modelRender);

	m_modelRender.Update();
	
}

void Brave::Move()
{
	m_moveSpeed = calcVelocity(GetStatus());
	m_moveSpeed.y = 0.0f;

	m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
}

void Brave::ProcessRotation()
{
	Rotation(ROT_SPEED, ROT_ONLY_SPEED);
}

void Brave::ProcessAttack()
{
	//�A�N�V�����t���O��true�Ȃ�U�����������Ȃ�
	if (GetIsActionFlag() == true)
	{
		return;
	}
	//�ʏ�U��
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		ProcessComboAttack();
		//3�R���{�ȏ�Ȃ�p�^�[���X�e�[�g���Z�b�g
		if (m_attackPatternState >= enAttackPattern_3)
		{
			m_attackPatternState = enAttackPattern_None;
		}
		return;
	}
	//�X�L��
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		//�A�N�V�����t���O���Z�b�g
		SetIsActionFlag(true);
		SetNextAnimationState(enAnimationState_Skill_start);
	}

}

void Brave::ProcessDefend()
{
	//�A�N�V�����t���O��true�Ȃ�U�����������Ȃ�
	if (GetIsActionFlag() == true)
	{
		return;
	}
	//�h��

	if (g_pad[0]->IsTrigger(enButtonY) == true)
	{
		//�A�N�V�����t���O���Z�b�g
		SetIsActionFlag(true);
		SetNextAnimationState(enAnimationState_Defend);
	}
}

void Brave::Damage(int damage)
{
	//HP��0���傫���Ȃ��_���[�W����
	if (GetStatus().hp > 0)
	{
		//�K�[�h���Ȃ�
		if (m_enAnimationState == enAnimationState_Defend)
		{
			//todo ���Ƀq�b�g�����Ȃ�
			 
			
			//�_���[�W��1/3�Ɍ��炷
			damage /= 3;
			//�ǂꂾ���_���[�W�����炵�Ă��P�ȉ��ɂ͂Ȃ�Ȃ�
			if (damage < 1){damage = 1;}
			//���q�b�g�X�e�[�g�ɑJ��
			SetNextAnimationState(enAnimationState_DefendHit);
		}
		else
		{
			//�q�b�g�X�e�[�g�ɑJ��
			SetNextAnimationState(enAnimationState_Hit);
		}

		m_status.CalcHp(damage, false);

		
	}
	//HP��0�ȉ��Ȃ�
	if (GetStatus().hp <= 0)
	{
		//���ꂽ�̂�dieFlag��true�ɂ���
		SetDieFlag(true);
		//HP��0�ɌŒ肷��
		m_status.SetHp(0);
		//���S�X�e�[�g�ɑJ��
		SetNextAnimationState(enAnimationState_Die);
	}
}

const bool& Brave::IsInaction() const
{
	//�s���o���Ȃ��Ȃ����
	//�v���C���[�N���X�̊֐��̓����Ȃ�������true�Ȃ�
	if (m_player->IsInaction() == true)
	{
		return true;
	}
	//���ꂽ�Ȃ�
	if (GetDieFlag() == true)
	{
		return true;
	}
	//�����܂ł�����s���\
	return false;
}

void Brave::MoveForward(float Speed)
{
	//to�O�����̎擾���~�߂�Έړ��������Œ�ł���
	//�U���������
	Vector3 attackDirection = m_forward;
	//�ړ����鑬�x
	Vector3 MoveSpeed = attackDirection * Speed;
	MoveSpeed.y = 0.0f;
	m_position = m_charaCon.Execute(MoveSpeed, 1.0f / 60.0f);
	m_modelRender.SetPosition(m_position);
}

void Brave::ChangeWeapon()
{
	//�A�N�V�����t���O��true�Ȃ�U�����������Ȃ�
	if (GetIsActionFlag() == true)
	{
		return;
	}
	//����̐؂�ւ�
	if (g_pad[0]->IsTrigger(enButtonRB1) == true)
	{
		ChangeUseWeapon();
		SetIsActionFlag(true);
		SetNextAnimationState(enAnimationState_ChangeSwordShield);
	}
}

void Brave::ProcessWin()
{
	SetNextAnimationState(enAnimationState_Win_Start);
}

void Brave::ProcessSwordShieldSkill(bool UpOrDownFlag)
{
	Vector3 Y = g_vec3AxisY;
	float mulYPos = 0.0f;
	if (UpOrDownFlag == true)
	{
		//Up����
		mulYPos += 
			g_gameTime->GetFrameDeltaTime() * m_useWeapon[enWeapon_Main].weapon->GetJampSpeed();
		Y.y += mulYPos;
	}
	else
	{
		//Down����
		mulYPos += 
			g_gameTime->GetFrameDeltaTime() * m_useWeapon[enWeapon_Main].weapon->GetJampSpeed() * 1.5f;
		Y.y -= mulYPos;
	}
	
	m_position = m_charaCon.Execute(Y, 1.0f / 30.0f);
	//�n�ʂɂ��Ă���Ȃ�
	if (m_charaCon.IsOnGround() == true)
	{
		m_position.y = 0.0f;
	}

	m_modelRender.SetPosition(m_position);
}

void Brave::SetNextAnimationState(int nextState)
{
	if (m_BraveState != nullptr) {
		// �Â��X�e�[�g���폜����B
		delete m_BraveState;
		m_BraveState = nullptr;
	}
	
	//�A�j���[�V�����X�e�[�g�����̃X�e�[�g�ɕς���
	m_enAnimationState = (EnAnimationState)nextState;

	switch (m_enAnimationState)
	{
	case Brave::enAninationState_Idle:
		m_BraveState = new BraveStateIdle(this);
		break;
	case Brave::enAninationState_Sprint:
		m_BraveState = new BraveStateSprint(this);
		break;
	case Brave::enAninationState_DashForward:
		break;
	case Brave::enAnimationState_KnockBack:
		break;
	case Brave::enAnimationState_Hit:
		m_BraveState = new BraveStateHit(this);
		break;
	case Brave::enAnimationState_Defend:
		m_BraveState = new BraveStateDefend(this);
		break;
	case Brave::enAnimationState_DefendHit:
		m_BraveState = new BraveStateDefendHit(this);
		break;
	case Brave::enAnimationState_Die:
		m_BraveState = new BraveStateDie(this);
		break;
	case Brave::enAnimationState_ChangeSwordShield:
		m_BraveState = new BraveStateChangeSwordShield(this);
		break;
	case Brave::enAnimationState_Win_Start:
		m_BraveState = new BraveStateWin_Start(this);
		break;
	case Brave::enAnimationState_Win_Main:
		m_BraveState = new BraveStateWin_Main(this);
		break;
	case Brave::enAnimationState_Attack_1:
		m_BraveState = new BraveStateAttack_1(this);
		break;
	case Brave::enAnimationState_Attack_2:
		m_BraveState = new BraveStateAttack_2(this);
		break;
	case Brave::enAnimationState_Attack_3:
		m_BraveState = new BraveStateAttack_3(this);
		break;
	case Brave::enAnimationState_Skill_start:
		m_BraveState = new BraveStateSkill_Start(this);
		break;
	case Brave::enAnimationState_Skill_Main:
		m_BraveState = new BraveStateSkill_Main(this);
		break;
	default:
		break;
	}
}

void Brave::PlayAnimation()
{
	//�h���N���X��PlayAnimation�֐������s
	m_BraveState->PlayAnimation();
}

void Brave::ManageState()
{
	//�h���N���X��ManageState�֐������s
	m_BraveState->ManageState();
}

void Brave::ProcessComboAttack()
{
	//�p�^�[���X�e�[�g����i�߂�
	m_attackPatternState =
		static_cast<EnAttackPattern>(m_attackPatternState + 1);
	//�ʏ�U���X�e�[�g�ݒ�
	SetNextAnimationState(m_attackPatternState);

	switch (m_attackPatternState)
	{
	case Brave::enAttackPattern_None:
		//�G�̂��߂̃R���{�X�e�[�g��ݒ�
		SetNowComboState(enNowCombo_None);
		break;
	case Brave::enAttackPattern_1:
		//�G�̂��߂̃R���{�X�e�[�g��ݒ�
		SetNowComboState(enNowCombo_1);
		break;
	case Brave::enAttackPattern_2:
		//�G�̂��߂̃R���{�X�e�[�g��ݒ�
		SetNowComboState(enNowCombo_2);
		break;
	case Brave::enAttackPattern_3:
		//�G�̂��߂̃R���{�X�e�[�g��ݒ�
		SetNowComboState(enNowCombo_3);
		break;
	case Brave::enAttackPattern_End:
		break;
	default:
		break;
	}
	
	//�A�N�V�����t���O���Z�b�g
	SetIsActionFlag(true);
}

void Brave::ProcessCommonStateTransition()
{
	if (fabsf(GetMoveSpeed().x) >= 0.001f ||
		fabsf(GetMoveSpeed().z) >= 0.001f
		)
	{
		//����
		SetNextAnimationState(enAninationState_Sprint);
	}
	else
	{
		//����
		SetNextAnimationState(enAninationState_Idle);
	}
}

void Brave::ProcessCommonWeaponChangeStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���݂̕���̃A�j���[�V�����N���b�v�̍ŏ��̔ԍ���ύX
		m_currentAnimationStartIndexNo
			= m_useWeapon[enWeapon_Main].weaponAnimationStartIndexNo;
		//�؂�ւ��A�j���[�V�������I������̂ŃA�N�V�����t���O��false�ɂ���
		SetIsActionFlag(false);
		//�X�e�[�g���ʂ̏�ԑJ�ڏ����ɑJ��
		ProcessCommonStateTransition();
	}
}

void Brave::ProcessNormalAttackStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃R���{�Ɍq���Ȃ��A�܂���3�R���{�ڂȂ烊�Z�b�g
		if (GetNextComboFlagFlag() == false ||
			m_attackPatternState >= enAttackPattern_3)
		{
			m_position.y = 0.0f;
			m_attackPatternState = enAttackPattern_None;
			//�U���A�j���[�V�������I������̂ŃA�N�V�����t���O��false�ɂ���
			SetIsActionFlag(false);
			//�R���{��Ԃ����Z�b�g
			SetComboStateNone();
			//�X�e�[�g���ʂ̏�ԑJ�ڏ����ɑJ��
			ProcessCommonStateTransition();
		}
		//���̃R���{�̍U���X�e�[�g�ݒ�
		else
		{
			//���̃R���{�Ɉڂ�O�ɉ�]����
			if (fabsf(m_SaveMoveSpeed.x) >= 0.001f || fabsf(m_SaveMoveSpeed.z) >= 0.001f)
			{
				m_rotation.SetRotationYFromDirectionXZ(m_SaveMoveSpeed);
			}

			//���̃R���{�̏���
			ProcessComboAttack();
		}
		//�K�����Z�b�g
		{
			//�R���{��t�t���O�����Z�b�g
			SetIsComboReceptionFlagFlag(false);
			//���̃R���{�t���O�����Z�b�g
			SetNextComboFlagFlag(false);
		}
	}
	//�R���{��t�t���O��true����A�{�^������������
	else if (GetIsComboReceptionFlagFlag() == true &&
		g_pad[0]->IsTrigger(enButtonA))
	{
		//���̃R���{�ɐi�ރt���O�Z�b�g
		SetNextComboFlagFlag(true);
	}
}

void Brave::ProcessSkillStartStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���C���X�e�[�g�ɑJ��
		SetNextAnimationState(enAnimationState_Skill_Main);
	}
}

void Brave::ProcessSkillMainStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�U���A�j���[�V�������I������̂ōU���\
		SetIsActionFlag(false);
		//�X�e�[�g���ʂ̏�ԑJ�ڏ����ɑJ��
		ProcessCommonStateTransition();
	}
}

void Brave::ProcessHitStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�A�N�V�������Ƀ_���[�W�󂯂���������Ȃ��̂�
		// �A�N�V�����t���O�֌W��S��false�ɂ���
		SetAllInfoAboutActionFlag(false);
		//�R���{��Ԃ����Z�b�g
		SetComboStateNone();
		//�X�e�[�g���ʂ̏�ԑJ�ڏ����ɑJ��
		ProcessCommonStateTransition();
	}
}

void Brave::ProcessDieStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���ꂽ�̂�dieFlag��true�ɂ���
		SetDieFlag(true);
	}
}

void Brave::ProcessDefendStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�A�N�V�������Ƀ_���[�W�󂯂���������Ȃ��̂�
		// �A�N�V�����t���O�֌W��S��false�ɂ���
		SetAllInfoAboutActionFlag(false);
		//�R���{��Ԃ����Z�b�g
		SetComboStateNone();
		//�X�e�[�g���ʂ̏�ԑJ�ڏ����ɑJ��
		ProcessCommonStateTransition();
	}
}

bool Brave::RotationOnly()
{
	//��]�\�ȃA�j���[�V�����Ȃ�
	if (isRotationEntable() == true &&
		m_useWeapon[enWeapon_Main].weapon->GetEnDefendTipe()==IWeapon::enDefendTipe_Defence)
	{
		return true;
	}

	return false;
}

void Brave::ReverseWeapon()
{
	//���C������ƃT�u����̏�ԃX�e�[�g���t�ɂ���
	m_useWeapon[enWeapon_Main].weapon->ReverseWeaponState();
	m_useWeapon[enWeapon_Sub].weapon->ReverseWeaponState();
}

void Brave::ChangeUseWeapon()
{
	UseWeapon temporary;
	temporary = m_useWeapon[enWeapon_Main];
	m_useWeapon[enWeapon_Main] = m_useWeapon[enWeapon_Sub];
	m_useWeapon[enWeapon_Sub] = temporary;

	//���݂̕���̃A�j���[�V�����N���b�v�̍ŏ��̔ԍ���ύX
	m_currentAnimationStartIndexNo
		= m_useWeapon[enWeapon_Main].weaponAnimationStartIndexNo;
}

void Brave::InitModel()
{
	// �Ў茕�̃A�j���[�V�����N���b�v�����[�h����
	const std::pair<const char*, bool> oneHandedSwordAnimClipFilePaths[] = {
		{"Assets/animData/character/Player/OneHandSword/Idle.tka",true},
		{"Assets/animData/character/Player/OneHandSword/Sprint.tka",true},
		{"Assets/animData/character/Player/OneHandSword/KnockBack.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Hit.tka",false},
		{ "Assets/animData/character/Player/OneHandSword/Defend.tka", true },
		{ "Assets/animData/character/Player/OneHandSword/DefendHit.tka", false },
		{ "Assets/animData/character/Player/OneHandSword/Die.tka", false },
		{"Assets/animData/character/Player/OneHandSword/ChangeSwordShield.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Win_start.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Win_main.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Attack_1.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Attack_2.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Attack_3.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Attack_4.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Attack_5.tka",false},
		{"None",false}
	};
	for (int i = 0; i < enAnimClip_Num; i++) {
		m_animationClip[i].Load(oneHandedSwordAnimClipFilePaths[i].first);
		m_animationClip[i].SetLoopFlag(oneHandedSwordAnimClipFilePaths[i].second);
	}

	// ���茕�̃A�j���[�V�����N���b�v�����[�h����
	const std::pair<const char*, bool> twoHandedSwordAnimClipFilePaths[] = {
		{"Assets/animData/character/Player/TwoHandSword/idle_BigSword.tka",true},
		{"Assets/animData/character/Player/TwoHandSword/Sprint.tka",true},
		{"Assets/animData/character/Player/TwoHandSword/KnockBack.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Hit.tka",false},
		{ "Assets/animData/character/Player/TwoHandSword/Rool.tka", false },
		{ "Assets/animData/character/Player/TwoHandSword/Rool.tka", false },
		{ "Assets/animData/character/Player/TwoHandSword/Die.tka", false },
		{"Assets/animData/character/Player/TwoHandSword/ChangeTwoHandSword.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Win_Start.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Win_Main.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Attack_1.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Attack_2.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Attack_3.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Skill_Start.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Skill_Main.tka",false},
		{"None",false}
	};
	for (int i = 0; i < enAnimClip_Num; i++) {
		m_animationClip[TwoHandSwordAnimationStartIndexNo + i].Load(twoHandedSwordAnimClipFilePaths[i].first);
		m_animationClip[TwoHandSwordAnimationStartIndexNo + i].SetLoopFlag(twoHandedSwordAnimClipFilePaths[i].second);
	}

	m_modelRender.Init("Assets/modelData/character/Player/NewHero/Hero_Smile.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		m_animationClip,
		enAnimClip_Num * AnimationClipGroup_Num,
		enModelUpAxisZ
	);
	
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	m_charaCenterBoonId = m_modelRender.FindBoneID(L"root");

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
}

void Brave::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

void Brave::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	///////////////////////////////////////////////////////////
	// �S���틤�ʏ���
	/////////////////////////////////////////////////////////// 
	//�R���{��t���\�Ȃ�
	if (wcscmp(eventName, L"ComboAcceptable") == 0)
	{
		SetIsComboReceptionFlagFlag(true);
	}
	//�O�i����n�܂�
	if (wcscmp(eventName, L"MoveForwardStart") == 0)
	{
		SetMoveforwardFlag(true);
	}

	//�O�i����I���
	if (wcscmp(eventName, L"MoveForwardEnd") == 0)
	{
		SetMoveforwardFlag(false);
	}

	//�������ւ�
	if (wcscmp(eventName, L"ArmedSwordShield") == 0)
	{
		ReverseWeapon();
	}

	//�����蔻��̗L����
	if (wcscmp(eventName, L"CollisionStart") == 0)
	{
		SetIsCollisionPossibleFlag(true);
	}
	//�����蔻��̖�����
	if (wcscmp(eventName, L"CollisionEnd") == 0)
	{
		SetIsCollisionPossibleFlag(false);
	}

	////////////////////////////////////////////////////////////
	// �����̏���
	////////////////////////////////////////////////////////////
	//�X�L���̃m�b�N�o�b�N�̃^�C�~���O
	if (wcscmp(eventName, L"KnockBack") == 0)
	{
		int a = 0;
	}
	//�X�L���̃W�����v����
	if (wcscmp(eventName, L"Jamp") == 0)
	{
		ProcessSwordShieldSkill(true);
	}

	//�X�L���̃W�����v����
	if (wcscmp(eventName, L"Down") == 0)
	{
		ProcessSwordShieldSkill(false);
	}

	
}

