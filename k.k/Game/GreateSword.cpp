#include "stdafx.h"
#include "GreateSword.h"
#include "Brave.h"
#include "PlayerMovement.h"
#include "PlayerController.h"
#include "DamageProvider.h"

#include "KnockBackInfoManager.h"

#include "UseEffect.h"

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

void GreateSword::DeleteThis()
{
	DeleteGO(this);
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
		(0, g_collisionObjectManager->m_playerAttackCollisionName
		);
	//�R���W�����̐���҂��v���C���[�ɐݒ�
	m_swordCollision->SetCreatorName(m_brave->GetName());
	//�R���W�����̌`��̓{�b�N�X
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
	auto skillCollision = NewGO<CollisionObject>(0, g_collisionObjectManager->m_playerAttackCollisionName);
	//�R���W�����̐���҂��v���C���[�ɐݒ�
	skillCollision->SetCreatorName(m_brave->GetName());
	//�R���W�����̌`��͋�
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

	m_defensiveActionDirection.y = 0.0f;

	//��𑬓x��������
	m_defensiveActionDirection *= m_uniqueStatus.GetDefenciveMoveSpeed();

	//��𒆂͖��G
	m_brave->EnableInvincible();
}

void GreateSword::UpdateDefensiveActionProcess()
{
	//��𒆂̈ړ��\�t���O�������Ă�����
	if (IsDefensiveActionMove())
	{
		//��𒆂̈ړ�����
		m_brave->CharaConExecute(m_defensiveActionDirection);
	}

	//����A�N�V�������I���Ȃ�
	if (IsEndDefensiveAction())
	{
		//�X�e�[�g�̋��ʏ���
		m_brave->ProcessCommonStateTransition();
	}

}

void GreateSword::ExitDefensiveActionProcess()
{
	//�X�^�~�i�������
	m_brave->GetStatus().TryConsumeStamina(m_status.GetDefensiveStaminaCost());
	//���G������
	m_brave->DisableInvincible();
}

bool GreateSword::CanDefensiveAction()
{
	//����ɕK�v�ȃX�^�~�i������ł��邩�`�F�b�N
	if (m_brave->GetStatus().CheckConsumeStamina(m_status.GetDefensiveStaminaCost()))
	{
		//����\
		return true;
	}

	//�X�^�~�i���s�����Ă���̂Ńt���O�𗧂Ă�
	m_brave->SetStaminaInsufficientFlag(true);
	//�s�\
	return false;
}

bool GreateSword::CanSkillAttack()
{
	//�X�L���ɕK�v�ȃX�^�~�i������ł��邩�`�F�b�N
	if (m_brave->GetStatus().CheckConsumeStamina(m_status.GetSkillStaminaCost()))
	{
		//�X�L���U���\
		return true;
	}

	//�X�^�~�i���s�����Ă���̂Ńt���O�𗧂Ă�
	m_brave->SetStaminaInsufficientFlag(true);
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

	//�R���{�X�e�[�g��ԍ��ɕϊ�����
	int comboNum = ConvertComboStateToNumber(comboState);

	//����X�e�[�^�X����U���X�s�[�h���擾���ĕ����ɂ�����
	m_normalAttackMoveDirection *= m_uniqueStatus.GetNormalAttackSpeed(comboNum);

	//�ʏ�U���ҋ@��ԃt���O�����Z�b�g
	SetStandbyPeriodFlag(false);
	//�L�����Z���A�N�V�����t���O�𗧂Ă�B(�L�����Z���A�N�V�����ł���)
	m_isImpossibleancelAction = true;

	//�_���[�W����ݒ�
	m_brave->GetDamageProvider()->SetDamageInfo(
		KnockBackInfoManager::GetInstance()->GetAddAttackId(), m_brave->GetCurrentPower(),
		m_uniqueStatus.GetAttackTimeScale(comboNum),
		m_status.GetComboKnockBackPattern(static_cast<WeaponStatus::EnCombo>(comboNum)),
		m_status.GetWeaponAttribute()
	);

}

