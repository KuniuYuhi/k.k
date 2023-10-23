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
	ModelRender m_modelBigSword;		//剣モデル

	Vector3 m_swordPos = g_vec3Zero;

	Matrix m_swordMatrix = g_matIdentity;

	//武器を持たせる時のボーンID
	int m_armedSwordBoonId = -1;

	const float m_jampSpeed = 30000.0f;

};

