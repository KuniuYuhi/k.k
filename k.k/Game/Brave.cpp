#include "stdafx.h"
#include "Brave.h"

////////////////////////////////////////
//�R���|�[�l���g
///////////////////////////////////////
#include "PlayerMovement.h"
#include "PlayerController.h"
#include "DamageProvider.h"
///////////////////////////////////////
#include "WeaponManager.h"

#include "SwordShield.h"
#include "GreateSword.h"
#include "Bow.h"


#include "KnockBackStatus.h"
#include "KnockBackInfoManager.h"

#include "GameSceneManager.h"

#include "EnemyManager.h"



namespace {
	const Vector3 START_POSITION = { 0.0f,0.0f,-600.0f };

	float STARK_TIME_LIMIT = 0.2f;
}

Brave::~Brave()
{
	m_braveAnimClip.release();
	WeaponManager::GetInstance()->DeleteAllWeaponInstance();
	WeaponManager::DeleteInstance();
}

bool Brave::Start()
{
	EnWeaponType firstWeaponType = enSwordShield;

	//�A�j���[�V�����N���b�v���[�h�N���X�𐶐�
	m_braveAnimClip = std::make_unique<LoadBraveAnimationClips>();
	//�A�j���[�V�����N���b�v�����[�h����
	m_braveAnimClip.get()->RoadWeaponsAnimClips();
	//�A�j���[�V�����̍ŏ��̔ԍ���ݒ�
	m_braveAnimClip.get()->SetCurrentWeaponAnimationStartIndexNo(firstWeaponType);

	//���f���ǂݍ���
	InitModel();

	//�L�����R���𐶐�
	CreateCharacterController();
	m_charaCon.get()->Init(17.0f, 40.0f, m_position);

	//�X�e�[�^�X��������
	m_status.InitPlayerStatus("Brave");

	//�X�e�[�g�Ǘ��N���X�𐶐�
	m_braveStateCotext = std::make_unique<BraveStateContext>();
	//������
	m_braveStateCotext.get()->Init(this, enBraveState_Idle);
	

	//�e��R���|�[�l���g�̃Z�b�e�B���O
	SettingDefaultComponent();


	//����Ǘ��}�l�[�W���[�𐶐�
	WeaponManager::CreateInstance();
	//�����������B����̖�����ݒ肷��
	WeaponManager::GetInstance()->Init(enSwordShield,enGreateSword,enBowArrow);
	//�������Ă��镐����擾
	m_armedWeapon = WeaponManager::GetInstance()->GetArmedWeapon();

	//�v���C���[�̍U���͂ɕ���̍U���͂����Z
	m_status.ChangeWeaponCalcCurrentPower(m_armedWeapon->GetWeaponCurrentPower());
	
	return true;
}

