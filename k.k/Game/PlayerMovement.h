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


	Vector3 CalcSimpleMovementVerocity(Status_Player playerStatus,Vector3 StickInput);



private:

	Brave* m_brave = nullptr;

	int m_speed = 10.0f;


};

