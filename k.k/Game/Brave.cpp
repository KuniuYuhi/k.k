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

//todo
//�U���͕͂���

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

	SetNextAnimationState(enAninationState_Idle);

	//m_modelRender.SetAnimationSpeed(0.4f);

	//�L�����R���̐ݒ�
	m_charaCon.Init(12.0f, 33.0f, m_position);

	return true;
}

void Brave::Update()
{
	//�s���s�\�ȏ�ԂłȂ��Ȃ�
	if (IsInaction() != true)
	{
		//MP�̉�
		RecoveryMP();

		//�ړ�����
		Move();
		//��]����
		ProcessRotation();
		//�U������
		ProcessAttack();
		//�h�䏈��
		ProcessDefend();
	}
	

	ManageState();
	PlayAnimation();
	

	UpdateWeapons();
	

	m_modelRender.Update();
	
}

void Brave::Move()
{
	if (isAnimationEntable() != true)
	{
		return;
	}
	//todo �ړ����Ȃ�����-�Ԃ����̌v�Z
	m_moveSpeed = calcVelocity(GetStatus());
	m_moveSpeed.y = 0.0f;

	m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);

	m_modelRender.SetPosition(m_position);
}

void Brave::ProcessRotation()
{
	Rotation(ROT_SPEED, ROT_ONLY_SPEED);
	m_modelRender.SetRotation(m_rotation);
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
	//���ꂽ�Ȃ�
	if (GetDieFlag() == true)
	{
		return true;
	}

	/*if (m_player->IsWinnerDecision() == true)
	{
		return true;
	}*/

	//�����܂ł�����s���\
	return false;
}

void Brave::CalcAttackDirection(float Speed)
{
	//�U���������
	Vector3 attackDirection = m_forward;
	//�ړ����鑬�x
	Vector3 MoveSpeed = attackDirection * Speed;
	MoveSpeed.y = 0.0f;
	m_position = m_charaCon.Execute(MoveSpeed, 1.0f / 60.0f);
	m_modelRender.SetPosition(m_position);
}

