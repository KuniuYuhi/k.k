#pragma once

/// <summary>
/// ����F�{�E���A���[�̌ŗL�̃X�e�[�^�X
/// </summary>
class BowArrowStatus
{
public:

	enum EnSkillChargeStage
	{
		enStage_1,
		enStage_2,
		enStage_max,
	};

private:

	/// <summary>
	/// �ŗL�X�e�[�^�X
	/// </summary>
	struct UniqueStatus
	{
		float firstAttackTimeScale = 1.0f;		//��R���{�ڃm�b�N�o�b�N�̐i�s���x
		float secondAttackTimeScale = 1.0f;		//��R���{�ڃm�b�N�o�b�N�̐i�s���x
		float thirdAttackTimeScale = 1.0f;		//�O�R���{�ڃm�b�N�o�b�N�̐i�s���x
		float skillAttackTimeScale = 1.0f;		//�X�L���U���m�b�N�o�b�N�̐i�s���x

		float defenciveMoveSpeed = 0.0f;		//����X�s�[�h

		float normalAttackSpeed[3] = { 0.0f };	//�ʏ�U���̈ړ����x

		float normalAttackSearchRadius = 0.0f;	//�ʏ�U���̍��G�͈�(���a)

		float normalAttackComparisonDot = 0.0f;	//�ʏ�U���Ōv�Z����x�N�g�����m�̓��ς̔�r�Ώۂ̓���


		float skillChargeCompletionTime[enStage_max] = { 0.0f };//�`���[�W�������ԁB�z��͒i�K

		float attackInfoUpdateInterval = 0.0f;			//�U�����X�V�C���^�[�o��

		int arrowStack = 0;						//��̃X�g�b�N
	};

	UniqueStatus m_uniqueStatus;

	EnSkillChargeStage m_enSkillChargeStage = enStage_1;

	/// <summary>
	/// �{�E���A���[�̌ŗL�X�e�[�^�Xcsv�t�@�C���̓ǂݍ���
	/// </summary>
	void LoadUniqueStatusCSV();

public:

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const UniqueStatus& GetUniqueStatus()
	{
		return m_uniqueStatus;
	}

	/// <summary>
	/// �{�E���A���[�̌ŗL�X�e�[�^�X��������
	/// </summary>
	void InitUniqueStatus();

	
	void SetSkillChargeStage(EnSkillChargeStage setStage)
	{
		m_enSkillChargeStage = setStage;
	}

	EnSkillChargeStage GetCurrentSkillChargeStage()
	{
		return m_enSkillChargeStage;
	}


	const float& GetAttackTimeScale(int num)
	{
		switch (num)
		{
		case 0:
			return FirstAttackTimeScale();
		case 1:
			return SecondAttackTimeScale();
		case 2:
			return ThirdAttackTimeScale();
		default:
			break;
		}
	}

	const float& FirstAttackTimeScale()
	{
		return m_uniqueStatus.firstAttackTimeScale;
	}

	const float& SecondAttackTimeScale()
	{
		return m_uniqueStatus.secondAttackTimeScale;
	}

	const float& ThirdAttackTimeScale()
	{
		return m_uniqueStatus.thirdAttackTimeScale;
	}

	const float& SkillAttackTimeScale()
	{
		return m_uniqueStatus.skillAttackTimeScale;
	}

	const float& GetDefenciveMoveSpeed()
	{
		return m_uniqueStatus.defenciveMoveSpeed;
	}

	float& GetNormalAttackSpeed(int num)
	{
		return m_uniqueStatus.normalAttackSpeed[num];
	}

	float& GetNormalAttackSearchRadius()
	{
		return m_uniqueStatus.normalAttackSearchRadius;
	}

	const float& GetNormalAttackComparisonDot()
	{
		return m_uniqueStatus.normalAttackComparisonDot;
	}

	const float& GetSkillChargeCompletionTime(EnSkillChargeStage stage)
	{
		return m_uniqueStatus.skillChargeCompletionTime[stage];
	}

	const float& GetAttackInfoUpdateInterval()
	{
		return m_uniqueStatus.attackInfoUpdateInterval;
	}

	const int& GetArrowStack()
	{
		return m_uniqueStatus.arrowStack;
	}



	//CSV�̏���ۑ������}�b�v
	static BowArrowStatus StatusDataMAP;
	//�{�E���A���[�̌ŗL�X�e�[�^�X�̏������Ŏg���ÓI�Ȍ^
	static const std::string uniqueStatusCsvFilePath;
	static bool IsLoadedUniqueStatusCSVFile;


};

