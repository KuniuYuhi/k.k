#include "stdafx.h"
#include "SwordShield.h"
#include "Brave.h"
#include "PlayerController.h"
#include "PlayerMovement.h"
#include "DamageProvider.h"

#include "KnockBackInfoManager.h"


#include "UseEffect.h"


namespace {

	const Vector3 SWORD_COLLISION_SIZE = { 90.0f,20.0f,10.0f };
	const Vector3 SHIELD_COLLISION_SIZE = { 28.0f,50.0f,16.0f };

	const float SKILL_ATTACK_RADIUS = 100.0f;		//�X�L���U���p�R���W�����̔��a


	const float SHIELD_BARRIER_EFFECT_SCALE = 8.0f;
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

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_brave->GetModelRender().AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
	});

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

void SwordShield::DeleteThis()
{
	DeleteGO(this);
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


	//���̓����蔻��
	m_shieldCollision = NewGO<CollisionObject>(0, "defence");
	//�R���W�����̐���҂��v���C���[�ɐݒ�
	m_shieldCollision->SetCreatorName(m_brave->GetName());
	//�R���W�����̌`��̓{�b�N�X
	m_shieldCollision->CreateSphere(
		m_stowedPosition,
		Quaternion::Identity,
		50.0f
	);
	//�����蔻��������ō폜���Ȃ��悤�ɂ���
	m_shieldCollision->SetIsEnableAutoDelete(false);
	m_shieldCollision->SetIsEnable(false);
}

void SwordShield::CreateSkillAttackCollision()
{
	Vector3 hitPosition = g_vec3Zero;
	//���̃��[���h���W���x�N�g���ɏ�Z
	m_swordCenterMatrix.Apply(hitPosition);
	hitPosition.y = 0.0f;

	//�X�L���U�����̓����蔻��̐���
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
	//�h��{�^���𗣂�����A�܂��̓V�[���h�Ƀq�b�g������
	if (!m_playerController->IsPressDefensiveActionButton()|| m_isHitShield)
	{
		//�h��A�N�V�������I���
		return true;
	}

	return false;
}

void SwordShield::EntryDefensiveActionProcess()
{
	Vector3 position = g_vec3Zero;
	m_shieldMatrix.Apply(position);
	//�G�t�F�N�g����
	m_shieldEffect = NewGO<UseEffect>(0, "ShieldBarrierEffect");
	m_shieldEffect->PlayEffect(enEffect_SwordShieldDefendBarrier,
		position, g_vec3One * SHIELD_BARRIER_EFFECT_SCALE, Quaternion::Identity, false);


	//���̓����蔻����g���̂Ńv���C���[�{�͖̂��G�ɂ���
	m_brave->EnableInvincible();
	//���̓����蔻���L��������
	m_shieldCollision->SetIsEnable(true);
	//�V�[���h�q�b�g�t���O�����Z�b�g
	m_isHitShield = false;
}

void SwordShield::UpdateDefensiveActionProcess()
{
	//���̓����蔻����`�F�b�N
	CheckShieldCollision();

	//����A�h��A�N�V�������I���Ȃ�
	if (IsEndDefensiveAction())
	{
		if (m_isHitShield)
		{
			//�V�[���h�Ƀq�b�g���Ă�����V�[���h�q�b�g�X�e�[�g�ɑJ��
			m_brave->ChangeBraveState(enBraveState_DefensiveHit);
		}
		else
		//�X�e�[�g�̋��ʏ���
		m_brave->ProcessCommonStateTransition();
	}
}

void SwordShield::ExitDefensiveActionProcess()
{
	//�G�t�F�N�g�폜
	if (m_shieldEffect != nullptr)
	{
		m_shieldEffect->Delete();
		m_shieldEffect = nullptr;
	}

	//�V�[���h�Ƀq�b�g�����ɃX�e�[�g���I�������
	if (!m_isHitShield)
	{
		//���G������
		m_brave->DisableInvincible();
	}

	//���̓����蔻��𖳌�������
	m_shieldCollision->SetIsEnable(false);
}

void SwordShield::EntryDefensiveHitProcess()
{
	//���̓����蔻����g���̂Ńv���C���[�{�͖̂��G�ɂ���
	m_brave->EnableInvincible();

}

