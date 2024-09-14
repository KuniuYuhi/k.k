#include "stdafx.h"
#include "Cactus.h"

#include "EnemyObjectPool.h"

#include "MobEnemyMovement.h"

#include "Brave.h"

#include "EnemyManager.h"

#include "KnockBackInfoManager.h"

#include "UseEffect.h"


using namespace KnockBackInfo;


namespace {
	const float ATTACK_COLLISION_RADIUS = 18.0f;
}

Cactus::~Cactus()
{
}

bool Cactus::Start()
{
	//����ăI�u�W�F�N�g�v�[���Ɋi�[���ꂽ���̂��Ƃ��l���Ď��S�t���O�����Z�b�g
	SetDieFlag(false);

	m_player = FindGO<Brave>("Brave");


	//���f���̓ǂݍ���
	InitModel();
	
	//�L�����R�������������Ă��Ȃ��Ȃ�
	if (m_charaCon == nullptr)
	{
		CreateCharacterController();
		m_charaCon.get()->Init(24.0f, 10.0f, m_position);
	}
	else
	{
		m_charaCon.get()->SetPosition(m_position);
	}

	//�X�e�[�^�X��������
	m_status.InitCommonEnemyStatus("Cactus");
	m_commonStatus.InitMobEnemyCommonStatus("Cactus");

	//�R���|�[�l���g�����������Ă��Ȃ��Ȃ�
	if (!m_isSettingComponents)
	{
		InitComponents();
	}

	//�G�l�~�[�}�l�[�W���[�̃��X�g�Ɏ��g������
	EnemyManager::GetInstance()->AddMobEnemyToList(this);
	
	//�R���e�L�X�g���Ȃ���������
	if (!m_cactusContext)
	{
		//�X�e�[�g�R���e�L�X�g�̏�����
		m_cactusContext = std::make_unique<CactusStateContext>();
	}

	//�ŏ��̃X�e�[�g�͏o���X�e�[�g
	m_cactusContext.get()->Init(this, enCactusState_Appear);

	return true;
}

void Cactus::LoadAnimationClip()
{
	m_animationClip[enCactusAnimClip_Idle].Load("Assets/animData/character/Cactus/Idle.tka");
	m_animationClip[enCactusAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enCactusAnimClip_Run].Load("Assets/animData/character/Cactus/Run.tka");
	m_animationClip[enCactusAnimClip_Run].SetLoopFlag(true);
	m_animationClip[enCactusAnimClip_Attack].Load("Assets/animData/character/Cactus/Attack1.tka");
	m_animationClip[enCactusAnimClip_Attack].SetLoopFlag(false);
	m_animationClip[enCactusAnimClip_Hit].Load("Assets/animData/character/Cactus/Damage.tka");
	m_animationClip[enCactusAnimClip_Hit].SetLoopFlag(false);
	m_animationClip[enCactusAnimClip_Die].Load("Assets/animData/character/Cactus/Die.tka");
	m_animationClip[enCactusAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enCactusAnimClip_Victory].Load("Assets/animData/character/Cactus/Victory.tka");
	m_animationClip[enCactusAnimClip_Victory].SetLoopFlag(true);
	m_animationClip[enCactusAnimClip_Appear].Load("Assets/animData/character/Cactus/Appear.tka");
	m_animationClip[enCactusAnimClip_Appear].SetLoopFlag(false);
}

void Cactus::InitComponents()
{
	//��{�I�ȃR���|�[�l���g���Z�b�e�B���O
	SettingDefaultComponent();
	//���̃N���X�Œǉ��������R���|�[�l���g���Z�b�e�B���O
	AddMoreComponent();

	m_isSettingComponents = true;
}

void Cactus::ReleaseThis()
{
	//�G�l�~�[�Ǘ��}�l�[�W���[�̃��X�g���玩�g���폜
	EnemyManager::GetInstance()->RemoveMobEnemyToList(this);
	//�L�����R�����Z�b�g
	m_charaCon.reset();
	//�I�u�W�F�N�g�v�[���Ɏ��g�̃I�u�W�F�N�g��Ԃ�
	EnemyObjectPool::GetInstance()->OnRelease("Cactus", this);
}

