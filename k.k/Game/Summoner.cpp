#include "stdafx.h"
#include "Summoner.h"
#include "Brave.h"
#include "EnemyManager.h"

#include "SummonerAIController.h"

#include "KnockBackInfoManager.h"

#include "GameSceneManager.h"



namespace {
	float MUL_SCALE = 4.0f;

	const Vector3 START_POSITION = { 0.0f,0.0f,400.0f };
	const Vector3 START_FORWARD = g_vec3Back;

}

Summoner::~Summoner()
{
}

bool Summoner::Start()
{
	//�{�X�͎���ł��Ȃ��I
	GameSceneManager::GetInstance()->ResetBossDelete();

	m_player = FindGO<Brave>("Brave");

	//���f���̓ǂݍ���
	InitModel();

	//�L�����R��������
	if (m_charaCon == nullptr)
	{
		CreateCharacterController();
		m_charaCon.get()->Init(90.0f, 160.0f, m_position);
	}
	else
	{
		m_charaCon.get()->SetPosition(m_position);
	}


	//�X�e�[�^�X��������
	m_status.InitCommonEnemyStatus("Summoner");
	//�Z�Ɋւ���X�e�[�^�X��������
	m_skillStatus.InitSkillStatus();

	//�{�X�G�l�~�[�̃C���X�^���X��ݒ�
	EnemyManager::GetInstance()->SetBossEnemyInstance(this);

	//�R���|�[�l���g�����������Ă��Ȃ��Ȃ�
	if (!m_isSettingComponents)
	{
		InitComponents();
	}


	if (m_stateContext == nullptr)
	{
		m_stateContext = std::make_unique<SummonerStateContext>();
		m_stateContext.get()->Init(this,enSummonerState_Idle);
	}



	//�C���X�^���X���Ȃ��Ȃ珉����
	if (m_StateMachineCotext == nullptr)
	{
		m_StateMachineCotext = std::make_unique<SummonerSMContext>();
		m_StateMachineCotext.get()->Init(this, enSubStateMachine_General);
	}





	return true;
}

void Summoner::ChangeState(EnSummonerState changeState)
{
	m_stateContext.get()->ChangeState(changeState);
}

bool Summoner::CheckAndTransitionStateMachine()
{
	return m_StateMachineCotext.get()->CheckAndTransitionStateMachine();
}

void Summoner::ProcessCommonTranstion()
{
	if (fabsf(GetMoveSpeed().x) >= 0.001f ||
		fabsf(GetMoveSpeed().z) >= 0.001f)
	{
		m_stateContext.get()->ChangeState(enSummonerState_Run);
	}
	else
	{
		m_stateContext.get()->ChangeState(enSummonerState_Idle);
	}
}

float Summoner::GetDistanceToPlayerPositionValue()
{
	return CalcDistanceToTargetPosition(m_player->GetPosition());
}

void Summoner::UpdateComboAttackProcess()
{
	//�����蔻��𐶐�����Ȃ�
	if (m_isCreateCollision)
	{
		m_aiController->CreateComboAttackCollision();
	}


	if (m_isAttackMove)
	{
		AttackChaseMovement(m_player->GetPosition(),300.0f);
	}
}

void Summoner::UpdateAttackTurnToTargetProcess()
{
	//�t���O�������Ă���Ԃ͉�]��������
	if (m_isAttackMove)
	{
		TurnToTarget();
	}
}

void Summoner::UpdateShockWaveProcess()
{
	if (m_isAttackMove)
	{
		AttackChaseMovement(m_player->GetPosition(), 200.0f);
	}
}

void Summoner::AfterDieProcess()
{
	//�{�X�������Ɠ����Ƀt�B�[���h��̃��u�G�l�~�[�����S������
	EnemyManager::GetInstance()->DeleteAllEnemy();


	//�{�X�����ꂽ
	GameSceneManager::GetInstance()->BossDelete();

	//���g���폜
	DeleteGO(this);
}

void Summoner::DieFlomOutside()
{
	//�{�X�������Ɠ����Ƀt�B�[���h��̃��u�G�l�~�[�����S������
	EnemyManager::GetInstance()->DeleteAllEnemy();

	//���g���폜
	DeleteGO(this);
}

void Summoner::ProcessHit(DamageInfo damageInfo)
{
	//�_���[�W���󂯂�
	TakeDamage(damageInfo.attackPower);

	//�_���[�W���󂯂Ď��S�����Ȃ�
	if (IsDie())
	{
		//���S�A�j���[�V�����X�e�[�g�ɑJ��
		m_stateContext.get()->ChangeState(enSummonerState_Die);
	}

}

