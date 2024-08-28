#include "stdafx.h"
#include "GreateSword.h"
#include "Brave.h"
#include "PlayerMovement.h"
#include "PlayerController.h"

namespace {


	const Vector3 SWORD_COLLISION_SIZE = { 100.0f,30.0f,10.0f };

	const float SKILL_ATTACK_RADIUS = 100.0f;		//�X�L���U���p�R���W�����̔��a
	
}

GreateSword::GreateSword()
{
}

GreateSword::~GreateSword()
{
	DeleteGO(m_swordCollision);
}

bool GreateSword::Start()
{
	m_brave = FindGO<Brave>("Brave");

	m_playerController = m_brave->GetComponent<PlayerController>();
	m_playerMovement = m_brave->GetComponent<PlayerMovement>();

	//���ʃX�e�[�^�X��������
	m_status.InitWeaponCommonStatus("GreateSword");
	//�ŗL�X�e�[�^�X��������
	m_uniqueStatus.InitUniqueStatus();

	//����������
	Init();

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_brave->GetModelRender().AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
	});

	return true;
}

void GreateSword::Update()
{
	if (m_enWeaponState == enArmed)
	{
		MoveArmed();
	}


	m_swordModelRender.Update();

	m_swordCollision->Update();

}

void GreateSword::Init()
{
	//�����f���̓ǂݍ���
	m_swordModelRender.Init("Assets/modelData/character/Player/NewHero/GreateSword.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0, 0, enModelUpAxisZ);
	//���̒��S�̃{�[��ID���擾
	m_swordCenterBoonId = m_swordModelRender.FindBoneID(L"Center");

	//���Ə��̍��W�ɑΉ�����{�[��ID���擾
	//������Ԃ̎��̃{�[��ID
	m_armedSwordBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_l");



	InitCollision();

	//���킪���[��ԂȂ�
	if (m_enWeaponState == enStowed)
	{
		ChangeStowedState();
	}
	//���킪������ԂȂ�
	else
	{
		ChangeArmedState();
	}

}

void GreateSword::ChangeStowedState()
{
	SetCurrentWeaponState(enStowed);
	m_swordModelRender.SetPosition(m_stowedPosition);

	if (m_swordCollision != nullptr) m_swordCollision->SetPosition(m_stowedPosition);
}

void GreateSword::ChangeArmedState()
{
	SetCurrentWeaponState(enArmed);
}

void GreateSword::InitCollision()
{
	//���̓����蔻��
	m_swordCollision =
		NewGO<CollisionObject>
		(0, g_collisionObjectManager->m_attackCollisionName
		);

	m_swordCollision->CreateBox(
		m_stowedPosition,
		Quaternion::Identity,
		SWORD_COLLISION_SIZE
	);

	//�����蔻��������ō폜���Ȃ��悤�ɂ���
	m_swordCollision->SetIsEnableAutoDelete(false);
	//�����蔻��𖳌���
	m_swordCollision->SetIsEnable(false);

}

void GreateSword::CreateSkillAttackCollision()
{
	//�X�L���U���p�R���W�����쐬
	
	Vector3 hitPosition = g_vec3Zero;

	m_swordCenterMatrix.Apply(hitPosition);
	hitPosition.y = 0.0f;

	//�X�L���U�����̓����蔻��̐���
	//�����ŏ�����
	auto skillCollision = NewGO<CollisionObject>(0, g_collisionObjectManager->m_attackCollisionName);
	skillCollision->CreateSphere(
		hitPosition,
		g_quatIdentity,
		SKILL_ATTACK_RADIUS
	);
}

void GreateSword::AttackAction()
{
}

void GreateSword::ProceedComboAttack()
{
	//�R�R���{�ȏ�Ȃ�
	if (m_enComboState >= enCombo_Third)
	{
		//�R���{�X�e�[�g���Ȃ��Ƀ��Z�b�g����
		m_enComboState = enCombo_None;
	}

	//�R���{����i�߂�
	m_enComboState = static_cast<EnComboState>(m_enComboState + 1);
}

void GreateSword::ResetComboAttack()
{
	//�R���{�X�e�[�g���Ȃ��Ƀ��Z�b�g����
	m_enComboState = enCombo_None;
}

bool GreateSword::IsEndDefensiveAction()
{
	//�A�j���[�V�������I�������
	if (m_brave->GetModelRender().IsPlayingAnimation() == false)
	{
		//����A�N�V�������I���
		return true;
	}

	return false;
}

void GreateSword::EntryDefensiveActionProcess()
{
	//������Ɉړ�������������߂�
	m_defensiveActionDirection = m_playerMovement->CalcMoveDirection(
		m_brave->GetForward(),
		m_playerController->GetLStickInput(),
		m_brave->GetMoveSpeed()
	);

	//�v���C���[�̉�]�����Ɉړ�������ݒ肷��
	m_brave->SetRotateDirection(m_defensiveActionDirection);
	m_brave->SetForward(m_defensiveActionDirection);

	//��𑬓x��������
	m_defensiveActionDirection.x *= m_uniqueStatus.GetDefenciveMoveSpeed();
	m_defensiveActionDirection.z *= m_uniqueStatus.GetDefenciveMoveSpeed();
}

void GreateSword::UpdateDefensiveActionProcess()
{
	//��𒆂̈ړ��\�t���O�������Ă�����
	if (IsDefensiveActionMove())
	{
		//��𒆂̈ړ�����
		m_brave->CharaConExecute(m_defensiveActionDirection);
	}
}

bool GreateSword::CanDefensiveAction()
{
	//����ɕK�v�ȃX�^�~�i������ł���Ȃ�
	if (m_brave->GetStatus().TryConsumeStamina(m_status.GetDefensiveStaminaCost()))
	{
		//����\
		return true;
	}
	//�s�\
	return false;
}

bool GreateSword::CanSkillAttack()
{
	//�X�L���ɕK�v�ȃX�^�~�i������ł���Ȃ�
	if (m_brave->GetStatus().TryConsumeStamina(m_status.GetSkillStaminaCost()))
	{
		//�X�L���U���\
		return true;
	}
	//�s�\
	return false;
}

void GreateSword::EntryNormalAttackProcess(EnComboState comboState)
{
	//�ړ�������O���������͕������v�Z����
	m_normalAttackMoveDirection =
		m_playerMovement->CalcMoveDirection(
			m_brave->GetForward(),
			m_playerController->GetLStickInput(),
			m_brave->GetMoveSpeed()
		);

	//�G�̈ʒu�����܂��Č������������߂�
	m_normalAttackMoveDirection = CalcAutoAimAtTarget(
		m_brave->GetPosition(),
		m_normalAttackMoveDirection,
		m_uniqueStatus.GetNormalAttackSearchRadius(),
		m_uniqueStatus.GetNormalAttackComparisonDot()
	);

	//�v���C���[�̉�]�����Ɉړ�������ݒ肷��
	m_brave->SetRotateDirection(m_normalAttackMoveDirection);
	m_brave->SetForward(m_normalAttackMoveDirection);

	int comboNum = 0;
	switch (comboState)
	{
	case WeaponBase::enCombo_First:
		comboNum = 0;
		break;
	case WeaponBase::enCombo_Second:
		comboNum = 1;
		break;
	case WeaponBase::enCombo_Third:
		comboNum = 2;
		break;
	default:
		std::abort();
		break;
	}

	//����X�e�[�^�X����U���X�s�[�h���擾���ĕ����ɂ�����
	m_normalAttackMoveDirection *= m_uniqueStatus.GetNormalAttackSpeed(comboNum);

	//�ʏ�U���ҋ@��ԃt���O�����Z�b�g
	SetStandbyPeriodFlag(false);
	//�L�����Z���A�N�V�����t���O�𗧂Ă�B(�L�����Z���A�N�V�����ł���)
	m_isPossibleCancelAction = true;

}

void GreateSword::UpdateNormalAttackProcess(EnComboState comboState)
{
	//�L�����Z���A�N�V�����ł����ԂȂ������\
	//�O�R���{�ڂ͉��s�\
	if (m_isPossibleCancelAction && 
		IsStandbyPeriod() &&
		m_playerController->IsPressDefensiveActionButton()&&
		comboState!=enCombo_Third)
	{
		//�h��X�e�[�g�ɐ؂�ւ���
		m_brave->ChangeBraveState(enBraveState_DefensiveActions);
		return;
	}

	//�ړ��t���O�������Ă���Ԃ͈ړ�
	if (IsAttackActionMove())
	{
		m_brave->CharaConExecute(m_normalAttackMoveDirection);
	}
}

void GreateSword::ExitNormalAttackProcess(EnComboState comboState)
{
	//�U�����̈ړ��t���O�����Z�b�g
	SetAttackActionMove(false);
	//�ʏ�U���ҋ@��ԃt���O�����Z�b�g
	SetStandbyPeriodFlag(false);
}

void GreateSword::EntrySkillAttackProcess(EnSkillProcessState skillProcessState)
{
	switch (skillProcessState)
	{
	case WeaponBase::enStart:
		EntrySkillStartProcess();
		break;
	case WeaponBase::enMain:
		EntrySkillMainProcess();
		break;
	default:
		break;
	}
}

void GreateSword::UpdateSkillAttackProcess(EnSkillProcessState skillProcessState)
{
	switch (skillProcessState)
	{
	case WeaponBase::enStart:
		UpdateSkillStartProcess();
		break;
	case WeaponBase::enMain:
		UpdateSkillMainProcess();
		break;
	default:
		break;
	}
}

void GreateSword::ExitSkillAttackProcess(EnSkillProcessState skillProcessState)
{
	switch (skillProcessState)
	{
	case WeaponBase::enStart:
		ExitSkillStartProcess();
		break;
	case WeaponBase::enMain:
		ExitSkillMainProcess();
		break;
	default:
		break;
	}
}

void GreateSword::EntrySkillStartProcess()
{
	//�؋󎞊ԃ^�C�}�[�����Z�b�g
	m_skillFlightTimer = 0.0f;
	//�X�L�����ړ��ł��邩�t���O�����Z�b�g
	isMoveSkillAttack = false;
}

void GreateSword::UpdateSkillStartProcess()
{
	//�������Ԃ��o�����烁�C���X�e�[�g�ɑJ��
	if (IsSkillFlightTimeOver())
	{
		//�A�j���[�V�������I�������
		if (m_brave->GetModelRender().IsPlayingAnimation() == false)
		{
			//���C���X�e�[�g�ɑJ��
			m_brave->ChangeBraveState(BraveState::enBraveState_SkillMain);
		}
	}
	else
	{
		//�ړ��ł���^�C�~���O�łȂ��Ȃ珈�����Ȃ�
		if (!isMoveSkillAttack) return;

		//�ړ��ł���
		Vector3 moveSpeed = m_playerMovement->CalcSimpleMovementVerocity(
			100.0f,
			m_brave->GetMoveSpeed(),
			m_playerController->GetLStickInput()
		);
		//�L�����R���ňړ�
		m_brave->CharaConExecute(moveSpeed);
		//��]������ݒ肷��
		m_brave->SetRotateDirection(moveSpeed);
	}
}

void GreateSword::ExitSkillStartProcess()
{
	//�X�L�����ړ��ł��邩�t���O�����Z�b�g
	isMoveSkillAttack = false;
}

void GreateSword::EntrySkillMainProcess()
{
}

void GreateSword::UpdateSkillMainProcess()
{
}

void GreateSword::ExitSkillMainProcess()
{
}

bool GreateSword::IsSkillFlightTimeOver()
{
	if (m_skillFlightTimer >= m_uniqueStatus.GetSkillFlightTimeLimit())
	{
		return true;
	}
	//�^�C�}�[�����Z
	m_skillFlightTimer += g_gameTime->GetFrameDeltaTime();
	return false;
}


void GreateSword::MoveArmed()
{
	Vector3 swordPos = g_vec3Zero;
	//���̃��[���h���W��ݒ�
	m_swordMatrix =
		m_brave->GetModelRender().GetBone(m_armedSwordBoonId)->GetWorldMatrix();
	m_swordModelRender.SetWorldMatrix(m_swordMatrix);


	//�R���W�����̃��[���h���W��ݒ�
	//���̒��S�̃{�[�����烏�[���h���W���擾���ăR���W�����̃��[���h���W�ɐݒ�
	m_swordCenterMatrix = m_swordModelRender.GetBone(m_swordCenterBoonId)->GetWorldMatrix();
	m_swordCollision->SetWorldMatrix(m_swordCenterMatrix);

}

void GreateSword::Render(RenderContext& rc)
{
	m_swordModelRender.Draw(rc);
}

void GreateSword::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�X�L���U���̃L�����N�^�[�㏸�A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"GreatSwordSkillRising") == 0)
	{
		//���������ړ��ł���
		isMoveSkillAttack = true;

		//�G�t�F�N�g�≹�̍Đ�

	}

	//�X�L���U���̃_���[�W����o���A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"GreatSwordSkillAttack") == 0)
	{
		//�����蔻��쐬
		CreateSkillAttackCollision();

	}

}