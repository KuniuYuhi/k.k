#include "stdafx.h"
#include "Bow.h"
#include "Brave.h"
#include "PlayerMovement.h"
#include "PlayerController.h"

#include "Arrow.h"

#include "DamageProvider.h"
#include "KnockBackInfoManager.h"

namespace {
	


}

Bow::Bow()
{
}

Bow::~Bow()
{
	if (m_arrow != nullptr)
	{
		DeleteGO(m_arrow);
	}

}

bool Bow::Start()
{
	m_brave = FindGO<Brave>("Brave");
	//�v���C���[�R���g���[���[�R���|�[�l���g���擾
	m_playerController = m_brave->GetComponent<PlayerController>();

	m_playerMovement = m_brave->GetComponent<PlayerMovement>();
	//���ʃX�e�[�^�X�̏�����
	m_status.InitWeaponCommonStatus("BowArrow");
	//�ŗL�X�e�[�^�X�̏�����
	m_uniqueStatus.InitUniqueStatus();

	//����������
	Init();

	

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

	//��𐶐�
	CreateArrow(m_enWeaponState);

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_brave->GetModelRender().AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
	});

	return true;
}

void Bow::Update()
{
	if(m_enWeaponState==enArmed)
	{
		MoveArmed();
	}



	m_bowModelRender.Update();

}

void Bow::Init()
{
	//�|���f���̏�����
	m_bowModelRender.Init("Assets/modelData/character/Player/NewHero/Bow.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,0,enModelUpAxisZ
	);

	//�|�Ɩ�̍��W�ɑΉ�����{�[��ID���擾
	//������Ԃ̎��̃{�[��ID
	m_armedBowBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_l");
	m_armedArrowBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_r");

}

void Bow::ChangeStowedState()
{
	SetCurrentWeaponState(enStowed);
	//���[���̍��W�ɕύX
	m_bowModelRender.SetPosition(m_stowedPosition);
	//������[��ԂɕύX
	if (m_arrow == nullptr) return;
	m_arrow->ChangeStowed();
}

void Bow::ChangeArmedState()
{
	SetCurrentWeaponState(enArmed);
	//���������ԂɕύX
	if (m_arrow == nullptr) return;
	m_arrow->ChangeArmed();
}

void Bow::AttackAction()
{
}

void Bow::ProceedComboAttack()
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

void Bow::ResetComboAttack()
{
	//�R���{�X�e�[�g���Ȃ��Ƀ��Z�b�g����
	m_enComboState = enCombo_None;
}

bool Bow::IsEndDefensiveAction()
{

	//�A�j���[�V�������I�������
	if (m_brave->GetModelRender().IsPlayingAnimation() == false)
	{
		//����A�N�V�������I���
		return true;
	}

	return false;
}

void Bow::EntryDefensiveActionProcess()
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

void Bow::UpdateDefensiveActionProcess()
{
	//��𒆂̈ړ��\�t���O�������Ă�����
	if (IsDefensiveActionMove())
	{
		m_brave->CharaConExecute(m_defensiveActionDirection);
	}	

	//����A�N�V�������I���Ȃ�
	if (IsEndDefensiveAction())
	{
		//�X�e�[�g�̋��ʏ���
		m_brave->ProcessCommonStateTransition();
	}
}

void Bow::ExitDefensiveActionProcess()
{
	//���G������
	m_brave->DisableInvincible();
}

bool Bow::CanDefensiveAction()
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

bool Bow::CanSkillAttack()
{
	//�X�L���ɕK�v�ȃX�^�~�i������ł���Ȃ�
	//�`���[�W����^�C�v�Ȃ̂ōU��������ɏ����
	if (m_brave->GetStatus().CheckConsumeStamina(m_status.GetSkillStaminaCost()))
	{
		//�X�L���U���\
		return true;
	}
	//�s�\
	return false;
}

void Bow::EntrySkillAttackProcess(EnSkillProcessState skillProcessState)
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

void Bow::UpdateSkillAttackProcess(EnSkillProcessState skillProcessState)
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

void Bow::ExitSkillAttackProcess(EnSkillProcessState skillProcessState)
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

