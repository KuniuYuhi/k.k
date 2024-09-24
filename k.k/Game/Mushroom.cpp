#include "stdafx.h"
#include "Mushroom.h"
#include "MobEnemyMovement.h"

#include "Brave.h"

#include "EnemyObjectPool.h"

#include "EnemyManager.h"

#include "KnockBackInfoManager.h"

#include "UseEffect.h"


using namespace KnockBackInfo;


namespace {
	const float ATTACK_COLLISION_RADIUS = 18.0f;
}

Mushroom::~Mushroom()
{
}

bool Mushroom::Start()
{
	//����ăI�u�W�F�N�g�v�[���Ɋi�[���ꂽ���̂��Ƃ��l���Ď��S�t���O�����Z�b�g
	SetDieFlag(false);

	m_player = FindGO<Brave>("Brave");

	//���f���̓ǂݍ���
	InitModel();

	//�L�����R��������
	if (m_charaCon == nullptr)
	{
		CreateCharacterController();
		m_charaCon.get()->Init(22.0f, 10.0f, m_position);
	}
	else
	{
		m_charaCon.get()->SetPosition(m_position);
	}

	//�X�e�[�^�X��������
	m_status.InitCommonEnemyStatus("Mushroom");
	m_commonStatus.InitMobEnemyCommonStatus("Mushroom");

	//�R���|�[�l���g�����������Ă��Ȃ��Ȃ�
	if (!m_isSettingComponents)
	{
		InitComponents();
	}

	//�G�l�~�[�}�l�[�W���[�̃��X�g�Ɏ��g������
	EnemyManager::GetInstance()->AddMobEnemyToList(this);


	//�R���e�L�X�g���Ȃ���������
	if (!m_mushroomContext)
	{
		//�X�e�[�g�R���e�L�X�g�̏�����
		m_mushroomContext = std::make_unique<MushroomStateContext>();
	}

	//�ŏ��̃X�e�[�g�͏o���X�e�[�g
	m_mushroomContext.get()->Init(this, enMushroomState_Appear);



	return true;
}

void Mushroom::LoadAnimationClip()
{
	m_animationClip[enMushroomAnimClip_Idle].Load("Assets/animData/character/Mushroom/Idle_Normal.tka");
	m_animationClip[enMushroomAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enMushroomAnimClip_Run].Load("Assets/animData/character/Mushroom/Run.tka");
	m_animationClip[enMushroomAnimClip_Run].SetLoopFlag(true);
	m_animationClip[enMushroomAnimClip_Attack].Load("Assets/animData/character/Mushroom/Attack1.tka");
	m_animationClip[enMushroomAnimClip_Attack].SetLoopFlag(false);
	m_animationClip[enMushroomAnimClip_Hit].Load("Assets/animData/character/Mushroom/Damage.tka");
	m_animationClip[enMushroomAnimClip_Hit].SetLoopFlag(false);
	m_animationClip[enMushroomAnimClip_Die].Load("Assets/animData/character/Mushroom/Die.tka");
	m_animationClip[enMushroomAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enMushroomAnimClip_Victory].Load("Assets/animData/character/Mushroom/Victory.tka");
	m_animationClip[enMushroomAnimClip_Victory].SetLoopFlag(true);
	m_animationClip[enMushroomAnimClip_Appear].Load("Assets/animData/character/Mushroom/Appear.tka");
	m_animationClip[enMushroomAnimClip_Appear].SetLoopFlag(false);
}

void Mushroom::InitComponents()
{
	//��{�I�ȃR���|�[�l���g���Z�b�e�B���O
	SettingDefaultComponent();
	//���̃N���X�Œǉ��������R���|�[�l���g���Z�b�e�B���O
	AddMoreComponent();

	m_isSettingComponents = true;
}

void Mushroom::ReleaseThis()
{
	//�G�l�~�[�Ǘ��}�l�[�W���[�̃��X�g���玩�g���폜
	EnemyManager::GetInstance()->RemoveMobEnemyToList(this);
	//�L�����R�����Z�b�g
	m_charaCon.reset();
	//�I�u�W�F�N�g�v�[���Ɏ��g�̃I�u�W�F�N�g��Ԃ�
	EnemyObjectPool::GetInstance()->OnRelease("Mushroom", this);
}

void Mushroom::AddMoreComponent()
{

}

void Mushroom::CreateCollisionObject()
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

