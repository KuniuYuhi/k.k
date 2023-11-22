#pragma once
#include "AIActor.h"
#include "DamageFont.h"

/// <summary>
/// モンスター系のクラス
/// </summary>
class MonsterBase:public AIActor
{
public:
	MonsterBase();
	virtual ~MonsterBase();

	/////////////////////////////////////////////////////////////////////////////
	// 仮想関数、純粋仮想関数の宣言
	/////////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// 攻撃処理
	/// </summary>
	virtual void Attack() override {}
	/// <summary>
	/// Update()処理を止めるか
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

	/// <summary>
	/// ダメージフォント生成
	/// </summary>
	virtual void CreateDamageFont(int damage);

	/// <summary>
	/// 被ダメージ用当たり判定
	/// </summary>
	virtual void DamageCollision(CharacterController& characon){}

	/// <summary>
	/// 当たり判定の処理をするか
	/// </summary>
	virtual bool IsCollisionDetection();

	/// <summary>
	///  移動時の回転
	/// </summary>
	/// <param name="rotSpeed">線形補間時の回転速度(移動時)</param>
	/// <param name="rotOnlySpeed">線形補間時の回転速度(回転のみの時)</param>
	/// <returns></returns>
	virtual Quaternion Rotation(float rotSpeed, float rotOnlySpeed);

	/// <summary>
	/// 回転のみを行う処理条件
	/// </summary>
	/// <returns></returns>
	virtual bool RotationOnly() = 0;

	/////////////////////////////////////////////////////////////////////////////
	// その他の関数
	/////////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="status">自身のステータス</param>
	/// <param name="targetposition">向かいたいターゲットの座標</param>
	/// <param name="dashFlag">ダッシュフラグ。trueでダッシュ。主にプレイヤーを追いかけるときに使う</param>
	/// <returns></returns>
	Vector3 CalcVelocity(Status status, Vector3 targetposition, bool dashFlag = false);
	/// <summary>
	/// 視野角判定
	/// </summary>
	/// <param name="toPlayerDir">自身からターゲットに向かうベクトル</param>
	/// <param name="forward">前方向</param>
	/// <param name="angle">視野角</param>
	/// <returns>視野角の中ならtrue,いなかったらfalse</returns>
	bool IsInFieldOfView(Vector3 toPlayerDir, Vector3 forward, float angle);
	/// <summary>
	/// プレイヤーを見つける
	/// </summary>
	/// <param name="distance">プレイヤーを見つけることができる上限の距離</param>
	/// <returns>距離内だったらtrueを返す</returns>
	bool IsFindPlayer(float distance);












	//todo　なくすかも
	/// <summary>
	/// 攻撃してから次の攻撃を行うまでのインターバル
	/// </summary>
	/// <returns></returns>
	bool AttackInterval(const float attackintarvaltime);
	/// <summary>
	/// 向かうベクトルを変えるインターバルの計算
	/// </summary>
	/// <returns></returns> 
	bool AngleChangeTimeIntarval(float LimitTime);




protected:


};

