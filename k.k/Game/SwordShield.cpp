#include "stdafx.h"
#include "SwordShield.h"
#include "Brave.h"
#include "PlayerController.h"
#include "PlayerMovement.h"


namespace {

	const Vector3 SWORD_COLLISION_SIZE = { 90.0f,20.0f,10.0f };
	const Vector3 SHIELD_COLLISION_SIZE = { 28.0f,50.0f,16.0f };
}

SwordShield::SwordShield()
{
}

SwordShield::~SwordShield()
{
	DeleteGO(m_swordCollision);
	DeleteGO(m_shieldCollision);


}

bool SwordShield::Start()
{

	m_brave = FindGO<Brave>("Brave");
	//�v���C���[�R���g���[���[�R���|�[�l���g���擾
	m_playerController = m_brave->GetComponent<PlayerController>();
	//���ʃX�e�[�^�X��������
	m_status.InitWeaponCommonStatus("SwordShield");
	//�ŗL�X�e�[�^�X�̏�����
	m_uniqueStatus.InitUniqueStatus();
	
	//����������
	Init();

	return true;
}

void SwordShield::Update()
{


	if (m_enWeaponState == enArmed)
	{
		MoveArmed();
	}



	m_swordModelRender.Update();
	m_shieldModelRender.Update();

	m_swordCollision->Update();
	m_shieldCollision->Update();

}


void SwordShield::Init()
{
	//�����f���̓ǂݍ���
	m_swordModelRender.Init("Assets/modelData/character/Player/NewHero/OneHandSword2.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,0,enModelUpAxisZ);
	//���̒��S�̃{�[��ID���擾
	m_swordCenterBoonId = m_swordModelRender.FindBoneID(L"Center");


	//�����f���̓ǂݍ���
	m_shieldModelRender.Init("Assets/modelData/character/Player/NewHero/Shield.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0, 0, enModelUpAxisZ);


	//���Ə��̍��W�ɑΉ�����{�[��ID���擾
	//������Ԃ̎��̃{�[��ID
	m_armedSwordBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_r");
	m_armedShieldBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_l");


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

void SwordShield::ChangeStowedState()
{
	SetCurrentWeaponState(enStowed);

	m_swordModelRender.SetPosition(m_stowedPosition);
	m_shieldModelRender.SetPosition(m_stowedPosition);

	if(m_swordCollision != nullptr) m_swordCollision->SetPosition(m_stowedPosition);

	if (m_shieldCollision != nullptr) m_shieldCollision->SetPosition(m_stowedPosition);

}

void SwordShield::ChangeArmedState()
{
	SetCurrentWeaponState(enArmed);
}

void SwordShield::InitCollision()
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


	//���̓����蔻��
	m_shieldCollision = NewGO<CollisionObject>(0, "defence");
	m_shieldCollision->CreateBox(
		m_stowedPosition,
		Quaternion(0.0f, 90.0f, 180.0f, 1.0f),
		SHIELD_COLLISION_SIZE
	);
	m_shieldCollision->SetIsEnableAutoDelete(false);
	m_shieldCollision->SetIsEnable(false);
}

void SwordShield::AttackAction()
{



}

void SwordShield::ProceedComboAttack()
{
	//�R�R���{�ȏ�Ȃ�
	if(m_enComboState >= enCombo_Third)
	{
		//�R���{�X�e�[�g���Ȃ��Ƀ��Z�b�g����
		m_enComboState = enCombo_None;
	}

	//�R���{����i�߂�
	m_enComboState= static_cast<EnComboState>(m_enComboState + 1);

	
}

void SwordShield::ResetComboAttack()
{
	//�R���{�X�e�[�g���Ȃ��Ƀ��Z�b�g����
	m_enComboState = enCombo_None;
}

bool SwordShield::IsEndDefensiveAction()
{
	//�h��{�^����b������
	if (!m_playerController->IsPressDefensiveActionButton())
	{
		//�h��A�N�V�������I���
		return true;
	}



	return false;
}

void SwordShield::EntryDefensiveActionProcess()
{
	//�G�t�F�N�g����
}

void SwordShield::UpdateDefensiveActionProcess()
{
}

void SwordShield::ExitDefensiveActionProcess()
{
	//�G�t�F�N�g�폜
}

void SwordShield::EntryNormalAttackProcess(EnComboState comboState)
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

void SwordShield::UpdateNormalAttackProcess(EnComboState comboState)
{
	//�ړ��t���O�������Ă���Ԃ͈ړ�
	if (IsAttackActionMove())
	{
		m_brave->CharaConExecute(m_normalAttackMoveDirection);
	}

}

void SwordShield::ExitNormalAttackProcess(EnComboState comboState)
{
	//
	SetAttackActionMove(false);
}

void SwordShield::EntrySkillAttackProcess(EnSkillProcessState skillProcessState)
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

void SwordShield::UpdateSkillAttackProcess(EnSkillProcessState skillProcessState)
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

void SwordShield::ExitSkillAttackProcess(EnSkillProcessState skillProcessState)
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

void SwordShield::EntrySkillStartProcess()
{
}

void SwordShield::UpdateSkillStartProcess()
{
	//�A�j���[�V�������I�������
	if (m_brave->GetModelRender().IsPlayingAnimation() == false)
	{
		//���C���X�e�[�g�ɑJ��
		m_brave->ChangeBraveState(BraveState::enBraveState_SkillMain);
	}
}

void SwordShield::ExitSkillStartProcess()
{
}

void SwordShield::EntrySkillMainProcess()
{
}

void SwordShield::UpdateSkillMainProcess()
{
}

void SwordShield::ExitSkillMainProcess()
{
}

void SwordShield::MoveArmed()
{
	Vector3 swordPos = g_vec3Zero;
	//���̃��[���h���W��ݒ�
	m_swordMatrix =
		m_brave->GetModelRender().GetBone(m_armedSwordBoonId)->GetWorldMatrix();
	m_swordModelRender.SetWorldMatrix(m_swordMatrix);

	//���̃��[���h���W��ݒ�
	m_shieldMatrix =
		m_brave->GetModelRender().GetBone(m_armedShieldBoonId)->GetWorldMatrix();
	m_shieldModelRender.SetWorldMatrix(m_shieldMatrix);

	//�R���W�����̃��[���h���W��ݒ�
	//���̒��S�̃{�[�����烏�[���h���W���擾���ăR���W�����̃��[���h���W�ɐݒ�
	Matrix matrix;
	matrix = m_swordModelRender.GetBone(m_swordCenterBoonId)->GetWorldMatrix();
	m_swordCollision->SetWorldMatrix(matrix);
	//���̃��[���h���W��ݒ�
	m_shieldCollision->SetWorldMatrix(m_shieldMatrix);
}


void SwordShield::Render(RenderContext& rc)
{
	m_swordModelRender.Draw(rc);
	m_shieldModelRender.Draw(rc);

}