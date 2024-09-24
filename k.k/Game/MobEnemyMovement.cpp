#include "stdafx.h"
#include "MobEnemyMovement.h"

#include "Summoner.h"


MobEnemyMovement::MobEnemyMovement()
{
	Start();
}

MobEnemyMovement::~MobEnemyMovement()
{
}

void MobEnemyMovement::Start()
{
	m_summoner = FindGO<Summoner>("Summoner");
}

void MobEnemyMovement::UpdateComponent()
{
}

Vector3 MobEnemyMovement::CalcChaseCharacterVerocity(
	CommonEnemyStatus status, 
	Vector3 targetPositon,
	Vector3 m_currentPosition, Vector3 moveSpeed,
	bool isBossPosCheck)
{
	Vector3 currentMoveSpeed = moveSpeed;
	currentMoveSpeed.x = 0.0f;
	currentMoveSpeed.z = 0.0f;
	
	Vector3 diff = targetPositon - m_currentPosition;

	float toPlayerLength = diff.Length();

	diff.y = 0.0f;
	diff.Normalize();
	

	

	//�{�X���悯��
	if (isBossPosCheck)
	{
		//�{�X���玩�g�Ɍ������x�N�g�������߂�
		Vector3 bossDistance = m_summoner->GetPosition() - m_currentPosition;
		bossDistance.y = 0.0f;
		//���ۂ̃{�X�Ɍ������x�N�g���̒���
		int realToBossDistance = bossDistance.Length();
		//�N�����v�����{�X�Ɍ������x�N�g���̒���
		int clampToBossDistance = Clamp(bossDistance.Length(),0.0f, 150.0f);

		//�����̒�������`��ԁB
		//�{�X�Ɍ������x�N�g���̒����ɂ���ċ����͕ς��
		int variableDIstance = Math::Lerp(
			(clampToBossDistance - 0.0f) / (200.0f - 0.0f), 80.0f, 150.0f
		);

		//�v���C���[�ɋ߂Â��قǏ���������
		//���ۂɃ{�X�Ɍ������x�N�g���̒��������`��Ԃ��������ł��钷������
		//�{�X�Ɍ������x�N�g���̒������A�v���C���[�Ɍ������x�N�g���̒����������Ȃ�
		if ((realToBossDistance < variableDIstance + m_addDistance) && 
			(toPlayerLength > realToBossDistance))
		{
			diff =
			{ -diff.z, diff.y, diff.x };
			
			m_addDistance = 20;

			float mul = 1 - (clampToBossDistance - 0.0f) / (250.0f - 0.0f);

			diff *= (2.0f * mul );
		}


		if (m_addDistance >= 0)
			m_addDistance -= 4;

	}

	
	diff.Normalize();

	diff *= status.GetDefaultSpeed();

	currentMoveSpeed += diff;

	return currentMoveSpeed;
}

float MobEnemyMovement::Clamp(float value, float min, float max)
{
	if (value < min) return min;

	else if (value > max) return max;

	return value;
}