void Mushroom::ProcessHit(DamageInfo damageInfo)
{
	//�m�b�N�o�b�N�p�^�[�����擾
	//todo ���x���ɂ���ĕύX
	m_hitKnockBackPattern = damageInfo.knockBackPattern;

	//�m�b�N�o�b�N�̎��ԊԊu���擾
	m_knockBackTimeScale = damageInfo.knockBackTimeScale;

	//�_���[�W���󂯂�
	TakeDamage(damageInfo.attackPower);

	//�q�b�g�X�e�[�g�ɐ؂�ւ���
	m_mushroomContext.get()->ChangeMushroomState(this, enMushroomState_Hit);
}


void Mushroom::Attack()
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
	m_mushroomContext.get()->ChangeMushroomState(this, enMushroomState_Attack);

}

void Mushroom::EntryAttackActionProcess()
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

void Mushroom::UpdateAttackActionProcess()
{
	//�U���p�R���W�����𐶐��ł���Ȃ�
	if (m_isCreateAttackCollision)
	{
		//�R���W�����𐶐�
		CreateCollisionObject();
	}
}

void Mushroom::ExitAttackActionProcess()
{
	//�A�N�V�������I���
	ActionDeactive();
	//���̍U���܂ł̃^�C�}�[�����Z�b�g
	m_attackIntarvalTimer = 0.0f;

}

void Mushroom::EntryHitActionProcess()
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

void Mushroom::UpdateHitActionProcess()
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
				m_mushroomContext.get()->ChangeMushroomState(this, enMushroomState_Die);
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

void Mushroom::ExitHitActionProcess()
{
	if (IsDie()) return;


	//�A�N�V�������I���
	ActionDeactive();
}

void Mushroom::DieProcess()
{
	//�A�C�e���𗎂Ƃ������߂�
	if (IsDropBuffItem())
	{
		DropBuffItem();
	}

	//�_���[�W�ɂ���Ă��ꂽ���̏���
	DieFromDamage();
}

void Mushroom::WinProcess()
{
	m_mushroomContext.get()->ChangeMushroomState(this, enMushroomState_Victory);
}

void Mushroom::ForceChangeStateIdle()
{
	m_mushroomContext.get()->ChangeMushroomState(this, enMushroomState_Idle);
	m_moveSpeed = g_vec3Zero;
}

void Mushroom::DieFlomOutside(bool isPlayEffect)
{
	//�L�����R�����Z�b�g
	m_charaCon.reset();
	//�I�u�W�F�N�g�v�[���Ɏ��g�̃I�u�W�F�N�g��Ԃ�
	EnemyObjectPool::GetInstance()->OnRelease("Mushroom", this);

	//�G�t�F�N�g���Đ����Ȃ��Ȃ�
	if (!isPlayEffect) return;

	//���S�G�t�F�N�g����
	UseEffect* effect = NewGO<UseEffect>(0, "DieEffect");
	effect->PlayEffect(enEffect_Mob_Dead,
		m_position, g_vec3One * 5.0f, Quaternion::Identity, false);

}

void Mushroom::ProcessCommonTranstion()
{
	if (fabsf(GetMoveSpeed().x) >= 0.001f ||
		fabsf(GetMoveSpeed().z) >= 0.001f)
	{
		m_mushroomContext.get()->ChangeMushroomState(this, enMushroomState_Run);
	}
	else
	{
		m_mushroomContext.get()->ChangeMushroomState(this, enMushroomState_Idle);
	}

}

void Mushroom::TurnToPlayer()
{
	TurnToTarget();
}

void Mushroom::Update()
{

	//�������~�߂�v�����Ȃ����菈��������
	if (!IsStopRequested())
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
	m_mushroomContext.get()->UpdateCurrentState();
	m_mushroomContext.get()->PlayAnimationCurrentState();

	//���f���̃g�����X�t�H�[���̐ݒ�
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	//���f�������X�V
	m_modelRender.Update();
}




void Mushroom::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

void Mushroom::InitModel()
{
	//���f�������������Ă��Ȃ��Ȃ�
	if (!m_modelRender.IsInit())
	{
		//�A�j���[�V�����̓ǂݍ���
		LoadAnimationClip();

		//���f����������
		m_modelRender.Init(
			"Assets/modelData/character/Mushroom/Mushroom.tkm",
			L"Assets/shader/ToonTextrue/lamp_Mushroom.DDS",
			m_animationClip,
			enMushroomAnimClip_num,
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
	m_headBoonId = m_modelRender.FindBoneID(L"mushroom_spine03");

}

void Mushroom::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
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
