#include "stdafx.h"

#include "Lich.h"
#include "LichStateIdle.h"
#include "LichStateWalk.h"
#include "LichStateAttack_1.h"
#include "LichStateAttack_2.h"
#include "LichStateDie.h"
#include "Game.h"
#include "DarkWall.h"
#include "DarkBall.h"
#include "LichStateDamage.h"
#include "LichStateDarkMeteorite_Start.h"
#include "LichStateDarkMeteorite_Main.h"
#include "LichStateDarkMeteorite_End.h"
#include "DarkMeteorite.h"
#include "LichStateSummon.h"
#include "LichStateVictory.h"

#include "LichAction.h"
#include "Summon.h"
#include "LichStateAngry.h"
#include "LichStateWarp.h"

#include "CharactersInfoManager.h"
#include "GameManager.h"

#include "InitEffect.h"



namespace {
	const float SCALE_UP = 4.0f;									//�L�����N�^�[�̃T�C�Y
	const Vector3 FIRST_POSITION = Vector3(0.0f, 0.0f, -250.0f);	//�ŏ��̍��W
	const float DISTANCE = 4000.0f;									//�v���C���[�𔭌��ł��鋗��
	const float NON_WARP_DISTANCE = 400.0f;							//���[�v�s�v�ȋ���

	const float FARST_ATTACK_INTERVAL = 3.0f;

	const Vector3 WARP_UP = { 0.0f,800.0f,0.0f };
	const Vector3 WARP_DOWN = { 0.0f,-800.0f,0.0f };

	const float WARP_POS_Y_UP = 2500.0f;

	const float ADD_CREATE_DARK_BALL_1_Y = 30.0f;
	const float ADD_CREATE_DARK_BALL_2_Y = -30.0f;

	const float HULF_HP_ATK_INTERVAL = 3;						//HP�������ɂȂ������̍U���Ԋu����

	const char* DARL_BALL_LIGHT = "darkball_light";
	const char* DARL_BALL_LEFT = "darkball_left";

	const float ROT_SPEED = 8.0f;
	const float ROT_ONLY_SPEED = 5.0f;

	//�X�e�[�^�X
	int MAXHP = 200;
	int MAXMP = 500;
	int ATK = 20;
	float SPEED = 160.0f;
	const char* NAME = "Lich";
}

Lich::Lich()
{
	//�ŏ��̍��W��ݒ肷��
	m_position = FIRST_POSITION;
	//�v���C���[�𔭌��ł��鋗����ݒ肷��
	m_distance = DISTANCE;
	//���b�`�̃T�C�Y��ݒ�
	m_scale *= SCALE_UP;

	//�ŏ��̍U���̃C���^�[�o���𑁂߂�
	m_attackIntervalTimer = FARST_ATTACK_INTERVAL;
}

Lich::~Lich()
{
	//
	if (m_lichAction != nullptr)
	{
		delete m_lichAction;
	}
	//���s�����܂�����ꍇ�łȂ��Ȃ珈�����Ȃ��B�I��鎞�ɃG�t�F�N�g���Đ�����邽�߃G���[���ł邩��
	if (GameManager::GetInstance()->GetOutComeState()==GameManager::enOutComeState_None)
	{
		return;
	}

	//���񂾂Ƃ��Ƀ��u�����X�^�[�������Ă���̂ŏ����K�v�Ȃ�
	if (m_dieFlag == true)
	{
		return;
	}

	//���u�����X�^�[��0�̂łȂ��Ȃ烊�X�g���̃��u�����X�^�[�����S
	int mobMonsterNum = CharactersInfoManager::GetInstance()->GetMobMonsters().size();
	if (mobMonsterNum != 0)
	{
		for (auto mob : CharactersInfoManager::GetInstance()->GetMobMonsters())
		{
			mob->ProcessDead(false);
			mob->Dead();
			//���X�g����폜
			//CharactersInfoManager::GetInstance()->RemoveMobMonsterFormList(mob);
		}
	}

	
}

