#pragma once
#include "Status.h"
#include "Player.h"
#include "FireBall.h"
#include "FlamePillar.h"
#include "DamageFont.h"

class AIActor:public IGameObject
{
public:
	AIActor();
	virtual ~AIActor();

	/// <summary>
	/// モデルの座標、回転、サイズの設定
	/// </summary>
	/// <param name="modelRender"></param>
	void SetTransFormModel(ModelRender& modelRender);

	/// <summary>
	/// 座標、回転、サイズの設定
	/// </summary>
	/// <param name="position"></param>
	/// <param name="scale"></param>
	/// <param name="rotation"></param>
	void SetTransForm(Vector3 position, Quaternion rotation, Vector3 scale = Vector3::One);
	
	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="status">自身のステータス</param>
	/// <param name="targetposition">向かいたいターゲットの座標</param>
	/// <param name="dashFlag">ダッシュフラグ。trueでダッシュ。主にプレイヤーを追いかけるときに使う</param>
	/// <returns></returns>
	Vector3 CalcVelocity(Status status,Vector3 targetposition, bool dashFlag = false);

	/// <summary>
	/// 視野角判定
	/// </summary>
	/// <param name="toPlayerDir">自身からターゲットに向かうベクトル</param>
	/// <param name="forward">前方向</param>
	/// <param name="angle">視野角</param>
	/// <returns>視野角の中ならtrue,いなかったらfalse</returns>
	bool IsInFieldOfView(Vector3 toPlayerDir,Vector3 forward, float angle);

	/// <summary>
	/// 攻撃処理
	/// </summary>
	virtual void Attack();

	/// <summary>
	/// 処理を止めるか
	/// </summary>
	/// <returns></returns>
	virtual bool IsStopProcessing();

	/// <summary>
	/// ターゲットの座標をm_targetPsitionに代入する
	/// </summary>
	void SetTargetPosition();

	/// <summary>
	/// 攻撃してから次の攻撃を行うまでのインターバル
	/// </summary>
	/// <returns></returns>
	bool AttackInterval(const float attackintarvaltime);

	/// <summary>
	/// ダメージを受けた後のダメージを受けるまでのインターバル
	/// </summary>
	/// <returns></returns>
	bool DamageInterval(const float damageintarvaltime);

	/// <summary>
	/// 向かうベクトルを変えるインターバルの計算
	/// </summary>
	/// <returns></returns> 
	bool AngleChangeTimeIntarval(float LimitTime);

	/// <summary>
	/// プレイヤーを見つける
	/// </summary>
	/// <param name="distance">プレイヤーを見つけることができる上限の距離</param>
	/// <returns>距離内だったらtrueを返す</returns>
	bool IsFindPlayer(float distance);

	/// <summary>
	/// 特定のアニメーションが再生中か
	/// </summary>
	/// <returns></returns>
	virtual bool isAnimationEntable() const = 0;

	/// <summary>
	/// 回転可能か
	/// </summary>
	/// <returns></returns>
	virtual bool isRotationEntable() const = 0;

	/// <summary>
	/// 攻撃可能か
	/// </summary>
	/// <returns></returns>
	virtual bool IsAttackEntable() const = 0;

	/// <summary>
	/// 被ダメージ時処理
	/// </summary>
	virtual void Damage(int attack) = 0;

	/// <summary>
	/// ダメージフォント生成
	/// </summary>
	virtual void CreateDamageFont(int damage);

	/// <summary>
	/// 被ダメージ用当たり判定
	/// </summary>
	virtual void DamageCollision(CharacterController& characon);

	/// <summary>
	/// 当たり判定の処理をするか
	/// </summary>
	virtual bool IsCollisionDetection();

	//通常攻撃に当たった時の処理
	virtual void HitNormalAttack();

	//ヒーローのスキルに当たった時の処理
	virtual void HitHeroSkillAttack();
	/// <summary>
	/// ウィザードのファイヤーボールに当たった時の処理。派生クラスで実装
	/// </summary>
	virtual void HitFireBall();
	/// <summary>
	/// ウィザードのフレイムピラーに当たった時の処理。派生クラスで実装
	/// </summary>
	virtual void HitFlamePillar(bool damageFlag = false);

	/// <summary>
	/// ヒットエフェクト生成
	/// </summary>
	virtual void CreateHitEffect();


	/// <summary>
	/// ステータスの取得
	/// </summary>
	/// <returns></returns>
	const Status& GetStatus()
	{
		return m_status;
	}

	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector3 position)
	{
		m_position = position;
	}
	/// <summary>
	/// 回転を設定
	/// </summary>
	/// <param name="rotation"></param>
	void SetRotation(Quaternion rotation)
	{
		m_rotation = rotation;
	}

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// 拡大率の取得
	/// </summary>
	/// <returns></returns>
	const Vector3& GetScale() const
	{
		return m_scale;
	}
	/// <summary>
	/// 回転の取得
	/// </summary>
	/// <returns></returns>
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}
	/// <summary>
	/// 前方向を取得
	/// </summary>
	/// <returns></returns>
	const Vector3& GetForward() const
	{
		return m_forward;
	}

	/// <summary>
	/// 勝利フラグの設定
	/// </summary>
	/// <param name="flag"></param>
	void SetWinFlag(bool flag)
	{
		m_winFlag = flag;
	}
	/// <summary>
	/// 勝利フラグの取得
	/// </summary>
	/// <returns></returns>
	const bool& GetWinFlag() const
	{
		return m_winFlag;
	}

protected:

	/// <summary>
	/// モデルの初期化
	/// </summary>
	virtual void InitModel() = 0;

	/// <summary>
	/// アニメーションを再生
	/// </summary>
	virtual void PlayAnimation() = 0;

	/// <summary>
	/// ステート管理
	/// </summary>
	virtual void ManageState() = 0;

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

	Player*				m_player = nullptr;

	Status						m_status;

	Vector3						m_targetPosition = Vector3::Zero;		//ターゲットの座標を格納

	Vector3						m_toTarget= Vector3::Zero;				//ターゲットに向かうベクトル

	Vector3						m_position = Vector3::Zero;
	Vector3						m_rotMove = Vector3::Zero;
	Vector3						m_moveSpeed = Vector3::Zero;

	Vector3						m_forward = Vector3::Zero;				//前方向

	Vector3						m_SaveMoveSpeed = Vector3::Zero;		//回転のみ使用する時に使う

	Quaternion					m_rotation = Quaternion::Identity;
	Vector3						m_scale = Vector3::One;

	int							m_damage = 0;							//受けたダメージを代入する

	bool						m_dashFlag = false;						//ダッシュするかのフラグ
	bool						m_attackFlag = true;					//攻撃していいかのフラグ。falseで攻撃可能
	bool						m_damageFlag = false;					//ダメージを受けられるかのフラグ。falseで被ダメージ可能

	float						m_attackIntervalTimer = 0.0f;			//アタックのインターバルタイマー
	float						m_damageIntervalTimer = 0.0f;			//ダメージのインターバルタイマー

	float						m_distance = 0.0f;						//プレイヤーを発見できる距離

	float						m_angle = 0.0f;							//視野角

	bool						m_winFlag = false;						//勝ったかのフラグ

	float						m_angleChangeTimer = 0.0f;				//移動する方向をかえるタイミングを計算するタイマー
	bool						m_angleChangeTimeFlag = false;			//移動する方向をかえるかのフラグ

	bool						m_elaseListFlag = false;				//自身をリストから削除したかのフラグ

	float						m_rotTimer = 0.0f;						//回転の線形補間で使うタイマー



};