void GreateSword::UpdateNormalAttackProcess(EnComboState comboState)
{
	//�L�����Z���A�N�V�����ł����ԂȂ������\
	//�O�R���{�ڂ͉��s�\
	if (m_isImpossibleancelAction && 
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
	//�ꉞ�����蔻��𖳌������Ă���
	m_swordCollision->SetIsEnable(false);
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

void GreateSword::AttackImpactProcess(bool startOrEnd)
{
	//�L�����Z���A�N�V�����t���O��ݒ�
	m_isImpossibleancelAction = startOrEnd;

	//�����蔻��̗L�����A�������̐ݒ�
	m_swordCollision->SetIsEnable(startOrEnd);
}

void GreateSword::EntrySkillStartProcess()
{
	//�؋󎞊ԃ^�C�}�[�����Z�b�g
	m_skillFlightTimer = 0.0f;
	//�X�L�����ړ��ł��邩�t���O�����Z�b�g
	isMoveSkillAttack = false;
	//�X�L�����C���X�e�[�g�ɐi�ނ��̃t���O�����Z�b�g
	m_brave->SetProceedSkillMainFlag(false);
}

void GreateSword::UpdateSkillStartProcess()
{
	//�������Ԃ��o�����烁�C���X�e�[�g�ɑJ��
	if (IsSkillFlightTimeOver())
	{
		//�A�j���[�V�������I�������
		if (m_brave->GetModelRender().IsPlayingAnimation() == false)
		{
			//�X�L�����C���X�e�[�g�ɐi�ނ�
			m_brave->SetProceedSkillMainFlag(true);
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
	//�_���[�W����ݒ�
	m_brave->GetDamageProvider()->SetDamageInfo(
		KnockBackInfoManager::GetInstance()->GetAddAttackId(), m_brave->GetCurrentPower(),
		m_uniqueStatus.SkillAttackTimeScale(),
		m_status.GetSkillKnockBackPattern(),
		m_status.GetWeaponAttribute()
	);

	//���C���ɐi�񂾂̂Ŗ��G�ɂ���
	m_brave->EnableInvincible();
}

void GreateSword::UpdateSkillMainProcess()
{
}

void GreateSword::ExitSkillMainProcess()
{
	//�X�^�~�i�������
	m_brave->GetStatus().TryConsumeStamina(m_status.GetSkillStaminaCost());
	//���G�𖳌�������
	m_brave->DisableInvincible();
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

		//�󒆂ɕ����Ă���͖��G�ɂ���
		m_brave->EnableInvincible();

		//�G�t�F�N�g�≹�̍Đ�
		UseEffect* effect = NewGO<UseEffect>(0, "SkillRisingEffect");
		effect->PlayEffect(enEffect_GreatSwordSkillRising,
			m_brave->GetPosition(), g_vec3One * 11.0f, Quaternion::Identity, false);
		//���Đ�
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_GreatSwordSkillRising,
			g_soundManager->GetSEVolume()
		);
	}

	//�X�L���U���̃_���[�W����o���A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"GreatSwordSkillAttack") == 0)
	{
		//�����蔻��쐬
		CreateSkillAttackCollision();


		Vector3 hitPosition = g_vec3Zero;
		m_swordCenterMatrix.Apply(hitPosition);
		hitPosition.y = 0.0f;

		//�G�t�F�N�g�≹�̍Đ�
		UseEffect* effect = NewGO<UseEffect>(0, "SkillAttackEffect");
		effect->PlayEffect(enEffect_GreatSwordSkillAttack,
			hitPosition, g_vec3One * 10.0f, Quaternion::Identity, false);
		//���Đ�
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_GreatSwordSkillAttack,
			g_soundManager->GetSEVolume()
		);
	}


	//�ʏ�U���P�̃A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"GreatSwordPlayCombo1Effect") == 0)
	{
		Vector3 pos = g_vec3Zero;
		Quaternion rot = Quaternion::Identity;
		Vector3 forward = m_brave->GetForward();
		forward.y = 0;
		rot.SetRotationYFromDirectionXZ(forward);
		rot.AddRotationDegZ(-280.0f);

		m_swordMatrix.Apply(pos);


		UseEffect* effect = NewGO<UseEffect>(0, "SrashEffect");
		effect->PlayEffect(enEffect_GreatSwordCombo1,
			pos, g_vec3One * 11.0f, rot, false);
	}

	//�ʏ�U���Q�̃A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"GreatSwordPlayCombo2Effect") == 0)
	{
		Vector3 pos = g_vec3Zero;
		Quaternion rot = Quaternion::Identity;
		Vector3 forward = m_brave->GetForward();
		forward.y = 0;
		rot.SetRotationYFromDirectionXZ(forward);
		rot.AddRotationDegZ(80.0f);

		m_swordCenterMatrix.Apply(pos);


		UseEffect* effect = NewGO<UseEffect>(0, "SrashEffect");
		effect->PlayEffect(enEffect_GreatSwordCombo2,
			pos, g_vec3One * 11.0f, rot, false);
	}

	//�ʏ�U���R�̃A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"GreatSwordPlayCombo3Effect") == 0)
	{
		Vector3 pos = g_vec3Zero;
		Quaternion rot = Quaternion::Identity;
		Vector3 forward = m_brave->GetForward();
		forward.y = 0;
		rot.SetRotationYFromDirectionXZ(forward);
		//rot.AddRotationDegZ(230.0f);

		m_swordCenterMatrix.Apply(pos);


		UseEffect* effect = NewGO<UseEffect>(0, "SrashEffect");
		effect->PlayEffect(enEffect_GreatSwordCombo3,
			pos, g_vec3One * 18.0f, rot, false);
	}

}
