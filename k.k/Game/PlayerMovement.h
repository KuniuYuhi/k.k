#pragma once
//#include "Component.h"

#include "IComponent.h"


class Brave;
class Status_Player;

/// <summary>
/// キャラクター移動用コンポーネント
/// </summary>
class PlayerMovement : public IComponent
{
public:
	PlayerMovement();
	~PlayerMovement();

	void Start() override;

	void UpdateComponent() override;


	int GetSpeed()
	{
		return m_speed;
	}

	/// <summary>
	/// 普段の移動速度の
	/// 計算
	/// </summary>
	/// <param name="playerStatus">ステータス</param>
	/// <param name="moveSpeed">現在の移動速度</param>
	/// <param name="StickInput">コントローラーの移動スティックの入力量</param>
	/// <returns>移動速度</returns>
	Vector3 CalcSimpleMovementVerocity(
		Status_Player playerStatus,
		Vector3 moveSpeed,
		Vector3 StickInput
	);



private:

	Brave* m_brave = nullptr;

	int m_speed = 10;


};

