#include "stdafx.h"
#include "Slime.h"

#include "MobEnemyMovement.h"

#include "Brave.h"

#include "EnemyObjectPool.h"

#include "EnemyManager.h"

#include "KnockBackInfoManager.h"


using namespace KnockBackInfo;


namespace {
	const float ATTACK_COLLISION_RADIUS = 18.0f;
}

Slime::~Slime()
{
}

bool Slime::Start()
{
	
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
	m_status.InitCommonEnemyStatus("Slime");
	m_commonStatus.InitMobEnemyCommonStatus("Slime");

	//�R���|�[�l���g�����������Ă��Ȃ��Ȃ�
	if (!m_isSettingComponents)
	{
		InitComponents();
	}

	//�G�l�~�[�}�l�[�W���[�̃��X�g�Ɏ��g������
	EnemyManager::GetInstance()->AddMobEnemyToList(this);


	//�R���e�L�X�g���Ȃ���������
	if (!m_slimeContext)
	{
		//�X�e�[�g�R���e�L�X�g�̏�����
		m_slimeContext = std::make_unique<SlimeStateContext>();
	}

	//�ŏ��̃X�e�[�g�͏o���X�e�[�g
	m_slimeContext.get()->Init(this, enSlimeState_Appear);

	

	return true;
}

void Slime::LoadAnimationClip()
{
	m_animationClip[enSlimeAnimClip_Idle].Load("Assets/animData/character/Slime/Idle_Normal.tka");
	m_animationClip[enSlimeAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enSlimeAnimClip_Run].Load("Assets/animData/character/Slime/Run.tka");
	m_animationClip[enSlimeAnimClip_Run].SetLoopFlag(true);
	m_animationClip[enSlimeAnimClip_Attack].Load("Assets/animData/character/Slime/Attack1.tka");
	m_animationClip[enSlimeAnimClip_Attack].SetLoopFlag(false);
	m_animationClip[enSlimeAnimClip_Hit].Load("Assets/animData/character/Slime/Damege.tka");
	m_animationClip[enSlimeAnimClip_Hit].SetLoopFlag(false);
	m_animationClip[enSlimeAnimClip_Die].Load("Assets/animData/character/Slime/Die.tka");
	m_animationClip[enSlimeAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enSlimeAnimClip_Victory].Load("Assets/animData/character/Slime/Victory.tka");
	m_animationClip[enSlimeAnimClip_Victory].SetLoopFlag(true);
	m_animationClip[enSlimeAnimClip_Appear].Load("Assets/animData/character/Slime/Appear.tka");
	m_animationClip[enSlimeAnimClip_Appear].SetLoopFlag(false);
}

void Slime::InitComponents()
{
	//��{�I�ȃR���|�[�l���g���Z�b�e�B���O
	SettingDefaultComponent();
	//���̃N���X�Œǉ��������R���|�[�l���g���Z�b�e�B���O
	AddMoreComponent();

	m_isSettingComponents = true;
}

void Slime::ReleaseThis()
{
	//�G�l�~�[�Ǘ��}�l�[�W���[�̃��X�g���玩�g���폜
	EnemyManager::GetInstance()->RemoveMobEnemyToList(this);
	//�L�����R�����Z�b�g
	m_charaCon.reset();
	//�I�u�W�F�N�g�v�[���Ɏ��g�̃I�u�W�F�N�g��Ԃ�
	EnemyObjectPool::GetInstance()->OnRelease("Slime", this);
}

void Slime::AddMoreComponent()
{

}

void Slime::CreateCollisionObject()
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

void Slime::ProcessHit(DamageInfo damageInfo)
{
	//�m�b�N�o�b�N�p�^�[�����擾
	//todo ���x���ɂ���ĕύX
	m_hitKnockBackPattern = damageInfo.knockBackPattern;

	//�m�b�N�o�b�N�̎��ԊԊu���擾
	m_knockBackTimeScale = damageInfo.knockBackTimeScale;

	//�q�b�g�X�e�[�g�ɐ؂�ւ���
	m_slimeContext.get()->ChangeSlimeState(this, enSlimeState_Hit);
}


