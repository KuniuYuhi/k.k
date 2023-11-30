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
#include "KnockBack.h"

#include "SwordShield.h"
#include "BigSword.h"
#include "Bow.h"
#include "ManagerPreCompile.h"


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
	DeleteGO(m_mainUseWeapon.weapon);
	DeleteGO(m_subUseWeapon.weapon);
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

	//�������镐��̐ݒ�
	SettingWeapons();

	InitModel();

	//�L�����R���̐ݒ�
	m_charaCon.Init(12.0f, 33.0f, m_position);

	

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
		//���G���Ԃ̌v�Z
		CalcInvincibleTime();
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

	//����̃A�j���[�V�������Đ����Ȃ�ړ��Ȃ�
	if (isAnimationEntable() != true)
	{
		if (m_mainUseWeapon.weapon->GetRotationDelectionFlag() == true)
		{
			//�O�����̌v�Z
			CalcForward(m_moveSpeed);
		}
		m_moveSpeed = g_vec3Zero;
	}
	else
	{
		CalcForward(m_moveSpeed);
	}


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
		//�R���{���q�����Ă��鎞�Ƀ_���[�W���󂯂���������Ȃ��̂Ń��Z�b�g
		m_attackPatternState = enAttackPattern_None;

		//����؂�ւ����Ƀ_���[�W���󂯂���
		if (m_enAnimationState == enAnimationState_ChangeWeapon)
		{
			//����؂�ւ��O�̕���̃A�j���[�V�������Đ������悤�ɂ���
			m_currentAnimationStartIndexNo
				= m_mainUseWeapon.weaponAnimationStartIndexNo;
		}

		if (IsDefendHit() == true)
		{
			//�_���[�W��1/3�Ɍ��炷
			damage /= 3;
			//�ǂꂾ���_���[�W�����炵�Ă��P�ȉ��ɂ͂Ȃ�Ȃ�
			if (damage < 1) { damage = 1; }
			//���q�b�g�X�e�[�g�ɑJ��
			SetNextAnimationState(enAnimationState_DefendHit);
		}
		else
		{
			//���ʂɃ_���[�W���󂯂�
			//�q�b�g�X�e�[�g�ɑJ��
			SetNextAnimationState(enAnimationState_Hit);
		}

		//�󂯂�_���[�W������
		m_hitDamage = damage;
		m_status.CalcHp(m_hitDamage, false);
		//�_���[�W���󂯂��̂Ŗ��G���Ԃɓ���
		SetInvicibleTimeFlag(true);
	}
	//HP��0�ȉ��Ȃ�
	if (GetStatus().hp <= 0)
	{
		//���ꂽ�̂�dieFlag��true�ɂ���
		SetDieFlag(true);
		//�_�ł��Ȃ��悤�ɂ���
		SetInvicibleTimeFlag(false);
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
	//�T�u�P�ƕ���؂�ւ�
	if (g_pad[0]->IsTrigger(enButtonUp))
	{
		//����؂�ւ��̂��߂̑O����
		SettingChangeWeapon(
			m_subUseWeapon.weaponAnimationStartIndexNo, enWeapon_Sub);
	}
	//�T�u�Q�ƕ���؂�ւ�
	if (g_pad[0]->IsTrigger(enButtonDown))
	{
		//����؂�ւ��̂��߂̑O����
		SettingChangeWeapon(
			m_subUseWeapon2.weaponAnimationStartIndexNo, enWeapon_Sub2);
	}
	

}

void Brave::ProcessWin()
{
	//���G���Ԃœ_�ł��Ȃ��悤�ɂ���
	SetInvicibleTimeFlag(false);
	//�����X�^�[�g�̃X�e�[�g
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
			g_gameTime->GetFrameDeltaTime() * m_mainUseWeapon.weapon->GetJampSpeed();
		Y.y += mulYPos;
	}
	else
	{
		//Down����
		mulYPos += 
			g_gameTime->GetFrameDeltaTime() * m_mainUseWeapon.weapon->GetJampSpeed() * 1.2f;
		Y.y -= mulYPos;
	}
	
	m_position = m_charaCon.Execute(Y, 1.0f / 30.0f);
	//�n�ʂɂ��Ă���Ȃ�


	if (m_charaCon.IsOnGround() == true|| m_position.y < 0.0f)
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
	case Brave::enAnimationState_ChangeWeapon:
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

