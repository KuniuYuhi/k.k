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
	/// キャラクターを追いかける方向の計算
	/// </summary>
	/// <param name="status">ステータス</param>
	/// <param name="chaseCharacter">追いかけるキャラクターの座標</param>
	/// <param name="moveSpeed">移動速度</param>
	/// <returns></returns>
	Vector3 CalcChaseCharacterVerocity(
		CommonEnemyStatus status,
		Vector3 targetPositon,
		Vector3 m_currentPosition,
		Vector3 moveSpeed
	);

private:

};

