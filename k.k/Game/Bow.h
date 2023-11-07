#pragma once

#include "IWeapon.h"

class Arrow;

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

	/// <summary>
	/// 矢のワールド座標を取得
	/// </summary>
	/// <returns></returns>
	const Matrix GetArrowMatrix() const
	{
		return m_arrowMatrix;
	}

	/// <summary>
	/// 弓のウェポンステートを取得
	/// </summary>
	/// <returns></returns>
	const EnWeaponState& GetBowEnWeaponState() const
	{
		return m_enWeaponState;
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
	/// <summary>
	/// 遠距離攻撃処理
	/// </summary>
	void ProcessLongRangeAttack() override;


	/// <summary>
	/// 矢の保持フラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetStockArrowFlag(bool flag)
	{
		m_stockArrowFlag = flag;
	}
	/// <summary>
	/// 矢の保持フラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& GetStockArrowFlag() const
	{
		return m_stockArrowFlag;
	}

private:
	Arrow* m_arrow = nullptr;

	ModelRender m_modelBow;		//弓モデル

	Vector3 m_bowPos = g_vec3Zero;

	Matrix m_bowMatrix = g_matIdentity;
	Matrix m_arrowMatrix = g_matIdentity;

	//武器を持たせる時のボーンID
	int m_armedBowBoonId = -1;
	int m_armedArrowBoonId = -1;
	//武器をしまった時のボーンID
	int m_stowedBowBoonId = -1;
	int m_stowedArrowBoonId = -1;

	bool m_stockArrowFlag = false;

};

