#include "stdafx.h"
#include "DarkMeteo.h"
#include "InitEffect.h"
#include "Meteo.h"
#include "CharactersInfoManager.h"
#include "GameManager.h"

#include "DecisionMeteoEndpoint.h"

namespace {
	const float DARK_METEO_MAX_SIZE = 20.0f;

	const Vector3 DARK_METEO_POSITION = { 0.0f,550.0f,0.0f };

	const int CREATE_METEO_COUNT = 5;	//���e�I�𐶐����鐔


	const float METEO_START_UP_POS = 20.0f;
	const float METEO_END_UP_POS = -20.0f;

	const float SHOT_METEO_TIMER_LIMMIT = 0.7f;

}

//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
struct IsWallResult :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//�Փ˃t���O�B
	virtual	btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//�n�ʂƂԂ����ĂȂ�������B
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall) {
			//�Փ˂����͕̂ǂł͂Ȃ��B
			isHit = false;
			return 0.0f;
		}
		else
		{
			//�n�ʂƂԂ�������B
		//�t���O��true�ɁB
			isHit = true;
			return 0.0f;
		}

	}
};

DarkMeteo::DarkMeteo()
{
}

DarkMeteo::~DarkMeteo()
{
	if (m_darkMeteoriteEffect != nullptr)
	{
		//�G�t�F�N�g���~�߂�
		m_darkMeteoriteEffect->Stop();
	}

	//�Z�������I����Ă���̍폜�Ȃ̂Ń��e�I�͍폜���Ȃ�
	if (m_endFlag == true)
	{
		return;
	}

	//���e�I���폜
	if (m_meteoList.size() != 0)
	{
		QueryGOs<Meteo>("meteo", [&](Meteo* meteo_) {
			DeleteGO(meteo_);
			return true;
			});
	}

}

bool DarkMeteo::Start()
{
	//�_�[�N���e�I�̍��W�ɍ����𑫂�
	m_darkMeteoPositon += DARK_METEO_POSITION;

	//�_�[�N���e�I�̏�����
	InitDarkMeteoEffect();

	return true;
}

void DarkMeteo::Update()
{
	//�����Ă���ԂɃQ�[�����I�������
	if (GameManager::GetInstance()->GetOutComeState() != 
		GameManager::enOutComeState_None)
	{
		DeleteGO(this);
		return;
	}


	ManageState();
}

void DarkMeteo::ManageState()
{
	switch (m_enDarkMeteoState)
	{
	case DarkMeteo::enDarkMeteoState_Charge:
		OnProcessDarkMeteoChargeTranstion();
		break;
	case DarkMeteo::enDarkMeteoState_ShotMeteo:
		OnProcessShotMeteoTranstion();
		break;
	case DarkMeteo::enDarkMeteoState_ShotDarkMeteo:
		OnProcessShotDarkMeteoTranstion();
		break;
	case DarkMeteo::enDarkMeteoState_End:
		OnProcessEndTranstion();
		break;
	default:
		break;
	}
}

void DarkMeteo::OnProcessDarkMeteoChargeTranstion()
{
	//�ő�܂Ń_�[�N���e�I���傫���Ȃ�����
	if (m_mulScaleDarkMeteo > DARK_METEO_MAX_SIZE)
	{
		//�`���[�W�����t���O���Z�b�g
		m_chargeCompleteFlag = true;
	}
	else
	{
		m_mulScaleDarkMeteo += g_gameTime->GetFrameDeltaTime() * 7.5f;
		//�X�P�[���A�b�v
		m_darkMeteoScale = g_vec3One;
		m_darkMeteoScale *= m_mulScaleDarkMeteo;

	}
	//�g�嗦��ݒ�
	m_darkMeteoriteEffect->SetScale(m_darkMeteoScale);
	m_darkMeteoriteEffect->Update();

}

