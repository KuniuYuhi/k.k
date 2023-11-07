#pragma once

#include "IWeapon.h"

class Bow;

class Arrow:public IWeapon
{
public:
	Arrow();
	~Arrow();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// 武器の移動処理
	/// </summary>
	void MoveWeapon() override;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="bow"></param>
	void SetBow(Bow* bow)
	{
		m_bow = bow;
	}

	/// <summary>
	/// 矢のワールド座標を設定
	/// </summary>
	/// <param name="matrix"></param>
	void SetArrowMatrix(const Matrix matrix)
	{
		m_arrowMatrix = matrix;
	}
	/// <summary>
	/// 矢の情報を更新
	/// </summary>
	void ArrowUpdate()
	{
		m_modelArrow.Update();
		//m_arrowCollision->Update();
	}

	/// <summary>
	/// 射撃フラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetShotFlag(bool flag)
	{
		m_shotFlag = flag;
	}

	/// <summary>
	/// 前方向を設定
	/// </summary>
	/// <param name="forward"></param>
	void SetForward(Vector3 forward)
	{
		m_forward = forward;
	}

	/// <summary>
	/// 射撃開始座標を設定
	/// </summary>
	/// <param name="startPosition"></param>
	void SetShotStartPosition(Vector3 startPosition)
	{
		m_shotStartPosition = startPosition;
	}
	/// <summary>
	/// 矢の座標を取得
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_arrowPos;
	}
	/// <summary>
	/// ワールド座標をローカル座標に適応
	/// </summary>
	void ApplyMatrixToLocalPosition()
	{
		m_arrowPos = g_vec3Zero;
		m_arrowMatrix.Apply(m_arrowPos);
	}

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

private:
	/// <summary>
	/// モデルの初期化
	/// </summary>
	void InitModel() override;
	/// <summary>
	/// 当たり判定の初期化
	/// </summary>
	void InitCollision() override;

	

	Bow* m_bow = nullptr;

	ModelRender m_modelArrow;		//矢モデル
	CollisionObject* m_arrowCollision = nullptr;

	Vector3 m_arrowPos = g_vec3Zero;
	Vector3 m_forward = g_vec3Zero;
	Vector3 m_shotStartPosition = g_vec3Zero;

	Matrix m_arrowMatrix = g_matIdentity;


	bool m_shotFlag = false;

};

