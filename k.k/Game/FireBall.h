#pragma once
#include "MagicBall.h"

class Wizard;

class FireBall:public MagicBall
{
public:
	FireBall();
	~FireBall();

	bool Start();
	void Update();

	void Move();

	void SettingCollision();

	void Explosion();

	/*void SetAtk(const int attack)
	{
		m_attack = attack;
	}*/

	/// <summary>
	/// 攻撃力を返す
	/// </summary>
	/// <returns></returns>
	const int& GetBallAtk() const
	{
		return m_attack;
	}

	/// <summary>
	/// ウィザードのオブジェクトの設定
	/// </summary>
	/// <param name="wizard"></param>
	void SetWizard(Wizard* wizard)
	{
		m_wizard = wizard;
	}

private:
	Wizard* m_wizard = nullptr;

	CollisionObject* m_BallCollision;
	EffectEmitter* m_fireBallEffect;

	Vector3 m_collisionPosition= Vector3::Zero;
	Vector3 m_forLichBallScale = { 2.3f,2.3f,2.3f };

	//攻撃力
	const int m_attack = 10;

	const int m_wizardAttack = 30;		//ウィザード用の攻撃力

	const int m_moveLimitTimer = 3;

};