bool Summoner::IgnoreCollision()
{
	//���G���Ȃ珈�����Ȃ�
	if (IsInvincible()) return true;

	return false;
}

void Summoner::ReleaseThis()
{
}

void Summoner::AddMoreComponent()
{
}

void Summoner::InitComponents()
{
	//��{�I�ȃR���|�[�l���g��ݒ�
	SettingDefaultComponent();


	AddComponent<SummonerAIController>();
	m_aiController = GetComponent<SummonerAIController>();

}

void Summoner::Rotation()
{
	//��]�������Ȃ��Ȃ珈�����Ȃ�
	if (fabsf(m_rotateDirection.x) < 0.001f && fabsf(m_rotateDirection.z) < 0.001f) {
		return;
	}

	//���`��Ԃŉ�]����������v�Z�B���X�ɉ�]������
	m_currentRotDirection = Math::Lerp(
		g_gameTime->GetFrameDeltaTime() * 3.0f, m_currentRotDirection, m_rotateDirection
	);
	
	//��]
	m_rotation.SetRotationYFromDirectionXZ(m_currentRotDirection);

	//�O��������]�����ɍ��킹��
	m_forward = m_currentRotDirection;
}


void Summoner::Update()
{

	//�ړ�����
	ChaseMovement(m_player->GetPosition());

	//��]����
	Rotation();

	//�����蔻��
	CheckSelfCollision();


	//�T�u�X�e�[�g�}�V���̍X�V����
	m_StateMachineCotext.get()->CurrentSubStateMachineUpdate();
	//�X�e�[�g�̍X�V����
	m_stateContext.get()->UpdateCurrentState();
	//�A�j���[�V����
	m_stateContext.get()->PlayAnimationCurrentState();
	



	//���f���̍X�V
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void Summoner::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

void Summoner::InitModel()
{
	//���f�������������Ă��Ȃ��Ȃ�
	if (!m_modelRender.IsInit())
	{
		//�A�j���[�V�����̓ǂݍ���
		LoadAnimationClip();

		//���f����������
		m_modelRender.Init(
			"Assets/modelData/character/Lich/Lich_real_Selllook.tkm",
			L"Assets/shader/ToonTextrue/lamp_Lich.DDS",
			m_animationClip,
			enSummonerAnimClip_Num,
			enModelUpAxisZ
		);

		//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
		m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
			OnAnimationEvent(clipName, eventName);
		});

	}

	//�g�嗦��ύX
	m_scale *= MUL_SCALE;

	m_position = START_POSITION;

	m_forward = START_FORWARD;

	m_rotateDirection = m_forward;

	//�g�����X�t�H�[���̐ݒ�
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);

	//��̃{�[��ID���擾
	m_staffBoonId = m_modelRender.FindBoneID(L"Staff");
	//�E��̃{�[��ID���擾
	m_rightHandBoonId = m_modelRender.FindBoneID(L"Index_Proximal_R");
	//����̃{�[��ID���擾
	m_leftHandBoonId = m_modelRender.FindBoneID(L"Hand_L");

	
}

