#include "stdafx.h"

#include "Arrow.h"
#include "Brave.h"
#include "Bow.h"
#include "DamageProvider.h"
#include "KnockBackInfoManager.h"

using namespace KnockBackInfo;


namespace {
	const float NORMAL_SHOT_ARROW_Y_ROT_OFFSET = 180.0f;	//�ʏ�U���̖�̉�]�̃I�t�Z�b�g
}

Arrow::Arrow()
{
}

Arrow::~Arrow()
{
	if (m_arrowCollision != nullptr)
	{
		DeleteGO(m_arrowCollision);
	}

}

bool Arrow::Start()
{
	m_brave = FindGO<Brave>("Brave");

	ArrowStatus normalStatus,skillStatus;
	normalStatus.InitArrowStatus("Normal");
	skillStatus.InitArrowStatus("Skill");

	m_statusMap.insert(std::make_pair(enNormalShot, normalStatus));
	m_statusMap.insert(std::make_pair(enSkillShot, skillStatus));

	//m_status.InitPlayerStatus("Normal");

	Init();

	InitComponent();

	return true;
}

void Arrow::Update()
{
	ManageShotPatternState();

	m_damageProvider->UpdateComponent();

	//���f���̍X�V�����BTRS�͈ړ����ɐݒ�
	m_arrowModelRender.Update();
}

void Arrow::Init()
{
	//��f���̏�����
	m_arrowModelRender.Init("Assets/modelData/character/Player/NewHero/Arrow.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,0,enModelUpAxisZ
	);

	//��̐^�񒆂Ɛ�[�̃{�[��ID���擾
	m_arrowCentorBoonId = m_arrowModelRender.FindBoneID(L"Center");
	m_arrowTipBoonId = m_arrowModelRender.FindBoneID(L"tip");

	//��������W�̃{�[��ID���擾
	m_armedArrowBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_r");

}

void Arrow::InitComponent()
{
	AddComponent<DamageProvider>();
	m_damageProvider = GetComponent<DamageProvider>();
}

void Arrow::ManageShotPatternState()
{
	switch (m_enShotPatternState)
	{
	case Arrow::enNormalShot:
		UpdateNormalShotState();
		break;
	case Arrow::enSkillShot:
		UpdateSkillShotState();
		break;
	case Arrow::enNone:
		UpdateNoneState();
		break;
	default:
		break;
	}
}

void Arrow::UpdateNormalShotState()
{
	if (m_deleteTimer >= 2.0f)
	{
		DeleteGO(this);
	}

	//todo �����蔻�菈��
	if (m_damageProvider->IsHit())
	{
		DeleteGO(this);
		return;
	}


	m_deleteTimer += g_gameTime->GetFrameDeltaTime();

	ShotArrowMove(enNormalShot);

	/*Vector3 moveSpeed = m_forward;

	moveSpeed *= m_statusMap.at(enNormalShot).GetArrowSpeed();

	m_arrowModelRender.AddPosition(moveSpeed);

	m_arrowCenterMatrix = m_arrowModelRender.GetBone(m_arrowCentorBoonId)->GetWorldMatrix();

	m_arrowCollision->SetWorldMatrix(m_arrowCenterMatrix);

	m_arrowCollision->Update();*/
}

void Arrow::UpdateSkillShotState()
{

	if (m_deleteTimer >= 2.0f)
	{
		DeleteGO(this);
	}


	//��莞�Ԃ��ƂɃ_���[�WID��ύX���đ��i�q�b�g�ł���悤�ɂ���
	if (m_attackInfoUpdateTimer > m_attackInfoUpdateTimeLimit)
	{
		//ID��V�����ݒ�
		m_damageProvider->SetAttackId(KnockBackInfoManager::GetInstance()->GetAddAttackId());
		//�^�C�}�[���Z�b�g
		m_attackInfoUpdateTimer = 0.0f;
	}

	//�e��^�C�}�[���Z
	m_deleteTimer += g_gameTime->GetFrameDeltaTime();
	m_attackInfoUpdateTimer+= g_gameTime->GetFrameDeltaTime();

	ShotArrowMove(enSkillShot);

	/*Vector3 moveSpeed = m_forward;

	moveSpeed *= m_statusMap.at(enSkillShot).GetArrowSpeed();

	m_arrowModelRender.AddPosition(moveSpeed);

	m_arrowCenterMatrix = m_arrowModelRender.GetBone(m_arrowCentorBoonId)->GetWorldMatrix();

	m_arrowCollision->SetWorldMatrix(m_arrowCenterMatrix);

	m_arrowCollision->Update();*/
}

