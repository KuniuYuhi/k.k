#include "stdafx.h"
#include "WaveManager.h"

#include "InitEffect.h"

#include "EnemyObjectPool.h"

#include "EnemyBase.h"

#include "MobEnemyheaderFiles.h"

#include "EnemyManager.h"


WaveManager::WaveManager()
{
}

WaveManager::~WaveManager()
{
}

bool WaveManager::Start()
{
	//�������������B
	srand((unsigned)time(NULL));

	//�X�e�[�^�X��������
	m_waveStatus.InitWaveStatus();


	m_currentWaveNumber = 0;

	return true;
}

void WaveManager::Update()
{
	//�i�s�Ǘ�
	ManageWaveProgression();

}

void WaveManager::CalcWaveTimer(EnWaveManageState nextWaveState)
{
	//�^�C�����~�b�g�̒B������
	if (m_currentWaveTimer >= m_currentTimeLimit)
	{
		//���̃X�e�[�g�ɐ؂�ւ���
		m_currentWaveManageState = nextWaveState;
		//�^�C�}�[�����Z�b�g
		m_currentWaveTimer = 0.0f;
	}
	else
	{
		//�^�C�}�[�����Z
		m_currentWaveTimer += g_gameTime->GetFrameDeltaTime();
	}

}

void WaveManager::EvaluateWaveResults()
{
}

void WaveManager::SummonMobEnemys()
{
	//�������W�̐������J��Ԃ�
	for (int i = 0; i < m_createPositions.size(); i++)
	{
		//todo �G�l�~�[��������𒴂��Ȃ��悤�ɂ���
		if (EnemyManager::GetInstance()->GetMobEnemyList().size() >=
			m_waveStatus.GetMaxMobEnemys())
		{
			return;
		}


		EnemyBase* enemy = nullptr;
		
		////�����X�^�[�̎�ނ��������_���ɑI��
		//int num = rand() % (int)enEnemy_Num;

		//switch (num)
		//{
		//case enEnemy_Slime:

		//	enemy = EnemyObjectPool::GetInstance()->OnGetEnemy<Slime>("Slime");
		//	break;
		//case enEnemy_Cactus:

		//	enemy = EnemyObjectPool::GetInstance()->OnGetEnemy<Cactus>("Cactus");
		//	break;

		//default:
		//	break;
		//}

		//enemy = EnemyObjectPool::GetInstance()->OnGetEnemy<Slime>("Slime");

		//enemy = EnemyObjectPool::GetInstance()->OnGetEnemy<Cactus>("Cactus");

		//enemy = EnemyObjectPool::GetInstance()->OnGetEnemy<Mushroom>("Mushroom");


		enemy = EnemyObjectPool::GetInstance()->OnGetEnemy<BeholderEye>("BeholderEye");

		//enemy = EnemyObjectPool::GetInstance()->OnGetEnemy<Mimic>("Mimic");


		//�G�l�~�[�̍��W�𐶐����W�ɂ���
		enemy->SetPosition(m_createPositions[i]);


	}


}

void WaveManager::CastSummmonCircle(int castAmount)
{
	//�����X�^�[�𐶐�������W�ɖ��@�w�G�t�F�N�g���Đ�
	for (int num = 0; num < castAmount; num++)
	{
		Vector3 Pos = m_createPositions[num];
		Pos.y += 5.0f;
		EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
		effectEmitter->Init(InitEffect::enEffect_Mob_Summon_Circle);
		effectEmitter->Play();
		effectEmitter->SetPosition(Pos);
		effectEmitter->SetScale(g_vec3One * 15.0f);
		effectEmitter->Update();
	}
}

void WaveManager::SetSummonRandomPosition(const int radius, int amount)
{
	//�O��̏����Ŋi�[�������W���X�g���N���A
	m_createPositions.clear();
	//���S
	Vector3 Center = g_vec3Zero;

	//�������郂���X�^�[�̐��J��Ԃ�
	for (int num = 0; num < amount; num++)
	{
		//���W�����X�g�Ɋi�[
		m_createPositions.emplace_back(g_vec3Zero);
	}

	int count = 0;

	Vector3 setPos = g_vec3Zero;

	while (count < amount)
	{
		//�����_���Ȓl�̍ő�l�̌v�Z
		int max = radius * 2 + 1;

		//�~�͈͓̔��Ń����_���Ȓl��ݒ�
		setPos.x = rand() % max - radius;
		setPos.z = rand() % max - radius;
		//����������W
		m_createPositions[count] = setPos;
		//����������W�����a��菬�����Ȃ�
		if (m_createPositions[count].Length() < radius)
		{
			//���̏���������W�A�{�X�A�v���C���[�ƈ��̋������󂢂Ă��邩����
			if (IsWithInDistances(count, m_waveStatus.GetCoordinateDistance()) == true)
			{
				//���̍��W�����߂Ē��ׂ�
				count++;
			}
		}
	}

}

bool WaveManager::IsWithInDistances(int count, float distance)
{
	if (count <= 1) return true;

	int distanceCount = 0;
	//���̐���������W�ƈ��̊Ԋu���J����
	for (int i = 0; i < count; i++)
	{
		Vector3 diff = m_createPositions[count] - m_createPositions[i];
		//����������W���m�̋������v�Z
		float length = diff.Length();
		//����������Ă�����A����������W�Ɍ���
		if (length > distance)
		{
			distanceCount++;
		}
	}

	//�S�Ă̍��W�Ƌ������󂢂Ă�����
	if (distanceCount == count)
	{
		//�󂢂Ă���
		return true;
	}
	//�󂢂Ă��Ȃ�
	return false;
}

