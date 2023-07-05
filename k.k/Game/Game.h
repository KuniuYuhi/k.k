#pragma once
#include "Level3DRender.h"
//#include "Level2DRender.h"


class Game:public IGameObject
{
public:
	Game();
	~Game();

	bool Start();
	void Update();

	
	void SpriteTransform();

	void Spotmove();

	const Vector3 GetPosition() const
	{
		return m_position;
	}

	void Render(RenderContext& rc);

private:

	FontRender fontTest;
	float m_fontScale = 1.0f;

	//Level2DRender level2DSp;
	SpriteRender spriteTest;
	float wipSize = 5.0f;

	//spotLight
	Vector3 spPosition = Vector3::Zero;
	Vector3 spDirection = Vector3::Zero;

	Vector3 m_position = Vector3(0.0f,0.0f,0.0f);
	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;
};

