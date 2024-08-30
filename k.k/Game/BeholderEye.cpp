#include "stdafx.h"
#include "BeholderEye.h"

#include "MobEnemyMovement.h"

#include "Brave.h"

#include "EnemyObjectPool.h"

#include "EnemyManager.h"

#include "KnockBackInfoManager.h"

#include "EyeBall.h"



using namespace KnockBackInfo;


namespace {
	const float ATTACK_COLLISION_RADIUS = 18.0f;
}


BeholderEye::~BeholderEye()
{
}

bool BeholderEye::Start()
{
	m_player = FindGO<Brave>("Brave");

	//���f���̓ǂݍ���
	InitModel();

	//�L�����R��������
	if (m_charaCon == nullptr)
	{
		CreateCharacterController();
		m_charaCon.get()->Init(20.0f, 40.0f, m_position);
	}
	else
	{
		m_charaCon.get()->SetPosition(m_position);
	}

	//�X�e�[�^�X��������
	m_status.InitCommonEnemyStatus("BeholderEye");
	m_commonStatus.InitMobEnemyCommonStatus("BeholderEye");

	//�R���|�[�l���g�����������Ă��Ȃ��Ȃ�
	if (!m_isSettingComponents)
	{
		InitComponents();
	}

	//�G�l�~�[�}�l�[�W���[�̃��X�g�Ɏ��g������
	EnemyManager::GetInstance()->AddMobEnemyToList(this);

	//�R���e�L�X�g���Ȃ���������
	if (!m_beholderEyeContext)
	{
		//�X�e�[�g�R���e�L�X�g�̏�����
		m_beholderEyeContext = std::make_unique<BeholderEyeStateContext>();
	}

	//�ŏ��̃X�e�[�g�͏o���X�e�[�g
	m_beholderEyeContext.get()->Init(this, enBeholderEyeState_Appear);

	


	return true;
}

void BeholderEye::LoadAnimationClip()
{
	m_animationClip[enBeholderEyeAnimClip_Idle].Load("Assets/animData/character/BeholderEye/Idle.tka");
	m_animationClip[enBeholderEyeAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enBeholderEyeAnimClip_Run].Load("Assets/animData/character/BeholderEye/Run.tka");
	m_animationClip[enBeholderEyeAnimClip_Run].SetLoopFlag(true);
	m_animationClip[enBeholderEyeAnimClip_Attack].Load("Assets/animData/character/BeholderEye/Attack.tka");
	m_animationClip[enBeholderEyeAnimClip_Attack].SetLoopFlag(false);
	m_animationClip[enBeholderEyeAnimClip_Hit].Load("Assets/animData/character/BeholderEye/Hit.tka");
	m_animationClip[enBeholderEyeAnimClip_Hit].SetLoopFlag(false);
	m_animationClip[enBeholderEyeAnimClip_Die].Load("Assets/animData/character/BeholderEye/Die.tka");
	m_animationClip[enBeholderEyeAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enBeholderEyeAnimClip_Victory].Load("Assets/animData/character/BeholderEye/Victory.tka");
	m_animationClip[enBeholderEyeAnimClip_Victory].SetLoopFlag(true);
	m_animationClip[enBeholderEyeAnimClip_Appear].Load("Assets/animData/character/BeholderEye/Appear.tka");
	m_animationClip[enBeholderEyeAnimClip_Appear].SetLoopFlag(false);
}

void BeholderEye::InitComponents()
{
	//��{�I�ȃR���|�[�l���g���Z�b�e�B���O
	SettingDefaultComponent();
	//���̃N���X�Œǉ��������R���|�[�l���g���Z�b�e�B���O
	AddMoreComponent();

	m_isSettingComponents = true;
}

void BeholderEye::ReleaseThis()
{
	//�G�l�~�[�Ǘ��}�l�[�W���[�̃��X�g���玩�g���폜
	EnemyManager::GetInstance()->RemoveMobEnemyToList(this);
	//�L�����R�����Z�b�g
	m_charaCon.reset();
	//�I�u�W�F�N�g�v�[���Ɏ��g�̃I�u�W�F�N�g��Ԃ�
	EnemyObjectPool::GetInstance()->OnRelease("BeholderEye", this);
}

void BeholderEye::AddMoreComponent()
{

}

void BeholderEye::CreateCollisionObject()
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