void Brave::Update()
{
	//�R���g���[���[�̓��͂��󂯕t���Ă���Ԃ�
	if (!IsStopRequested())
	{
		//�X�^�~�i�̎�����
		AutoRecoveryStamina();

		//�{�^���������čs���A�N�V����
		ButtonAction();

		//�ړ�����
		Movement();

		//��]
		Rotation();

		//�����蔻��
		CheckSelfCollision();
	}
	
	UpdateDamagedInvisibleTimer();

	//���݂̃X�e�[�g�̏���
	m_braveStateCotext.get()->UpdateCurrentState();
	m_braveStateCotext.get()->PlayAnimationCurrentState();


	//���f���̍X�V����
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void Brave::SetCurrentAnimationStartIndexNoForMainWeaponType()
{
	m_braveAnimClip.get()->SetCurrentWeaponAnimationStartIndexNo(
		WeaponManager::GetInstance()->GetMainWeaponType()
	);
}

void Brave::CharaConExecute(Vector3 moveSpeed)
{
	//�L�����R�����g���č��W���ړ�
	m_position = m_charaCon.get()->Execute(moveSpeed, g_gameTime->GetFrameDeltaTime());

}

bool Brave::IsButtonAction()
{
	////�A�N�V�������͑��̏��������Ȃ�
	//if (IsAction() == true)
	//{
	//	return true;
	//}


	//Attack();


	//ChangeWeapon();

	return false;
}

void Brave::ProcessCommonStateTransition()
{
	if (fabsf(GetMoveSpeed().x) >= 0.001f ||
		fabsf(GetMoveSpeed().z) >= 0.001f)
	{
		m_braveStateCotext.get()->ChangeBraveState(enBraveState_Sprint);
	}
	else
	{
		m_braveStateCotext.get()->ChangeBraveState(enBraveState_Idle);
	}
}

void Brave::SettingDefaultComponent()
{
	//�v���C���[�ړ��R���|�[�l���g���擾
	AddComponent<PlayerMovement>();
	m_playerMovement = GetComponent<PlayerMovement>();

	//�R���g���[���[�R���|�[�l���g���擾
	AddComponent<PlayerController>();
	m_playerContoller = GetComponent<PlayerController>();

	AddComponent<DamageProvider>();
	m_damageProvider = GetComponent<DamageProvider>();
}

void Brave::Movement()
{
	//�A�N�V�������͑��̏��������Ȃ�
	if (IsAction() == true)
	{
		return;
	}

	//�R���g���[���[�̓��͂��󂯕t���Ȃ��Ȃ�ړ��������Ȃ�
	if (!m_playerContoller->IsControllerInputEnabledFlag()) return;

	//�ړ��ʂ��擾
	m_moveSpeed = m_playerMovement->CalcSimpleMovementVerocity(
		m_status.GetDefaultSpeed(), m_moveSpeed, m_playerContoller->GetLStickInput()
	);

	//���̃W�����v����
	/*if (m_playerContoller->IsButtonTrigger(enButtonX) && m_charaCon.get()->IsOnGround())
	{
		m_moveSpeed.y = 400.0f;
	}*/

	//�d�͂̌v�Z
	m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();

	//�L�����R�����g���č��W���ړ�
	m_position = m_charaCon.get()->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	if (m_charaCon.get()->IsOnGround()) {
		//�n�ʂɂ����B
		m_moveSpeed.y = 0.0f;
	}
	

	//��]�p�x�N�g���Ɉړ��ʂ�ۑ�
	m_rotateDirection = m_moveSpeed;
}

void Brave::Rotation()
{
	
	if (fabsf(m_rotateDirection.x) < 0.001f
		&& fabsf(m_rotateDirection.z) < 0.001f) {
		//m_moveSpeed.x��m_moveSpeed.z�̐�Βl���Ƃ���0.001�ȉ��Ƃ������Ƃ�
		//���̃t���[���ł̓L�����͈ړ����Ă��Ȃ��̂Ő��񂷂�K�v�͂Ȃ��B
		return;
	}

	m_rotation.SetRotationYFromDirectionXZ(m_rotateDirection);

}

void Brave::ButtonAction()
{
	//�A�N�V�������͑��̏��������Ȃ�
	if (IsAction() == true)
	{
		return;
	}

	//�U������
	AttackAction();

	//����A�h��A�N�V����
	DefensiveAction();

	//����؂�ւ�
	ChangeWeaponAction();

}

void Brave::AttackAction()
{
	//�A�N�V�������͑��̏��������Ȃ�
	if (IsAction() == true)
	{
		return;
	}

	//�ق�̏����d��������
	if (IsStarkTime() == true) return;


	//�ʏ�U���{�^�����������Ȃ�
	if (m_playerContoller->IsTriggerNromalAttackButton())
	{
		//�v���C���[�̍U���͂ɕ���̍U���͂����Z
		m_status.ChangeWeaponCalcCurrentPower(m_armedWeapon->GetWeaponCurrentPower());

		NormalAttackProcess();
		//�A�N�V�������ɂ���
		ActionActive();
		return;
	}

	//�X�L���U���{�^��������������
	//���푤���X�L���U�����s�����ԂȂ�
	if (m_playerContoller->IsTriggerSkillAttackButton() &&
		m_armedWeapon->CanSkillAttack())
	{
		//�v���C���[�̍U���͂ɕ���̃X�L���U���͂����Z
		m_status.ChangeWeaponCalcCurrentPower(m_armedWeapon->GetWeaponSkillPower());

		m_braveStateCotext.get()->ChangeBraveState(enBraveState_SkillStart);
		//�A�N�V�������ɂ���
		ActionActive();
	}


}

void Brave::DefensiveAction()
{
	//�A�N�V�������͑��̏��������Ȃ�
	if (IsAction() == true)
	{
		return;
	}
	//����A�h��{�^��������������
	//���푤������A�h��A�N�V�������s�����ԂȂ�
	if (m_playerContoller->IsTriggerDefensiveActionButton() && 
		m_armedWeapon->CanDefensiveAction())
	{
		//�X�e�[�g��؂�ւ���
		m_braveStateCotext.get()->ChangeBraveState(enBraveState_DefensiveActions);
		//�A�N�V�������ɂ���
		ActionActive();

	}


}

void Brave::NormalAttackProcess()
{
	//�R���{����������
	m_armedWeapon->ProceedComboAttack();
	//�ʏ�U���X�e�[�g�ɐ؂�ւ���B�R���{�ɂ���ĕς��
	m_braveStateCotext.get()->ChangeBraveState(m_armedWeapon->GetCurrentComboState());
}

void Brave::ExitAttackAction()
{
	//�R���{�����̃��Z�b�g
	m_armedWeapon->ResetComboAttack();
	//�A�N�V�������I���
	ActionDeactive();
	//�d���^�C�}�[�����Z�b�g
	m_starkTimer = 0.0f;
}

void Brave::ChangeBraveState(BraveState::EnBraveState nextState)
{
	m_braveStateCotext.get()->ChangeBraveState(nextState);
}

void Brave::EntryHitActionProcess()
{
	//�A�N�V�������ɂ���
	ActionActive();
	//�m�b�N�o�b�N���邽�߂̏���ݒ�
	SettingKnockBackProcess();
	//�U������������Ȃ��̂ŃC���p�N�g���̏��������Z�b�g
	m_armedWeapon->AttackImpactProcess(false);

	count = 0.0f;

	//���G�ɂ���
	EnableInvincible();
}

void Brave::UpdateHitActionProcess()
{
	//�܂��J�[�u�f�[�^�����Ɉړ�
	if (count < m_curvePointList.size())
	{
		//�m�b�N�o�b�N�ɂ��ړ�
		KnockBackMove(count);
		//�^�C���X�P�[�������Z
		count += m_knockBackTimeScale;
	}
	//���ɋ󒆂ɕ����Ă�����n�ʂɍ~���
	else if (m_charaCon.get()->IsJump())
	{
		KnockBackGravityFall();

	}
	//�Ō�ɏ����d�������ċ��ʃX�e�[�g�����Ɉڍs
	else
	{
		//�A�j���[�V�������I�������
		if (GetModelRender().IsPlayingAnimation() == false)
		{
			if (IsDie())
			{
				//���S���Ă���Ƃ��̓X�e�[�g�J��
				m_braveStateCotext.get()->ChangeBraveState(enBraveState_Die);
				return;
			}

			//�����d�����ċ��ʃX�e�[�g�����Ɉڍs
			if (m_starkTimer >= 0.1f)
			{
				//���ʃX�e�[�g�Ɉڍs
				ProcessCommonStateTransition();
			}
			m_starkTimer += g_gameTime->GetFrameDeltaTime();
		}
	}
}

void Brave::ExitHitActionProcess()
{
	//�A�N�V�������I���
	ActionDeactive();

	//�ۑ����Ă����O�������擾
	m_forward = m_saveForward;

	//���t���[���������G���Ԃɂ��邽�߂Ƀ^�C�}�[���X�^�[�g����
	m_isDamagedInvincible = true;

	//���G����
	//DisableInvincible();
}



void Brave::ChangeWeaponAction()
{
	//�A�N�V�������͑��̏��������Ȃ�
	if (IsAction() == true)
	{
		return;
	}

	bool isTrigger = false;

	//�T�u����P�ɐ؂�ւ���
	if (m_playerContoller->IsButtonTrigger(enButtonRB1))
	{
		//�T�u����P��؂�ւ�����ɂ���
		WeaponManager::GetInstance()->ChangeSubWeaponTypeToChangeWeaponType();
		isTrigger = true;
	}
	//�T�u����Q�ɐ؂�ւ���
	if (m_playerContoller->IsButtonTrigger(enButtonLB1))
	{
		//�T�u����Q��؂�ւ�����ɂ���
		WeaponManager::GetInstance()->ChangeSubWeaponType2ToChangeWeaponType();
		isTrigger = true;
	}

	//�{�^���������Ă�����
	if (isTrigger)
	{
		//�؂�ւ��镐��̃A�j���[�V�����̍ŏ��̔ԍ���؂�ւ��Ώە���̔ԍ��ɂ���
		m_braveAnimClip.get()->SetCurrentWeaponAnimationStartIndexNo(
			WeaponManager::GetInstance()->GetChangeTargetWeaponType()
		);
		//�X�e�[�g��؂�ւ���
		m_braveStateCotext.get()->ChangeBraveState(enBraveState_ChangeWeapon);


		//�A�N�V�������ɂ���
		ActionActive();
	}

}

bool Brave::IsStarkTime()
{
	if (m_starkTimer >= STARK_TIME_LIMIT)
	{
		//�d�����Ԃ��I������̂�true
		return false;
	}
	else
	{
		//�^�C�}�[���Z
		m_starkTimer += g_gameTime->GetFrameDeltaTime();
	}
	//�d�����Ȃ̂�true
	return true;
}

void Brave::AutoRecoveryStamina()
{
	//�A�N�V�������͉񕜂��Ȃ�
	//if (IsAction()) return;

	//�X�^�~�i���񕜂���ʂ��v�Z
	float recoveryValue = g_gameTime->GetFrameDeltaTime() * m_status.GetStaminaRecoveryRate();
	//��
	m_status.RecoveryStamina(recoveryValue);
}

void Brave::CheckSelfCollision()
{
	//�����蔻�薳������
	//���G�Ȃ珈�����Ȃ�
	if (IsInvincible()) {
		return; 
	}

	//�G�l�~�[�̍U���R���W�������擾
	const auto& Collisions =
		g_collisionObjectManager->FindCollisionObjects(
			g_collisionObjectManager->m_enemyAttackCollisionName
		);

	int nazenaze = 0;

	//�R���W�����̔z���for���ŉ�
	for (auto collision : Collisions)
	{
		//�����蔻�肪�L���łȂ��Ȃ��΂�
		if (!collision->IsEnable()) continue;

		//���g�̃L�����R���ƏՓ˂�����
		if (collision->IsHit(*m_charaCon) == true)
		{
			//�R���W�����������Ă���L�����̃_���[�W�v���o�C�_�[�R���|�[�l���g���擾
			DamageProvider* dp = FindGOComponent<DamageProvider>(collision->GetCreatorName());

			if (dp == nullptr)
			{
				return;
			}

			//�U�����q�b�g�������Ƃ��R���W�����������Ă���DamageProvider�N���X�ɓ`����
			dp->Hit();

			//�_���[�W�v���o�C�_�[�̍��W���擾
			m_damageProviderPosition = dp->GetProviderPostion();

			//�U��ID���擾
			int currentAttackId = dp->GetAttackId();

			//�O���ID�������Ȃ珈�����Ȃ�
			if (currentAttackId == oldAttackId) return;

			nazenaze++;

			//�_���[�W�������_���ɒ������āA�t�H���g�Ƃ��ĕ\��
			CreateDamageFont(dp->GetAdjustedDamage(),DamageFont::enDamageActor_Player);

			//��_���[�W�������s�������ɏ�������
			ProcessHit(dp->GetProviderDamageInfo());

			//�����ID��O�t���[����ID�ɕۑ�
			oldAttackId = currentAttackId;


			if (nazenaze >= 5)
			{
				nazenaze = 0;
			}

			//todo ���G���Ԃɓ���
			EnableInvincible();
		}
	}

}

void Brave::SettingKnockBackProcess()
{
	m_curvePointList.clear();
	//�m�b�N�o�b�N�J�[�u�̃|�C���g���X�g���擾
	m_curvePointList = KnockBackInfoManager::GetInstance()->GetCurvePoint(
		m_hitKnockBackPattern
	);

	//����̃p�^�[���̃m�b�N�o�b�N�̃X�s�[�h���擾
	m_knockBackSpeed = KnockBackInfoManager::
		GetInstance()->GetStatus().GetKnockBackSpeed(m_hitKnockBackPattern);

	//�O������ύX����O�ɍ��̕�����ۑ����Ă���
	m_saveForward = m_forward;

	//�O�����𐳋K�����ĕ����𔽓]������
	Vector3 diff = m_position - m_damageProviderPosition;
	m_forward = diff;
	m_forward.Normalize();

	//�d���^�C�}�[���Z�b�g
	m_starkTimer = 0.0f;
}

void Brave::KnockBackMove(int listNum)
{
	//���K��
	m_curvePointList[listNum].curvePosition.Normalize();
	//�ړ����x���v�Z
	m_moveSpeed.x = m_forward.x * m_curvePointList[listNum].curvePosition.x * m_knockBackSpeed.x;
	m_moveSpeed.y = m_curvePointList[listNum].curvePosition.y * m_knockBackSpeed.y;
	m_moveSpeed.z = m_forward.z * m_curvePointList[listNum].curvePosition.z * m_knockBackSpeed.x;
	//�L�����R�����g���Ĉړ�
	m_position = m_charaCon.get()->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
}

void Brave::KnockBackGravityFall()
{
	//�d�͂̌v�Z
	m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();
	//�L�����R�����g���č��W���ړ�
	m_position = m_charaCon.get()->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.get()->IsOnGround()) {
		//�n�ʂɂ����B
		m_moveSpeed.y = 0.0f;
	}
}

