#include "stdafx.h"
#include "Summoner.h"

#include "InitEffect.h"
//�}�l�[�W���[
#include "CharactersInfoManager.h"
#include "GameManager.h"
//�X�e�[�g�}�V��
#include "SummonerSM_Attack.h"
#include "SummonerSM_Vigilance.h"
//�e��X�e�[�g
#include "SummonerState_Idle.h"
#include "SummonerState_DarkBall.h"
#include "SummonerState_DarkWall.h"
#include "SummonerState_KnockBack.h"
#include "SummonerState_NAttack_1.h"
#include "SummonerState_NAttack_2.h"
#include "SummonerState_NAttack_3.h"
#include "SummonerState_DMeteo_Start.h"
#include "SummonerState_DMeteo_Main.h"
#include "SummonerState_Angry.h"
#include "SummonerState_Command.h"
#include "SummonerState_Hit.h"
#include "SummonerState_Die.h"
#include "SummonerState_Summon.h"
#include "SummonerState_Walk.h"
#include "SummonerState_Warp.h"
#include "SummonerState_Victory.h"
#include "SummonerState_DarkSpearMain.h"
#include "SummonerState_DarkSpearEnd.h"
#include "SummonerState_DarkSpearStart.h"
//�Z�ł����I�u�W�F�N�g
#include "DarkBall.h"
#include "DarkWall.h"
#include "ComboFinishBomb.h"

#include "GameSceneManager.h"
#include "AllGameSceneState.h"

namespace {
	const float SCALE_UP = 4.0f;		//�L�����N�^�[�̃T�C�Y

	const float MAX_SUPERARMOR_POINT = 180.0f;

	const float KNOCKBACK_RANGE = 300.0f;
	const float KNOCKBACK_POWER = 400.0f;
	const float KNOCKBACK_LIMMIT = 2.0f;

	const float ROT_SPEED = 5.0f;
	const float ROT_ONLY_SPEED = 5.0f;

	const Vector3 NORMAL_ATTACK_COLLISION_SIZE = { 20.0f, 430.0f,100.0f };
	const float NORMAL_ATTACK_1_SIZE = 17.0f;
	const float NORMAL_ATTACK_2_SIZE = 30.0f;
	const Vector3 KNOCKBACK_EFFECT_SIZE = { 30.0f,17.0f,30.0f };

	const float RECOVERY_SUPERARMOR_SPEED = 9.0f;
}

Summoner::Summoner()
{
}

Summoner::~Summoner()
{
	//���񂾂Ƃ��Ƀ��u�����X�^�[�������Ă���̂ŏ����K�v�Ȃ�
	if (GameManager::GetInstance()->GetBossDeleteOkFlag() == true)
	{
		return;
	}

	//���u�����X�^�[���폜
	DeleteMobMonsters(false);
}

bool Summoner::Start()
{
	//�@�������������B
	srand((unsigned)time(NULL));

	//�v���C���[�̃C���X�^���X��T��
	m_player = FindGO<Player>("player");

	//�X�e�[�^�X�̏�����
	m_status.InitCharacterStatus(GetName());

	//���f���̏�����
	InitModel();
	//�X�e�[�g�}�V���̐���
	SetStartStateMachine(enStateMachineState_Vigilance);

	//�ŏ��̃A�j���[�V�����ݒ�
	SetNextAnimationState(enAnimationState_Idle);

	//�v���C���[�̃C���X�^���X�̑��
	m_player = CharactersInfoManager::GetInstance()->GetPlayerInstance();

	return true;
}

void Summoner::Update()
{
	//�|�[�Y��ʂȂ珈�������Ȃ�
	if (GameManager::GetInstance()->GetGameSeenState() ==
		GameManager::enGameSeenState_Pause)
	{
		return;
	}

	if (IsStopProcessing() != true)
	{
		//��]����
		ProcessRotation();
		//�����蔻��̏���
		DamageCollision(m_charaCon);
	}
	
	//�X�[�p�[�A�[�}�[�̉�
	RecoverySuperArmor();

	//�X�e�[�g�}�V���̖��t���[���̏���
	m_stateMachine->Execute();
	//��ԊǗ�
	ManageState();

	//�A�j���[�V����
	PlayAnimation();

	//���f����TRS�Ɛݒ�ƍX�V
	SetTransFormModel(m_modelRender);
	m_modelRender.Update();
}

