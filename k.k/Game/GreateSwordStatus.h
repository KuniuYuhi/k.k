#pragma once

/// <summary>
/// ����F�O���C�g�\�[�h�̌ŗL�̃X�e�[�^�X
/// </summary>
class GreateSwordStatus
{
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

		float skillFlightTimeLimit = 0.0f;		//�X�L���̑؋󐧌�����

	};

	UniqueStatus m_uniqueStatus;


	/// <summary>
	/// �O���C�g�\�[�h�̌ŗL�X�e�[�^�Xcsv�t�@�C���̓ǂݍ���
	/// </summary>
	void LoadUniqueStatusCSV();

public:

	const UniqueStatus& GetUniqueStatus()
	{
		return m_uniqueStatus;
	}

	/// <summary>
	/// �O���C�g�\�[�h�̌ŗL�X�e�[�^�X��������
	/// </summary>
	void InitUniqueStatus();



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


	const float& GetSkillFlightTimeLimit()
	{
		return m_uniqueStatus.skillFlightTimeLimit;
	}





	//CSV�̏���ۑ������}�b�v
	static GreateSwordStatus StatusDataMAP;
	//�O���C�g�\�[�h�̌ŗL�X�e�[�^�X�̏������Ŏg���ÓI�Ȍ^
	static const std::string uniqueStatusCsvFilePath;
	static bool IsLoadedUniqueStatusCSVFile;


};

