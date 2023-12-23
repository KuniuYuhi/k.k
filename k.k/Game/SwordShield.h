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
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	
	/// <summary>
	/// 武器の移動処理
	/// </summary>
	void MoveWeapon() override;

	/// <summary>
	/// 攻撃やスキル時のジャンプの速度の取得
	/// </summary>
	/// <returns></returns>
	float GetJampSpeed() override
	{
		return m_jampSpeed;
	}

	/// <summary>
	/// 盾の当たり判定がヒットしたか
	/// </summary>
	/// <returns>ヒットしたらtrue、ヒットしなかったらfalse</returns>
	bool IsHitCollision() override;

	/// <summary>
	/// スキル攻撃処理
	/// </summary>
	void ProcessSkillAttack() override;
	
	/// <summary>
	/// 耐久値がなくなったらときの処理
	/// </summary>
	void ProcessNoEndurance() override
	{
		//盾の耐久値がなくなったので、防御不可能にする
		SetIsDefendEnableFlag(false);
	}
	/// <summary>
	/// 耐久値が0より大きくなったときの処理(0から以上になった時)
	/// </summary>
	void ProcessOnEndurance() override
	{
		//盾の耐久値が増えたので、防御可能にする
		SetIsDefendEnableFlag(true);
	}

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
	/// 剣の当たり判定の初期化
	/// </summary>
	void InitSwordCollision();
	/// <summary>
	/// 盾の当たり判定の初期化
	/// </summary>
	void InitShieldCollision();
	/// <summary>
	/// 武器を装備している時の移動処理
	/// </summary>
	void MoveArmed() override;
	/// <summary>
	/// 武器を収納している時の移動処理
	/// </summary>
	void MoveStowed() override;

	/// <summary>
	/// 上昇処理
	/// </summary>
	void ProcessRising();

	/// <summary>
	/// 下降処理
	/// </summary>
	void ProcessFall();

private:
	ModelRender m_modelSword;		//剣モデル
	ModelRender m_modelShield;		//盾モデル

	CollisionObject* m_swordCollision = nullptr;	//片手剣の当たり判定
	CollisionObject* m_shieldCollision = nullptr;	//盾の当たり判定

	Vector3 m_skillMovePos = g_vec3Zero;	//スキルを使うときの座標
	Vector3 m_swordPos = g_vec3Zero;
	Vector3 m_shieldPos = g_vec3Zero;

	Matrix m_swordMatrix = g_matIdentity;
	Matrix m_shieldMatrix = g_matIdentity;

	Vector3 m_skillAttackPosition = g_vec3Zero;

	Quaternion rot;

	const float m_jampSpeed = 3000.0f;

	//武器を持たせる時のボーンID
	int m_armedSwordBoonId = -1;
	int m_armedShieldBoonId = -1;
	//武器をしまった時のボーンID
	int m_stowedSwordBoonId = -1;
	int m_stowedShieldBoonId = -1;

	const float m_knockBackPower = 200.0f;	//ノックバックパワー

};