void Summoner::PlayAnimation()
{
	m_nowBossState->PlayAnimation();
}

void Summoner::ManageState()
{
	m_nowBossState->ManageState();
}

bool Summoner::IsStopProcessing()
{
	if (GameSceneManager::GetInstance()->GetCurrentGameSceneState() != enGameSceneState_Game)
	{
		return true;
	}

	switch (GameSceneManager::GetInstance()->GetBattleOutCome())
	{
	case GameSceneManager::enBattleOutCome_PlayerWin:
		//����A�j���[�V�����X�e�[�g
		SetNextAnimationState(enAnimationState_Die);
		m_modelRender.SetAnimationSpeed(0.7f);
		return true;
	case GameSceneManager::enBattleOutCome_PlayerLose:
		//�����A�j���[�V�����Đ�
		SetNextAnimationState(enAnimationState_Victory);
		return true;

	default:
		break;
	}

	return false;
}

void Summoner::Damage(int attack)
{
	//�_���[�W���󂯂鏈��
	ProcessHit(attack);
	//���ꂽ�Ƃ�
	if (m_status.GetHp() <= 0)
	{
		//�Q�[���}�l�[�W���[�̃v���C���[�̏����t���O��ݒ�
		GameManager::GetInstance()->SetPlayerWinFlag(true);

		//�Q�[���V�[���}�l�[�W���[�Ƀv���C���[�������������Ƃ�`����
		GameSceneManager::GetInstance()->
			SetBattleOutCome(GameSceneManager::enBattleOutCome_PlayerWin);

		//���ꂽ���̏���
		ProcessDead();
		return;
	}
}

void Summoner::HitNormalAttack()
{
	if (GetDamageHitEnableFlag() == true)
	{
		Damage(m_player->GetAtk());
		CreateDamageFont(m_player->GetAtk());
		SetDamageHitEnableFlag(false);
		PlayHitSound();
	}
	return;
}

void Summoner::HitSkillAttack()
{
	//�X�L���U�����󂯂��Ȃ��Ȃ�_���[�W���������Ȃ�(���i�q�b�g�̂���)
	if (m_player->GetHittableFlag() != true)
	{
		return;
	}
	Damage(m_player->GetSkillAtk());
	CreateDamageFont(m_player->GetSkillAtk());
	//���i�q�b�g�����̂Ńt���O�����Z�b�g�B���i�q�b�g�łȂ��Ƃ�
	m_player->SetHittableFlag(false);
}

void Summoner::ProcessDead(bool seFlag)
{
	//�����Ƃ�����������ɂ���
	//�t���[�����[�g�𗎂Ƃ�
	g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Variable, 30);
	m_status.SetHp(0);
	//���ʉ��̍Đ�
	if (seFlag == true)
	{

	}
}

void Summoner::ProcessRotation()
{
	//�v���C���[�Ɍ������Ĉړ�
	//�v���C���[�̍��W���擾
	m_targetPosition = m_player->GetPosition();
	//�ړ�������ݒ�
	m_moveSpeed = CalcVelocity(m_status, m_targetPosition);

	//�O�����̐ݒ�
	m_forward = m_moveSpeed;
	m_forward.Normalize();

	//��]�\�łȂ��Ȃ�
	if (isRotationEnable() == false)
	{
		//��]���Ȃ�
		return;
	}
	//�O�������g���ĉ�]�̐ݒ�
	Rotation(ROT_SPEED, ROT_ONLY_SPEED);
}