bool Lich::Start()
{
	//�������������B
	srand((unsigned)time(NULL));

	m_game = FindGO<Game>("game");

	//�����̃A�j���[�V�����X�e�[�g��ҋ@��Ԃɂ���B
	SetNextAnimationState(enAninationState_Idle);

	//�X�e�[�^�X�̏�����
	m_status.InitCharacterStatus(
		MAXHP,
		MAXMP,
		ATK,
		SPEED,
		NAME
	);

	InitModel();
	//
	SetStageLevelPosition();
	//��ԃX�e�[�g
	SetSpecialActionState(enSpecialActionState_Normal);
	
	//todo�@new����K�v�Ȃ�
	m_lichAction = new LichAction(this);
	//�D��x�ݒ肷��
	//m_lichAction->SettingPriority();
	
	return true;
}

void Lich::InitModel()
{
	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/Lich/Idle.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Walk].Load("Assets/animData/character/Lich/Walk.tka");
	m_animationClip[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClip[enAnimClip_Attack_1].Load("Assets/animData/character/Lich/Attack1.tka");
	m_animationClip[enAnimClip_Attack_1].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_2].Load("Assets/animData/character/Lich/Attack2.tka");
	m_animationClip[enAnimClip_Attack_2].SetLoopFlag(false);
	m_animationClip[enAnimClip_Die].Load("Assets/animData/character/Lich/Die.tka");
	m_animationClip[enAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enAnimClip_Damage].Load("Assets/animData/character/Lich/Damage.tka");
	m_animationClip[enAnimClip_Damage].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_DarkMeteorite_start].Load("Assets/animData/character/Lich/DarkMeteorite_Start.tka");
	m_animationClip[enAnimClip_Attack_DarkMeteorite_start].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_DarkMeteorite_main].Load("Assets/animData/character/Lich/DarkMeteorite.tka");
	m_animationClip[enAnimClip_Attack_DarkMeteorite_main].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_DarkMeteorite_end].Load("Assets/animData/character/Lich/DarkMeteorite_End.tka");
	m_animationClip[enAnimClip_Attack_DarkMeteorite_end].SetLoopFlag(false);
	m_animationClip[enAnimClip_Summon].Load("Assets/animData/character/Lich/Summon.tka");
	m_animationClip[enAnimClip_Summon].SetLoopFlag(false);
	m_animationClip[enAnimClip_Victory].Load("Assets/animData/character/Lich/Victory.tka");
	m_animationClip[enAnimClip_Victory].SetLoopFlag(true);
	m_animationClip[enAnimClip_Angry].Load("Assets/animData/character/Lich/Angry.tka");
	m_animationClip[enAnimClip_Angry].SetLoopFlag(false);


	m_modelRender.Init("Assets/modelData/character/Lich/Lich_real.tkm",
		L"Assets/shader/ToonTextrue/lamp_Lich.DDS",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);

	//���f����TRS�̐ݒ�
	m_modelRender.SetTransform(
		m_position,
		m_rotation,
		m_scale
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
}

void Lich::Update()
{	
	//���s�����܂����瑼�̏��������s���Ȃ��悤�ɂ���
	if (IsStopProcessing()==true)
	{
		ManageState();
		PlayAnimation();
		m_modelRender.Update();
		return;
	}

	//��_���[�W�̓����蔻��
	DamageCollision(m_charaCon);
	//�C���^�[�o���̌v�Z
	AttackInterval(m_attackIntervalTime);
	//DamageInterval(m_damageIntervalTime);

	//�{�胂�[�h�Ȃ�{�胂�[�h�^�C�}�[�̌v�Z
	CalcAngryTime();
	
	Move();
	Rotation(ROT_SPEED, ROT_ONLY_SPEED);

	DecideNextAction();

	ManageState();
	PlayAnimation();

	SetTransFormModel(m_modelRender);
	m_modelRender.Update();
}