void Brave::ProcessSwordShieldSkill(bool UpOrDownFlag)
{
	Vector3 Y = g_vec3AxisY;
	float mulYPos = 0.0f;
	if (UpOrDownFlag == true)
	{
		//Up����
		mulYPos += g_gameTime->GetFrameDeltaTime() * 1000.0f;
		Y.y += mulYPos;
	}
	else
	{
		//Down����
		mulYPos += g_gameTime->GetFrameDeltaTime() * 1300.0f;
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

void Brave::ProcessNormalAttackStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃R���{�Ɍq���Ȃ��A�܂���3�R���{�ڂȂ烊�Z�b�g
		if (GetNextComboFlagFlag() == false ||
			m_attackPatternState >= enAttackPattern_3)
		{
			m_attackPatternState = enAttackPattern_None;
			//�U���A�j���[�V�������I������̂ŃA�N�V�����t���O��false�ɂ���
			SetIsActionFlag(false);
			//�X�e�[�g���ʂ̏�ԑJ�ڏ����ɑJ��
			ProcessCommonStateTransition();
		}
		//���̃R���{�̍U���X�e�[�g�ݒ�
		else
		{
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

		//�X�e�[�g���ʂ̏�ԑJ�ڏ����ɑJ��
		ProcessCommonStateTransition();
	}
}

void Brave::ProcessDieStateTransition()
{

}



bool Brave::RotationOnly()
{
	//��]�\�ȃA�j���[�V�����Ȃ�
	if (isRotationEntable() == true)
	{
		m_SaveMoveSpeed = calcVelocity(GetStatus());
		return true;
	}

	return false;
}

void Brave::UpdateWeapons()
{
	Matrix swordMt = m_modelRender.GetBone(m_swordBoonId)->GetWorldMatrix();
	Sword.SetWorldMatrix(swordMt);

	Matrix shieldMt = m_modelRender.GetBone(m_shieldBoonId)->GetWorldMatrix();
	Shield.SetWorldMatrix(shieldMt);

	Sword.Update();
	Shield.Update();
}

void Brave::InitModel()
{
	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/Player/NewHero/Idle.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Sprint].Load("Assets/animData/character/Player/NewHero/Sprint.tka");
	m_animationClip[enAnimClip_Sprint].SetLoopFlag(true);
	m_animationClip[enAnimClip_DashForward].Load("Assets/animData/character/Player/NewHero/DashForward.tka");
	m_animationClip[enAnimClip_DashForward].SetLoopFlag(false);
	m_animationClip[enAnimClip_KnockBack].Load("Assets/animData/character/Player/NewHero/KnockBack.tka");
	m_animationClip[enAnimClip_KnockBack].SetLoopFlag(false);
	m_animationClip[enAnimClip_Hit].Load("Assets/animData/character/Player/NewHero/Hit.tka");
	m_animationClip[enAnimClip_Hit].SetLoopFlag(false);
	m_animationClip[enAnimClip_Defend].Load("Assets/animData/character/Player/NewHero/Defend.tka");
	m_animationClip[enAnimClip_Defend].SetLoopFlag(true);
	m_animationClip[enAnimClip_DefendHit].Load("Assets/animData/character/Player/NewHero/DefendHit.tka");
	m_animationClip[enAnimClip_DefendHit].SetLoopFlag(false);
	m_animationClip[enAnimClip_Die].Load("Assets/animData/character/Player/NewHero/Die.tka");
	m_animationClip[enAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enAnimClip_ChangeSwordShield].Load("Assets/animData/character/Player/NewHero/ChangeSwordShield.tka");
	m_animationClip[enAnimClip_ChangeSwordShield].SetLoopFlag(false);
	m_animationClip[enAnimClip_Win_Start].Load("Assets/animData/character/Player/NewHero/Win_start.tka");
	m_animationClip[enAnimClip_Win_Start].SetLoopFlag(false);
	m_animationClip[enAnimClip_Win_Main].Load("Assets/animData/character/Player/NewHero/Win_main.tka");
	m_animationClip[enAnimClip_Win_Main].SetLoopFlag(false);
	m_animationClip[enAnimClip_attack1].Load("Assets/animData/character/Player/NewHero/Attack_1.tka");
	m_animationClip[enAnimClip_attack1].SetLoopFlag(false);
	m_animationClip[enAnimClip_attack2].Load("Assets/animData/character/Player/NewHero/Attack_2.tka");
	m_animationClip[enAnimClip_attack2].SetLoopFlag(false);
	m_animationClip[enAnimClip_attack3].Load("Assets/animData/character/Player/NewHero/Attack_3.tka");
	m_animationClip[enAnimClip_attack3].SetLoopFlag(false);
	m_animationClip[enAnimClip_Skill_Start].Load("Assets/animData/character/Player/NewHero/Attack_4.tka");
	m_animationClip[enAnimClip_Skill_Start].SetLoopFlag(false);
	m_animationClip[enAnimClip_Skill_Main].Load("Assets/animData/character/Player/NewHero/Attack_5.tka");
	m_animationClip[enAnimClip_Skill_Main].SetLoopFlag(false);

	m_modelRender.Init("Assets/modelData/character/Player/NewHero/Hero_Smile.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);
	/*m_position = { 0.0f,0.0f,200.0f };
	m_modelRender.SetPosition(m_position);*/
	m_modelRender.Update();

	m_charaCenterBoonId = m_modelRender.FindBoneID(L"root");

	Sword.Init("Assets/modelData/character/Player/NewHero/OneHandSword.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ);
	m_swordBoonId = m_modelRender.FindBoneID(L"weaponShield_r");

	Shield.Init("Assets/modelData/character/Player/NewHero/Shield.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ);
	m_shieldBoonId = m_modelRender.FindBoneID(L"weaponShield_l");


	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
}

void Brave::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	Sword.Draw(rc);
	Shield.Draw(rc);
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
	//�O�i����U���̎n�܂�
	if (wcscmp(eventName, L"DashAttackStart") == 0)
	{
		SetDashAttackFlag(true);
	}

	//�O�i����U���̏I���
	if (wcscmp(eventName, L"DashAttackEnd") == 0)
	{
		SetDashAttackFlag(false);
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

