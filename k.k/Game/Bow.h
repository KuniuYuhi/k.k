#pragma once

#include "WeaponBase.h"

class Arrow;

class Bow :public WeaponBase
{
public:
	Bow();
	~Bow();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	/// <summary>
	/// 武器の移動処理
	/// </summary>
	void MoveWeapon() override;

	/// <summary>
	/// スキル攻撃処理
	/// </summary>
	void ProcessSkillAttack() override;

	/// <summary>
	/// 耐久値がなくなったらときの処理
	/// </summary>
	void ProcessNoEndurance() override
	{
		//矢のストックがなくなったので、攻撃不可能にする
		SetIsAttackEnableFlag(false);
	}
	/// <summary>
	/// 耐久値が0より大きくなったときの処理(0から以上になった時)
	/// </summary>
	void ProcessOnEndurance() override
	{
		//矢のストックが増えたので、攻撃可能になる
		SetIsAttackEnableFlag(true);
	}

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

	const Vector3& GetPlayerForward() const;

	/// <summary>
	/// 攻撃がヒットしたかのフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetAttackHitFlag(bool flag);
	/// <summary>
	/// 攻撃がヒットしたかのフラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& GetAttackHitFlag() const;
	
	/// <summary>
	/// 被ダメージなどで、正常に戻すはずだった変数を強制的にリセットする
	/// </summary>
	void ResetVariable() override
	{
		m_RotationDelectionFlag = false;
	}

	/// <summary>
	/// 矢を生成するかどうか
	/// </summary>
	void IsCreatArrow();

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
	void ProcessLongRangeAttack();
	/// <summary>
	/// スキルを撃った時の処理
	/// </summary>
	void SkillShot();

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

	/// <summary>
	/// 矢の生成処理
	/// </summary>
	void CreateArrow();



private:

	EffectEmitter* m_chargeEffect = nullptr;

	Arrow* m_arrow = nullptr;

	ModelRender m_modelBow;		//弓モデル

	Vector3 m_bowPos = g_vec3Zero;

	Vector3 m_playerForward = g_vec3Zero;

	Matrix m_bowMatrix = g_matIdentity;
	Matrix m_arrowMatrix = g_matIdentity;



	//武器を持たせる時のボーンID
	int m_armedBowBoonId = -1;
	int m_armedArrowBoonId = -1;
	//武器をしまった時のボーンID
	int m_stowedBowBoonId = -1;
	int m_stowedArrowBoonId = -1;

	bool m_stockArrowFlag = false;

	float m_ChargeTimer = 0.0f;				//スキルのチャージタイマー

	bool m_playChargeEffectFlag = false;	//スキルのチャージエフェクトを再生したかのフラグ
};

