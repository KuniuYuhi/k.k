#include "stdafx.h"
#include "Bow.h"
#include "Brave.h"
#include "PlayerMovement.h"
#include "PlayerController.h"

namespace {
	


}

Bow::Bow()
{
}

Bow::~Bow()
{
}

bool Bow::Start()
{
	m_brave = FindGO<Brave>("Brave");
	//�v���C���[�R���g���[���[�R���|�[�l���g���擾
	m_playerController = m_brave->GetComponent<PlayerController>();
	//���ʃX�e�[�^�X�̏�����
	m_status.InitWeaponCommonStatus("BowArrow");
	//�ŗL�X�e�[�^�X�̏�����
	m_uniqueStatus.InitUniqueStatus();

	//����������
	Init();

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

void Bow::ChangeStowedState()
{
	SetCurrentWeaponState(enStowed);
	//���[���̍��W�ɕύX
	m_bowModelRender.SetPosition(m_stowedPosition);

}

void Bow::ChangeArmedState()
{
	SetCurrentWeaponState(enArmed);
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
	m_defensiveActionDirection = m_brave->
		GetPlayerMovement()->CalcForwardDirection(
			m_brave->GetForward(),
			m_brave->GetMoveSpeed()
		);

	//
	m_defensiveActionDirection.x *= m_uniqueStatus.GetDefenciveMoveSpeed();
	m_defensiveActionDirection.z *= m_uniqueStatus.GetDefenciveMoveSpeed();
}

void Bow::UpdateDefensiveActionProcess()
{
	m_brave->CharaConExecute(m_defensiveActionDirection);
}

void Bow::EntryNormalAttackProcess(EnComboState comboState)
{
	//�ړ�������O���������͕������v�Z����
	m_normalAttackMoveDirection =
		m_brave->GetPlayerMovement()->CalcMoveDirection(
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
}

void Bow::UpdateNormalAttackProcess(EnComboState comboState)
{
	//�ړ��t���O�������Ă���Ԃ͈ړ�
	if (IsAttackActionMove())
	{
		m_brave->CharaConExecute(m_normalAttackMoveDirection);
	}
}

void Bow::ExitNormalAttackProcess(EnComboState comboState)
{
	//
	SetAttackActionMove(false);
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

void Bow::EntrySkillStartProcess()
{
	m_skillChargeTimer = 0.0f;
}

void Bow::UpdateSkillStartProcess()
{
	//�{�^���������Ă���Ԃ̓`���[�W����
	if (m_playerController->IsPressSkillAttackButton())
	{
		//
		SkillChargeTimeProcess();
	}
	else
	{


		//���C���X�e�[�g�ɑJ��
		m_brave->ChangeBraveState(BraveState::enBraveState_SkillMain);
	}
}

void Bow::ExitSkillStartProcess()
{
}

void Bow::EntrySkillMainProcess()
{
}

void Bow::UpdateSkillMainProcess()
{
}

void Bow::ExitSkillMainProcess()
{
}

void Bow::SkillChargeTimeProcess()
{
	//�^�C�}�[�����Z
	m_skillChargeTimer += g_gameTime->GetFrameDeltaTime();

	//��i�K�ڂ̃`���[�W������������

}

void Bow::MoveArmed()
{
	Vector3 swordPos = g_vec3Zero;
	//�|�̃��[���h���W��ݒ�
	m_bowMatrix = m_brave->GetModelRender().GetBone(m_armedBowBoonId)->GetWorldMatrix();
	m_bowModelRender.SetWorldMatrix(m_bowMatrix);

}

void Bow::Render(RenderContext& rc)
{
	m_bowModelRender.Draw(rc);
}