void Brave::SetAttackPosition(Vector3 attackPosition)
{
	m_player->SetAttackPosition(attackPosition);
}

void Brave::ProcessComboAttack()
{
	//�p�^�[���X�e�[�g����i�߂�
	m_attackPatternState =
		static_cast<EnAttackPattern>(m_attackPatternState + 1);

	//�ʏ�U���X�e�[�g�ݒ�
	SetNextAnimationState(m_attackPatternState);
	//�G�̂��߂̃R���{�X�e�[�g��ݒ�
	switch (m_attackPatternState)
	{
	case Brave::enAttackPattern_None:
		SetNowComboState(enNowCombo_None);
		break;
	case Brave::enAttackPattern_1:
		SetNowComboState(enNowCombo_1);
		break;
	case Brave::enAttackPattern_2:
		SetNowComboState(enNowCombo_2);
		break;
	case Brave::enAttackPattern_3:
		SetNowComboState(enNowCombo_3);
		break;
	case Brave::enAttackPattern_End:
		break;
	default:
		break;
	}
	//3�R���{�ȏ�Ȃ�p�^�[���X�e�[�g���Z�b�g
	if (m_attackPatternState >= enAttackPattern_3)
	{
		m_attackPatternState = enAttackPattern_None;
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
		//���G��ԃt���O�̃��Z�b�g
		SetInvicibleFlag(false);
		//���݂̕���̃A�j���[�V�����N���b�v�̍ŏ��̔ԍ���ύX
		m_currentAnimationStartIndexNo
			= m_mainUseWeapon.weaponAnimationStartIndexNo;
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
			m_attackPatternState == enAttackPattern_None)
		{
			m_position.y = 0.0f;
			m_attackPatternState = enAttackPattern_None;
			//�U���A�j���[�V�������I������̂ŃA�N�V�����\���̂̃t���O��S�ă��Z�b�g
			SetAllInfoAboutActionFlag(false);
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
				m_forward = m_SaveMoveSpeed;
				m_forward.Normalize();
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
		//todo ���G��ԃt���O�̃��Z�b�g
		SetInvicibleFlag(false);

		//�m�b�N�o�b�N�U���t���O�������Ă����烊�Z�b�g
		if (GetKnockBackAttackFalg() == true)
		{
			SetKnockBackAttackFalg(false);
		}

		//�U���A�j���[�V�������I������̂ŃA�N�V�����\���̂̃t���O��S�ă��Z�b�g
		SetAllInfoAboutActionFlag(false);
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
		//��_���[�W�ɂ���Ė߂��Ȃ������ϐ������Z�b�g
		m_mainUseWeapon.weapon->ResetVariable();
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
		//��_���[�W�ɂ���Ė߂��Ȃ������ϐ������Z�b�g
		m_mainUseWeapon.weapon->ResetVariable();
		//�X�e�[�g���ʂ̏�ԑJ�ڏ����ɑJ��
		ProcessCommonStateTransition();
	}
}

bool Brave::RotationOnly()
{
	//��]�̂݉\�ȃA�j���[�V�����܂��́A���݂̕���̖h��^�C�v�����Ŏ��^�C�v�Ȃ�
	if (isRotationEntable() == false &&
		m_mainUseWeapon.weapon->GetEnDefendTipe()==IWeapon::enDefendTipe_Defence)
	{
		return true;
	}
	//�������̕���̉�]�\�t���O��true�Ȃ�
	else if (m_mainUseWeapon.weapon->GetRotationDelectionFlag() == true)
	{
		return true;
	}
		

	return false;
}

void Brave::ReverseWeapon(EnWeapons changeTargetWeaponType)
{
	UseWeapon* ChangeUseSubWeapon = nullptr;

	if (changeTargetWeaponType == enWeapon_Sub)
	{
		//����̃T�u�ƃ��C���̒��g�����ւ���
		ChangeUseWeapon(m_subUseWeapon);
		ChangeUseSubWeapon = &m_subUseWeapon;
	}
	else
	{
		//����̃T�u�Q�ƃ��C���̒��g�����ւ���
		ChangeUseWeapon(m_subUseWeapon2);
		ChangeUseSubWeapon = &m_subUseWeapon2;
	}

	//���C������ƃT�u����̏�ԃX�e�[�g���t�ɂ���
	m_mainUseWeapon.weapon->ReverseWeaponState();
	ChangeUseSubWeapon->weapon->ReverseWeaponState();
	//�t�Q�Ƃ�����
	//�U���͂����݂̕���̂��̂ɕύX�B
	m_status.atk = m_mainUseWeapon.weapon->GetWeaponPower();
	//���i�q�b�g����t���O���Z�b�g
	m_mainUseWeapon.weapon->SetHittableFlag(true);
}