void Bow::EntryNormalAttackProcess(EnComboState comboState)
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
	m_comboNumber = ConvertComboStateToNumber(comboState);

	//����X�e�[�^�X����U���X�s�[�h���擾���ĕ����ɂ�����
	m_normalAttackMoveDirection *= m_uniqueStatus.GetNormalAttackSpeed(m_comboNumber);

	//�ʏ�U���ҋ@��ԃt���O�����Z�b�g
	SetStandbyPeriodFlag(false);
	//�L�����Z���A�N�V�����t���O�𗧂Ă�B(�L�����Z���A�N�V�����ł���)
	m_isImpossibleancelAction = true;

	
}

void Bow::UpdateNormalAttackProcess(EnComboState comboState)
{
	//�L�����Z���A�N�V�����ł����ԂȂ������\
	if (m_isImpossibleancelAction && 
		IsStandbyPeriod() &&
		m_playerController->IsPressDefensiveActionButton())
	{
		//�h��X�e�[�g�ɐ؂�ւ���
		m_brave->ChangeBraveState(enBraveState_DefensiveActions);
		return;
	}

	//�ړ��t���O�������Ă���Ԃ͈ړ�
	if (IsAttackActionMove())
	{
		//���������Ƃ��̔����̈ړ�
		m_brave->CharaConExecute(m_normalAttackMoveDirection);
	}
}

void Bow::ExitNormalAttackProcess(EnComboState comboState)
{
	//�U�����̈ړ��t���O�����Z�b�g
	SetAttackActionMove(false);
	//�ʏ�U���ҋ@��ԃt���O�����Z�b�g
	SetStandbyPeriodFlag(false);

	if (m_arrow == nullptr)
	{
		//��𐶐�
		CreateArrow(m_enWeaponState);
	}
}

void Bow::EntrySkillStartProcess()
{
	m_skillChargeTimer = 0.0f;
	//�`���[�W�i�K�����Z�b�g
	m_uniqueStatus.SetSkillChargeStage(BowArrowStatus::enStage_1);
	//�X�L���U���\���t���O�����Z�b�g
	m_ispossibleSkillAttack = false;
}

void Bow::UpdateSkillStartProcess()
{
	//�{�^���������Ă���Ԃ̓`���[�W����
	if (m_playerController->IsPressSkillAttackButton())
	{
		//�`���[�W���̏���������
		SkillChargeTimeProcess();

		
		//�ړ��͂ł��Ȃ����ǉ�]�����ł���悤�ɂ���
		Vector3 moveSpeed = m_playerMovement->CalcSimpleMovementVerocity(
			100.0f,
			m_brave->GetMoveSpeed(),
			m_playerController->GetLStickInput()
		);
		//��]������ݒ肷��
		m_brave->SetRotateDirection(moveSpeed);

	}
	else
	{
		//��i�K�ȏ�`���[�W���Ă�����
		if (m_uniqueStatus.GetCurrentSkillChargeStage() >= BowArrowStatus::enStage_2)
		{
			//���C���X�e�[�g�ɑJ��
			m_brave->ChangeBraveState(BraveState::enBraveState_SkillMain);
			//�X�L���U���ł���̂Ńt���O�𗧂Ă�
			m_ispossibleSkillAttack = true;
			return;
		}

		//�X�L�����I���(�U���͕s��)
		m_brave->ProcessCommonStateTransition();
		//�X�L���U���ł��Ȃ��̂Ńt���O��������
		m_ispossibleSkillAttack = false;
	}
}

void Bow::ExitSkillStartProcess()
{
	//�X�L���U�����Ȃ������ꍇ��
	if (!m_ispossibleSkillAttack)
	{
		//�A�N�V�������I���
		m_brave->ActionDeactive();
	}

}

void Bow::EntrySkillMainProcess()
{
	//�U�����邱�Ɗm��Ȃ̂ŃX�^�~�i�������
	m_brave->GetStatus().TryConsumeStamina(m_status.GetSkillStaminaCost());

	//���C���ɐi�񂾂̂Ŗ��G�ɂ���
	m_brave->EnableInvincible();
}

void Bow::UpdateSkillMainProcess()
{
}

void Bow::ExitSkillMainProcess()
{
	//�U�����̈ړ��t���O�����Z�b�g
	SetAttackActionMove(false);

	if (m_arrow == nullptr)
	{
		//��𐶐�
		CreateArrow(m_enWeaponState);
	}

	//���G�𖳌�������
	m_brave->DisableInvincible();
}

