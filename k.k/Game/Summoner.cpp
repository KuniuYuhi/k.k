#include "stdafx.h"
#include "Summoner.h"
#include "Brave.h"
#include "EnemyManager.h"

#include "SummonerAIController.h"

#include "KnockBackInfoManager.h"

#include "GameSceneManager.h"

#include "UseEffect.h"

namespace {
	float MUL_SCALE = 4.0f;

	const Vector3 START_POSITION = { 0.0f,0.0f,400.0f };
	const Vector3 START_FORWARD = g_vec3Back;


	const float FIRST_COMBO_EFFECT_SCALE = 20.0f;

	const float SECOND_AND_THIRD_COMBO_EFFECT_SCALE = 15.0f;

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
		m_stateContext.get()->Init(this,enSummonerState_Appear);
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
	//
	if (!m_isViewModel)
	{
		m_dieEffect = FindGO<UseEffect>("SummonerDieEffect");

		if (m_dieEffect == nullptr)
		{
			//�{�X�����ꂽ
			//�J�������v���C���[�Ɉڂ�
			GameSceneManager::GetInstance()->BossDelete();

			//���g���폜
			DeleteGO(this);
			return;
		}

		return;
	}


	//�{�X�������Ɠ����Ƀt�B�[���h��̃��u�G�l�~�[�����S������
	//���u�G�l�~�[�̎��S�G�t�F�N�g���Đ�������
	EnemyManager::GetInstance()->DeleteAllEnemy(true);

	//���f���͕\�������Ȃ�
	m_isViewModel = false;

	//���S�G�t�F�N�g����
	m_dieEffect = NewGO<UseEffect>(0, "SummonerDieEffect");
	m_dieEffect->PlayEffect(enEffect_Mob_Dead,
		m_position, g_vec3One * 10.0f, Quaternion::Identity, false);

	
}

void Summoner::WinProcess()
{
	//�����A�j���[�V�����X�e�[�g�ɑJ��
	m_stateContext.get()->ChangeState(enSummonerState_Victory);
}

void Summoner::DieFlomOutside()
{
	//�{�X�������Ɠ����Ƀt�B�[���h��̃��u�G�l�~�[�����S������
	EnemyManager::GetInstance()->DeleteAllEnemy();

	//���g���폜
	DeleteGO(this);
}

