#include "stdafx.h"

#include "Lich.h"
#include "LichStateIdle.h"
#include "LichStateWalk.h"
#include "LichStateAttack_1.h"
#include "LichStateAttack_2.h"
#include "LichStateDie.h"
#include "Game.h"
#include "FireBall.h"
#include "FlamePillar.h"
#include "DarkWall.h"
#include "LichStateDamage.h"
#include "LichStateDarkMeteorite_Start.h"
#include "LichStateDarkMeteorite_Main.h"
#include "LichStateDarkMeteorite_End.h"
#include "DarkMeteorite.h"
#include "LichStateSummon.h"
#include "LichStateVictory.h"

#include "LichAction.h"
#include "Summon.h"

//#include "DamageFont.h"

//todo �^�[�Q�b�g�����΂炭�߂��ɂ����瓦����

namespace {
	const float SCALE_UP = 3.0f;									//�L�����N�^�[�̃T�C�Y
	const Vector3 FIRST_POSITION = Vector3(0.0f, 0.0f, -250.0f);	//�ŏ��̍��W
	const float DISTANCE = 4000.0f;									//�v���C���[�𔭌��ł��鋗��
	const float NON_WARP_DISTANCE = 400.0f;							//���[�v�s�v�ȋ���


	//�X�e�[�^�X
	int MAXHP = 1000;
	int MAXMP = 500;
	int ATK = 20;
	float SPEED = 80.0f;
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
}

Lich::~Lich()
{
	//�����c���Ă��郂���X�^�[�����S������
	for (auto monster : m_monsters)
	{
		monster->Dead();
	}

	delete m_lichAction;
}

