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
	

	

	//ボスをよける
	if (isBossPosCheck)
	{
		//ボスから自身に向かうベクトルを求める
		Vector3 bossDistance = m_summoner->GetPosition() - m_currentPosition;
		bossDistance.y = 0.0f;
		//実際のボスに向かうベクトルの長さ
		int realToBossDistance = bossDistance.Length();
		//クランプしたボスに向かうベクトルの長さ
		int clampToBossDistance = Clamp(bossDistance.Length(),0.0f, 150.0f);

		//距離の長さを線形補間。
		//ボスに向かうベクトルの長さによって距離は変わる
		int variableDIstance = Math::Lerp(
			(clampToBossDistance - 0.0f) / (200.0f - 0.0f), 80.0f, 150.0f
		);

		//プレイヤーに近づくほど小さくする
		//実際にボスに向かうベクトルの長さより線形補間した調査できる長さかつ
		//ボスに向かうベクトルの長さより、プレイヤーに向かうベクトルの長さが長いなら
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