void Brave::ChangeUseWeapon(UseWeapon& ChangeUseSubWeapon)
{
	UseWeapon temporary;
	temporary = m_mainUseWeapon;
	m_mainUseWeapon = ChangeUseSubWeapon;
	ChangeUseSubWeapon = temporary;
	//���݂̕���̃A�j���[�V�����N���b�v�̍ŏ��̔ԍ���ύX
	m_currentAnimationStartIndexNo
		= m_mainUseWeapon.weaponAnimationStartIndexNo;
}

bool Brave::IsDefendHit()
{
	//�K�[�h���Ȃ�
	if (m_enAnimationState == enAnimationState_Defend)
	{
		//���ɓ������Ă���Ȃ�
		if (m_mainUseWeapon.weapon->IsHitCollision() == true)
		{
			return true;
		}
		return false;
	}
	else
	{
		return false;
	}
}

void Brave::SettingWeapons()
{
	//���C������̓\�[�h���V�[���h
	SwordShield* swordShield = NewGO<SwordShield>(0, "swordshield");
	m_mainUseWeapon.weapon = swordShield;
	m_mainWeaponType = enWeaponType_SwordShield;
	//�A�j���[�V�����N���b�v�̍ŏ��̔ԍ���ݒ�
	SetCurrentAnimationStartIndexNo(m_mainUseWeapon, enWeapon_Main);
	//������Ԃɐݒ�
	m_mainUseWeapon.weapon->StartSetWeaponState(SwordShield::enWeaponState_Armed);


	//�T�u�P�̓O���C�g�\�[�h
	BigSword* greatSword = NewGO<BigSword>(0, "greatSword");
	m_subUseWeapon.weapon = greatSword;
	m_subWeaponType = enWeaponType_TwoHandSword;
	//�A�j���[�V�����N���b�v�̍ŏ��̔ԍ���ݒ�
	SetCurrentAnimationStartIndexNo(m_subUseWeapon, enWeapon_Sub);
	m_subUseWeapon.weapon->StartSetWeaponState(BigSword::enWeaponState_Stowed);

	//�T�u�Q�̓{�E���A���[
	Bow* bowArrow = NewGO<Bow>(0, "bowarrow");
	m_subUseWeapon2.weapon = bowArrow;
	m_subWeapon2Type = enWeaponType_Bow;
	//�A�j���[�V�����N���b�v�̍ŏ��̔ԍ���ݒ�
	SetCurrentAnimationStartIndexNo(m_subUseWeapon2, enWeapon_Sub2);
	m_subUseWeapon2.weapon->StartSetWeaponState(BigSword::enWeaponState_Stowed);

	//���݂̕���̃A�j���[�V�����N���b�v�̍ŏ��̔ԍ�
	m_currentAnimationStartIndexNo
		= m_mainUseWeapon.weaponAnimationStartIndexNo;
	//����̍U���͂����g�̍U���͂ɐݒ�
	m_status.atk = m_mainUseWeapon.weapon->GetWeaponPower();


	//���C������̐����ƃC���X�^���X�̑��
	//m_mainWeaponType = WeaponManager::GetInstance()->GetMainWeapon();
	//m_mainUseWeapon.weapon =
	//	WeaponManager::GetInstance()->CreateWeapon(m_mainWeaponType);
	////���C������𑕔���Ԃɂ���
	//m_mainUseWeapon.weapon->StartSetWeaponState(WeaponBase::enWeaponState_Armed);
	////�A�j���[�V�����N���b�v�̍ŏ��̔ԍ���ݒ�
	//SetCurrentAnimationStartIndexNo(m_mainUseWeapon, enWeapon_Main);
	////�T�u����̐����ƃC���X�^���X�̑��
	//m_subWeaponType = WeaponManager::GetInstance()->GetSubWeapon();
	//m_subUseWeapon.weapon =
	//	WeaponManager::GetInstance()->CreateWeapon(m_subWeaponType);
	////�T�u��������[��Ԃɂ���
	//m_subUseWeapon.weapon->StartSetWeaponState(WeaponBase::enWeaponState_Stowed);
	////�A�j���[�V�����N���b�v�̍ŏ��̔ԍ���ݒ�
	//SetCurrentAnimationStartIndexNo(m_subUseWeapon, enWeapon_Sub);
	////���݂̕���̃A�j���[�V�����N���b�v�̍ŏ��̔ԍ�
	//m_currentAnimationStartIndexNo
	//	= m_mainUseWeapon.weaponAnimationStartIndexNo;
	////����̍U���͂����g�̍U���͂ɐݒ�
	//m_status.atk = m_mainUseWeapon.weapon->GetWeaponPower();

	////�E�F�|���}�l�[�W���[�̍폜
	//WeaponManager::DeleteInstance();
}