void Cactus::AddMoreComponent()
{
}

void Cactus::CreateCollisionObject()
{
	m_attackCollision = NewGO<CollisionObject>(0, g_collisionObjectManager->m_enemyAttackCollisionName);
	m_attackCollision->SetCreatorName(GetName());
	m_attackCollision->CreateSphere(
		m_position,
		m_rotation,
		ATTACK_COLLISION_RADIUS
	);
	//���[���h���W�擾
	Matrix HeadMatrix = m_modelRender.GetBone(m_headBoonId)->GetWorldMatrix();
	//�����蔻��̏ꏊ�𓪂̃{�[���̈ʒu�ɕύX
	m_attackCollision->SetWorldMatrix(HeadMatrix);
}

void Cactus::ProcessHit(DamageInfo damageInfo)
{
	//�m�b�N�o�b�N�p�^�[�����擾
	//todo ���x���ɂ���ĕύX
	m_hitKnockBackPattern = damageInfo.knockBackPattern;

	//�m�b�N�o�b�N�̎��ԊԊu���擾
	m_knockBackTimeScale = damageInfo.knockBackTimeScale;

	//�_���[�W���󂯂�
	TakeDamage(damageInfo.attackPower);

	//�q�b�g�X�e�[�g�ɐ؂�ւ���
	m_cactusContext.get()->ChangeCactusState(this, enCactusState_Hit);
}

void Cactus::Attack()
{
	if (IsAction()) return;

	//�U���\�łȂ��Ȃ珈�����Ȃ�
	if (!IsAttackable() || m_isWaitingFlag) return;

	//���͈͓̔��Ƀv���C���[��������U���ł���
	float toPlayerDistance = CalcDistanceToTargetPosition(m_player->GetPosition());

	//���g����v���C���[�Ɍ����������̒��������̋����łȂ��Ȃ珈�����Ȃ�
	if (toPlayerDistance > m_status.GetApproachDistance()) return;

	//�U������I�I
	//�X�e�[�g��؂�ւ���
	m_cactusContext.get()->ChangeCactusState(this, enCactusState_Attack);
}

void Cactus::ProcessCommonTranstion()
{
	if (fabsf(GetMoveSpeed().x) >= 0.001f ||
		fabsf(GetMoveSpeed().z) >= 0.001f)
	{
		m_cactusContext.get()->ChangeCactusState(this, enCactusState_Run);
	}
	else
	{
		m_cactusContext.get()->ChangeCactusState(this, enCactusState_Idle);
	}
}

void Cactus::TurnToPlayer()
{
	TurnToTarget();
}

void Cactus::EntryAttackActionProcess()
{
	//�A�N�V�������ɂ���
	ActionActive();
	//�U������̂Ń^�C�}�[�����Z�b�g
	m_attackIntarvalTimer = 0.0f;

	//�U������ݒ�
	m_damageProvider->SetDamageInfo(
		KnockBackInfoManager::GetInstance()->GetAddAttackId(), m_status.GetCurrentAtk(),
		m_commonStatus.GetNormalAttackTimeScale(),
		m_commonStatus.GetNormalAttackKnockBackPattern()
	);
}

void Cactus::UpdateAttackActionProcess()
{
	//�U���p�R���W�����𐶐��ł���Ȃ�
	if (m_isCreateAttackCollision)
	{
		//�R���W�����𐶐�
		CreateCollisionObject();
	}
}

void Cactus::ExitAttackActionProcess()
{
	//�A�N�V�������I���
	ActionDeactive();
	//���̍U���܂ł̃^�C�}�[�����Z�b�g
	m_attackIntarvalTimer = 0.0f;
}

