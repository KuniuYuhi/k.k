#pragma once
#include "Status.h"
#include "Player.h"

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
	/// <param name="status">ステータス</param>
	/// <returns>moveSpeed</returns>
	Vector3 calcVelocity(Status status);

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
	/// 
	/// </summary>
	/// <returns></returns>
	virtual bool isRotationEntable() const = 0;

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
	//ヒーローのスキルに当たった時の処理
	virtual void HitHeroSkillAttack();
	//ウィザードのファイヤーボールに当たった時の処理
	virtual void HitFireBall();
	//ウィザードのフレイムピラーに当たった時の処理
	virtual void HitFlamePillar();

	/// <summary>
	/// ステータスの取得
	/// </summary>
	/// <returns></returns>
	const Status& GetStatus()
	{
		return m_status;
	}

	void SetPosition(Vector3 position)
	{
		m_position = position;
	}

	void SetRotation(Quaternion rotation)
	{
		m_rotation = rotation;
	}

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns></returns>
	const Vector3 GetPosition()
	{
		return m_position;
	}

	/// <summary>
	/// 拡大率の取得
	/// </summary>
	/// <returns></returns>
	const Vector3 GetScale()
	{
		return m_scale;
	}

	/// <summary>
	/// 回転の取得
	/// </summary>
	/// <returns></returns>
	const Quaternion GetRotation()
	{
		return m_rotation;
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
	/// 移動時の回転
	/// </summary>
	/// <returns></returns>
	Quaternion Rotation();

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	virtual bool RotationOnly() = 0;

	Player* m_player = nullptr;

	Status m_status;

	Vector3 m_targetPosition = Vector3::Zero;		//ターゲットの座標を格納

	Vector3 m_position = Vector3::Zero;
	Vector3 m_oldMoveSpeed = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;

	Vector3 m_forward = Vector3::Zero;				//前方向

	Vector3 m_SaveMoveSpeed = Vector3::Zero;		//回転のみ使用する時に使う

	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;

	bool m_dashFlag = false;		//ダッシュするかのフラグ
	bool m_attackFlag = false;		//攻撃していいかのフラグ。falseで攻撃可能
	bool m_damageFlag = false;		//ダメージを受けられるかのフラグ。falseで被ダメージ可能

	float m_attackIntervalTimer = 0.0f;		//アタックのインターバルタイマー
	float m_damageIntervalTimer = 0.0f;		//ダメージのインターバルタイマー

	float m_distance = 0.0f;	//プレイヤーを発見できる距離

	float m_angle = 0.0f;			//視野角

	
	float m_angleChangeTimer = 0.0f;
	bool m_angleChangeTimeFlag = false;

};

