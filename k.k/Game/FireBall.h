#pragma once

#include "Wizard.h"

class FireBall:public IGameObject
{
public:
	FireBall();
	~FireBall();

	bool Start();

	void Update();

	void Timer();

	void Move();

	void Render(RenderContext& rc);



	void SetWizard(Wizard* wizard)
	{
		m_wizard = wizard;
	}

private:
	Wizard* m_wizard = nullptr;

	ModelRender m_model;

	Vector3 m_position = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;

	//ファイヤーボールのスピード
	const float m_speed = 150.0f;
	//ボールの生存時間
	const int m_limitTimer = 3;
	//タイマー
	float m_moveTime = 0.0f;
	//プレイヤーからボールの距離
	const float m_distance = 60.0f;


};