void Brave::SettingChangeWeapon(
	int animationStartInbexNo, EnWeapons changeTargetWeapon)
{
	//���݂̕���̃A�j���[�V�����N���b�v�̍ŏ��̔ԍ����T�u�ɕύX
	m_currentAnimationStartIndexNo
		= animationStartInbexNo;
	m_changeTargetUseWeapon = changeTargetWeapon;
	//�T�u����̕���؂�ւ��A�j���[�V�����X�e�[�g��ݒ�
	SetNextAnimationState(enAnimationState_ChangeWeapon);
	SetIsActionFlag(true);
}

void Brave::SetCurrentAnimationStartIndexNo(UseWeapon& useWeapon, EnWeapons mainOrSub)
{
	switch (mainOrSub)
	{
	case Brave::enWeapon_Main:
		useWeapon.weaponAnimationStartIndexNo = m_mainWeaponAnimationStartIndexNo;
		break;
	case Brave::enWeapon_Sub:
		useWeapon.weaponAnimationStartIndexNo = m_subWeaponAnimationStartIndexNo;
		break;
	case Brave::enWeapon_Sub2:
		useWeapon.weaponAnimationStartIndexNo = m_subWeapon2AnimationStartIndexNo;
		break;
	case Brave::enWeapon_num:
		std::abort();
		break;
	default:
		break;
	}
}

void Brave::RoadWeaponTypeAnimetionClip(EnWeaponType weaponType, int weaponAnimationStartIndexNo)
{
	switch (weaponType)
	{
	case enWeaponType_SwordShield:
		RoadOneHandSwordAnimationClip(weaponAnimationStartIndexNo);
		break;
	case enWeaponType_TwoHandSword:
		RoadTwoHandSwordAnimationClip(weaponAnimationStartIndexNo);
		break;
	case enWeaponType_Bow:
		RoadBowAnimationClip(weaponAnimationStartIndexNo);
		break;
	case enWeaponType_Num:
		std::abort();
		break;
	default:
		break;
	}
}

void Brave::RoadOneHandSwordAnimationClip(int mainWeaponAnimationStartIndexNo)
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
		m_animationClip[mainWeaponAnimationStartIndexNo + i].Load(oneHandedSwordAnimClipFilePaths[i].first);
		m_animationClip[mainWeaponAnimationStartIndexNo + i].SetLoopFlag(oneHandedSwordAnimClipFilePaths[i].second);
	}
}
void Brave::RoadTwoHandSwordAnimationClip(int mainWeaponAnimationStartIndexNo)
{
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
		m_animationClip[mainWeaponAnimationStartIndexNo + i].Load(twoHandedSwordAnimClipFilePaths[i].first);
		m_animationClip[mainWeaponAnimationStartIndexNo + i].SetLoopFlag(twoHandedSwordAnimClipFilePaths[i].second);
	}
}
void Brave::RoadBowAnimationClip(int mainWeaponAnimationStartIndexNo)
{
	// �|�̃A�j���[�V�����N���b�v�����[�h����
	const std::pair<const char*, bool> bowAnimClipFilePaths[] = {
		{"Assets/animData/character/Player/Bow/Idle.tka",true},
		{"Assets/animData/character/Player/Bow/Sprint.tka",true},
		{"Assets/animData/character/Player/Bow/KnockBack.tka",false},
		{"Assets/animData/character/Player/Bow/Hit.tka",false},
		{ "Assets/animData/character/Player/Bow/Rool.tka", false },
		{ "Assets/animData/character/Player/Bow/Rool.tka", false },
		{ "Assets/animData/character/Player/Bow/Die.tka", false },
		{"Assets/animData/character/Player/Bow/ChangeBow.tka",false},
		{"Assets/animData/character/Player/Bow/Win_Start.tka",false},
		{"Assets/animData/character/Player/Bow/Win_Main.tka",false},
		{"Assets/animData/character/Player/Bow/Attack_1.tka",false},
		{"Assets/animData/character/Player/Bow/Attack_2.tka",false},
		{"Assets/animData/character/Player/Bow/Attack_3.tka",false},
		{"Assets/animData/character/Player/Bow/Skill_Start.tka",true},
		{"Assets/animData/character/Player/Bow/Skill_Main.tka",false},
		{"None",false}
	};
	for (int i = 0; i < enAnimClip_Num; i++) {
		m_animationClip[mainWeaponAnimationStartIndexNo + i].Load(bowAnimClipFilePaths[i].first);
		m_animationClip[mainWeaponAnimationStartIndexNo + i].SetLoopFlag(bowAnimClipFilePaths[i].second);
	}
}