bool Lich::Start()
{
	//�����̃A�j���[�V�����X�e�[�g��ҋ@��Ԃɂ���B
	SetNextAnimationState(enAninationState_Idle);


	//�X�e�[�^�X�̏�����
	m_status.InitStatus(
		MAXHP,
		MAXMP,
		ATK,
		SPEED,
		NAME
	);

	InitModel();
	//
	SetStageLevelPosition();
	//
	SetSpecialActionState(enSpecialActionState_Normal);
	
	m_lichAction = new LichAction(this);

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


	m_modelRender.Init("Assets/modelData/character/Lich/Lich.tkm",
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

	/*m_hpFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_hpFont.SetScale(1.5f);
	m_hpFont.SetPosition(-800.0f, 500.0f);*/

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//�_�[�N�E�H�[���Ɏg���{�[��ID�擾
	m_darkWallBoonId = m_modelRender.FindBoneID(L"Index_Proximal_L");

}

void Lich::Update()
{
	//MP�̕\��
	/*int NowActorMP = m_status.hp;
	int NowActorMaxMP = m_status.maxHp;
	wchar_t MP[255];
	swprintf_s(MP, 255, L"�{�X HP %3d/%d", NowActorMP, NowActorMaxMP);
	m_hpFont.SetText(MP);*/

	//��_���[�W�̓����蔻��
	DamageCollision(m_charaCon);

	//�U��������Ȃ��Ȃ�
	/*if (IsAttackEntable() != true)
	{*/
		//�v���C���[���S�ł����珟���A�j���[�V�����ݒ�
		if (m_player->IsAnnihilation() == true && m_enAnimationState!= enAnimationState_Victory)
		{
			SetWinFlag(true);
			//�U��������Ȃ��Ȃ�
			if (IsAttackEntable() == true)
			{
				SetNextAnimationState(enAnimationState_Victory);
			}
		}
	//}
	

	//�|���ꂽ�瑼�̏��������s���Ȃ��悤�ɂ���
	if (m_dieFlag==true|| GetWinFlag() == true)
	{
		ManageState();
		PlayAnimation();
		m_modelRender.Update();
		return;
	}

	//�_�[�N�E�H�[���̐���
	/*if (m_CreateDarkWallFlag == true)
	{
		CreateDarkWall();
	}*/

	//�C���^�[�o���̌v�Z
	AttackInterval(m_attackIntervalTime);
	DamageInterval(m_damageIntervalTime);

	Move();
	Rotation();

	DecideNextAction();

	ManageState();
	PlayAnimation();

	SetTransFormModel(m_modelRender);
	m_modelRender.Update();

	m_oldMoveSpeed = m_moveSpeed;
}

void Lich::Move()
{
	//�v���C���[�̍��W���擾
	SetTargetPosition();

	//�ړ�����
	m_moveSpeed = CalcVelocity(m_status, m_targetPosition);

	//��_���[�W���͏��������Ȃ�
	if (isAnimationEntable() != true)
	{
		return;
	}

	//�U�����Ȃ�ړ����Ȃ�
	if (IsAttackEntable() != true)
	{
		//�ړ����Ȃ��悤�ɂ���
		m_moveSpeed = Vector3::Zero;
	}
	//�v���C���[����������
	else if (IsFindPlayer(m_distance) == true)
	{
		//���̋����ɂȂ����炻��ȏ㓮���Ȃ�(�^�[�Q�b�g�ɋ߂Â��Ȃ��悤�ɂ���)
		if (IsDistanceToPlayer() == true)
		{
			//�ړ����Ȃ��悤�ɂ���
			m_moveSpeed = Vector3::Zero;
		}

		//�ړ�����
		m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	}
	else
	{
		//�ړ����Ȃ��悤�ɂ���
		m_moveSpeed = Vector3::Zero;
	}
}

void Lich::Damage(int attack)
{
	//�q�b�g�J�E���g�𑝂₷�A�~�σ_���[�W�𑝂₷
	SetHitCountAndDamage(1, attack);

	if (m_status.hp > 0)
	{
		//���m���ŋ���
		if (Isflinch() == true)
		{
			//�Z�̓r����������Ȃ�
			m_CreateDarkWallFlag = false;
			SetNextAnimationState(enAnimationState_Damage);
		}

		//HP�����炷
		m_status.hp -= attack;

		//HP�������ɂȂ����烏�[�v����
		if (m_status.hp <= m_status.maxHp / 2 && m_halfHpFlag == false)
		{
			//���[�v����X�e�[�g�ɂ���
			SetSpecialActionState(enSpecialActionState_Warp);
			m_halfHpFlag = true;
			//�_�[�N���e�I���g���I���܂Ń_���[�W���󂯂Ȃ��悤�ɂ���
			SetInvincibleFlag(true);
		}
	}
	
	if(m_status.hp <= 0)
	{
		//Die�t���O��true�ɂ���
		m_dieFlag = true;
		m_status.hp = 0;
		//�Z�̓r���ł��ꂽ��������Ȃ�
		m_CreateDarkWallFlag = false;
		SetNextAnimationState(enAnimationState_Die);
	}
}

void Lich::CreateDamageFont(int damage)
{
	DamageFont* damagefont = NewGO<DamageFont>(0, "damagefont");
	damagefont->Setting(
		DamageFont::enDamageActor_Boss,
		damage,
		m_position
	);
}

bool Lich::Isflinch()
{
	//�������������B
	srand((unsigned)time(NULL));

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
	Vector3 diff = m_targetPosition - m_position;
	//�v���C���[�Ƃ̋���
	if (diff.Length() < m_distanceToPlayer)
	{
		//�������ɂ���
		return true;
	}
	else
	{
		//�������ɂ��Ȃ�
		return false;
	}
	
}

bool Lich::RotationOnly()
{
	//����̃A�j���[�V�������Đ����̂Ƃ�
	if (isRotationEntable() != true)
	{
		//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
		if (fabsf(m_SaveMoveSpeed.x) >= 0.001f || fabsf(m_SaveMoveSpeed.z) >= 0.001f)
		{
			m_rotation.SetRotationYFromDirectionXZ(m_SaveMoveSpeed);
		}
		return true;
	}

	return false;
}

void Lich::DecideNextAction()
{
	//��_���[�W���͏��������Ȃ�
	if (isAnimationEntable() != true)
	{
		return;
	}

	//�U�����͏��������Ȃ�
	if (IsAttackEntable() != true)
	{
		return;
	}

	//HP�������Ȃ炷���ɍs��
	if (m_enSpecialActionState == enSpecialActionState_Warp)
	{
		//�_�[�N���e�I�m��
		m_lichAction->NextAction();
		return;
	}
	
	//�U���\�Ȃ�
	if (m_attackFlag == false)
	{
		//m_lichAction = new LichAction(this);
		//���̍s����I��
		m_lichAction->NextAction();
		//delete m_lichAction;

		m_attackFlag = true;
	}

	//switch (m_enSpecialActionState)
	//{
	//	case Lich::enSpecialActionState_Normal:
	//		//�U���\�Ȃ�
	//		if (m_attackFlag == false)
	//		{
	//			//m_lichAction = new LichAction(this);
	//			//���̍s����I��
	//			m_lichAction->NextAction();
	//			//delete m_lichAction;

	//			m_attackFlag = true;
	//		}
	//		break;

	//	case Lich::enSpecialActionState_Warp:
	//		//�^�[�Q�b�g�����ԉ����Ƃ���Ɉړ�����
	//		Warp(enSpecialActionState_Warp);
	//		//
	//		//m_lichAction = new LichAction(this);
	//		m_lichAction->NextAction();
	//		//delete m_lichAction;
	//		//��Ԃ����ɖ߂�
	//		SetSpecialActionState(enSpecialActionState_Normal);
	//		break;

	//	/*case Lich::enSpecialActionState_CenterWarp:

	//		break;*/


	//	default:
	//		break;
	//}
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
				m_WarpPosition.emplace_back(warpPos);
				return true;
			}
			return false;
		});
}