void Summoner::ProcessHit(int hitDamage)
{
	//HP�����炷
	m_status.CalcHp(hitDamage, false);
	//��_���[�W�J�E���g���Z
	m_damageCount++;
	//�X�[�p�[�A�[�}�[���u���C�N���Ă��Ȃ��Ȃ�
	if (GetBreakSuperArmorFlag() == false)
	{
		//�X�[�p�[�A�[�}�[�����炷
		CalcSuperArmor(false, hitDamage);
	}
	//�X�[�p�[�A�[�}�[���u���C�N���Ă���Ȃ�
	if (GetBreakSuperArmorFlag() == true)
	{
		//���m���ŋ���
		//�u���C�N�����u�ԂȂ�m��ŋ���
		if (IsFlinch() == true || (m_oldBreakSuperArmorFlag != GetBreakSuperArmorFlag()))
		{
			//�Z�̓r����������Ȃ��̂Ń_�[�N�E�H�[�����폜
			if (m_darkWall != nullptr)
			{
				DeleteGO(m_darkWall);
			}
			//��_���[�W�A�j���[�V����
			SetNextAnimationState(enAnimationState_CriticalHit);
		}
	}
	//�O�t���[���̃X�[�p�[�A�[�}�[�u���C�N�t���O���擾
	m_oldBreakSuperArmorFlag = GetBreakSuperArmorFlag();
}

void Summoner::RecoverySuperArmor()
{
	//�X�[�p�[�A�[�}�[���u���C�N���Ă��Ȃ��Ȃ珈�����Ȃ�
	if (GetBreakSuperArmorFlag() != true)
	{
		return;
	}
	//�X�[�p�[�A�[�}�[�̉�
	CalcSuperArmor(true, g_gameTime->GetFrameDeltaTime() * RECOVERY_SUPERARMOR_SPEED);
}

void Summoner::SetNextAnimationState(EnAnimationState nextState)
{
	//�Â��X�e�[�g���폜����
	if (m_nowBossState != nullptr)
	{
		delete m_nowBossState;
		m_nowBossState = nullptr;
	}
	//�V�����X�e�[�g���쐬
	m_enAnimationState = nextState;

	switch (nextState)
	{
	case Summoner::enAnimationState_Idle://�ҋ@���
		m_nowBossState = new SummonerState_Idle(this);
		break;
	case Summoner::enAnimationState_Walk://�������
		m_nowBossState = new SummonerState_Walk(this);
		break;
	case Summoner::enAnimationState_DarkBall://�_�[�N�{�[��
		m_nowBossState = new SummonerState_DarkBall(this);
		break;
	case Summoner::enAnimationState_DarkWall://�_�[�N�E�H�[��
		m_nowBossState = new SummonerState_DarkWall(this);
		break;
	case Summoner::enAnimationState_DarkSpear_Start://�_�[�N�X�s�A���C��
		m_nowBossState = new SummonerState_DarkSpearStart(this);
		break;
	case Summoner::enAnimationState_DarkSpear_Main://�_�[�N�X�s�A���C��
		m_nowBossState = new SummonerState_DarkSpearMain(this);
		break;
	case Summoner::enAnimationState_DarkSpear_End://�_�[�N�X�s�A�G���h
		m_nowBossState = new SummonerState_DarkSpearEnd(this);
		break;
	case Summoner::enAnimationState_KnockBack://�m�b�N�o�b�N
		m_nowBossState = new SummonerState_KnockBack(this);
		break;
	case Summoner::enAnimationState_NormalAttack_1://�ʏ�U���P
		m_nowBossState = new SummonerState_NAttack_1(this);
		break;
	case Summoner::enAnimationState_NormalAttack_2://�ʏ�U���Q
		m_nowBossState = new SummonerState_NAttack_2(this);
		break;
	case Summoner::enAnimationState_NormalAttack_3://�ʏ�U���R
		m_nowBossState = new SummonerState_NAttack_3(this);
		break;
	case Summoner::enAnimationState_Attack_DarkMeteorite_start://�_�[�N���e�I�X�^�[�g
		m_nowBossState = new SummonerState_DMeteo_Start(this);//�_�[�N���e�I���C��
		break;
	case Summoner::enAnimationState_Attack_DarkMeteorite_main:
		m_nowBossState = new SummonerState_DMeteo_Main(this);
		break;
	case Summoner::enAnimationState_Angry://�{�胂�[�h
		m_nowBossState = new SummonerState_Angry(this);
		break;
	case Summoner::enAninationState_Summon://����
		m_nowBossState = new SummonerState_Summon(this);
		break;
	case Summoner::enAnimationState_Command://���u�ɍ���
		m_nowBossState = new SummonerState_Command(this);
		break;
	case Summoner::enAnimationState_CriticalHit://�X�[�p�[�A�[�}�[����ꂽ��
		m_nowBossState = new SummonerState_Hit(this);
		break;
	case Summoner::enAnimationState_Die://���ꂽ
		m_nowBossState = new SummonerState_Die(this);
		break;
	case Summoner::enAnimationState_Victory://����
		m_nowBossState = new SummonerState_Victory(this);
		break;

	default:
		// �����ɗ�����X�e�[�g�̃C���X�^���X�쐬�����̒ǉ��Y��B
		std::abort();
		break;
	}
	
}