bool Lich::IsStopProcessing()
{
	//�Q�[���X�e�[�g�ȊO�Ȃ�
	if (GameManager::GetInstance()->GetGameSeenState() !=
		GameManager::enGameSeenState_Game)
	{
		return true;
	}

	//���s�����܂�����
	if (m_enOutCome != enOutCome_None)
	{
		return true;
	}

	switch (GameManager::GetInstance()->GetOutComeState())
	{
		//������
	case GameManager::enOutComeState_PlayerWin:
		//���s�X�e�[�g�̐ݒ�
		SetEnOutCome(enOutCome_Lose);
		return true;
		break;

		//������
	case GameManager::enOutComeState_PlayerLose:
		//���s�X�e�[�g�̐ݒ�
		SetEnOutCome(enOutCome_Win);
		//�_�[�N���e�I�̍폜
		DeleteDarkMeteo();
		//�����A�j���[�V�����Đ�
		SetNextAnimationState(enAnimationState_Victory);
		return true;
		break;
	default:
		break;
	}

	//�S�ĈႤ�Ȃ�
	return false;
}

void Lich::Move()
{
	//�v���C���[�̍��W���擾
	SetTargetPosition();
	//�ړ�����
	m_moveSpeed = CalcVelocity(m_status, m_targetPosition);
	//��_���[�W���͏��������Ȃ�
	if (isAnimationEnable() != true)
	{
		return;
	}
	//�{�胂�[�h�łȂ��Ȃ珈�����Ȃ�
	if (m_enSpecialActionState != enSpecialActionState_AngryMode)
	{
		//�ړ����Ȃ��悤�ɂ���
		m_moveSpeed = Vector3::Zero;
		return;
	}
	//�U�����Ȃ�ړ����Ȃ�
	if (IsAttackEnable() != true)
	{
		//�ړ����Ȃ��悤�ɂ���
		m_moveSpeed = Vector3::Zero;
	}
	//�v���C���[����������
	else if (IsFindPlayer(m_distance) == true)
	{
		//�͈͓��ł�����т���ѓ����Ȃ��悤�ɂ�����
		//���̋����ɂȂ����炻��ȏ㓮���Ȃ�(�^�[�Q�b�g�ɋ߂Â��Ȃ��悤�ɂ���)
		if (IsDistanceToPlayer() == true)
		{
			//�ړ����Ȃ��悤�ɂ���
			m_moveSpeed = Vector3::Zero;
		}
		//�ړ�����
		m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	}
}

void Lich::Damage(int attack)
{
	//�q�b�g�J�E���g�𑝂₷�A�~�σ_���[�W�𑝂₷
	SetHitCountAndDamage(1, attack);
	//�{�胂�[�h�J�E���g�𑝂₷
	m_angryModeCount++;

	if (m_status.GetHp() > 0)
	{
		//���m���ŋ��ށB�{�胂�[�h�̎��͂Ђ�܂Ȃ�
		if (m_enAnimationState != enAnimationState_Angry && Isflinch() == true)
		{
			//�Z�̓r����������Ȃ�
			if (m_darkWall != nullptr)
			{
				DeleteGO(m_darkWall);
			}
			//�N���e�B�J���q�b�g���Đ�
			g_soundManager->InitAndPlaySoundSource(enSoundName_CriticalHit);
			//��_���[�W�A�j���[�V�����X�e�[�g�ɑJ��
			SetNextAnimationState(enAnimationState_Damage);
		}

		//HP�����炷
		m_status.CalcHp(attack, false);
		//HP�������ɂȂ�����
		if (m_status.GetHp() <= m_status.GetMaxHp() / 2)
		{
			m_halfHpFlag = true;
			//�U���Ԋu��Z������
			m_attackIntervalTime = HULF_HP_ATK_INTERVAL;
		}
	}
	//���ꂽ�Ƃ�
	if(m_status.GetHp() <= 0)
	{
		//�����Ƃ�����������ɂ���
		//�t���[�����[�g�𗎂Ƃ�
		g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Variable, 30);
		m_modelRender.SetAnimationSpeed(0.4f);
		//�Q�[���}�l�[�W���[�̃v���C���[�̏����t���O��ݒ�
		GameManager::GetInstance()->SetPlayerWinFlag(true);
		m_status.SetHp(0);
		//�Z�̓r���ł��ꂽ��������Ȃ�
		if (m_darkWall != nullptr)
		{
			DeleteGO(m_darkWall);
		}

		//����X�e�[�g
		SetNextAnimationState(enAnimationState_Die);
		m_modelRender.SetAnimationSpeed(0.8f);
	}
}

