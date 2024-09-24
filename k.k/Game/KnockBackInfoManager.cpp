#include "stdafx.h"
#include "KnockBackInfoManager.h"



KnockBackInfoManager* KnockBackInfoManager::m_instance = nullptr;

KnockBackInfoManager::KnockBackInfoManager()
{
	if (m_instance != nullptr)
	{
		std::abort();
	}
	m_instance = this;
}

KnockBackInfoManager::~KnockBackInfoManager()
{
	m_instance = nullptr;
}

void KnockBackInfoManager::CreateInstance()
{
	m_instance = new KnockBackInfoManager();
}

void KnockBackInfoManager::DeleteInstance()
{
	delete m_instance;
	m_instance = nullptr;
}

void KnockBackInfoManager::Init()
{
	//�X�e�[�^�X��������
	m_status.InitKnockBackStatus();

	//�J�[�u�f�[�^�쐬
	CreateKnockBackCurveData();


}

int KnockBackInfoManager::GetAddAttackId()
{
	//ID�����Z
	m_attackId++;

	if (m_attackId > 1000)
	{
		m_attackId = 0;
	}

	return m_attackId;
}

void KnockBackInfoManager::CreateKnockBackCurveData()
{
	//�}�b�v���N���A
	m_curveDataMap.clear();

	//�m�b�N�o�b�N�̍ŏ��̃p�^�[��
	EnKnockBackPattern knockBackPattern = enKBPattern_ShortGroundedRetreat;

	//�m�b�N�o�b�N�p�^�[���̐��J��Ԃ�
	while (knockBackPattern != enKnockBackPattern_Num)
	{
		//�J�[�u�f�[�^��ۑ����郊�X�g
		std::vector<CurvePoint> curvePointList;

		//���X�g�ɃJ�[�u�f�[�^�����Ă���
		GenerateKnockBackCurve(knockBackPattern,curvePointList);

		//�}�b�v�Ƀy�A��ǉ�
		m_curveDataMap.insert(std::make_pair(knockBackPattern, curvePointList));

		//���̃p�^�[���ɂ���
		knockBackPattern = static_cast<EnKnockBackPattern>(knockBackPattern + 1);
	}
}

void KnockBackInfoManager::GenerateKnockBackCurve(
	EnKnockBackPattern knockBackPattern, std::vector<CurvePoint>& curvePointList)
{
	float deltaTime = 0.1f; // �T���v�����O�̊Ԋu
	//�����x
	float initialVelocity = m_status.GetGetKnockBackStatus(knockBackPattern).initialVelocity;
	//�p�x
	float angle = m_status.GetGetKnockBackStatus(knockBackPattern).angle;
	//�d��
	float gravity = m_status.GetGetKnockBackStatus(knockBackPattern).gravity;

	angle = Math::DegToRad(angle);

	float v_xz = initialVelocity * cos(angle);  // X,Z������
	float v_y = initialVelocity * sin(angle);  // Y������


	//�J�[�u�̍��W���T���v�����O
	for (float t = 0; 
		t <= m_status.GetGetKnockBackStatus(knockBackPattern).duration; 
		t += deltaTime
		)
	{
		CurvePoint point;

		
		//���Ԃ�ۑ�
		point.time = t;
		//���W���v�Z
		point.curvePosition.x = v_xz * t;

		if (angle != 0.0f)
		{
			point.curvePosition.y = v_y * t - 0.5f * gravity * t * t;
		}
		
		
		point.curvePosition.z = v_xz * t;

		//���W��ۑ�
		curvePointList.emplace_back(point);
	}

}