void SwordShield::UpdateDefensiveHitProcess()
{
}

void SwordShield::ExitDefensiveHitProcess()
{
	////�܂��h��{�^���������Ă�����h��X�e�[�g�ɐ؂�ւ���
	//if (m_playerController->IsPressDefensiveActionButton())
	//{
	//	m_brave->ChangeBraveState(enBraveState_DefensiveActions);
	//	return;
	//}

	//���G������
	m_brave->DisableInvincible();
}

bool SwordShield::CanDefensiveAction()
{
	//���݂̃V�[���h�̑ϋv�l��0���傫�����
	if (m_uniqueStatus.GetCurrentShieldEnduranceValue() > 0)
	{
		//�h��\
		return true;
	}
	//�s�\
	return false;
}

bool SwordShield::CanSkillAttack()
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

void SwordShield::UpdateNormalAttackProcess(EnComboState comboState)
{
	//�L�����Z���A�N�V�����ł����ԂȂ�h����\
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
		m_brave->CharaConExecute(m_normalAttackMoveDirection);
	}

}

void SwordShield::ExitNormalAttackProcess(EnComboState comboState)
{
	//�U�����̈ړ��t���O�����Z�b�g
	SetAttackActionMove(false);
	//�ʏ�U���ҋ@��ԃt���O�����Z�b�g
	SetStandbyPeriodFlag(false);
	//�ꉞ�����蔻��𖳌������Ă���
	m_swordCollision->SetIsEnable(false);
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

void SwordShield::AttackImpactProcess(bool startOrEnd)
{
	//�L�����Z���A�N�V�����t���O��ݒ�
	m_isImpossibleancelAction = startOrEnd;

	//�����蔻��̗L�����A�������̐ݒ�
	m_swordCollision->SetIsEnable(startOrEnd);
}

void SwordShield::EntrySkillStartProcess()
{
	//�X�L�����C���X�e�[�g�ɐi�ނ��̃t���O�����Z�b�g
	m_brave->SetProceedSkillMainFlag(false);
}

void SwordShield::UpdateSkillStartProcess()
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

void SwordShield::ExitSkillStartProcess()
{

}

void SwordShield::EntrySkillMainProcess()
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

void SwordShield::UpdateSkillMainProcess()
{
}

void SwordShield::ExitSkillMainProcess()
{
	//�X�^�~�i�������
	m_brave->GetStatus().TryConsumeStamina(m_status.GetSkillStaminaCost());
	//���G�𖳌�������
	m_brave->DisableInvincible();
}

void SwordShield::CheckShieldCollision()
{
	if (m_isHitShield) return;

	//�G�l�~�[�̍U���R���W�������擾
	const auto& Collisions =
		g_collisionObjectManager->FindCollisionObjects(
			g_collisionObjectManager->m_enemyAttackCollisionName
		);

	//�R���W�����̔z���for���ŉ�
	for (auto collision : Collisions)
	{
		//�����蔻�肪�L���łȂ��Ȃ��΂�
		if (!collision->IsEnable()) continue;

		//���̓����蔻��ƏՓ˂�����
		if (collision->IsHit(m_shieldCollision) == true)
		{
			//�R���W�����������Ă���L�����̃_���[�W�v���o�C�_�[�R���|�[�l���g���擾
			DamageProvider* dp = FindGOComponent<DamageProvider>(collision->GetCreatorName());

			if (dp == nullptr) return;

			dp->Hit();

			//�G�̈ʒu�ۑ�
			m_brave->SetDamageProviderPosition(
				dp->GetProviderPostion()
			);

			//�m�b�N�o�b�N�̏���ݒ�
			m_brave->SettingKnockBackInfoForDamageInfo(dp->GetProviderDamageInfo());

			//���̑ϋv�l���_���[�W�����炷
			m_uniqueStatus.SubShieldEnduranceValue(
				dp->GetAdjustedDamage()
			);

			m_brave->CreateDamageFont(
				dp->GetProviderDamageInfo().attackPower,
				DamageFont::enDamageActor_Player
			);

			//�V�[���h�Ƀq�b�g�����I
			m_isHitShield = true;


			//�_���[�W�����̑ϋv�l��������
			ProcessShieldHit();
		}
	}

}

void SwordShield::ProcessShieldHit()
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
	m_swordCenterMatrix = m_swordModelRender.GetBone(m_swordCenterBoonId)->GetWorldMatrix();
	m_swordCollision->SetWorldMatrix(m_swordCenterMatrix);
	//���̃��[���h���W��ݒ�
	m_shieldCollision->SetWorldMatrix(m_shieldMatrix);
}


