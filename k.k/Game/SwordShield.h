#pragma once

#include "WeaponBase.h"

class SwordShield:public WeaponBase
{
public:
	SwordShield();
	~SwordShield();

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
	/// 盾の当たり判定がヒットしたか
	/// </summary>
	/// <returns>ヒットしたらtrue、ヒットしなかったらfalse</returns>
	bool IsHitCollision();

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
	void InitCollision();
	/// <summary>
	/// 武器を装備している時の移動処理
	/// </summary>
	void MoveArmed() override;
	/// <summary>
	/// 武器を収納している時の移動処理
	/// </summary>
	void MoveStowed() override;

private:
	ModelRender m_modelSword;		//剣モデル
	ModelRender m_modelShield;		//盾モデル

	CollisionObject* m_swordCollision = nullptr;	//片手剣の当たり判定
	CollisionObject* m_shieldCollision = nullptr;	//盾の当たり判定

	Vector3 m_swordPos = g_vec3Zero;
	Vector3 m_shieldPos = g_vec3Zero;

	Matrix m_swordMatrix = g_matIdentity;
	Matrix m_shieldMatrix = g_matIdentity;

	Vector3 m_skillAttackPosition = g_vec3Zero;

	const float m_jampSpeed = 3000.0f;

	//武器を持たせる時のボーンID
	int m_armedSwordBoonId = -1;
	int m_armedShieldBoonId = -1;
	//武器をしまった時のボーンID
	int m_stowedSwordBoonId = -1;
	int m_stowedShieldBoonId = -1;

	const float m_knockBackPower = 200.0f;	//ノックバックパワー

};