void Cactus::EntryHitActionProcess()
{
	//�m�b�N�o�b�N����O�̏���
	SettingKnockBackProcess();
	//�m�b�N�o�b�N�J�E���g���Z�b�g
	count = 0.0f;
	//�d���^�C�}�[�����Z�b�g
	m_starkTimer = 0.0f;
	//�U������������Ȃ��̂ŃR���W���������t���O�����Z�b�g���Ă���
	m_isCreateAttackCollision = false;

	PlayHitSound();
}

void Cactus::UpdateHitActionProcess()
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
			//��������ł���Ȃ�
			if (IsDie())
			{
				//���S�X�e�[�g�ɑJ��
				m_cactusContext.get()->ChangeCactusState(this, enCactusState_Die);
				return;
			}
			//�����d�����ċ��ʃX�e�[�g�����Ɉڍs
			if (m_starkTimer >= 0.1f)
			{
				//���ʃX�e�[�g�Ɉڍs
				ProcessCommonTranstion();
			}
			m_starkTimer += g_gameTime->GetFrameDeltaTime();
		}
	}
}

void Cactus::ExitHitActionProcess()
{
	if (IsDie()) return;


	//�A�N�V�������I���
	ActionDeactive();
}

void Cactus::DieProcess()
{
	//�_���[�W�ɂ���Ă��ꂽ���̏���
	DieFromDamage();
}

void Cactus::WinProcess()
{
	m_cactusContext.get()->ChangeCactusState(this, enCactusState_Victory);
}

void Cactus::DieFlomOutside(bool isPlayEffect)
{
	//�L�����R�����Z�b�g
	m_charaCon.reset();
	//�I�u�W�F�N�g�v�[���Ɏ��g�̃I�u�W�F�N�g��Ԃ�
	EnemyObjectPool::GetInstance()->OnRelease("Cactus", this);

	//�G�t�F�N�g���Đ����Ȃ��Ȃ�
	if (!isPlayEffect) return;

	//���S�G�t�F�N�g����
	UseEffect* effect = NewGO<UseEffect>(0, "DieEffect");
	effect->PlayEffect(enEffect_Mob_Dead,
		m_position, g_vec3One * 5.0f, Quaternion::Identity, false);

}

void Cactus::Update()
{
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		/*ReleaseThis();
		return;*/
	}

	//�������~�߂�v��������Ȃ�
	if (IsStopRequested())return;

	//����ł��Ȃ��Ȃ珈������
	if (!IsDie())
	{
		//�U������
		Attack();
		//�L�����N�^�[�̈ړ�
		ChaseMovement(m_player->GetPosition());
		//��]
		Rotation();

		//�����蔻��
		CheckSelfCollision();
	}

	//�R���e�L�X�g�̏���
	m_cactusContext.get()->UpdateCurrentState();
	m_cactusContext.get()->PlayAnimationCurrentState();

	//���f���̃g�����X�t�H�[���̐ݒ�
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	//���f�������X�V
	m_modelRender.Update();
}

void Cactus::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

void Cactus::InitModel()
{
	//���f�������������Ă��Ȃ��Ȃ�
	if (!m_modelRender.IsInit())
	{
		//�A�j���[�V�����̓ǂݍ���
		LoadAnimationClip();

		m_modelRender.Init(
			"Assets/modelData/character/Cactus/Cactus.tkm",
			L"Assets/shader/ToonTextrue/lamp_Cactus.DDS",
			m_animationClip,
			enCactusAnimClip_num,
			enModelUpAxisZ
		);

		//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
		m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
			OnAnimationEvent(clipName, eventName);
		});

	}

	//�g�����X�t�H�[���̐ݒ�
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);

	//���̃{�[��Id���擾
	m_headBoonId = m_modelRender.FindBoneID(L"cactus_leftHandTip");
	
}

void Cactus::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�����蔻�萶���^�C�~���O
	if (wcscmp(eventName, L"Collision_Start") == 0)
	{
		m_isCreateAttackCollision = true;
	}
	//�����蔻�萶���I���
	if (wcscmp(eventName, L"Collision_End") == 0)
	{
		m_isCreateAttackCollision = false;
	}
}