void SwordShield::Render(RenderContext& rc)
{
	m_swordModelRender.Draw(rc);
	m_shieldModelRender.Draw(rc);

}

void SwordShield::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{

	//�X�L���U���̃_���[�W����o���A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"SwordShieldSkillAttack") == 0)
	{
		//�X�L���U���p�R���W��������
		CreateSkillAttackCollision();
		//�X�L���U���̏Ռ��G�t�F�N�g�Đ�

		Vector3 hitPosition = g_vec3Zero;
		//���̃��[���h���W���x�N�g���ɏ�Z
		m_swordCenterMatrix.Apply(hitPosition);
		hitPosition.y = 0.0f;

		UseEffect* effect = NewGO<UseEffect>(0, "SkillShockEffect");
		effect->PlayEffect(enEffect_SwordShieldSkillAttack,
			hitPosition, g_vec3One * 17.0f, Quaternion::Identity, false);

		//�X�L���̉��Đ�
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_SwordShieldSkillAttack,
			g_soundManager->GetSEVolume()
		);
	}

	//�X�L���̏㏸�A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"SwordShieldSkillRising") == 0)
	{
		//�G�t�F�N�g�Đ�
		UseEffect* effect = NewGO<UseEffect>(0, "SkillRisingEffect");
		effect->PlayEffect(enEffect_SwordShieldSkillRising,
			m_brave->GetPosition(), g_vec3One * 10.0f, Quaternion::Identity, false);
		//���Đ�
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_SwordShieldSkillRising,
			g_soundManager->GetSEVolume()
		);
	}


	if (wcscmp(eventName, L"PlayCombo1Effect") == 0)
	{
		//�G�t�F�N�g�Đ��̂��߂̍��W�Ɖ�]�ݒ�
		
		Vector3 pos = g_vec3Zero;
		Quaternion rot = Quaternion::Identity;
		Vector3 forward = m_brave->GetForward();
		forward.y = 0;
		rot.SetRotationYFromDirectionXZ(forward);
		rot.AddRotationDegZ(230.0f);

		m_swordCenterMatrix.Apply(pos);
		

		UseEffect* effect = NewGO<UseEffect>(0, "SrashEffect");
		effect->PlayEffect(enEffect_SwordShieldCombo12,
			pos, g_vec3One * 15.0f, rot, false);

	}

	//�ʏ�U���Q�̃A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"PlayCombo2Effect") == 0)
	{
		//�G�t�F�N�g�Đ��̂��߂̍��W�Ɖ�]�ݒ�

		Vector3 pos = g_vec3Zero;
		Quaternion rot = Quaternion::Identity;
		Vector3 forward = m_brave->GetForward();
		forward.y = 0;
		rot.SetRotationYFromDirectionXZ(forward);

		m_swordMatrix.Apply(pos);


		UseEffect* effect = NewGO<UseEffect>(0, "SrashEffect");
		effect->PlayEffect(enEffect_SwordShieldCombo12,
			pos, g_vec3One * 15.0f, rot, false);
	}

	//�ʏ�U���Q�̃A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"PlayCombo3Effect") == 0)
	{
		//�G�t�F�N�g�Đ��̂��߂̍��W�Ɖ�]�ݒ�

		Vector3 pos = g_vec3Zero;
		Quaternion rot = Quaternion::Identity;
		Vector3 forward = m_brave->GetForward();
		forward.y = 0;
		rot.SetRotationYFromDirectionXZ(forward);
		m_swordMatrix.Apply(pos);

		UseEffect* effect = NewGO<UseEffect>(0, "SrashEffect");
		effect->PlayEffect(enEffect_SwordShieldCombo3,
			pos, g_vec3One * 15.0f, rot, false);
		
	}


	//�h��q�b�g�̃A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"SwordShieldDifendHit") == 0)
	{
		//���Đ�
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_SwordShieldDefendHit,
			g_soundManager->GetSEVolume()
		);
	}

}
