#pragma once

#include "IWeapon.h"

class BigSword:public IWeapon
{
public:
	BigSword();
	~BigSword();

	bool Start();
	void Update();
	void Render(RenderContext& rc);


	/// <summary>
	/// 武器の移動処理
	/// </summary>
	void MoveWeapon() override;

	/// <summary>
	/// 攻撃やスキル時のジャンプの速度の取得
	/// </summary>
	/// <returns></returns>
	float GetJampSpeed()
	{
		return m_jampSpeed;
	}

	/// <summary>
	/// スキル攻撃処理
	/// </summary>
	void ProcessSkillAttack();

private:
	/// <summary>
	/// モデルの初期化
	/// </summary>
	void InitModel() override;
	/// <summary>
	/// 当たり判定の生成
	/// </summary>
	void InitCollision() override;
	/// <summary>
	/// 武器を装備している時の移動処理
	/// </summary>
	void MoveArmed() override;
	/// <summary>
	/// 武器を収納している時の移動処理
	/// </summary>
	void MoveStowed() override;

private:
	ModelRender m_modelBigSword;		//剣モデル

	CollisionObject* m_bigSwordCollision;	//両手剣の当たり判定

	Vector3 m_swordPos = g_vec3Zero;

	Matrix m_swordMatrix = g_matIdentity;

	Vector3 m_skillAttackPosition = g_vec3Zero;

	//武器を持たせる時のボーンID
	int m_armedSwordBoonId = -1;

	const float m_jampSpeed = 30000.0f;

};

