#pragma once


/// <summary>
/// �E�F�[�u�Ɋւ���X�e�[�^�X
/// </summary>
class WaveStatus
{
private:


	struct Status
	{
		int totalWaves = 0;						//�g�[�^���E�F�[�u��
		int breakTimeWaveNumber = 0;			//�x�e�E�F�[�u�̔ԍ�
		float waveTimeLimit = 0.0f;				//1�E�F�[�u�̎���
		float shortBreakTime = 0.0f;			//�E�F�[�u�ƃE�F�[�u�̊Ԃ̎���
		int maxMobEnemys = 0;					//���u�G�l�~�[�̍ő吔
		int firstCreateMobEnemyAmount = 0;		//�ŏ��ɐ������郂�u�G�l�~�[�̐�
		int addCreateMobEnemyAmonut = 0;		//�ǉ��Ő������郂�u�G�l�~�[�̐�
		float createRadius = 0.0f;				//�����͈�(���a)
		float coordinateDistance = 0.0f;		//�������W���m�Œ���󂯂鋗��
		float bossAvoidanceRadius = 0.0f;		//�{�X�̒��S���琶�����Ȃ��͈�(���a)
		float playerAvoidanceRadius = 0.0f;		//�v���C���[�̒��S���琶�����Ȃ��͈�(���a)
		float incantationTimeLimit = 0.0f;		//�����ʒu���肩�琶���܂ł̎���
	};

	Status m_status;		//�E�F�[�u�Ɋւ���X�e�[�^�X

	/// <summary>
	/// �E�F�[�u�̃X�e�[�^�Xcsv�t�@�C���̓ǂݍ���
	/// </summary>
	void LoadWaveStatusCSV();


public:

	/// <summary>
	/// �E�F�[�u�̃X�e�[�^�X���擾
	/// </summary>
	/// <returns></returns>
	const Status& GetWaveStatus()
	{
		return m_status;
	}

	/// <summary>
	/// �E�F�[�u�̃X�e�[�^�X��������
	/// </summary>
	void InitWaveStatus();



	const int& GetTotalWaves()
	{
		return m_status.totalWaves;
	}

	const int& GetBreakTimeWaveNumber()
	{
		return m_status.breakTimeWaveNumber;
	}

	const float& GetCurrentWaveTimeLimit()
	{
		return m_status.waveTimeLimit;
	}

	const float& GetShortBreakTime()
	{
		return m_status.shortBreakTime;
	}

	const int& GetMaxMobEnemys()
	{
		return m_status.maxMobEnemys;
	}

	const int& GetAddCreateMobEnemyAmonut()
	{
		return m_status.addCreateMobEnemyAmonut;
	}

	const float& GetCreateRadius()
	{
		return m_status.createRadius;
	}

	const float& GetCoordinateDistance()
	{
		return m_status.coordinateDistance;
	}

	const float& GetBossAvoidanceRadius()
	{
		return m_status.bossAvoidanceRadius;
	}

	const float& GetPlayerAvoidanceRadius()
	{
		return m_status.playerAvoidanceRadius;
	}

	const float& GetIncantationTimeLimit()
	{
		return m_status.incantationTimeLimit;
	}



	//CSV�̏���ۑ������}�b�v
	static WaveStatus StatusDataMAP;
	//�E�F�[�u�̃X�e�[�^�X�̏������Ŏg���ÓI�Ȍ^
	static const std::string waveStatusCsvFilePath;
	static bool IsLoadedWaveStatusCSVFile;

};

