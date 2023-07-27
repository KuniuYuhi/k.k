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

#include "LichAction.h"



namespace {
	const float SCALE_UP = 3.0f;									//�L�����N�^�[�̃T�C�Y
	const Vector3 FIRST_POSITION = Vector3(0.0f, 0.0f, -250.0f);	//�ŏ��̍��W
	const float DISTANCE = 1500.0f;									//�v���C���[�𔭌��ł��鋗��


	//�X�e�[�^�X
	int MAXHP = 2000;
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

	m_hpFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_hpFont.SetScale(2.0f);
	m_hpFont.SetPosition(-800.0f, 500.0f);

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
	int NowActorMP = m_status.hp;
	int NowActorMaxMP = m_status.maxHp;
	wchar_t MP[255];
	swprintf_s(MP, 255, L"HP %3d/%d", NowActorMP, NowActorMaxMP);
	m_hpFont.SetText(MP);

	if (m_CreateDarkWallFlag == true)
	{
		CreateDarkWall();
	}

	//��_���[�W�̓����蔻��
	DamageCollision(m_charaCon);

	//�|���ꂽ�瑼�̏��������s���Ȃ��悤�ɂ���
	if (m_dieFlag==true)
	{
		ManageState();
		PlayAnimation();
		m_modelRender.Update();
		return;
	}

	//�C���^�[�o���̌v�Z
	AttackInterval(m_attackIntervalTime);
	DamageInterval(m_damageIntervalTime);

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
	m_moveSpeed = calcVelocity(m_status);

	//���̋����ɂȂ����炻��ȏ㓮���Ȃ�
	if (IsDistanceToPlayer() == true)
	{
		//�ړ����Ȃ��悤�ɂ���
		m_moveSpeed = Vector3::Zero;
	}
	//�U�����Ȃ�ړ����Ȃ�
	else if (IsAttackEntable() != true)
	{
		//�ړ����Ȃ��悤�ɂ���
		m_moveSpeed = Vector3::Zero;
	}
	//�v���C���[����������
	else if (IsFindPlayer(m_distance) == true)
	{
		//�ړ�����
		m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	}
	else
	{
		//�ړ����Ȃ��悤�ɂ���
		m_moveSpeed = Vector3::Zero;
	}

	
	Rotation();

}

void Lich::Damage(int attack)
{
	if (m_status.hp > 0)
	{
		//���m���ŋ���
		if (Isflinch() == true)
		{
			//�Z�̓r����������Ȃ�
			m_CreateDarkWallFlag = false;
			SetNextAnimationState(enAnimationState_Damage);
		}

		
		m_status.hp -= attack;
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


bool Lich::Attack()
{
	//�A�^�b�N�P�̍U���͈͂Ƀ^�[�Q�b�g��������
	if (IsFindPlayer(m_InfoAboutAttack.m_Attack_1Distance) == true)
	{
		//�C���^�[�o�����Ȃ��Ȃ�U���\
		if (m_attackFlag == false)
		{
			SetNextAnimationState(enAnimationState_Attack_1);
			m_attackFlag = true;
			//�U���͈͂Ƀ^�[�Q�b�g������
			m_attackRangeFlag = true;

			return true;
		}
	}
	else
	{
		//�U���͈͂Ƀ^�[�Q�b�g�����Ȃ�
		m_attackRangeFlag = false;
	}

	return false;
}

bool Lich::Attack2()
{
	//�A�^�b�N�P�̍U���͈͂Ƀ^�[�Q�b�g��������
	if (IsFindPlayer(m_InfoAboutAttack.m_Attack_2Distance) == true)
	{
		//�C���^�[�o�����Ȃ��Ȃ�U���\
		if (m_attackFlag == false)
		{
			SetNextAnimationState(enAnimationState_Attack_2);
			m_attackFlag = true;
			//�U���͈͂Ƀ^�[�Q�b�g������
			m_attackRangeFlag = true;

			return true;
		}
	}
	else
	{
		//�U���͈͂Ƀ^�[�Q�b�g�����Ȃ�
		m_attackRangeFlag = false;
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

	
	Move();

	

	
	if (m_attackFlag == false)
	{
		//�A�N�V���������߂�
		m_lichAction->NextAction();
		
		
		m_attackFlag = true;

		////�͈͂�������
		//if (Attack2() == true)
		//{
		//	return;
		//}

		//if (Attack() == true)
		//{
		//	return;
		//}
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
	case Lich::enAnimationState_Attack_3_start:
		break;
	case Lich::enAnimationState_Attack_3_main:
		break;
	case Lich::enAnimationState_Damage:
		//��_���[�W�X�e�[�g���쐬����
		m_state = new LichStateDamage(this);
		break;
	case Lich::enAnimationState_Die:
		//Die�X�e�[�g���쐬����
		m_state = new LichStateDie(this);
		break;
	default:
		// �����ɗ�����X�e�[�g�̃C���X�^���X�쐬�����̒ǉ��Y��B
		std::abort();
		break;
	}
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
		//�U���p�^�[�����Ȃ���Ԃɂ���
		//m_enAttackPatternState = enAttackPattern_None;
		//���ʂ̏�ԑJ�ڏ����Ɉڍs
		ProcessCommonStateTransition();
	}
}

void Lich::OnProcessAttack_2StateTransition()
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

void Lich::CreateDarkWall()
{
	//�{�[���擾
	DarkWall* darkball = NewGO<DarkWall>(0, "darkwall");
	darkball->SetLich(this);
}

void Lich::DamageCollision(CharacterController& characon)
{
	//�����o������
	//���񂾂珈�������Ȃ�todo

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
}

void Lich::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	m_hpFont.Draw(rc);
}