bool Lich::Isflinch()
{
	//�{�胂�[�h�̎��͂Ђ�܂Ȃ�
	if (m_enSpecialActionState == enSpecialActionState_AngryMode)
	{
		//���܂Ȃ�
		return false;
	}

	int value = rand() % 10;
	if (value > 7)
	{
		//����
		return true;
	}
	else
	{
		//���܂Ȃ�
		return false;
	}
}

bool Lich::IsDistanceToPlayer()
{
	//���g����^�[�Q�b�g�Ɍ������x�N�g�������߂�
	Vector3 diff = m_targetPosition - m_position;
	//�v���C���[�Ƃ̋���
	if (diff.Length() < m_distanceToPlayer)
	{
		//�������ɂ���
		return true;
	}
	//�������ɂ��Ȃ�
	return false;
}

bool Lich::CalcAngryTime()
{
	//�{�胂�[�h�łȂ��Ƃ��͏������Ȃ�
	if (m_enSpecialActionState != enSpecialActionState_AngryMode)
	{
		return false;
	}

	if (IsAttackEnable() != true)
	{
		return false;
	}

	if (m_angryLimitTime < m_angryLimitTimer)
	{
		m_angryLimitTimer = 0.0f;
		//�ʏ�ɖ߂�
		SetSpecialActionState(enSpecialActionState_Normal);
		//�{�胂�[�h�J�E���g�����Z�b�g����
		m_angryModeCount = 0;
		//�{�胂�[�h����߂�Ƃ��Ƀv���C���[�����ԉ����Ƃ���Ƀ��[�v����
		SetNextAnimationState(enAnimationState_Warp);
		//���[�v���Đ�
		g_soundManager->InitAndPlaySoundSource(enSoundName_Boss_Warp, g_soundManager->GetSEVolume());
		//���G���Ԃɂ���
		SetInvincibleFlag(true);
		return false;
	}
	else
	{
		m_angryLimitTimer += g_gameTime->GetFrameDeltaTime();
	}

	return true;
}

bool Lich::RotationOnly()
{
	//����̃A�j���[�V�������Đ����̂Ƃ�
	if (isRotationEnable() != true)
	{
		return true;
	}
	return false;
}

void Lich::DecideNextAction()
{
	//��_���[�W���͏��������Ȃ�
	if (isAnimationEnable() != true)
	{
		return;
	}

	//�U�����͏��������Ȃ�
	if (IsAttackEnable() != true)
	{
		return;
	}

	//�U���\�Ȃ�
	if (m_attackEnableFlag == false)
	{
		SetTargetPosition();
		m_toPlayerDir =  m_targetPosition - m_position;
		//�v���C���[������p���ɂ���Ȃ�
		if (IsInFieldOfView(m_toPlayerDir, m_forward, 10.0f))
		{
			//���̍s����I��
			m_lichAction->NextAction();

			m_attackEnableFlag = true;
		}
	}
}

void Lich::PlayAnimation()
{
	m_state->ManageState();
}

void Lich::ManageState()
{
	m_state->PlayAnimation();
}

