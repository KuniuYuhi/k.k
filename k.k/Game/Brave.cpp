#include "stdafx.h"
#include "Brave.h"

////////////////////////////////////////
//�R���|�[�l���g
///////////////////////////////////////
#include "PlayerMovement.h"
#include "PlayerController.h"



///////////////////////////////////////
//�X�e�[�g�R���e�L�X�g
///////////////////////////////////////
//#include "BraveStateContext.h"


#include "WeaponManager.h"

#include "SwordShield.h"
#include "GreateSword.h"
#include "Bow.h"


namespace {
	float STARK_TIME_LIMIT = 0.2f;
}

Brave::~Brave()
{
	m_braveAnimClip.release();
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

	//���f��������
	m_modelRender.Init("Assets/modelData/character/Player/NewHero/Hero_Smile_Selllook.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		m_braveAnimClip.get()->GetBraveAnimationClip(),
		m_braveAnimClip.get()->GetNumAnimationClips(),
		enModelUpAxisZ
	);

	m_modelRender.SetTransform(m_position, m_rotation, m_scale);

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
	});


	//�L�����R���𐶐�
	CreateCharacterController();
	m_charaCon.get()->Init(20.0f, 50.0f, m_position);

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


	return true;
}

void Brave::Update()
{

	//����̏���


	ButtonAction();

	//�ړ�����
	Movement();


	//��]���Ō�
	Rotation();

	m_braveStateCotext.get()->UpdateCurrentState();
	m_braveStateCotext.get()->PlayAnimationCurrentState();


	//���f���̍X�V����
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void Brave::Damage()
{
}

void Brave::Dead()
{
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
	if (m_playerContoller->IsButtonTrigger(enButtonX) && m_charaCon.get()->IsOnGround())
	{
		m_moveSpeed.y = 400.0f;
	}

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
		NormalAttackProcess();
		//�A�N�V�������ɂ���
		ActionActive();
		return;
	}
	//�X�L���U���{�^�����������Ȃ�
	if (m_playerContoller->IsTriggerSkillAttackButton())
	{
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
	//����A�h��{�^�����������Ȃ�
	if (m_playerContoller->IsTriggerDefensiveActionButton())
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




void Brave::Render(RenderContext& rc)
{
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

		//�؂�ւ�����^�C�v�����C������^�C�v�ɐ؂�ւ���
		//WeaponManager::GetInstance()->ChangeChangeWeaponTypeToMainWeaponType();
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
		
	}
	//������̈ړ��̏I���
	if (wcscmp(eventName, L"AvoidMoveEnd") == 0)
	{
		
	}

}