bool WaveManager::IsChangeSummonState()
{
	if (m_changeSummonTimer >= m_waveStatus.GetIncantationTimeLimit())
	{
		m_changeSummonTimer = 0.0f;
		return true;
	}
	else
	{
		m_changeSummonTimer += g_gameTime->GetFrameDeltaTime();
	}

	return false;
}

void WaveManager::ManageWaveProgression()
{
	switch (m_currentWaveManageState)
	{
	case WaveManager::enWaveState_FirstSummonMobEnemys://�ŏ��̃G�l�~�[����
		ProcessFirstSummonMobEnemysStateTransition();
		break;
	case WaveManager::enWaveState_ProgressWaveTime://�E�F�[�u�i�s��
		ProcessProgressWaveTimeStateTransition();
		break;
	case WaveManager::enWaveState_EvaluateWaveResults://�E�F�[�u�̕]��
		ProcessEvaluateWaveResultsStateTransition();
		break;
	case WaveManager::enWaveState_ShortBreakTime://���x�e
		ProcessShortBreakTimeStateTransition();
		break;
	case WaveManager::enWaveState_SettingCreatePos://�������W�ݒ�
		ProcessSettingCreatePosStateTransition();
		break;
	case WaveManager::enWaveState_SummonMobEnemys://���u�G�l�~�[�̏���
		ProcessSummonMobEnemysStateTransition();
		break;
	case WaveManager::enWaveState_BreakTimeWave:
		ProcessBreakTimeWaveStateTransition();
		break;

	default:
		break;
	}

}


void WaveManager::ProcessFirstSummonMobEnemysStateTransition()
{
	//�ŏ��̃G�l�~�[������


	//�E�F�[�u�P�ɂ���
	m_currentWaveNumber = 1;

	m_currentTimeLimit = m_waveStatus.GetCurrentWaveTimeLimit();

	//�E�F�[�u�i�s���X�e�[�g�ɑJ��
	m_currentWaveManageState = enWaveState_ProgressWaveTime;
}

void WaveManager::ProcessProgressWaveTimeStateTransition()
{
	//�E�F�[�u��i�s����B
	//�E�F�[�u���I�������]���X�e�[�g�ɑJ��
	CalcWaveTimer(enWaveState_EvaluateWaveResults);
}

void WaveManager::ProcessEvaluateWaveResultsStateTransition()
{
	//���݂̃E�F�[�u���x�e�E�F�[�u��������]�����Ȃ�
	//���x�e���΂�
	if (m_currentWaveNumber == m_waveStatus.GetBreakTimeWaveNumber())
	{
		//���x�e���΂��ď������W�ݒ�X�e�[�g�ɑJ��
		m_currentWaveManageState = enWaveState_SettingCreatePos;

	}
	else
	{
		//�]������
		EvaluateWaveResults();
		//���x�e�X�e�[�g�ɑJ��
		m_currentWaveManageState = enWaveState_ShortBreakTime;
		//�^�C�}�[�����x�e�p�̃��~�b�g�ɕύX
		m_currentTimeLimit = m_waveStatus.GetShortBreakTime();
	}
	


	//���݂̃E�F�[�u���Ō�̃E�F�[�u�Ȃ�
	if (m_currentWaveNumber == m_waveStatus.GetTotalWaves())
	{
		//�E�F�[�u��1�Ƀ��Z�b�g
		m_currentWaveNumber = 1;
	}
	else
	{
		//�E�F�[�u�ԍ���1�i�s
		m_currentWaveNumber++;
	}
	//���̃E�F�[�u�ԍ����u���C�N�^�C���E�F�[�u�ԍ��Ɠ����Ȃ�
	if (m_currentWaveNumber == m_waveStatus.GetBreakTimeWaveNumber())
	{
		//���x�e���΂��ċx�e�E�F�[�u�ɑJ��
		m_currentWaveManageState = enWaveState_BreakTimeWave;
		//�^�C�}�[���E�F�[�u�i�s���̃��~�b�g�ɕύX
		m_currentTimeLimit = m_waveStatus.GetCurrentWaveTimeLimit();
	}

}

void WaveManager::ProcessShortBreakTimeStateTransition()
{
	//���x�e�^�C�}�[���v�Z
	//���̃X�e�[�g�͏������W�ݒ�X�e�[�g
	CalcWaveTimer(enWaveState_SettingCreatePos);
}

void WaveManager::ProcessSettingCreatePosStateTransition()
{
	//���W�ݒ�
	SetSummonRandomPosition(
		m_waveStatus.GetCreateRadius(), 
		m_waveStatus.GetAddCreateMobEnemyAmonut()
	);
	//�������@�w�̐ݒu
	CastSummmonCircle(m_waveStatus.GetAddCreateMobEnemyAmonut());
	//���u�G�l�~�[����������X�e�[�g�ɑJ��
	m_currentWaveManageState = enWaveState_SummonMobEnemys;
}

void WaveManager::ProcessSummonMobEnemysStateTransition()
{
	//�������邶����ɂȂ�܂ŏ��������Ȃ�
	if (!IsChangeSummonState())
	{
		return;
	}

	//���u�G�l�~�[������
	SummonMobEnemys();

	//�^�C�}�[���E�F�[�u�i�s���̃��~�b�g�ɕύX
	m_currentTimeLimit = m_waveStatus.GetCurrentWaveTimeLimit();

	//�E�F�[�u�i�s���X�e�[�g�ɑJ��
	m_currentWaveManageState = enWaveState_ProgressWaveTime;
}

void WaveManager::ProcessBreakTimeWaveStateTransition()
{
	//�E�F�[�u��i�s����B
	//�E�F�[�u���I�������]���X�e�[�g�ɑJ��
	CalcWaveTimer(enWaveState_EvaluateWaveResults);
}