void Brave::SettingKnockBackInfoForDamageInfo(DamageInfo damageInfo)
{
	//�m�b�N�o�b�N�p�^�[�����擾
	//todo ���x���ɂ���ĕύX
	m_hitKnockBackPattern = damageInfo.knockBackPattern;

	//�m�b�N�o�b�N�̎��ԊԊu���擾
	m_knockBackTimeScale = damageInfo.knockBackTimeScale;
}

void Brave::AfterDieProcess()
{
	//�G�l�~�[�����ɏ������̏�����������
	EnemyManager::GetInstance()->WinProcessAllEnemy();

}

void Brave::GameOutComeProcess()
{
	//�X�e�[�g��؂�ւ���
	m_braveStateCotext.get()->ChangeBraveState(enBraveState_Idle);
	m_moveSpeed = g_vec3Zero;
}

void Brave::ProcessHit(DamageInfo damageInfo)
{
	//�m�b�N�o�b�N�̏���ݒ�
	SettingKnockBackInfoForDamageInfo(damageInfo);

	//�U������������Ȃ��̂ŃC���p�N�g���̏��������Z�b�g
	m_armedWeapon->AttackImpactProcess(false);

	//��_���[�W����
	TakeDamage(damageInfo.attackPower);

	//�X�e�[�g��؂�ւ���
	m_braveStateCotext.get()->ChangeBraveState(enBraveState_Hit);
}

