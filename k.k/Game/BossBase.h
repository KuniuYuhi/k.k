#pragma once
#include "MonsterBase.h"

class BossBase:public MonsterBase
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

	//通常攻撃に当たった時の処理
	virtual void HitNormalAttack() override;

	//ヒーローのスキルに当たった時の処理
	virtual void HitSkillAttack() override;
	/// <summary>
	/// ウィザードのファイヤーボールに当たった時の処理。派生クラスで実装
	/// </summary>
	virtual void HitFireBall() override {}
	/// <summary>
	/// ウィザードのフレイムピラーに当たった時の処理。派生クラスで実装
	/// </summary>
	virtual void HitFlamePillar(bool damageFlag = false) override {}
	
	/// <summary>
	/// 回転のみを行う処理条件
	/// </summary>
	/// <returns></returns>
	virtual bool RotationOnly() override = 0;

	/////////////////////////////////////////////////////////////////////////////
	// その他の関数
	/////////////////////////////////////////////////////////////////////////////



protected:
	/// <summary>
	/// モデルの初期化
	/// </summary>
	virtual void InitModel() override = 0;

	/// <summary>
	/// アニメーションを再生
	/// </summary>
	virtual void PlayAnimation() override = 0;

	/// <summary>
	/// ステート管理
	/// </summary>
	virtual void ManageState() override = 0;


};

