#pragma once
#include "IComponent.h"
#include "CommonEnemyStatus.h"

class CharacterBase;
class CommonEnemyStatus;

/// <summary>
/// モブエネミーの移動用コンポーネント
/// </summary>
class MobEnemyMovement : public IComponent
{
public:
	MobEnemyMovement();
	~MobEnemyMovement();

	void Start() override;

	void UpdateComponent() override;

	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="status"></param>
	/// <param name="chaseCharacter"></param>
	/// <param name="moveSpeed"></param>
	/// <returns></returns>
	Vector3 CalcChaseCharacterVerocity(
		CommonEnemyStatus status,
		Vector3 targetPositon,
		Vector3 m_currentPosition,
		Vector3 moveSpeed
	);

private:

};