void Brave::TakeDamage(int damage)
{
	//�_���[�W���󂯂�
	//���ꂽ�ꍇ�͎��S�t���O������
	SetDieFlag(m_status.TakeDamage(damage));

	//���ꂽ�ꍇ�͓��͂��󂯕t���Ȃ�
	if (IsDie())
	{
		m_playerContoller->SetControllerInputEnabledFlag(false);
	}
}

bool Brave::IsStopRequested()
{
	//�R���g���[���[�̓��͂��󂯕t���Ȃ��Ȃ�
	if (m_playerContoller->IsControllerInputEnabledFlag()) true;

	//���s����������
	if (GameSceneManager::GetInstance()->IsGameOutcome())return true;

	//���S������
	if (IsDie()) return true;

	return false;
}

void Brave::UpdateDamagedInvisibleTimer()
{
	//�_���[�W��̖��G�t���O�������Ă��Ȃ��Ȃ珈�����Ȃ�
	if (!m_isDamagedInvincible) return;
	//�������ԂɒB������
	if (m_damagedInvisibleTimer >= m_status.GetDamagedInvisibleTimeLimit())
	{
		m_damagedInvisibleTimer = 0.0f;
		//��_���[�W�㖳�G�t���O�����Z�b�g
		m_isDamagedInvincible = false;

		//if(m_braveStateCotext.get())
		//���G������
		DisableInvincible();

		//���f���͐�Ε\��
		m_isViewModel = true;

		return;
	}
	//�^�C�}�[�����Z
	m_damagedInvisibleTimer += g_gameTime->GetFrameDeltaTime();


	int a = m_damagedInvisibleTimer * 10.0f;

	if (a % 2 == 0)
	{
		m_isViewModel = !m_isViewModel;
	}

}