void BeholderEye::ProcessHit(DamageInfo damageInfo)
{
	//�m�b�N�o�b�N�p�^�[�����擾
	//todo ���x���ɂ���ĕύX
	m_hitKnockBackPattern = damageInfo.knockBackPattern;

	//�m�b�N�o�b�N�̎��ԊԊu���擾
	m_knockBackTimeScale = damageInfo.knockBackTimeScale;

	//�_���[�W���󂯂�
	TakeDamage(damageInfo.attackPower);

	//�q�b�g�X�e�[�g�ɐ؂�ւ���
	m_beholderEyeContext.get()->ChangeBeholderEyeState(this, enBeholderEyeState_Hit);
}


void BeholderEye::Attack()
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
	m_beholderEyeContext.get()->ChangeBeholderEyeState(this, enBeholderEyeState_Attack);

}

void BeholderEye::EntryAttackActionProcess()
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

void BeholderEye::UpdateAttackActionProcess()
{
	//�U���p�R���W�����𐶐��ł���Ȃ�
	if (m_isCreateAttackCollision)
	{
		//�R���W�����𐶐�
		CreateCollisionObject();
	}
}

void BeholderEye::ExitAttackActionProcess()
{
	//�A�N�V�������I���
	ActionDeactive();
	//���̍U���܂ł̃^�C�}�[�����Z�b�g
	m_attackIntarvalTimer = 0.0f;

}

void BeholderEye::EntryHitActionProcess()
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

void BeholderEye::UpdateHitActionProcess()
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
				m_beholderEyeContext.get()->ChangeBeholderEyeState(this, enBeholderEyeState_Die);
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

void BeholderEye::ExitHitActionProcess()
{
	if (IsDie()) return;


	//�A�N�V�������I���
	ActionDeactive();
}

void BeholderEye::DieProcess()
{
	//�_���[�W�ɂ���Ă��ꂽ���̏���
	DieFromDamage();
}

void BeholderEye::ProcessCommonTranstion()
{
	if (fabsf(GetMoveSpeed().x) >= 0.001f ||
		fabsf(GetMoveSpeed().z) >= 0.001f)
	{
		m_beholderEyeContext.get()->ChangeBeholderEyeState(this, enBeholderEyeState_Run);
	}
	else
	{
		m_beholderEyeContext.get()->ChangeBeholderEyeState(this, enBeholderEyeState_Idle);
	}

}

void BeholderEye::TurnToPlayer()
{
	TurnToTarget();
}

void BeholderEye::Update()
{
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		//ReleaseThis();
		//return;
	}

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
	m_beholderEyeContext.get()->UpdateCurrentState();
	m_beholderEyeContext.get()->PlayAnimationCurrentState();

	//���f���̃g�����X�t�H�[���̐ݒ�
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	//���f�������X�V
	m_modelRender.Update();
}




void BeholderEye::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

void BeholderEye::InitModel()
{
	//���f�������������Ă��Ȃ��Ȃ�
	if (!m_modelRender.IsInit())
	{
		//�A�j���[�V�����̓ǂݍ���
		LoadAnimationClip();

		//���f����������
		m_modelRender.Init(
			"Assets/modelData/character/BeholderEye/BeholderEye.tkm",
			L"Assets/shader/ToonTextrue/lamp_Slime.DDS",
			m_animationClip,
			enBeholderEyeAnimClip_num,
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
	m_headBoonId = m_modelRender.FindBoneID(L"EyeBallCTRL");


}

void BeholderEye::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�ʏ�U���̃{�[�������^�C�~���O
	if (wcscmp(eventName, L"ShotNormalBall") == 0)
	{
		//�{�[�������
		ShotNormalBall();
	}
	

}

void BeholderEye::ShotNormalBall()
{
	//�{�[���̖��O�����߂�
	std::string result = std::string("EyeBall") + std::to_string(m_eyeBallCount);
	const char* finalStr = result.c_str();

	//�{�[���𐶐�
	EyeBall* m_eyeBall = NewGO<EyeBall>(0, finalStr);

	//�{�[���������W���v�Z
	Vector3 createPos = g_vec3Zero;
	Matrix m = m_modelRender.GetBone(m_headBoonId)->GetWorldMatrix();
	m.Apply(createPos);

	//���Ƃ��̃p�����[�^�̐ݒ�
	m_eyeBall->SetShotMagicBallParameters(
		createPos,
		m_forward
	);

	//�{�[�����������̂ŃJ�E���g���Z
	m_eyeBallCount++;
}