void Summoner::ProcessCommonStateTransition()
{
	SetNextAnimationState(enAnimationState_Idle);
}

void Summoner::InitModel()
{
	//�e��A�j���[�V�����N���b�v�̃��[�h
	m_animationClip[enAnimClip_Idle].Load(
		"Assets/animData/character/Lich/Idle.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Walk].Load(
		"Assets/animData/character/Lich/Walk.tka");
	m_animationClip[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClip[enAnimClip_DarkBall].Load(
		"Assets/animData/character/Lich/Attack1.tka");
	m_animationClip[enAnimClip_DarkBall].SetLoopFlag(false);
	m_animationClip[enAnimClip_DarkWall].Load(
		"Assets/animData/character/Lich/Attack2.tka");
	m_animationClip[enAnimClip_DarkWall].SetLoopFlag(false);
	m_animationClip[enAnimClip_DarkSpear_Start].Load(
		"Assets/animData/character/Lich/DarkSpear_Start.tka");
	m_animationClip[enAnimClip_DarkSpear_Start].SetLoopFlag(false);
	m_animationClip[enAnimClip_DarkSpear_Main].Load(
		"Assets/animData/character/Lich/DarkSpear_Main.tka");
	m_animationClip[enAnimClip_DarkSpear_Main].SetLoopFlag(false);
	m_animationClip[enAnimClip_DarkSpear_End].Load(
		"Assets/animData/character/Lich/DarkSpear_End.tka");
	m_animationClip[enAnimClip_DarkSpear_End].SetLoopFlag(false);
	m_animationClip[enAnimClip_KnockBack].Load(
		"Assets/animData/character/Lich/KnockBack.tka");
	m_animationClip[enAnimClip_KnockBack].SetLoopFlag(false);
	m_animationClip[enAnimClip_NormalAttack_1].Load(
		"Assets/animData/character/Lich/NormalAttack_1.tka");
	m_animationClip[enAnimClip_NormalAttack_1].SetLoopFlag(false);
	m_animationClip[enAnimClip_NormalAttack_2].Load(
		"Assets/animData/character/Lich/NormalAttack_2.tka");
	m_animationClip[enAnimClip_NormalAttack_2].SetLoopFlag(false);
	m_animationClip[enAnimClip_NormalAttack_3].Load(
		"Assets/animData/character/Lich/NormalAttack_3.tka");
	m_animationClip[enAnimClip_NormalAttack_3].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_DarkMeteorite_start].Load(
		"Assets/animData/character/Lich/DarkMeteorite_Start.tka");
	m_animationClip[enAnimClip_Attack_DarkMeteorite_start].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_DarkMeteorite_main].Load(
		"Assets/animData/character/Lich/DarkMeteorite_Main.tka");
	m_animationClip[enAnimClip_Attack_DarkMeteorite_main].SetLoopFlag(false);
	m_animationClip[enAnimClip_Command].Load(
		"Assets/animData/character/Lich/Command.tka");
	m_animationClip[enAnimClip_Command].SetLoopFlag(false);
	m_animationClip[enAnimClip_Summon].Load(
		"Assets/animData/character/Lich/Summon.tka");
	m_animationClip[enAnimClip_Summon].SetLoopFlag(false);
	m_animationClip[enAnimClip_Victory].Load(
		"Assets/animData/character/Lich/Victory.tka");
	m_animationClip[enAnimClip_Victory].SetLoopFlag(true);
	m_animationClip[enAnimClip_Angry].Load(
		"Assets/animData/character/Lich/Angry.tka");
	m_animationClip[enAnimClip_Angry].SetLoopFlag(false);
	m_animationClip[enAnimClip_Die].Load(
		"Assets/animData/character/Lich/Die.tka");
	m_animationClip[enAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enAnimClip_CriticalHit].Load(
		"Assets/animData/character/Lich/Damage.tka");
	m_animationClip[enAnimClip_CriticalHit].SetLoopFlag(false);
	
	m_modelRender.Init("Assets/modelData/character/Lich/Lich_real_Selllook.tkm",
		L"Assets/shader/ToonTextrue/lamp_Lich.DDS",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);

	m_targetPosition = m_player->GetPosition();
	m_moveSpeed= CalcVelocity(m_status, m_targetPosition);
	m_forward = m_moveSpeed;
	m_forward.Normalize();
	//�v���C���[�̕����ɉ�]
	Rotation(ROT_SPEED, ROT_ONLY_SPEED);

	//���f����TRS�̐ݒ�
	m_modelRender.SetTransform(
		m_position,
		m_rotation,
		m_scale *= SCALE_UP
	);
	m_modelRender.Update();

	//�L�����R���̐ݒ�
	m_charaCon.Init(
		70.0f,
		50.0f,
		m_position
	);
	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
	});

	//�_�[�N�E�H�[���Ɏg���{�[��ID�擾
	m_darkWallBoonId = m_modelRender.FindBoneID(L"Staff");
	//�ʏ�U���R�Ŏg���{�[����ID�擾
	m_comboFinishBoonId = m_modelRender.FindBoneID(L"Index_Proximal_R");
	//�n���hL�̃{�[����ID�擾
	m_handLBoonId = m_modelRender.FindBoneID(L"Hand_L");

}