void Lich::SetNextAnimationState(EnAnimationState nextState)
{
	if (m_state != nullptr) {
		// �Â��X�e�[�g���폜����B
		delete m_state;
		m_state = nullptr;
	}

	//�A�j���[�V�����X�e�[�g�����̃X�e�[�g�ɕς���
	m_enAnimationState = nextState;

	switch (m_enAnimationState)
	{
	case Lich::enAninationState_Idle:
		// �ҋ@�X�e�[�g���쐬����B
		m_state = new LichStateIdle(this);
		break;
	case Lich::enAninationState_Walk:
		// �ҋ@�X�e�[�g���쐬����B
		m_state = new LichStateWalk(this);
		break;
	case Lich::enAninationState_Run:
		break;
	case Lich::enAnimationState_Attack_1:
		// �A�^�b�N�P�X�e�[�g���쐬����B
		m_state = new LichStateAttack_1(this);
		break;
	case Lich::enAnimationState_Attack_2:
		// �A�^�b�N�Q�X�e�[�g���쐬����B
		m_state = new LichStateAttack_2(this);
		break;
	case Lich::enAnimationState_Damage:
		//��_���[�W�X�e�[�g���쐬����
		m_state = new LichStateDamage(this);
		break;
	case Lich::enAnimationState_Die:
		//Die�X�e�[�g���쐬����
		m_state = new LichStateDie(this);
		break;
	case Lich::enAnimationState_Attack_DarkMeteorite_start:
		//�_�[�N���e�I�X�^�[�g�X�e�[�g���쐬����
		m_state = new LichStateDarkMeteorite_Start(this);
		break;
	case Lich::enAnimationState_Attack_DarkMeteorite_main:
		//�_�[�N���e�I���C���X�e�[�g���쐬����
		m_state = new LichStateDarkMeteorite_Main(this);
		break;
	case Lich::enAnimationState_Attack_DarkMeteorite_end:
		//�_�[�N���e�I�G���h�X�e�[�g���쐬����
		m_state = new LichStateDarkMeteorite_End(this);
		break;
	case Lich::enAninationState_Summon:
		//�_�[�N���e�I�G���h�X�e�[�g���쐬����
		m_state = new LichStateSummon(this);
		break;
	case Lich::enAnimationState_Victory:
		//�����X�e�[�g���쐬����
		m_state = new LichStateVictory(this);
		break;
	case Lich::enAnimationState_Angry:
		//�{�胂�[�h�X�e�[�g���쐬����
		m_state = new LichStateAngry(this);
		break;
	case Lich::enAnimationState_Warp:
		//���[�v�X�e�[�g���쐬����
		m_state = new LichStateWarp(this);
		break;
	default:
		// �����ɗ�����X�e�[�g�̃C���X�^���X�쐬�����̒ǉ��Y��B
		std::abort();
		break;
	}
}

void Lich::SetStageLevelPosition()
{
	m_stageLevel.Init(
		"Assets/level3D/BossStage1.tkl",
		[&](LevelObjectData& objData)
		{
			if (objData.ForwardMatchName(L"Pos") == true) {
				//���[�v������W��z��Ɋi�[����
				Vector3 warpPos = objData.position;
				m_warpPositions.emplace_back(warpPos);
				return true;
			}
			return false;
		});
}

void Lich::DecideWarpPosition()
{
	float MaxLength = 0.0f;
	//�^�[�Q�b�g�����ԉ����Ƃ�����W�𒲂ׂ�
	for (int amount = 0; amount < m_warpPositions.size(); amount++)
	{
		//�^�[�Q�b�g���玩�g�ւ̃x�N�g��
		Vector3 diff = m_warpPositions[amount] - m_targetPosition;
		//�^�[�Q�b�g�����ԉ����Ƃ���̍��W�ɂ���
		if (MaxLength < diff.Length())
		{
			MaxLength = diff.Length();
			//��ԉ����Ƃ���̍��W
			m_warpPosition = m_warpPositions[amount];
		}
	}
}

void Lich::MoveWarpPosition()
{
	m_warpPosition.y += WARP_POS_Y_UP;
	m_position = m_warpPosition;
	m_charaCon.SetPosition(m_position);
	Vector3 zero = g_vec3Zero;
	m_charaCon.Execute(zero, 1.0f / 60.0f);
}

void Lich::ProcessCommonStateTransition()
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

