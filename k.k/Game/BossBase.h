#pragma once
#include "AIActor.h"

class BossBase:public AIActor
{
public:
	BossBase();
	virtual ~BossBase();



	/////////////////////////////////////////////////////////////////////////////
	// 仮想関数、純粋仮想関数の宣言
	/////////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// 攻撃処理
	/// </summary>
	virtual void Attack() override{}
	/// <summary>
	/// 処理を止めるか
	/// </summary>
	/// <returns></returns>
	virtual bool IsStopProcessing() override { return false; }

	/// <summary>
	/// 特定のアニメーションが再生中か
	/// </summary>
	/// <returns></returns>
	virtual bool isAnimationEntable() const override = 0;

	/// <summary>
	/// 回転可能か
	/// </summary>
	/// <returns></returns>
	virtual bool isRotationEntable() const override = 0;

	/// <summary>
	/// 攻撃可能か
	/// </summary>
	/// <returns></returns>
	virtual bool IsAttackEntable() const override = 0;

	/// <summary>
	/// 被ダメージ時処理
	/// </summary>
	virtual void Damage(int attack) override = 0;



	/////////////////////////////////////////////////////////////////////////////
	// その他の関数
	/////////////////////////////////////////////////////////////////////////////



protected:



};