void Summoner::LoadAnimationClip()
{
	//�e��A�j���[�V�����N���b�v�̃��[�h
	m_animationClip[enSummonerAnimClip_Idle].Load("Assets/animData/character/Lich/Idle.tka");
	m_animationClip[enSummonerAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enSummonerAnimClip_Run].Load("Assets/animData/character/Lich/Walk.tka");
	m_animationClip[enSummonerAnimClip_Run].SetLoopFlag(true);
	m_animationClip[enSummonerAnimClip_DarkBall].Load("Assets/animData/character/Lich/Attack1.tka");
	m_animationClip[enSummonerAnimClip_DarkBall].SetLoopFlag(false);
	m_animationClip[enSummonerAnimClip_DarkWall].Load("Assets/animData/character/Lich/Attack2.tka");
	m_animationClip[enSummonerAnimClip_DarkWall].SetLoopFlag(false);
	m_animationClip[enSummonerAnimClip_ShockWave].Load("Assets/animData/character/Lich/ShockWave.tka");
	m_animationClip[enSummonerAnimClip_ShockWave].SetLoopFlag(false);
	m_animationClip[enSummonerAnimClip_KnockBackAttack].Load("Assets/animData/character/Lich/KnockBack.tka");
	m_animationClip[enSummonerAnimClip_KnockBackAttack].SetLoopFlag(false);
	m_animationClip[enSummonerAnimClip_FirstAttack].Load("Assets/animData/character/Lich/NormalAttack_1.tka");
	m_animationClip[enSummonerAnimClip_FirstAttack].SetLoopFlag(false);
	m_animationClip[enSummonerAnimClip_SecondAttack].Load("Assets/animData/character/Lich/NormalAttack_2.tka");
	m_animationClip[enSummonerAnimClip_SecondAttack].SetLoopFlag(false);
	m_animationClip[enSummonerAnimClip_ThirdAttack].Load("Assets/animData/character/Lich/NormalAttack_3.tka");
	m_animationClip[enSummonerAnimClip_ThirdAttack].SetLoopFlag(false);
	m_animationClip[enSummonerState_Attack_DarkMeteorite].Load("Assets/animData/character/Lich/DarkMeteorite.tka");
	m_animationClip[enSummonerState_Attack_DarkMeteorite].SetLoopFlag(false);
	m_animationClip[enSummonerAnimClip_Summon].Load("Assets/animData/character/Lich/Summon.tka");
	m_animationClip[enSummonerAnimClip_Summon].SetLoopFlag(false);
	m_animationClip[enSummonerAnimClip_Victory].Load("Assets/animData/character/Lich/Victory.tka");
	m_animationClip[enSummonerAnimClip_Victory].SetLoopFlag(true);
	m_animationClip[enSummonerAnimClip_Angry].Load("Assets/animData/character/Lich/Angry.tka");
	m_animationClip[enSummonerAnimClip_Angry].SetLoopFlag(false);
	m_animationClip[enSummonerAnimClip_Die].Load("Assets/animData/character/Lich/Die.tka");
	m_animationClip[enSummonerAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enSummonerAnimClip_Hit].Load("Assets/animData/character/Lich/Damage.tka");
	m_animationClip[enSummonerAnimClip_Hit].SetLoopFlag(false);
	m_animationClip[enSummonerAnimClip_Warp].Load("Assets/animData/character/Lich/Warp.tka");
	m_animationClip[enSummonerAnimClip_Warp].SetLoopFlag(false);

}


void Summoner::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�����蔻�萶���J�n�A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"StartCreateCollision") == 0)
	{
		m_isCreateCollision = true;
	}
	//�����蔻�萶���I���A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"EndCreateCollision") == 0)
	{
		m_isCreateCollision = false;
	}

	//�ړ��J�n�A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"StartAttackMove") == 0)
	{
		m_isAttackMove = true;
	}
	//�ړ��I���A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"EndAttackMove") == 0)
	{
		m_isAttackMove = false;
	}
	//�O�R���{�ڂ̔����A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"ThirdComboBurst") == 0)
	{
		m_aiController->CreateThirdComboCollision();
	}
	

	
	//�Ռ��g�U���A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"ShockWave") == 0)
	{
		m_aiController->CreateShockWaveCollision();
	}

	//�_�[�N�{�[�������A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"CreateDarkBall") == 0)
	{
		m_aiController->CreateDarkBall();
	}
	

	//�_�[�N���e�I�����ʒu����A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"DecisionShotPoint") == 0)
	{
		m_aiController->DecisionDarkMeteoriteFallPoint();
	}
	//�_�[�N���e�I�����A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"ShotDarkMeteo") == 0)
	{
		m_aiController->CreateDarkMeteorite();
	}


	//���[�v�ʒu����A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"DecisionWarpPoint") == 0)
	{
		m_aiController->DecisionWarpPoint();
	}
	//���[�v�X�^�[�g�A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"WarpStart") == 0)
	{
		m_aiController->WarpProcess();
	}



}

void Summoner::SetShockWaveDamageInfo()
{
	SetDamageInfo(
		KnockBackInfoManager::GetInstance()->GetAddAttackId(),
		40, 3.0f, enKBPattern_FullAirborneRetreat);
}

void Summoner::SettingDamageInfo(EnSkillSType skillType)
{
	SetDamageInfo(
		KnockBackInfoManager::GetInstance()->GetAddAttackId(),
		m_skillStatus.GetSkillPower(skillType),
		m_skillStatus.GetSkillKnockBackTimeScale(skillType),
		m_skillStatus.GetSkillKnockBackPattern(skillType)
	);
}


