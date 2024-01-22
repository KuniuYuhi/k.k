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
	virtual void Attack(){}
	/// <summary>
	/// Update()処理を止めるか
	/// </summary>
	/// <returns></returns>
	virtual bool IsStopProcessing(){ return false; }

	/// <summary>
	/// 特定のアニメーションが再生中か
	/// </summary>
	/// <returns></returns>
	virtual bool isAnimationEnable() const override = 0;
	/// <summary>
	/// 回転可能か
	/// </summary>
	/// <returns></returns>
	virtual bool isRotationEnable() const = 0;
	/// <summary>
	/// 攻撃可能か
	/// </summary>
	/// <returns></returns>
	virtual bool IsAttackEnable() const = 0;
	/// <summary>
	/// 被ダメージ時処理
	/// </summary>
	virtual void Damage(int attack) = 0;

	/// <summary>
	/// 被ダメージ用当たり判定
	/// </summary>
	virtual void DamageCollision(CharacterController& characon);

	//通常攻撃に当たった時の処理
	virtual void HitNormalAttack();

	//スキルに当たった時の処理
	virtual void HitSkillAttack();

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

	/// <summary>
	/// ヒットエフェクト生成
	/// </summary>
	virtual void CreateHitEffect() {}

	/// <summary>
	/// やられたときの処理
	/// </summary>
	/// <param name="seFlag">やられたときの効果音を再生するかのフラグ</param>
	virtual void ProcessDead(bool seFlag = true) = 0;

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
	/// 円形で検索
	/// </summary>
	/// <param name="distance">プレイヤーを見つけることができる上限の距離</param>
	/// <returns>距離内だったらtrueを返す</returns>
	bool IsFindPlayer(float distance);

	/// <summary>
	/// ダメージフォント生成
	/// </summary>
	void CreateDamageFont(int damage);



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



	int							m_damage = 0;							//受けたダメージを代入する

	bool						m_dashFlag = false;						//ダッシュするかのフラグ
	bool						m_attackEnableFlag = true;					//攻撃していいかのフラグ。falseで攻撃可能
	bool						m_damageFlag = false;					//ダメージを受けられるかのフラグ。falseで被ダメージ可能

	float						m_attackIntervalTimer = 0.0f;			//アタックのインターバルタイマー
	float						m_damageIntervalTimer = 0.0f;			//ダメージのインターバルタイマー

	float						m_distance = 0.0f;						//プレイヤーを発見できる距離

	float						m_angle = 0.0f;							//視野角

	float						m_angleChangeTimer = 0.0f;				//移動する方向をかえるタイミングを計算するタイマー
	bool						m_angleChangeTimeFlag = false;			//移動する方向をかえるかのフラグ

	bool						m_elaseListFlag = false;				//自身をリストから削除したかのフラグ

	float						m_rotTimer = 0.0f;						//回転の線形補間で使うタイマー


};