void Lich::OnProcessAttack_1StateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Lich::OnProcessAttack_2StateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		if (m_darkWall != nullptr)
		{
			DeleteGO(m_darkWall);
		}
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Lich::OnProcessDieStateTransition()
{
	if (m_isdeadEffectPlayFlag==true && m_deadEffect->IsPlay() != true)
	{
		//�S�Ă̏������I�������폜����Ă��悢
		GameManager::GetInstance()->SetBossDeleteOkFlag(true);
		return;
	}

	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		if (m_deadEffect == nullptr)
		{
			//���S�G�t�F�N�g�Đ�
			//���S���G�t�F�N�g�̍Đ�
			m_deadEffect = NewGO<EffectEmitter>(0);
			m_deadEffect->Init(InitEffect::enEffect_Mob_Dead);
			m_deadEffect->Play();
			m_deadEffect->SetPosition(m_position);
			m_deadEffect->SetScale(g_vec3One * 8.0f);
			m_deadEffect->Update();
			//�t���[�����[�g�𗎂Ƃ�
			g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Variable, 60);
		}
		m_isdeadEffectPlayFlag = true;
		//���ꂽ�̂Ń��f����\�����Ȃ��悤�ɂ���
		m_dieFlag = true;

		//���u�����X�^�[���폜
		DeleteMobMonster();
		//DeleteGO(this);
	}
}

void Lich::OnProcessDamageStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Lich::OnProcessDarkMeteorite_StartStateTransition()
{
	//�A�j���[�V�������ɏ��s�����܂�����
	if (IsStopProcessing() == true)
	{
		m_darkMeteorite->SetGameEndFlag(true);
		DeleteGO(m_darkMeteorite);
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
		return;
	}

	//�T�C�Y���ő�܂ő傫���Ȃ�����
	//�����̕ϐ��ɂ��Ă���������
	if (m_createDarkMeteoriteFlag == true)
	{
		if (m_darkMeteorite->GetSizeUpFlag() == true)
		{
			SetNextAnimationState(enAnimationState_Attack_DarkMeteorite_main);
			return;
		}
	}
	else
	{
		if (m_modelRender.IsPlayingAnimation() == false)
		{
			//��x�����_�[�N���e�I�𐶐�
			CreateDarkMeteorite(m_halfHpFlag);
			m_createDarkMeteoriteFlag = true;
		}
	}
	
}

void Lich::OnProcessDarkMeteorite_MainStateTransition()
{
	//�A�j���[�V�������ɏ��s�����܂�����
	if (IsStopProcessing() == true)
	{
		DeleteGO(m_darkMeteorite);
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
		return;
	}

	//���e�I��S�Đ���������
	if (m_darkMeteorite->GetShotEndFlag() == true)
	{
		DeleteGO(m_darkMeteorite);
		//�G���h�Ɉڂ�
		SetNextAnimationState(enAnimationState_Attack_DarkMeteorite_end);
	}
}

void Lich::OnProcessDarkMeteorite_EndStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_createDarkMeteoriteFlag = false;
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
		//���G���Ԃł͂Ȃ�
		SetInvincibleFlag(false);
	}
}

void Lich::OnProcessSummonStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���G���Ԃł͂Ȃ�
		SetInvincibleFlag(false);
		//�q�b�g�J�E���g�����Z�b�g
		m_hitCount = 0;
		//�~�σ_���[�W�����Z�b�g
		m_accumulationDamage = 0;
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Lich::OnProcessVictoryStateTransition()
{
	//�A�j���[�V���������[�v����̂œ��ɏ������Ȃ�
}

void Lich::OnProcessAngryStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		g_soundManager->StopSound(enSoundName_Boss_Angry);
		//�{�胂�[�h�Ɉڍs
		SetSpecialActionState(enSpecialActionState_AngryMode);
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
		//���G���Ԃł͂Ȃ�
		SetInvincibleFlag(false);
	}
}