void Bow::SkillChargeTimeProcess()
{
	//�`���[�W�i�K���}�b�N�X�Ȃ珈�����Ȃ�
	if (m_uniqueStatus.GetCurrentSkillChargeStage() == BowArrowStatus::enStage_max)
	{
		return;
	}

	//�^�C�}�[�����Z
	m_skillChargeTimer += g_gameTime->GetFrameDeltaTime();

	//�`���[�W���ԂɒB������
	if (m_skillChargeTimer >= m_uniqueStatus.GetSkillChargeCompletionTime(m_uniqueStatus.GetCurrentSkillChargeStage()))
	{
		//�`���[�W�G�t�F�N�g����


		//���̒i�K�ɂ���
		m_uniqueStatus.SetSkillChargeStage(
			static_cast<BowArrowStatus::EnSkillChargeStage>(m_uniqueStatus.GetCurrentSkillChargeStage() + 1)
		);
	}

}

void Bow::CreateArrow(EnWeaponState weaponState)
{
	//��̖��O�����߂�B
	//�����蔻������₷�����邽�߂ɖ��O�̌��ID��ݒ�
	const char* name = "Arrow" + m_arrowNameId;

	m_arrow = NewGO<Arrow>(0, name);
	//�|�̃C���X�^���X��ݒ�
	m_arrow->SetBowInstance(this);

	//����|�̏�ԂƓ�����Ԃɂ���
	if (weaponState == enStowed)
	{
		m_arrow->ChangeStowed();
	}
	else
	{
		m_arrow->ChangeArmed();
	}

	//ID�����Z
	m_arrowNameId++;
	//���̐��𒴂����ID�����Z�b�g
	if (m_arrowNameId > 100)
	{
		m_arrowNameId = -1;
	}
}

void Bow::ShotNromalAttackArrow()
{
	if (m_arrow == nullptr) return;
	//�����̃p�����[�^�̐ݒ�
	m_arrow->SetShotArrowParameters(Arrow::enNormalShot, m_brave->GetForward());

	//���������̂Ŗ�̂Ƀ_���[�W����ݒ�
	m_arrow->GetDamageProvider()->SetDamageInfo(
		KnockBackInfoManager::GetInstance()->GetAddAttackId(), m_brave->GetCurrentPower(),
		m_uniqueStatus.GetAttackTimeScale(m_comboNumber),
		m_status.GetComboKnockBackPattern(static_cast<WeaponStatus::EnCombo>(m_comboNumber)),
		m_status.GetWeaponAttribute()
	);

	//���������̂Ŗ�������Ă��Ȃ���Ԃɂ���
	m_arrow = nullptr;
}

void Bow::ShotSkillAttackArrow()
{
	if (m_arrow == nullptr) return;
	//�����̃p�����[�^�̐ݒ�
	m_arrow->SetShotArrowParameters(Arrow::enSkillShot, m_brave->GetForward());

	//���������̂Ŗ�̂Ƀ_���[�W����ݒ�
	m_arrow->GetDamageProvider()->SetDamageInfo(
		KnockBackInfoManager::GetInstance()->GetAddAttackId(), m_brave->GetCurrentPower(),
		m_uniqueStatus.SkillAttackTimeScale(),
		m_status.GetSkillKnockBackPattern(),
		m_status.GetWeaponAttribute()
	);

	//�X�L���U���ɕK�v�ȏ���ݒ�
	m_arrow->SetSkillShotInfo(
		m_brave->GetCurrentPower(),m_uniqueStatus.GetAttackInfoUpdateInterval()
	);

	//���������̂Ŗ�������Ă��Ȃ���Ԃɂ���
	m_arrow = nullptr;
}

void Bow::MoveArmed()
{
	//�|�̃��[���h���W��ݒ�
	m_bowMatrix = m_brave->GetModelRender().GetBone(m_armedBowBoonId)->GetWorldMatrix();
	m_bowModelRender.SetWorldMatrix(m_bowMatrix);

}

void Bow::Render(RenderContext& rc)
{
	m_bowModelRender.Draw(rc);
}

void Bow::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{

	//�ʏ�U���Ŗ����A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"LongRangeAttack") == 0)
	{
		//������
		ShotNromalAttackArrow();
	}

	//�X�L���U���Ŗ����A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"SkillShot") == 0)
	{
		ShotSkillAttackArrow();
	}


}