void Summoner::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�m�b�N�o�b�N�^�C�~���O�C�x���g�L�[�t���[��
	if (wcscmp(eventName, L"KnockBack") == 0)
	{
		//�m�b�N�o�b�N
		ProcessKnockBack();
	}
	//�_�[�N�E�H�[�������C�x���g�L�[�t���[��
	if (wcscmp(eventName, L"Create_DarkWall") == 0)
	{
		CreateDarkWall();
	}
	//�_�[�N�E�H�[�������I���C�x���g�L�[�t���[��
	if (wcscmp(eventName, L"CreateEnd_DarkWall") == 0)
	{
		if (m_darkWall != nullptr)
		{
			DeleteGO(m_darkWall);
		}
	}
	//�_�[�N�{�[�������C�x���g�L�[�t���[��
	if (wcscmp(eventName, L"Create_Ball") == 0)
	{
		//�{�[������
		CreateDarkBall();
	}
	//�ʏ�U�������蔻�萶��
	if (wcscmp(eventName, L"Create_NormalAttackCollision") == 0)
	{
		//���̃A�j���[�V�����L�[�t���[���̊ԓ����蔻�萶��
		CreateNormalAttackCollision();
	}
	//�ʏ�U���P�̃C�x���g�L�[�t���[��
	if (wcscmp(eventName, L"PlayNormalAttack1Effect") == 0)
	{
		Vector3 pos = g_vec3Zero;
		Quaternion rot = g_quatIdentity;
		SettingEffectInfo(
			pos, rot, m_modelRender.GetBone(m_handLBoonId)->GetWorldMatrix()
		);
		//�U���G�t�F�N�g�Đ�
		PlayNormalAttack1Effect(pos,rot);
	}
	//�ʏ�U���Q�̃C�x���g�L�[�t���[��
	if (wcscmp(eventName, L"PlayNormalAttack2Effect") == 0)
	{
		Vector3 pos = g_vec3Zero;
		Quaternion rot = g_quatIdentity;
		SettingEffectInfo(
			pos, rot, m_modelRender.GetBone(m_handLBoonId)->GetWorldMatrix()
		);
		//�U���G�t�F�N�g�Đ�
		PlayNormalAttack2Effect(pos, rot);
	}
	//�ʏ�U���̍Ō�̔����C�x���g�L�[�t���[��
	if (wcscmp(eventName, L"Combofinnish") == 0)
	{
		//���̃A�j���[�V�����L�[�t���[���̊ԓ����蔻�萶��
		NormalComboFinnish();
	}
	//�m�b�N�o�b�N�G�t�F�N�g�C�x���g�L�[�t���[��
	if (wcscmp(eventName, L"PlayKnockBackEffect") == 0)
	{
		//�m�b�N�o�b�N�G�t�F�N�g�Đ�
		PlayKnockBackEffect(m_position);
		//���Đ�
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_Boss_KnockBackWind,
			g_soundManager->GetSEVolume()
		);
	}

}