void Lich::OnProcessWarpStateTransition()
{
	switch (m_enWarpStep)
	{
	case Lich::enWarpStep_Up:
		OnProcessenWarpStepUp();
		break;
	case Lich::enWarpStep_Warp:
		OnProcessenWarpStepWarp();
		break;
	case Lich::enWarpStep_Down:
		OnProcessenWarpSteDown();
		break;
	case Lich::enWarpStep_End:
		OnProcessenWarpStepEnd();
		break;
	default:
		break;
	}
}

void Lich::OnProcessenWarpStepUp()
{
	DecideWarpPosition();
	Vector3 Up = WARP_UP;
	//���[�v����O�ɏ�ɏ㏸����
	m_position = m_charaCon.Execute(Up, 1.0f / 30.0f);
	//��̂ق��ɍs������
	if (m_position.y >= 2500.0f)
	{
		//���̃X�e�b�v�ɐi��
		m_enWarpStep = enWarpStep_Warp;
	}
}

void Lich::OnProcessenWarpStepWarp()
{
	MoveWarpPosition();
	//���̃X�e�b�v�ɐi��
	m_enWarpStep = enWarpStep_Down;
}

void Lich::OnProcessenWarpSteDown()
{
	//�n��ɂ�����
	if (m_charaCon.IsOnGround()==true)
	{
		m_position.y = 0.0f;
		m_charaCon.SetPosition(m_position);
		Vector3 zero = g_vec3Zero;
		m_charaCon.Execute(zero, 1.0f / 60.0f);
		//���̃X�e�b�v�ɐi��
		m_enWarpStep = enWarpStep_End;
	}
	else
	{
		Vector3 Down = WARP_DOWN;
		//���[�v��ŉ�����
		m_position = m_charaCon.Execute(Down, 1.0f / 30.0f);
	}
}

void Lich::OnProcessenWarpStepEnd()
{
	//���ʂ̏�ԑJ�ڏ����Ɉڍs
	ProcessCommonStateTransition();
	//���G���Ԃł͂Ȃ�
	SetInvincibleFlag(false);
	//�X�e�b�v�I���
	m_enWarpStep = enWarpStep_Up;
}

void Lich::CreateDarkWall()
{
	m_darkWall = NewGO<DarkWall>(0, "darkwall");
	m_darkWall->SetLich(this);
}

void Lich::CreateDarkBall(bool AddBallFlag)
{
	DarkBall* darkBall = NewGO<DarkBall>(0, "darkball");
	darkBall->SetAtk(m_status.GetAtk());
	darkBall->Setting(m_position, m_rotation);
	//�X�Ƀ_�[�N�{�[���𐶐����Ȃ��Ȃ�
	if (AddBallFlag != true)
	{
		return;
	}
	Quaternion right = m_rotation;
	right.AddRotationDegY(ADD_CREATE_DARK_BALL_1_Y);
	DarkBall* darkBall2 = NewGO<DarkBall>(0, "darkball");
	darkBall2->SetAtk(m_status.GetAtk());
	darkBall2->Setting(m_position, right);

	right = m_rotation;
	right.AddRotationDegY(ADD_CREATE_DARK_BALL_2_Y);
	DarkBall* darkBall3 = NewGO<DarkBall>(0, "darkball");
	darkBall3->SetAtk(m_status.GetAtk());
	darkBall3->Setting(m_position, right);

	//���Ɠ��������
	//AddCreateDarkBall(m_darkBall_left, DARL_BALL_LEFT,ADD_CREATE_DARK_BALL_1_Y);
	//AddCreateDarkBall(m_darkBall_light, DARL_BALL_LIGHT,ADD_CREATE_DARK_BALL_2_Y);
}

void Lich::AddCreateDarkBall(DarkBall* darkBall, const char* name, float degY)
{
	Quaternion right = m_rotation;
	right.AddRotationDegY(degY);
	darkBall = NewGO<DarkBall>(0, name);
	darkBall->SetAtk(m_status.GetAtk());
	darkBall->Setting(m_position, right);
}