void Lich::Warp(EnSpecialActionState SpecialActionState)
{
	if (SpecialActionState == enSpecialActionState_Warp)
	{
		float MaxLength = 0.0f;
		//�^�[�Q�b�g�����ԉ����Ƃ�����W�𒲂ׂ�
		for (int amount = 0; amount < m_WarpPosition.size(); amount++)
		{
			//
			Vector3 diff = m_WarpPosition[amount] - m_targetPosition;
			//
			if (MaxLength < diff.Length())
			{
				MaxLength = diff.Length();
				m_position = m_WarpPosition[amount];
			}
		}
	}
	else if (SpecialActionState == enSpecialActionState_CenterWarp)
	{
		Vector3 CenterPos = Vector3::Zero;
		Vector3 diff = CenterPos - m_position;

		if (diff.Length() > NON_WARP_DISTANCE)
		{
			m_position = CenterPos;
		}
	}


	//switch (m_enSpecialActionState)
	//{
	//	//��ԉ����Ƃ���Ƀ��[�v����
	//case Lich::enSpecialActionState_Warp:
	//	float MaxLength = 0.0f;
	//	//�^�[�Q�b�g�����ԉ����Ƃ�����W�𒲂ׂ�
	//	for (int amount = 0; amount < m_WarpPosition.size(); amount++)
	//	{
	//		//
	//		Vector3 diff = m_WarpPosition[amount] - m_targetPosition;
	//		//
	//		if (MaxLength < diff.Length())
	//		{
	//			MaxLength = diff.Length();
	//			m_position = m_WarpPosition[amount];
	//		}
	//	}
	//	break;
	//	//�^�񒆂��^�񒆋߂��Ƀ��[�v����
	//case Lich::enSpecialActionState_CenterWarp:
	//	Vector3 CenterPos = Vector3::Zero;
	//	Vector3 diff = CenterPos - m_position;

	//	if (diff.Length() > NON_WARP_DISTANCE)
	//	{
	//		m_position = CenterPos;
	//	}

	//	break;

	//default:
	//	break;
	//}
	//float MaxLength = 0.0f;
	////�^�[�Q�b�g�����ԉ����Ƃ�����W�𒲂ׂ�
	//for (int amount = 0; amount < m_WarpPosition.size(); amount++)
	//{
	//	//
	//	Vector3 diff = m_WarpPosition[amount] - m_targetPosition;
	//	//
	//	if (MaxLength < diff.Length())
	//	{
	//		MaxLength = diff.Length();
	//		m_position = m_WarpPosition[amount];
	//	}
	//}

	Vector3 o = Vector3::Zero;
	//���[�v����
	m_charaCon.SetPosition(m_position);
	m_charaCon.Execute(o, 1.0f / 60.0f);
}