void DarkMeteo::OnProcessShotMeteoTranstion()
{
	//���Ԋu�Ō���

	if (IsShotMeteo() != true)
	{
		return;
	}
	//���e�I�̍ő吶�����ɒB���Ă��Ȃ��Ȃ琶������
	if (m_shotMeteoCounter < CREATE_METEO_COUNT)
	{
		while (true)
		{
			//���W�̐ݒ�
			Vector3 meteoTargetPos = GetMeteoTargetPosition(
				CharactersInfoManager::GetInstance()->GetPlayerInstance()->GetPosition());
			//�ǂɗ����Ȃ��Ȃ烁�e�I�𐶐�����
			if (IsMeteoHitWall(meteoTargetPos) == false)
			{
				//���e�I�̐����ƃJ�E���g�����Z
				CreateMeteo(meteoTargetPos);
				//�J�E���g�����Z
				m_shotMeteoCounter++;
				break;
			}
			else
			{
				//��蒼��
				continue;
			}
		}
		return;
	}

	//�_�[�N���e�I�����Ȃ�A�_�[�N���e�I���������̃X�e�[�g�ɑJ��
	if (m_shotDarkMeteoFlag == true)
	{
		SetNextDarkMeteoState(enDarkMeteoState_ShotDarkMeteo);
	}
	//�����Ȃ��Ȃ�I��鏈���̃X�e�[�g�ɑJ��
	else
	{
		SetNextDarkMeteoState(enDarkMeteoState_End);
	}
	return;
}

void DarkMeteo::OnProcessShotDarkMeteoTranstion()
{
}

void DarkMeteo::OnProcessEndTranstion()
{
	m_endFlag = true;

}

bool DarkMeteo::IsMeteoHitWall(Vector3 endPosition)
{
	CCapsuleCollider m_capsuleCollider;
	m_capsuleCollider.Init(2.0f, 6.0f);
	//�n�_�ƏI�_��ݒ�
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//�n�_�̓G�l�~�[�̍��W�B
	start.setOrigin(
		btVector3(endPosition.x, endPosition.y + METEO_START_UP_POS, endPosition.z));
	//�I�_�̓v���C���[�̍��W�B
	end.setOrigin(
		btVector3(endPosition.x, endPosition.y + METEO_END_UP_POS, endPosition.z));
	//�ǂ̔����Ԃ�
	IsWallResult isWallResult;
	//�R���C�_�[���n�_����I�_�܂œ������āB
	//�ǂƏՓ˂��邩�ǂ����𒲂ׂ�B
	PhysicsWorld::GetInstance()->ConvexSweepTest(
		(const btConvexShape*)m_capsuleCollider.GetBody(),
		start, end, isWallResult);
	//�ǂɃq�b�g������
	if (isWallResult.isHit == true)
	{
		//�q�b�g����
		return true;
	}
	else
	{
		//�q�b�g���Ȃ�����
		return false;
	}
	
}

bool DarkMeteo::IsShotMeteo()
{
	if (m_shotMeteoTimer > SHOT_METEO_TIMER_LIMMIT)
	{
		m_shotMeteoTimer = 0.0f;
		return true;
	}
	else
	{
		m_shotMeteoTimer += g_gameTime->GetFrameDeltaTime();
	}

	return false;
}

void DarkMeteo::InitDarkMeteoEffect()
{
	m_darkMeteoriteEffect = NewGO<EffectEmitter>(0);
	m_darkMeteoriteEffect->Init(InitEffect::enEffect_Meteo);
	m_darkMeteoriteEffect->Play();
	m_darkMeteoriteEffect->SetPosition(m_darkMeteoPositon);
	m_darkMeteoriteEffect->Update();
}

void DarkMeteo::CreateMeteo(Vector3 endPosition)
{
	//���e�I�𐶐��B���W�ƏI�_��ݒ�
	Meteo* meteo = NewGO<Meteo>(0, "meteo");
	meteo->SetPosition(m_darkMeteoPositon);
	meteo->SetTargetPosition(endPosition);
	//���X�g�ɒǉ�
	m_meteoList.emplace_back(meteo);
	//���e�I�������Đ�
	g_soundManager->InitAndPlaySoundSource(
		enSoundName_Boss_Meteo_Shot, g_soundManager->GetSEVolume());
}