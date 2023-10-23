#pragma once

#include "IWeapon.h"

class SwordShield:public IWeapon
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

private:
	/// <summary>
	/// モデルの初期化
	/// </summary>
	void InitModel() override;

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

	Vector3 m_swordPos = g_vec3Zero;
	Vector3 m_shieldPos = g_vec3Zero;

	Matrix m_swordMatrix = g_matIdentity;
	Matrix m_shieldMatrix = g_matIdentity;

	const float m_jampSpeed = 3000.0f;

	//武器を持たせる時のボーンID
	int m_armedSwordBoonId = -1;
	int m_armedShieldBoonId = -1;
	//武器をしまった時のボーンID
	int m_stowedSwordBoonId = -1;
	int m_stowedShieldBoonId = -1;


};

