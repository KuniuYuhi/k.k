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

	/// <summary>
	/// 勇者(プレイヤー)のアニメーションクリップを自身の武器のものに変更
	/// </summary>
	/// <param name="modelrender"></param>
	void SetBraveAnimationClip(ModelRender* modelrender) override;


private:
	ModelRender m_modelBigSword;		//剣モデル

	Vector3 m_swordPos = g_vec3Zero;

	Matrix m_swordMatrix = g_matIdentity;

	//武器を持たせる時のボーンID
	int m_armedSwordBoonId = -1;

};

