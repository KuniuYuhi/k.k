#pragma once

#include "IWeapon.h"

class Bow :public IWeapon
{
public:
	Bow();
	~Bow();

	bool Start();
	void Update();
	void Render(RenderContext& rc);


	/// <summary>
	/// 武器の移動処理
	/// </summary>
	void MoveWeapon() override;

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
	ModelRender m_modelBow;		//弓モデル
	ModelRender m_modelArrow;		//矢モデル

	CollisionObject* m_arrowCollision = nullptr;	//矢の当たり判定

	Vector3 m_bowPos = g_vec3Zero;
	Vector3 m_arrowPos = g_vec3Zero;

	Matrix m_bowMatrix = g_matIdentity;
	Matrix m_arrowMatrix = g_matIdentity;

	//武器を持たせる時のボーンID
	int m_armedBowBoonId = -1;
	int m_armedArrowBoonId = -1;
	//武器をしまった時のボーンID
	int m_stowedBowBoonId = -1;
	int m_stowedArrowBoonId = -1;



};

