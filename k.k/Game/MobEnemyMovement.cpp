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

	diff.Normalize();
	diff.y = 0.0f;

	

	//�{�X���悯��
	{



		//�{�X���߂��ɂ���Ȃ�悯��

		//�{�X���玩�g�Ɍ������x�N�g�������߂�
		Vector3 bossDistance =  m_summoner->GetPosition() - m_currentPosition;
		bossDistance.y = 0.0f;
		int toBossDistance = bossDistance.Length();

		//�O����{�X�Ƃ̋���������Ă���Ȃ珈������K�v�Ȃ�
		if (isBossPosCheck)
		{
			
			//�{�X�Ɍ������x�N�g���̒�����200�ȉ��Ȃ�

			//���Z����m���͍ő�3�B�����̕�Ԃ�0�`1000
			int add = Math::Lerp(
				(toBossDistance - 0.0f) / (250.0f - 0.0f), 110.0f, 200.0f
			);

			//�v���C���[�ɋ߂Â��قǏ���������
			if ((toBossDistance < add + ADD) && (toPlayerLength > toBossDistance))
			{
				//�{�X�Ɍ������x�N�g���ƃv���C���[�Ɍ������x�N�g���̓��ς�
				//0�ȉ��Ȃ�v���C���[�̂ق����߂��̂łʂ���
				if (Dot(diff, bossDistance) > 0)
				{

				}

				/*if (Dot(diff, bossDistance) >= 0.9f)
				{

				}*/


				bossDistance.Normalize();

				bossDistance *= -1.0f;

				diff += (bossDistance * 0.8f);

				float mul = 1 - (toBossDistance - 0.0f) / (250.0f - 0.0f);

				diff *= 1.20f + mul;


				ADD += 5;
			}
			
			if (ADD >= 0)
				ADD -= 2;
			
		}

	
	}


	diff *= status.GetDefaultSpeed();

	currentMoveSpeed += diff;

	return currentMoveSpeed;
}