void Slime::Attack()
{
	if (IsAction()) return;

	//�U���\�łȂ��Ȃ珈�����Ȃ�
	if (!IsAttackable()||m_isWaitingFlag) return;

	//���͈͓̔��Ƀv���C���[��������U���ł���
	float toPlayerDistance = CalcDistanceToTargetPosition(m_player->GetPosition());

	//���g����v���C���[�Ɍ����������̒��������̋����łȂ��Ȃ珈�����Ȃ�
	if (toPlayerDistance > m_status.GetApproachDistance()) return;

	//�U������I�I
	//�X�e�[�g��؂�ւ���
	m_slimeContext.get()->ChangeSlimeState(this, enSlimeState_Attack);

}

void Slime::EntryAttackActionProcess()
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

void Slime::UpdateAttackActionProcess()
{
	//�U���p�R���W�����𐶐��ł���Ȃ�
	if (m_isCreateAttackCollision)
	{
		//�R���W�����𐶐�
		CreateCollisionObject();
	}
}

void Slime::ExitAttackActionProcess()
{
	//�A�N�V�������I���
	ActionDeactive();
	//���̍U���܂ł̃^�C�}�[�����Z�b�g
	m_attackIntarvalTimer = 0.0f;

}

void Slime::EntryHitActionProcess()
{
	//m_hitKnockBackPattern = enKBPattern_SlightAirborneRetreat;
	//�m�b�N�o�b�N����O�̏���
	SettingKnockBackProcess();
	//�m�b�N�o�b�N�J�E���g���Z�b�g
	count = 0.0f;
	//�d���^�C�}�[�����Z�b�g
	m_starkTimer = 0.0f;
	//�U������������Ȃ��̂ŃR���W���������t���O�����Z�b�g���Ă���
	m_isCreateAttackCollision = false;
}

void Slime::UpdateHitActionProcess()
{
	//�܂��J�[�u�f�[�^�����Ɉړ�
	if (count < m_curvePointList.size())
	{
		//�m�b�N�o�b�N�ɂ��ړ�
		KnockBackMove(count);
		//�^�C���X�P�[�������Z
		count+= m_knockBackTimeScale;
	}
	//���ɋ󒆂ɕ����Ă�����n�ʂɍ~���
	else if(m_charaCon.get()->IsJump())
	{
		KnockBackGravityFall();

	}
	//�Ō�ɏ����d�������ċ��ʃX�e�[�g�����Ɉڍs
	else
	{
		//�A�j���[�V�������I�������
		if (GetModelRender().IsPlayingAnimation() == false)
		{
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

void Slime::ProcessCommonTranstion()
{
	if (fabsf(GetMoveSpeed().x) >= 0.001f ||
		fabsf(GetMoveSpeed().z) >= 0.001f)
	{
		m_slimeContext.get()->ChangeSlimeState(this,enSlimeState_Run);
	}
	else
	{
		m_slimeContext.get()->ChangeSlimeState(this,enSlimeState_Idle);
	}

}

void Slime::TurnToPlayer()
{
	TurnToTarget();
}

void Slime::Update()
{
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		//ReleaseThis();
		//return;
	}



	//�U������
	Attack();
	//�L�����N�^�[�̈ړ�
	ChaseMovement(m_player->GetPosition());
	//��]
	Rotation();

	//�����蔻��
	CheckSelfCollision();

	//�R���e�L�X�g�̏���
	m_slimeContext.get()->UpdateCurrentState();
	m_slimeContext.get()->PlayAnimationCurrentState();

	//���f���̃g�����X�t�H�[���̐ݒ�
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	//���f�������X�V
	m_modelRender.Update();
}




void Slime::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

void Slime::InitModel()
{
	//���f�������������Ă��Ȃ��Ȃ�
	if (!m_modelRender.IsInit())
	{
		//�A�j���[�V�����̓ǂݍ���
		LoadAnimationClip();

		//���f����������
		m_modelRender.Init(
			"Assets/modelData/character/Slime/slime.tkm",
			L"Assets/shader/ToonTextrue/lamp_Slime.DDS",
			m_animationClip,
			enSlimeAnimClip_num,
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
	m_headBoonId= m_modelRender.FindBoneID(L"Head");

}

void Slime::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
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
