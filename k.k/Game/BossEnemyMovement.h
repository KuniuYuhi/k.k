#pragma once
#include "IComponent.h"
#include "CommonEnemyStatus.h"

class CommonEnemyStatus;

class BossEnemyMovement:public IComponent
{
public:
	BossEnemyMovement();
	~BossEnemyMovement();

	void Start() override;

	void UpdateComponent() override;


	/// <summary>
	/// 
	/// </summary>
	/// <param name="status"></param>
	/// <param name="targetPositon"></param>
	/// <param name="m_currentPosition"></param>
	/// <param name="moveSpeed"></param>
	/// <returns></returns>
	Vector3 CalcChaseCharacterVerocity(
		CommonEnemyStatus status,
		Vector3 targetPositon,
		Vector3 m_currentPosition,
		Vector3 moveSpeed
	);

	Vector3 CalcChaseCharacterVerocity(
		float speed,
		Vector3 targetPositon,
		Vector3 m_currentPosition,
		Vector3 moveSpeed
	);



private:

	Vector3 AdjustDistance(Vector3 position);


};

