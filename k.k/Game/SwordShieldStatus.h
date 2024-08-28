#pragma once

/// <summary>
/// ����F�\�[�h���V�[���h�̌ŗL�̃X�e�[�^�X
/// </summary>
class SwordShieldStatus
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

		float normalAttackSpeed[3] = { 0.0f };	//�ʏ�U���̈ړ����x

		float normalAttackSearchRadius = 0.0f;	//�ʏ�U���̍��G�͈�(���a)

		float normalAttackComparisonDot = 0.0f;	//�ʏ�U���Ōv�Z����x�N�g�����m�̓��ς̔�r�Ώۂ̓���

		int maxShieldEnduranceValue = 0;			//�V�[���h�̍ő�ϋv�l
		int currentShieldEnduranceValue = 0;		//���݂̃V�[���h�̑ϋv�l
	};

	UniqueStatus m_uniqueStatus;


	/// <summary>
	/// �\�[�h���V�[���h�̌ŗL�X�e�[�^�Xcsv�t�@�C���̓ǂݍ���
	/// </summary>
	void LoadUniqueStatusCSV();

public:

	const UniqueStatus& GetUniqueStatus()
	{
		return m_uniqueStatus;
	}

	/// <summary>
	/// �\�[�h���V�[���h�̌ŗL�X�e�[�^�X��������
	/// </summary>
	void InitUniqueStatus();



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


	const int& GetShieldEnduranceValue()
	{
		return m_uniqueStatus.maxShieldEnduranceValue;
	}

	const int& GetCurrentShieldEnduranceValue()
	{
		return m_uniqueStatus.currentShieldEnduranceValue;
	}

	void SetCurrentShieldEnduranceValue(int value)
	{
		m_uniqueStatus.currentShieldEnduranceValue = value;
	}




	/// <summary>
	/// �V�[���h�̑ϋv�l�̌��Z
	/// </summary>
	/// <param name="subValue">���Z�������l</param>
	void SubShieldEnduranceValue(int subValue)
	{
		m_uniqueStatus.currentShieldEnduranceValue -= subValue;

		if (m_uniqueStatus.currentShieldEnduranceValue <= 0)
		{
			m_uniqueStatus.currentShieldEnduranceValue = 0;
		}

	}


	//CSV�̏���ۑ������}�b�v
	static SwordShieldStatus StatusDataMAP;
	//�\�[�h���V�[���h�̌ŗL�X�e�[�^�X�̏������Ŏg���ÓI�Ȍ^
	static const std::string uniqueStatusCsvFilePath;
	static bool IsLoadedUniqueStatusCSVFile;


};