void Brave::Render(RenderContext& rc)
{
	if (!m_isViewModel) return;


	m_modelRender.Draw(rc);
}

void Brave::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{

	//�������ւ��^�C�~���O
	if (wcscmp(eventName, L"ArmedSwordShield") == 0)
	{
		//���펩�̂ƕ���^�C�v�����ւ���
		WeaponManager::GetInstance()->ChangeArmedWeapon(
			WeaponManager::GetInstance()->GetChangeTargetWeaponType()
		);
		//����ւ���̌��݂̕�����C���X�^���X���擾
		m_armedWeapon = WeaponManager::GetInstance()->GetArmedWeapon();

		//����̃X�e�[�^�X(�U���͂Ȃ�)�����g�̃X�e�[�^�X�ɉ��Z
		m_status.ChangeWeaponCalcCurrentPower(m_armedWeapon->GetWeaponCurrentPower());

		g_soundManager->InitAndPlaySoundSource(
			enSoundName_ChangeWeapon,
			g_soundManager->GetSEVolume()
		);
	}


	//�O�i����n�܂�
	if (wcscmp(eventName, L"MoveForwardStart") == 0)
	{
		m_armedWeapon->SetAttackActionMove(true);
	}
	//�O�i����I���
	if (wcscmp(eventName, L"MoveForwardEnd") == 0)
	{
		m_armedWeapon->SetAttackActionMove(false);
	}

	//������̈ړ��̎n�܂�
	if (wcscmp(eventName, L"AvoidMoveStart") == 0)
	{
		m_armedWeapon->SetDefensiveActionMove(true);
	}
	//������̈ړ��̏I���
	if (wcscmp(eventName, L"AvoidMoveEnd") == 0)
	{
		m_armedWeapon->SetDefensiveActionMove(false);
	}


	//���̍s���ҋ@��Ԏn�܂�
	if (wcscmp(eventName, L"StandbyPeriodStart") == 0)
	{
		//�ҋ@��ԃt���O�𗧂Ă�
		m_armedWeapon->SetStandbyPeriodFlag(true);
	}
	//���̍s���ҋ@��ԏI���
	if (wcscmp(eventName, L"StandbyPeriodEnd") == 0)
	{
		//�ҋ@��ԃt���O�����Z�b�g
		m_armedWeapon->SetStandbyPeriodFlag(false);
	}

	//�U��(�G�ɍU�����͂��Ȃ�)���n�܂����u�ԁB
	if (wcscmp(eventName, L"ImpactStart") == 0)
	{
		//�R���W�����̐����ȂǁA�L�����Z���A�N�V�����̐ݒ�Ȃ�
		m_armedWeapon->AttackImpactProcess(true);
	}
	
	//�U��(�G�ɍU�����͂��Ȃ�)���I������u�ԁB
	if (wcscmp(eventName, L"ImpactEnd") == 0)
	{
		//�R���W�����̐����ȂǁA�L�����Z���A�N�V�����̐ݒ�Ȃ�
		m_armedWeapon->AttackImpactProcess(false);
	}

}

void Brave::InitModel()
{
	//���f��������
	m_modelRender.Init("Assets/modelData/character/Player/NewHero/Hero_Smile_Selllook.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		m_braveAnimClip.get()->GetBraveAnimationClip(),
		m_braveAnimClip.get()->GetNumAnimationClips(),
		enModelUpAxisZ
	);

	m_position = START_POSITION;

	m_modelRender.SetTransform(m_position, m_rotation, m_scale);

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
	});
}

void Brave::ChangePlayerWinState()
{
	//�R���g���[���[�̓��͂��󂯕t���Ȃ��悤�ɂ���
	m_playerContoller->SetControllerInputEnabledFlag(false);
	//�����X�e�[�g�ɑJ��
	m_braveStateCotext.get()->ChangeBraveState(enBraveState_WinStart);
}

int Brave::GetCurrentPower()
{
	return m_status.GetCurrentPower();
}