void Arrow::UpdateNoneState()
{
	//���[��ԂȂ�ړ������Ȃ�
	if (m_enWeaponState == enStowed) return;

	//�v���C���[�̎�ɒǏ]
	MoveArmed();


}

void Arrow::ShotArrowMove(EnShotPatternState shotPattern)
{
	Vector3 moveSpeed = m_forward;
	moveSpeed *= m_statusMap.at(shotPattern).GetArrowSpeed();

	//���W�����Z
	m_arrowModelRender.AddPosition(moveSpeed);
	//�|�̐^�񒆂̃��[���h���W���擾
	m_arrowCenterMatrix = m_arrowModelRender.GetBone(m_arrowCentorBoonId)->GetWorldMatrix();
	//�R���W�����Ƀ��[���h���W��ݒ�A�X�V
	m_arrowCollision->SetWorldMatrix(m_arrowCenterMatrix);
	m_arrowCollision->Update();
}

void Arrow::FixedAttaackArrowTransform()
{
	//���[���h���W�����[�J�����W�ɑ��
	m_moveAttackArrowPosition.x = m_arrowMatrix.m[3][0];
	m_moveAttackArrowPosition.y = m_arrowMatrix.m[3][1];
	m_moveAttackArrowPosition.z = m_arrowMatrix.m[3][2];
	//�O�����̂ق��ɉ�]
	Quaternion rotation;
	rotation.SetRotationYFromDirectionXZ(m_forward);
	//���̂܂܂��Ɖ�]�����������̂ŃI�t�Z�b�g��������
	rotation.AddRotationDegY(NORMAL_SHOT_ARROW_Y_ROT_OFFSET);
	//�g�����X�t�H�[���̐ݒ�
	m_arrowModelRender.SetTransform(m_moveAttackArrowPosition, rotation, g_vec3One);
}

void Arrow::CreateCollision(
	EnShotPatternState shotPatternState, Vector3 createPosition, Quaternion rotation)
{
	//��̒ʏ�U���p�̓����蔻��̐���
	m_arrowCollision = NewGO<CollisionObject>(
		0, g_collisionObjectManager->m_playerAttackCollisionName);
	//�R���W�����̐���҂����g�ɐݒ�
	m_arrowCollision->SetCreatorName(GetName());
	//�R���W�����̌`��̓{�b�N�X
	m_arrowCollision->CreateBox(
		createPosition,
		rotation,
		m_statusMap.at(shotPatternState).GetCollisionSize()
	);
	m_arrowCollision->SetIsEnableAutoDelete(false);
	//��̐^�񒆂̃{�[���������Ă���
	m_arrowCenterMatrix = m_arrowModelRender.GetBone(m_arrowCentorBoonId)->GetWorldMatrix();
	m_arrowCollision->SetWorldMatrix(m_arrowCenterMatrix);
	m_arrowCollision->Update();

}

bool Arrow::IsHitCollision()
{


	return false;
}

void Arrow::SetShotArrowParameters(
	EnShotPatternState shotpatternState, Vector3 forward)
{
	//�U���p�^�[����ݒ�
	m_enShotPatternState = shotpatternState;
	//�O�����̐ݒ�Ɛ��K��
	m_forward = forward;
	m_forward.Normalize();

	//��̃g�����X�t�H�[���̏C��
	FixedAttaackArrowTransform();
	
	//�ʏ�U�����X�L���U���ɉ����������蔻��̍쐬
	CreateCollision(
		shotpatternState,
		m_arrowModelRender.GetPosition(),
		m_arrowModelRender.GetRotation()
	);

}

void Arrow::ChangeStowed()
{
	SetWeaponState(enStowed);
	//����̈ʒu��ύX
	MoveStowed();
}

void Arrow::ChangeArmed()
{
	SetWeaponState(enArmed);
}

void Arrow::MoveStowed()
{
	//���[��Ԃ̍��W�ɐݒ�
	m_arrowModelRender.SetPosition(m_stowedPosition);
}

void Arrow::MoveArmed()
{
	m_arrowMatrix = m_brave->GetModelRender().GetBone(m_armedArrowBoonId)->GetWorldMatrix();
	//���[���h���W��ݒ�
	m_arrowModelRender.SetWorldMatrix(m_arrowMatrix);

}

void Arrow::Render(RenderContext& rc)
{
	m_arrowModelRender.Draw(rc);
}