void Summoner::CreateDarkBall()
{
	DarkBall* darkBall = NewGO<DarkBall>(0, "darkball");
	darkBall->SetAttack(GetStatus().GetAtk());
	darkBall->Setting(GetPosition(),GetRotation());
}

void Summoner::CreateDarkWall()
{
	m_darkWall = NewGO<DarkWall>(0, "darkwall");
}

void Summoner::ProcessKnockBack()
{
	//���Ƀv���C���[���m�b�N�o�b�N�������Ȃ�
	if (GetPlayerKnockedBackFlag() == true)
	{
		return;
	}

	//�v���C���[���͈͓��ɂ�����m�b�N�o�b�N����悤�ɂ���
	Vector3 diff = m_position -
		CharactersInfoManager::GetInstance()->GetPlayerInstance()->GetPosition();
	//�m�b�N�o�b�N�͈͓��Ȃ�
	if (diff.Length() < KNOCKBACK_RANGE)
	{
		//�v���C���[���m�b�N�o�b�N�����̂Ńt���O���Z�b�g
		SetPlayerKnockedBackFlag(true);
		//�v���C���[���m�b�N�o�b�N���邽�߂̏���ݒ�
		CharactersInfoManager::GetInstance()->
			GetPlayerInstance()->SetKnockBackInfo(
				true, m_position, KNOCKBACK_POWER, KNOCKBACK_LIMMIT);
	}
}

void Summoner::CreateNormalAttackCollision()
{
	CollisionObject* collision = NewGO<CollisionObject>(0, "monsterattack");
	collision->CreateBox(
		m_position,
		g_quatIdentity,
		NORMAL_ATTACK_COLLISION_SIZE
	);
	collision->SetCreatorName(GetName());
	collision->SetWorldMatrix(m_modelRender.GetBone(m_darkWallBoonId)->GetWorldMatrix());
	collision->Update();
}

void Summoner::NormalComboFinnish()
{
	//�����̒��S���W�̃��[���h���W���擾
	Matrix matrix = m_modelRender.GetBone(m_comboFinishBoonId)->GetWorldMatrix();

	//�����G�t�F�N�g�Ɠ����蔻�萶��
	ComboFinishBomb* comboFinishBomb = 
		NewGO<ComboFinishBomb>(0, "combofinishbomb");
	
	//���[���h���W�����[�J�����W�ɓK��
	comboFinishBomb->ApplyPositionToMatrix(matrix);

}

void Summoner::SettingEffectInfo(
	Vector3& effectPos, Quaternion& rot, Matrix matrix)
{
	effectPos = g_vec3Zero;
	Vector3 forwardPos = GetForward();
	matrix.Apply(effectPos);
	rot = g_quatIdentity;
	rot.SetRotationYFromDirectionXZ(forwardPos);
}

void Summoner::PlayNormalAttack1Effect(Vector3 position, Quaternion rotation)
{
	EffectEmitter* attack1Effect = NewGO<EffectEmitter>(0);
	attack1Effect->Init(InitEffect::enEffect_Boss_Combo_1);
	attack1Effect->Play();
	attack1Effect->SetScale(g_vec3One * NORMAL_ATTACK_1_SIZE);
	attack1Effect->SetPosition(position);
	attack1Effect->SetRotation(rotation);
	attack1Effect->Update();
}