void Brave::InitModel()
{
	//���C������ɑΉ�����A�j���[�V�����N���b�v��ǂݍ���
	RoadWeaponTypeAnimetionClip(m_mainWeaponType, m_mainWeaponAnimationStartIndexNo);
	//�T�u����ɑΉ�����A�j���[�V�����N���b�v��ǂݍ���
	RoadWeaponTypeAnimetionClip(m_subWeaponType, m_subWeaponAnimationStartIndexNo);
	//�T�u����2�ɑΉ�����A�j���[�V�����N���b�v��ǂݍ���
	RoadWeaponTypeAnimetionClip(m_subWeapon2Type, m_subWeapon2AnimationStartIndexNo);


	//���f���̏�����
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
	if (IsFlashing() != true)
	{
		m_modelRender.Draw(rc);
	}
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
		CalcForward(m_moveSpeed);
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
		ReverseWeapon(m_changeTargetUseWeapon);
	}

	//�U�������蔻��̗L����
	if (wcscmp(eventName, L"CollisionStart") == 0)
	{
		SetIsCollisionPossibleFlag(true);
	}
	//�U�������蔻��̖�����
	if (wcscmp(eventName, L"CollisionEnd") == 0)
	{
		SetIsCollisionPossibleFlag(false);
	}

	//���G��Ԃ̗L����
	if (wcscmp(eventName, L"InvisibleStart") == 0)
	{
		SetInvicibleFlag(true);
	}
	//���G��Ԃ̖�����
	if (wcscmp(eventName, L"InvisibleEnd") == 0)
	{
		SetInvicibleFlag(false);
	}

	//�X�L���g�p���̍U������
	if (wcscmp(eventName, L"SkillAttack") == 0)
	{
		//���C������̃X�L���U������
		m_mainUseWeapon.weapon->ProcessSkillAttack();
	}
	//�X�L���̃W�����v����
	if (wcscmp(eventName, L"Jamp") == 0)
	{
		//�L�[�t���[����Jamp�̊ԏ�����������
		ProcessSwordShieldSkill(true);
	}
	//�X�L���̃W�����v����
	if (wcscmp(eventName, L"Down") == 0)
	{
		//�L�[�t���[����Jamp�̊ԏ�����������
		ProcessSwordShieldSkill(false);
	}

	//����ŗL�̏����͂��ꂼ��̕���N���X�̃A�j���[�V�����C�x���g�֐��ɂ���

}

bool Brave::isCollisionEntable() const
{
	//��������̖h��^�C�v�����ȂǂŖh���^�C�v�Ȃ�
	if (m_mainUseWeapon.weapon->GetEnDefendTipe()==IWeapon::enDefendTipe_Defence)
	{
		return m_enAnimationState == enAnimationState_Hit ||
			m_enAnimationState == enAnimationState_DefendHit;
	}
	//����ȊO(���)�Ȃ�
	else
	{
		return m_enAnimationState == enAnimationState_Hit ||
			m_enAnimationState == enAnimationState_Defend ||
			m_enAnimationState == enAnimationState_DefendHit;
	}
}

bool Brave::isRotationEntable() const
{
	//��������̖h��^�C�v�����ȂǂŖh���^�C�v�Ȃ�
	if (m_mainUseWeapon.weapon->GetEnDefendTipe() == IWeapon::enDefendTipe_Defence)
	{
		return m_enAnimationState != enAnimationState_Defend;
	}
	//����ȊO(���)�Ȃ�
	else
	{
		return m_enAnimationState == enAnimationState_Defend;
	}
}
