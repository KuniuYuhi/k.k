#pragma once

#include "Wizard.h"
#include "Lich.h"

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

	
	void SetForWizard(const char* collisionname,
		float speed,
		float distance,
		float y_up,
		float collisionradius,
		Vector3 scale
	);

	void SetForLich(
		const char* collisionname, 
		float speed, 
		float distance, 
		float y_up, 
		float collisionradius, 
		Vector3 scale
	);


	void SetWizard(Wizard* wizard)
	{
		m_wizard = wizard;
	}

	void SetLich(Lich* lich)
	{
		m_lich = lich;
	}

	/// <summary>
	/// 攻撃力の取得
	/// </summary>
	/// <returns></returns>
	int GetAtk() const
	{
		return m_atk;
	}
	
	/// <summary>
	/// 敵に当たったかのフラグを設定する
	/// </summary>
	/// <param name="flag"></param>
	void SetHitEnemeyFlag(bool flag)
	{
		m_hitEnemeyFlag = flag;
	}

private:
	Wizard* m_wizard = nullptr;

	Lich* m_lich = nullptr;

	ModelRender m_model;
	CollisionObject* m_BallCollision;

	Vector3 m_position = Vector3::Zero;
	Vector3 m_collisionPosition= Vector3::Zero;
	Vector3 m_forWizardBallScale = Vector3::One;
	Vector3 m_forLichBallScale = { 3.0f,3.0f,3.0f };
	Vector3 m_moveSpeed = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;

	const char* m_fireBall = "fireball";
	const char* m_darkBall = "darkball";

	//攻撃力
	int m_atk = 0;

	const int m_wizardAttack = 30;		//ウィザード用の攻撃力
	const int m_lichAttack = 20;		//リッチ用の攻撃力

	//ファイヤーボールのスピード
	const float m_speed = 150.0f;



	//ボールの生存時間
	int m_limitTimer = 0;

	const int m_forWizardLimitTimer = 3;
	const int m_forLichLimitTimer = 5;

	//タイマー
	float m_moveTime = 0.0f;
	//プレイヤーからボールの距離
	const float m_distance = 60.0f;

	//敵に当たったかのフラグ
	bool m_hitEnemeyFlag = false;

};