void Summoner::PlayNormalAttack2Effect(Vector3 position, Quaternion rotation)
{
	EffectEmitter* attack1Effect = NewGO<EffectEmitter>(0);
	attack1Effect->Init(InitEffect::enEffect_Boss_Combo_2);
	attack1Effect->Play();
	attack1Effect->SetScale(g_vec3One * NORMAL_ATTACK_2_SIZE);
	attack1Effect->SetPosition(position);
	attack1Effect->SetRotation(rotation);
	attack1Effect->Update();
}

void Summoner::PlayKnockBackEffect(Vector3 position)
{
	EffectEmitter* KnockBackEffect = NewGO<EffectEmitter>(0);
	KnockBackEffect->Init(InitEffect::enEffect_Boss_KnockBack);
	KnockBackEffect->Play();
	KnockBackEffect->SetScale(KNOCKBACK_EFFECT_SIZE);
	KnockBackEffect->SetPosition(position);
	KnockBackEffect->Update();
}

void Summoner::ProcessVigilance()
{
	//�ړ�����
	m_position = m_charaCon.Execute(
		m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	return;
}

void Summoner::SetNextStateMachine(EnStateMachineState nextStateMachine)
{
	//���݂̃X�e�[�g�}�V���̒��g���폜����
	if (m_stateMachine != nullptr)
	{
		delete m_stateMachine;
		m_stateMachine = nullptr;
	}

	m_stateMachineState = nextStateMachine;

	switch (m_stateMachineState)
	{
	case Summoner::enStateMachineState_Vigilance:
		//�x���X�e�[�g�}�V���̐���
		m_stateMachine = new SummonerSM_Vigilance(this);
		break;
	case Summoner::enStateMachineState_Attack:
		//�U���X�e�[�g�}�V���̐���
		m_stateMachine = new SummonerSM_Attack(this);
		break;
	default:
		std::abort();
		break;
	}

}

void Summoner::SetStartStateMachine(EnStateMachineState nextStateMachine)
{
	if (m_stateMachine != nullptr)
	{
		m_stateMachine = nullptr;
	}

	m_stateMachineState = nextStateMachine;

	switch (m_stateMachineState)
	{
	case Summoner::enStateMachineState_Vigilance:
		//�ŏ��̐����Ȃ̂ŁA�^�C�}�[�����Z�b�g����
		m_stateMachine = new SummonerSM_Vigilance(this, true);
		break;
	case Summoner::enStateMachineState_Attack:
		//�ŏ��̐����Ȃ̂ŁA�O�̃A�N�V���������Z�b�g����
		m_stateMachine = new SummonerSM_Attack(this,true);
		break;
	default:
		std::abort();
		break;
	}
}

void Summoner::ProcessEndAttackState()
{
	//�x���X�e�[�g�}�V���ɕύX
	SetNextStateMachine(Summoner::enStateMachineState_Vigilance);
	//�U������������̂ŘA���U���񐔂�0�Ƀ��Z�b�g����
	GetNowStateMachine()->SetContinuousAttackCount(0);
	//���ʃX�e�[�g����
	ProcessCommonStateTransition();
}

void Summoner::DeleteMobMonsters(bool processDeadFlag)
{
	//���u�����X�^�[��0�̂łȂ��Ȃ烊�X�g���̃��u�����X�^�[�����S
	int mobMonsterNum = CharactersInfoManager::GetInstance()->GetMobMonsters().size();
	if (mobMonsterNum == 0)
		return;
	//���݂��Ă��郂���X�^�[���폜
	for (auto monster : CharactersInfoManager::GetInstance()->GetMobMonsters())
	{
		//���S�������t���O���Z�b�g����Ă���Ȃ�
		if (processDeadFlag == true)
		{
			//�G�t�F�N�g�̐���(���ʉ��Ȃ�)
			monster->ProcessDead(false);
		}
		//���u�����X�^�[�̍폜
		DeleteGO(monster);
	}
	//���X�g���N���A
	CharactersInfoManager::GetInstance()->GetMobMonsters().clear();
}

void Summoner::Render(RenderContext& rc)
{
	if (GetIsDrawModelFlag() != true)
	{
		return;
	}

	m_modelRender.Draw(rc);

}