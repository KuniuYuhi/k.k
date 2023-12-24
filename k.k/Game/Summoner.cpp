#include "stdafx.h"
#include "Summoner.h"
//�}�l�[�W���[
#include "CharactersInfoManager.h"
#include "GameManager.h"
//�X�e�[�g�}�V��
#include "IBossStateMachine.h"
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

//todo �X�[�p�[�A�[�}�[������Ԃ͂̂�����Ȃ�
//��ꂽ�炽�܂ɂ̂�����
//�񕜂��Ă���

namespace {
	const float SCALE_UP = 4.0f;		//�L�����N�^�[�̃T�C�Y

	const float MAX_SUPERARMOR_POINT = 10.0f;

	//�X�e�[�^�X
	int MAXHP = 200;
	int MAXMP = 500;
	int ATK = 20;
	float SPEED = 160.0f;
	const char* NAME = "Summoner";
}

Summoner::Summoner()
{
	m_maxSuperArmorPoint = MAX_SUPERARMOR_POINT;
	m_superArmorPoint = m_maxSuperArmorPoint;
}

Summoner::~Summoner()
{
	//�X�e�[�g�}�V���̍폜
	IBossStateMachine::DeleteInstance();

	//���s�����܂�����ꍇ�łȂ��Ȃ珈�����Ȃ��B�I��鎞�ɃG�t�F�N�g���Đ�����邽�߃G���[���ł邩��
	if (GameManager::GetInstance()->GetOutComeState() == GameManager::enOutComeState_None)
	{
		return;
	}

	//���񂾂Ƃ��Ƀ��u�����X�^�[�������Ă���̂ŏ����K�v�Ȃ�
	if (GameManager::GetInstance()->GetBossDeleteOkFlag() == true)
	{
		return;
	}

	//���u�����X�^�[���폜
	DeleteMobMonsters();
}

bool Summoner::Start()
{
	//�X�e�[�^�X�̏�����
	m_status.InitCharacterStatus(
		MAXHP,
		MAXMP,
		ATK,
		SPEED,
		NAME
	);

	//���f���̏�����
	InitModel();
	//�X�e�[�g�}�V���̐���
	IBossStateMachine::CreateInstance(this);

	//�g���₷�����邽�߂�
	//m_stateMachine = IBossStateMachine::GetInstance();


	//�ŏ��̃A�j���[�V�����ݒ�
	SetNextAnimationState(enAninationState_Idle);

	//�v���C���[�̃C���X�^���X�̑��
	m_player = CharactersInfoManager::GetInstance()->GetPlayerInstance();

	return true;
}

void Summoner::Update()
{
	if (IsStopProcessing() != true)
	{
		//��]����
		ProcessRotation();
		//�ړ������B�{�胂�[�h�̎��̂�
		ProcessMove();

		//�X�e�[�g�}�V���̖��t���[���̏���
		IBossStateMachine::GetInstance()->Execute();

		//�����蔻��̏���
		DamageCollision(m_charaCon);
	}
	
	//�X�[�p�[�A�[�}�[�̉�
	RecoverySuperArmor();

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
	//�Q�[���X�e�[�g�ȊO�Ȃ�
	if (GameManager::GetInstance()->GetGameSeenState() !=
		GameManager::enGameSeenState_Game)
	{
		return true;
	}
	//���s���肵����
	switch (GameManager::GetInstance()->GetOutComeState())
	{
		//������
	case GameManager::enOutComeState_PlayerWin:
		return true;
		break;

		//������
	case GameManager::enOutComeState_PlayerLose:
		//�����A�j���[�V�����Đ�
		SetNextAnimationState(enAnimationState_Victory);
		return true;
		break;
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
		//���ꂽ���̏���
		ProcessDead();
	}

}

void Summoner::HitNormalAttack()
{
	//�P�R���{�̊Ԃ�1�񂾂�����
	//�_���[�W���󂯂����̃R���{�X�e�[�g�ƌ��݂̃R���{�X�e�[�g���Ⴄ�Ȃ�
	if (m_player->IsComboStateSame() == true)
	{
		Damage(m_player->GetAtk());
		CreateDamageFont(m_player->GetAtk());
		//�_���[�W���󂯂����̃R���{�X�e�[�g�Ɍ��݂̃R���{�X�e�[�g��������
		m_player->SetDamagedComboState(m_player->GetNowComboState());
		//�U�������g�Ƀq�b�g�����̂ŁA�v���C���[��attackHitFlag��true�ɂ���
		m_player->SetAttackHitFlag(true);
	}
}

