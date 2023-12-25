#pragma once

#include "WeaponBase.h"

class BigSword:public WeaponBase
{
public:
	BigSword();
	~BigSword();

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
	void InitCollision();
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

	/// <summary>
	/// 地面との衝突判定
	/// </summary>
	bool IsGround();

private:
	ModelRender m_modelBigSword;		//剣モデル

	CollisionObject* m_bigSwordCollision = nullptr;	//両手剣の当たり判定

	Vector3 m_swordPos = g_vec3Zero;
	Vector3 m_maxRisingPosition = g_vec3Zero;

	Vector3 m_skillMovePos = g_vec3Zero;	//スキルを使うときの座標

	Matrix m_swordMatrix = g_matIdentity;

	Vector3 m_skillAttackPosition = g_vec3Zero;

	//武器を持たせる時のボーンID
	int m_armedSwordBoonId = -1;

	const float m_jampSpeed = 30000.0f;

	const float m_knockBackPower = 300.0f;

};

