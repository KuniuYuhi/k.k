#pragma once

#include "KnockBackStatus.h"
#include "KnockBackInfo.h"


using namespace KnockBackInfo;

class KnockBackStatus;

/// <summary>
/// �m�b�N�o�b�N�̏������}�l�[�W���[
/// </summary>
class KnockBackInfoManager
{
private:
	KnockBackInfoManager();
	~KnockBackInfoManager();

	


public:

	static KnockBackInfoManager* m_instance; //�B��̃C���X�^���X�̃A�h���X���L�^����ϐ��B

	/// <summary>
	/// �C���X�^���X�𐶐�
	/// </summary>
	static void CreateInstance();

	/// <summary>
	/// �C���X�^���X���폜
	/// </summary>
	static void DeleteInstance();


	/// <summary>
	/// �C���X�^���X���擾�B
	/// </summary>
	/// <returns></returns>
	static KnockBackInfoManager* GetInstance()
	{
		return m_instance;
	}

	/// <summary>
	/// ����������
	/// </summary>
	void Init();

	
	/// <summary>
	/// �J�[�u�f�[�^�̃��X�g���擾
	/// </summary>
	/// <param name="knockBackPattern">�擾�������J�[�u�f�[�^�̃p�^�[��</param>
	/// <returns></returns>
	std::vector<CurvePoint> GetCurvePoint(EnKnockBackPattern knockBackPattern)
	{
		return m_curveDataMap.at(knockBackPattern);
	}


	KnockBackStatus GetStatus()
	{
		return m_status;
	}


	/// <summary>
	/// ���Z�����U��ID���擾
	/// </summary>
	/// <returns>���Z���ꂽ��̍U��ID</returns>
	int GetAddAttackId();



private:

	/// <summary>
	/// �m�b�N�o�b�N�J�[�u�f�[�^�̐���
	/// </summary>
	void CreateKnockBackCurveData();

	/// <summary>
	/// �J�[�u�̌v�Z
	/// </summary>
	void GenerateKnockBackCurve(
		EnKnockBackPattern knockBackPattern, 
		std::vector<CurvePoint>& curvePointList
	);



private:

	KnockBackStatus m_status;		//�m�b�N�o�b�N�Ɋւ���X�e�[�^�X

	std::map<EnKnockBackPattern, std::vector<CurvePoint>> m_curveDataMap;	//�J�[�u�f�[�^�̃}�b�v


	int m_attackId = 0;

};