void Lich::CreateDarkMeteorite(bool lastMeteoFlag)
{
	//�傫�ȃ��e�I���쐬
	m_darkMeteorite = NewGO<DarkMeteorite>(0, "darkmeteorite");
	m_darkMeteorite->SetPosition(m_position);
	m_darkMeteorite->SetRotation(m_rotation);
	m_darkMeteorite->SetmLastBigMeteoShotFlag(lastMeteoFlag);
}

void Lich::DeleteDarkMeteo()
{
	//�_�[�N���e�I����������Ă�����폜����
	m_darkMeteorite = FindGO<DarkMeteorite>("darkmeteorite");
	if (m_darkMeteorite != nullptr)
	{
		m_darkMeteorite->SetGameEndFlag(true);
		DeleteGO(m_darkMeteorite);
	}
}

void Lich::CreateSummon()
{
	//�����X�^�[����������
	m_summon = NewGO<Summon>(0, "summon");
	m_summon->SetLich(this);
	//�����O�̉r�����Đ�
	//g_soundManager->InitAndPlaySoundSource(enSoundName_Boss_Summon_charge, g_soundManager->GetSEVolume());
	//�ŏ��̏�������
	if (m_firstSummonFlag == true)
	{
		m_summon->SetFirstSummonFlag(m_firstSummonFlag);
		m_firstSummonFlag = false;
	}
}

bool Lich::IsCollisionDetection()
{
	//���G���Ԃ̊Ԃ͏��������Ȃ�
	if (GetInvincibleFlag() == true)
	{
		return true;
	}
	
	return false;
}

void Lich::HitNormalAttack()
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

void Lich::HitSkillAttack()
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

void Lich::HitFlamePillar(bool damageFlag)
{
	if (damageFlag == false)
	{
		//�_���[�W���󂯂�
		Damage(m_damage);
		CreateDamageFont(m_damage);
	}
}

void Lich::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�{�[�������^�C�~���O
	if (wcscmp(eventName, L"Create_Ball") == 0)
	{
		//�{�[������
		CreateDarkBall(m_halfHpFlag);
	}

	//�_�[�N�E�H�[�������^�C�~���O
	if (wcscmp(eventName, L"Create_DarkWall") == 0)
	{

		CreateDarkWall();
	}
	//�_�[�N�E�H�[�������I���
	if (wcscmp(eventName, L"CreateEnd_DarkWall") == 0)
	{
		if (m_darkWall != nullptr)
		{
			DeleteGO(m_darkWall);
		}
	}

	//���e�I�����^�C�~���O
	if (wcscmp(eventName, L"createMeteo") == 0)
	{
		m_darkMeteorite->SetShotStartFlag(true);
		SetTargetPosition();
		m_darkMeteorite->SetTargetPosition(m_targetPosition);
	}

	if (wcscmp(eventName, L"Summon") == 0)
	{
		//�����X�^�[�̏���
		//�r���̉��������ď����̉����Đ�����
		//g_soundManager->StopSound(enSoundName_Boss_Summon_charge);
		g_soundManager->InitAndPlaySoundSource(enSoundName_Boss_Summon_start, g_soundManager->GetSEVolume());
		//�����n��
		m_summon->SetSummonStartFlag(true);
	}

	if (wcscmp(eventName, L"AngrySound") == 0)
	{
		g_soundManager->InitAndPlaySoundSource(enSoundName_Boss_Angry, g_soundManager->GetSEVolume());
	}
}

void Lich::Render(RenderContext& rc)
{
	if (m_dieFlag == true)
	{
		return;
	}
	m_modelRender.Draw(rc);
}

void Lich::DeleteMobMonster()
{
	//���u�����X�^�[��0�̂łȂ��Ȃ烊�X�g���̃��u�����X�^�[�����S
	int mobMonsterNum = CharactersInfoManager::GetInstance()->GetMobMonsters().size();
	if (mobMonsterNum != 0)
	{
		for (auto mob : CharactersInfoManager::GetInstance()->GetMobMonsters())
		{
			mob->ProcessDead(false);
			mob->Dead();
			//���X�g����폜
			CharactersInfoManager::GetInstance()->RemoveMobMonsterFormList(mob);
		}
	}
}