void Summoner::HitSkillAttack()
{
	//�X�L���U�����󂯂��Ȃ��Ȃ�
	if (m_player->GetHittableFlag() != true)
	{
		return;
	}
	m_damageFlag = true;
	Damage(m_player->GetAtk());
	CreateDamageFont(m_player->GetAtk());
	//���i�q�b�g�����̂Ńt���O�����Z�b�g�B���i�q�b�g�łȂ��Ƃ�
	m_player->SetHittableFlag(false);
}

void Summoner::ProcessDead(bool seFlag)
{
	//�����Ƃ�����������ɂ���
	//�t���[�����[�g�𗎂Ƃ�
	g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Variable, 30);
	//�Q�[���}�l�[�W���[�̃v���C���[�̏����t���O��ݒ�
	GameManager::GetInstance()->SetPlayerWinFlag(true);
	m_status.SetHp(0);
	//�Z�̓r���ł��ꂽ��������Ȃ�

	//���ʉ��̍Đ�
	if (seFlag == true)
	{

	}

	//����X�e�[�g
	SetNextAnimationState(enAnimationState_Die);
	m_modelRender.SetAnimationSpeed(0.7f);
}

void Summoner::ProcessMove()
{

}

void Summoner::ProcessRotation()
{
	m_targetPosition = m_player->GetPosition();

	m_moveSpeed= CalcVelocity(m_status, m_targetPosition);

	//��]�\�łȂ��Ȃ�
	if (isRotationEnable() == false)
	{
		//��]���Ȃ�
		return;
	}

	//x��z�̈ړ����x����������
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//�ɂ₩�ɉ�]������
		m_rotMove = Math::Lerp(
			g_gameTime->GetFrameDeltaTime() * 5.0f, m_rotMove, m_moveSpeed);
		m_rotation.SetRotationYFromDirectionXZ(m_rotMove);
		//todo
	}
	//�O������ݒ�
	m_forward = m_rotMove;
	m_forward.Normalize();
	m_moveSpeed= g_vec3Zero;
}

void Summoner::ProcessHit(int hitDamage)
{
	//HP�����炷
	m_status.CalcHp(hitDamage, false);
	//��_���[�W�J�E���g���Z
	m_damageCount++;
	//�X�[�p�[�A�[�}�[�����炷
	CalcSuperArmor(false, hitDamage);

	//�X�[�p�[�A�[�}�[���u���C�N���Ă���Ȃ�
	if (GetBreakSuperArmorFlag() == true)
	{
		//���m���ŋ���
		if (IsFlinch() == true)
		{
			SetNextAnimationState(enAnimationState_CriticalHit);
		}
	}
}

void Summoner::RecoverySuperArmor()
{
	//�X�[�p�[�A�[�}�[���u���C�N���Ă��Ȃ��Ȃ珈�����Ȃ�
	if (GetBreakSuperArmorFlag() != true)
	{
		return;
	}
	//�X�[�p�[�A�[�}�[�̉�
	CalcSuperArmor(true, g_gameTime->GetFrameDeltaTime());
}

void Summoner::SetNextAnimationState(EnAnimationState nextState)
{
	//�Â��X�e�[�g���폜����
	if (m_nowBossState != nullptr)
	{
		delete m_nowBossState;
	}
	//�V�����X�e�[�g���쐬
	m_enAnimationState = nextState;

	switch (nextState)
	{
	case Summoner::enAninationState_Idle://�ҋ@���
		m_nowBossState = new SummonerState_Idle(this);
		break;
	case Summoner::enAninationState_Walk://�������
		m_nowBossState = new SummonerState_Idle(this);
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
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		SetNextAnimationState(enAninationState_Walk);
	}
	else
	{
		SetNextAnimationState(enAninationState_Idle);
	}

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
	//m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
	//	OnAnimationEvent(clipName, eventName);
	//	});

}


void Summoner::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
}

void Summoner::DeleteMobMonsters()
{
	//���u�����X�^�[��0�̂łȂ��Ȃ烊�X�g���̃��u�����X�^�[�����S
	int mobMonsterNum = CharactersInfoManager::GetInstance()->GetMobMonsters().size();
	if (mobMonsterNum == 0)
		return;

	for (auto monster : CharactersInfoManager::GetInstance()->GetMobMonsters())
	{
		monster->ProcessDead(false);
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