bool Summoner::IsStopRequested()
{
	//���s����������
	if (GameSceneManager::GetInstance()->IsGameOutcome())return true;

	//���S������
	if (IsDie()) return true;

	//�����܂ŗ����珈���͎~�܂�Ȃ�
	return false;
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
	else
	{
		//�q�b�g���Đ�
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_Monster_Hit,
			g_soundManager->GetSEVolume()
		);
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
	//�������~�߂�v�����Ȃ����菈��������
	if (!IsStopRequested())
	{
		//�ړ�����
		ChaseMovement(m_player->GetPosition());

		//��]����
		Rotation();

		//�����蔻��
		CheckSelfCollision();


		//�T�u�X�e�[�g�}�V���̍X�V����
		m_StateMachineCotext.get()->CurrentSubStateMachineUpdate();
	}
	
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
	//���f����\�������Ȃ��Ȃ�
	if (!m_isViewModel) return;

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
	m_animationClip[enSummonerAnimClip_Appear].Load("Assets/animData/character/Lich/FogRemoval.tka");
	m_animationClip[enSummonerAnimClip_Appear].SetLoopFlag(false);
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
	

	
	//�Ռ��g�U���A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"ShockWave") == 0)
	{
		m_aiController->CreateShockWaveCollision();

		//�G�t�F�N�g����
		UseEffect* effect = NewGO<UseEffect>(0, "ShockWaveEffect");
		effect->PlayEffect(enEffect_Boss_KnockBack,
			m_position, g_vec3One * SECOND_AND_THIRD_COMBO_EFFECT_SCALE, Quaternion::Identity, false);

		//���Đ�
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_Boss_ShockWave,
			g_soundManager->GetSEVolume()
		);

		//�J������h�炷
		g_camera3D->StartCameraShake(
			60.0f, 60.0f, 0.25f
		);
	}

	//�_�[�N�{�[�������A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"CreateDarkBall") == 0)
	{
		//���Đ�
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_Boss_DarkBall,
			g_soundManager->GetSEVolume()
		);

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

		Vector3 position = m_aiController->GetWarpPosition();
		position.y = 3.0f;

		//���[�v��ɃG�t�F�N�g����
		UseEffect* warpEffect = NewGO<UseEffect>(0, "WarpEffect");
		warpEffect->PlayEffect(enEffect_Boss_WarpSircle,
			position, g_vec3One * 20.0f, Quaternion::Identity, false);
	}
	//���[�v�X�^�[�g�A�j���[�V�����L�[�t���[��
	if (wcscmp(eventName, L"WarpStart") == 0)
	{
		//���Đ�
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_Boss_Warp,
			g_soundManager->GetSEVolume()
		);

		//�G�t�F�N�g����
		UseEffect* effect = NewGO<UseEffect>(0, "WarpWindEffect");
		effect->PlayEffect(enEffect_Boss_WarpWind,
			m_position, g_vec3One * 30.0f, Quaternion::Identity, false);

		//���W�����[�v��ɐݒ�
		m_aiController->WarpProcess();

		

	}

	//�ʏ�U��1�̃G�t�F�N�g�����C�x���g�L�[�t���[��
	if (wcscmp(eventName, L"PlayNormalAttack1Effect") == 0)
	{
		Vector3 position = g_vec3Zero;
		Matrix m = m_modelRender.GetBone(m_staffBoonId)->GetWorldMatrix();
		m.Apply(position);
		Quaternion rot;
		rot.SetRotationYFromDirectionXZ(m_forward);

		UseEffect* effect = NewGO<UseEffect>(0, "SrashEffect");
		effect->PlayEffect(enEffect_Boss_Combo_1,
			position, g_vec3One * FIRST_COMBO_EFFECT_SCALE, rot, false);

		//���Đ�
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_Boss_Slash,
			g_soundManager->GetSEVolume()
		);
	}

	//�ʏ�U��2�̃G�t�F�N�g�����C�x���g�L�[�t���[��
	if (wcscmp(eventName, L"PlayNormalAttack2Effect") == 0)
	{
		Vector3 position = g_vec3Zero;
		Matrix m = m_modelRender.GetBone(m_leftHandBoonId)->GetWorldMatrix();

		Quaternion rot;
		rot.SetRotationYFromDirectionXZ(m_forward);

		m.Apply(position);
		UseEffect* effect = NewGO<UseEffect>(0, "PokeEffect");
		effect->PlayEffect(enEffect_Boss_Combo_2,
			position, g_vec3One * SECOND_AND_THIRD_COMBO_EFFECT_SCALE, rot, false);

		//���Đ�
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_Boss_Poke,
			g_soundManager->GetSEVolume()
		);
	}

	//�ʏ�U��3�̃G�t�F�N�g�����C�x���g�L�[�t���[��
	if (wcscmp(eventName, L"PlayNormalAttack3Effect") == 0)
	{
		//�����蔻�蔭��
		m_aiController->CreateThirdComboCollision();

		Vector3 position = g_vec3Zero;
		Matrix m = m_modelRender.GetBone(m_rightHandBoonId)->GetWorldMatrix();

		m.Apply(position);
		UseEffect* effect = NewGO<UseEffect>(0, "ComboFinishEffect");
		effect->PlayEffect(enEffect_ComboFinishExplosion,
			position, g_vec3One * SECOND_AND_THIRD_COMBO_EFFECT_SCALE, Quaternion::Identity, false);

		//���Đ�
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_Boss_DarkBall_Explosion,
			g_soundManager->GetSEVolume()
		);
	}


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