void Lich::ProcessCommonStateTransition()
{
	//���G���Ԃł͂Ȃ�
	SetInvincibleFlag(false);
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
		//�U���p�^�[�����Ȃ���Ԃɂ���
		//m_enAttackPatternState = enAttackPattern_None;
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Lich::OnProcessAttack_2StateTransition()
{
	//�_�[�N�E�H�[���̐���
	if (m_CreateDarkWallFlag == true)
	{
		CreateDarkWall();
	}

	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�U���p�^�[�����Ȃ���Ԃɂ���
		//m_enAttackPatternState = enAttackPattern_None;
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Lich::OnProcessDieStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���g���|���ꂽ�炱�Ƃ��Q�[���ɓ`����
		Game* game = FindGO<Game>("game");
		game->SetDeathBossFlag(true);

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
	

	//�㏸���Ă��Ȃ��Ȃ珈�������Ȃ�
	if (IsRisingDarkMeteorite() != true)
	{
		return;
	}
	//��x�����_�[�N���e�I�𐶐�
	if (m_createDarkMeteoriteFlag == false)
	{
		CreateDarkMeteorite();
		m_createDarkMeteoriteFlag = true;
	}

	//�T�C�Y���ő�܂ő傫���Ȃ�����
	//�����̕ϐ��ɂ��Ă���������
	if (m_darkMeteorite->GetSizeUpFlag() == true)
	{
		//���C���Ɉڂ�
		SetNextAnimationState(enAnimationState_Attack_DarkMeteorite_main);
		return;
	}
}

void Lich::OnProcessDarkMeteorite_MainStateTransition()
{
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
	//�n�ʂɍ~���
	m_moveSpeed = Vector3::AxisY;
	m_moveSpeed.y *= -m_status.defaultSpeed;

	m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	
	//�n�ʂɒ�������
	if (m_charaCon.IsOnGround() == true)
	{
		m_moveSpeed = Vector3::Zero;
		m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Lich::OnProcessSummonStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
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
}

void Lich::CreateDarkWall()
{
	DarkWall* darkball = NewGO<DarkWall>(0, "darkwall");
	darkball->SetLich(this);
}

void Lich::CreateDarkMeteorite()
{
	//�ʂ𐶐�
	m_darkMeteorite = NewGO<DarkMeteorite>(0, "darkmeteorite");
	Vector3 pos = m_position;
	pos.y += 370.0f;
	m_darkMeteorite->SetPosition(pos);
	m_darkMeteorite->SetRotation(m_rotation);
}

bool Lich::IsRisingDarkMeteorite()
{
	//Y���W������ɓ��B������
	if (m_RisingLimit <= m_position.y)
	{
		return true;
	}
	else
	{
		Vector3 moveSpeed = Vector3::Zero;
		moveSpeed.y += g_gameTime->GetFrameDeltaTime() * 250.0f;
		m_moveSpeed = moveSpeed;

		m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 2.0f);

		return false;
	}

}

void Lich::DamageCollision(CharacterController& characon)
{
	//�����o������
	//���񂾂珈�������Ȃ� todo
	//���G���Ԃ̊Ԃ͏��������Ȃ�
	if (GetInvincibleFlag() == true)
	{
		return;
	}
	////��_���[�W���A�f�X���͏��������Ȃ�
	//if (isAnimationEntable() != true)
	//{
	//	return;
	//}
	////�U�����͏��������Ȃ�
	//if (IsAttackEntable() != true)
	//{
	//	return;
	//}

	//�ʏ�U���̓����蔻��
	const auto& Attack_1Collisions = g_collisionObjectManager->FindCollisionObjects("Attack");
	//�R���W�����̔z���for���ŉ�
	for (auto collision : Attack_1Collisions)
	{
		//���g�̃L�����R���ƏՓ˂�����
		if (collision->IsHit(characon) == true)
		{
			//�P�R���{�̊Ԃ�1�񂾂�����
			//�_���[�W���󂯂����̃R���{�X�e�[�g�ƌ��݂̃R���{�X�e�[�g���Ⴄ�Ȃ�
			if (m_player->IsComboStateSame()==true)
			{
				Damage(m_player->GetAtk());
				CreateDamageFont(m_player->GetAtk());
				//�_���[�W���󂯂����̃R���{�X�e�[�g�Ɍ��݂̃R���{�X�e�[�g��������
				m_player->SetDamagedComboState(m_player->GetNowComboState());
			}
		}
	}
	
	//�q�[���[�̃X�L���̓����蔻��
	const auto& SkillCollisions = g_collisionObjectManager->FindCollisionObjects("SkillAttack");
	//�R���W�����̔z���for���ŉ�
	for (auto collision : SkillCollisions)
	{
		//���g�̃L�����R���ƏՓ˂�����
		if (collision->IsHit(characon) == true)
		{
			//���Ԋu�Ń_���[�W���󂯂�
			if (m_damageFlag == false)
			{
				m_damageFlag = true;
				Damage(m_player->GetSkillAtk());
				CreateDamageFont(m_player->GetSkillAtk());
			}
		}
	}

	//�E�B�U�[�h�̃t�@�C���[�{�[���̓����蔻��
	const auto& FireBallCollisions = g_collisionObjectManager->FindCollisionObjects("fireball");
	//�R���W�����̔z���for���ŉ�
	for (auto collision : FireBallCollisions)
	{
		//���g�̃L�����R���ƏՓ˂�����
		if (collision->IsHit(characon) == true)
		{
			auto fireball = FindGO<FireBall>(collision->GetName());
			Damage(fireball->GetAtk());
			CreateDamageFont(fireball->GetAtk());
			//�Ԃ������̂Ńt�@�C���[�{�[���������t���O�𗧂Ă�
			fireball->SetHitEnemeyFlag(true);
		}
	}

	//�E�B�U�[�h�̃t���C���s���[�̓����蔻��
	const auto& FlamePillarCollisions = g_collisionObjectManager->FindCollisionObjects("flamepillar");
	//�R���W�����̔z���for���ŉ�
	for (auto collision : FlamePillarCollisions)
	{
		//���g�̃L�����R���ƏՓ˂�����
		if (collision->IsHit(characon) == true)
		{
			//���Ԋu�Ń_���[�W���󂯂�
			if (m_damageFlag == false)
			{
				auto flamepillar = FindGO<FlamePillar>(collision->GetName());

				m_damageFlag = true;
				Damage(flamepillar->GetAtk());
				CreateDamageFont(flamepillar->GetAtk());
			}
		}
	}

}

void Lich::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�{�[�������^�C�~���O
	if (wcscmp(eventName, L"Create_Ball") == 0)
	{
		//�{�[������
		FireBall* fireball = NewGO<FireBall>(0, "darkball");
		fireball->SetLich(this);
	}

	//�_�[�N�E�H�[�������^�C�~���O
	if (wcscmp(eventName, L"Create_DarkWall") == 0)
	{
		m_CreateDarkWallFlag = true;
	}
	//�_�[�N�E�H�[�������I���
	if (wcscmp(eventName, L"CreateEnd_DarkWall") == 0)
	{
		m_CreateDarkWallFlag = false;
	}

	//���e�I�����^�C�~���O
	if (wcscmp(eventName, L"createMeteo") == 0)
	{
		m_darkMeteorite->SetShotStartFlag(true);
		SetTargetPosition();
		m_darkMeteorite->SetTargetPosition(m_targetPosition);
	}

	//
	//if (wcscmp(eventName, L"Stop") == 0)
	//{
	//	m_modelRender.
	//}
	//
	if (wcscmp(eventName, L"Summon") == 0)
	{
		//�����X�^�[����������
		Summon* summon = NewGO<Summon>(0, "summon");
		summon->SetLich(this);
	}

}

void Lich::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	//m_hpFont.Draw(rc);
}


