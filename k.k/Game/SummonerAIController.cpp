#include "stdafx.h"
#include "SummonerAIController.h"
#include "Level3DRender.h"
#include "Summoner.h"
#include "Brave.h"


#include "SummonerInfo.h"

#include "DarkBall.h"
#include "DarkMeteorite.h"


using namespace SummonerSkillType;


namespace {
	const Vector3 DARKMETEORITE_CREATE_POSITION = { 0.0f,1400.0f,0.0f };
}

SummonerAIController::SummonerAIController()
{
	Start();
}

SummonerAIController::~SummonerAIController()
{
}

void SummonerAIController::Start()
{
	//�T���i�[�̃C���X�^���X��T��
	m_summoner = FindGO<Summoner>("Summoner");
	//�v���C���[�̃C���X�^���X��T��
	m_player = FindGO<Brave>("Brave");



	InitWarpPointsFromStageLevel();
}

void SummonerAIController::UpdateComponent()
{
}

void SummonerAIController::InitWarpPointsFromStageLevel()
{
	Level3DRender levelbg;

	//���x�����烏�[�v����W��ǂݍ���ł���
	levelbg.Init(
		"Assets/level3D/WarpPointsLevel.tkl",
		[&](LevelObjectData& objData)
		{
			//���[�v����W
			if (objData.ForwardMatchName(L"Pos") == true)
			{
				//���W������i�[���Ă���
				m_warpPointsList.emplace_back(objData.position);
				return true;
			}
			return false;
		});

}

void SummonerAIController::DecisionWarpPoint()
{
	Vector3 bossPos = m_summoner->GetPosition();
	Vector3 playerPos = m_player->GetPosition();


	Vector3 warpPos = playerPos;
	float maxLength = 0;

	for (auto pos : m_warpPointsList)
	{
		//�v���C���[���烏�[�v����W�Ɍ������x�N�g�������߂�
		Vector3 diff = pos - playerPos;
		//�������ׂ�
		if (maxLength < diff.Length())
		{
			maxLength = diff.Length();
			warpPos = pos;
		}
	}

	m_warpPosition = warpPos;

}

void SummonerAIController::WarpProcess()
{
	Vector3 m = g_vec3Zero;
	m_summoner->SetPosition(m_warpPosition);
	m_summoner->GetCharaCon()->SetPosition(m_warpPosition);
	m_summoner->GetCharaCon()->Execute(m, 1.0f);
}

void SummonerAIController::CreateShockWaveCollision()
{
	//�_���[�W�v���o�C�_�[�̐ݒ�
	m_summoner->SettingDamageInfo(enSkillType_ShockWave);

	m_shockWaveCollision = NewGO<CollisionObject>(0,g_collisionObjectManager->m_enemyAttackCollisionName);
	//
	m_shockWaveCollision->SetCreatorName(m_summoner->GetName());
	//
	m_shockWaveCollision->CreateSphere(
		m_summoner->GetPosition(),
		Quaternion::Identity,
		240.0f
	);
}

void SummonerAIController::CreateComboAttackCollision()
{
	
	m_comboAttackCollision = NewGO<CollisionObject>(0, g_collisionObjectManager->m_enemyAttackCollisionName);
	//����҂̖��O
	m_comboAttackCollision->SetCreatorName(m_summoner->GetName());
	//�����蔻��̓{�b�N�X
	m_comboAttackCollision->CreateBox(
		m_summoner->GetPosition(),
		Quaternion::Identity,
		{ 90.0f,360.0f,160.0f }
	);
	//���[���h���W��ݒ肷��
	Matrix staffMatrix = m_summoner->GetModelRender().GetBone(m_summoner->GetStaffBoonId())->GetWorldMatrix();
	m_comboAttackCollision->SetWorldMatrix(staffMatrix);

}

void SummonerAIController::CreateThirdComboCollision()
{
	m_comboAttackCollision = NewGO<CollisionObject>(0, g_collisionObjectManager->m_enemyAttackCollisionName);
	//����҂̖��O
	m_comboAttackCollision->SetCreatorName(m_summoner->GetName());
	//�����蔻��̓{�b�N�X
	m_comboAttackCollision->CreateSphere(
		m_summoner->GetPosition(),
		Quaternion::Identity,
		100.0f
	);
	//���[���h���W��ݒ肷��
	Matrix rightHandMatrix = m_summoner->GetModelRender().GetBone(m_summoner->GetRightHandBoonId())->GetWorldMatrix();
	m_comboAttackCollision->SetWorldMatrix(rightHandMatrix);


}

void SummonerAIController::CreateDarkBall()
{
	//�{�[���̖��O�����߂�
	std::string result = std::string("DarkBall") + std::to_string(m_darkBallCreateCount);
	const char* finalStr = result.c_str();
	//�_�[�N�{�[������
	DarkBall* darkBall = NewGO<DarkBall>(0, finalStr);
	//�J�n���W���v�Z
	Vector3 createPos = g_vec3Zero;
	Matrix m = m_summoner->GetModelRender().GetBone(m_summoner->GetLeftHandBoonId())->GetWorldMatrix();
	m.Apply(createPos);


	//���i���ǔ��^�����߂�
	//�v���C���[�Ƃ̋����������قǊm�����オ��

	//�m��
	int probability = rand() % 10;

	//�{�X����v���C���[�Ɍ������x�N�g�����v�Z
	float length = m_summoner->GetDistanceToPlayerPositionValue();

	//���Z����m���͍ő�3�B�����̕�Ԃ�0�`1000
	int add = Math::Lerp((length - 0.0f) / (1000.0f - 0.0f), 0.0f, 3.0f);

	//�m�����������ƒǂ�������悤�ɂȂ�
	if (probability < 4 + add)
	{
		//�ǂ�������
		darkBall->SetChasePatternState();
	}
	else
	{
		//���i
		darkBall->SetStraightPatternState();
	}


	//���Ƃ��̃p�����[�^�̐ݒ�
	darkBall->SetShotMagicBallParameters(
		createPos,
		m_summoner->GetForwardYZero()
	);

	//�����J�E���g���Z
	m_darkBallCreateCount++;

}

void SummonerAIController::DecisionDarkMeteoriteFallPoint()
{
	//�_�[�N���e�I�����n�_��ݒ�
	m_darkMeteoriteFallPoint = m_player->GetPosition();
	m_darkMeteoriteFallPoint.y = 0.0f;

	//�_�[�N���e�I�𐶐�
	m_darkMeteorite = NewGO<DarkMeteorite>(0, "DarkMeteorite");
	//�͈̓G�t�F�N�g���Đ�
	m_darkMeteorite->PlayRangeEffect(m_darkMeteoriteFallPoint);

}

void SummonerAIController::CreateDarkMeteorite()
{
	
	Vector3 createPos = m_darkMeteoriteFallPoint;
	createPos += DARKMETEORITE_CREATE_POSITION;

	//���Ƃ��̃p�����[�^�̐ݒ�
	m_darkMeteorite->SetShotMagicBallParameters(
		createPos,
		m_summoner->GetForwardYZero()
	);
	//���e�I�������n�߂�
	m_darkMeteorite->ShotStartDarkMeteorite();

}
