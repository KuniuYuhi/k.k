#pragma once
#include "Status.h"
#include "DamageFont.h"
#include "KnockBack.h"

class AIActor;

class Actor:public IGameObject
{
public:

	Actor();
	virtual ~Actor();

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
	void SetTransForm(Vector3 position, Quaternion rotation,Vector3 scale=Vector3::One);

	/// <summary>
	/// 座標設定
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector3 position)
	{
		m_position = position;
	}

	///////////////////////////////////////////////////////////////
	//仮想関数、純粋仮想関数
	///////////////////////////////////////////////////////////////

	/// <summary>
	/// コリジョンオブジェクトに当たった時の処理
	/// </summary>
	/// <param name="characon"></param>
	virtual void DamageCollision(CharacterController& characon);

	/// <summary>
	/// 被ダメージ処理
	/// </summary>
	/// <param name="attack">相手の攻撃力</param>
	virtual void Damage(int attack) = 0;
	/// <summary>
	/// 特定のアニメーションが再生中か
	/// </summary>
	/// <returns></returns>
	virtual bool isAnimationEntable() const = 0;
	/// <summary>
	/// 当たり判定可能なアニメーションか
	/// </summary>
	/// <returns></returns>
	virtual bool isCollisionEntable() const = 0;
	/// <summary>
	/// 回転可能なアニメーションが再生中か
	/// </summary>
	/// <returns></returns>
	virtual bool isRotationEntable() const = 0;
	/// <summary>
	/// 勝利時の処理
	/// </summary>
	virtual void ProcessWin() = 0;

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
	/// スキルの使用時などの移動はしないが回転はしたいときに使う
	/// </summary>
	virtual bool RotationOnly() = 0;

	/// <summary>
	/// 前進するフラグの取得
	/// </summary>
	/// <returns></returns>
	virtual const bool& GetMoveForwardFlag() const
	{
		return false;
	}

	/// <summary>
	/// 前方向が設定できる条件か
	/// </summary>
	/// <returns>trueで可能、falseで不可能</returns>
	virtual const bool& IsSetForwardCondition() const
	{
		return true;
	}

	///////////////////////////////////////////////////////////////
	//その他の関数
	///////////////////////////////////////////////////////////////

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="status">ステータス</param>
	/// <returns>moveSpeed</returns>
	Vector3 calcVelocity(Status status);
	/// <summary>
	/// 前方向の計算
	/// </summary>
	/// <param name="moveSpeed"></param>
	void CalcForward(Vector3 moveSpeed);

public:
	/// <summary>
	/// 無敵時間の計算
	/// </summary>
	bool CalcInvincibleTime();

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
	/// ステータスの取得
	/// </summary>
	/// <returns></returns>
	const Status& GetStatus() const
	{
		return m_status;
	}
	/// <summary>
	/// 前方向の取得
	/// </summary>
	/// <returns></returns>
	const Vector3& GetForward() const
	{
		return m_forward;
	}
	/// <summary>
	/// moveSpeedの取得
	/// </summary>
	/// <returns></returns>
	const Vector3& GetMoveSpeed() const
	{
		return m_moveSpeed;
	}

	/// <summary>
	/// 自身がやられたかのフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetDieFlag(bool flag)
	{
		m_dieFlag = flag;
	}
	/// <summary>
	/// 自身がやられたかのフラグを取得
	/// </summary>
	/// <returns></returns>
	const bool GetDieFlag() const
	{
		return m_dieFlag;
	}

	/// <summary>
	/// 無敵時間フラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetInvicibleTimeFlag(bool flag)
	{
		m_invincibleTimeFlag = flag;
	}
	/// <summary>
	/// 無敵時間フラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& GetInvincibleTimeFlag()
	{
		return m_invincibleTimeFlag;
	}

	/// <summary>
	/// 無敵状態フラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetInvicibleFlag(bool flag)
	{
		m_invicibleFlag = flag;
	}
	/// <summary>
	/// 無敵状態フラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& GetInvicibleFlag() const
	{
		return m_invicibleFlag;
	}

	/// <summary>
	/// ノックバックパワーを設定
	/// </summary>
	/// <param name="power"></param>
	void SetKnockBackPower(float power)
	{
		m_knockBackPower = power;
	}
	/// <summary>
	/// ノックバックパワーを取得
	/// </summary>
	/// <returns></returns>
	const float& GetKnockBackPower() const
	{
		return m_knockBackPower;
	}
	/// <summary>
	/// ノックバックヒットフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetHitKnockBackFlag(bool flag)
	{
		m_hitKnockBackFlag = flag;
	}
	/// <summary>
	/// ノックバックヒットフラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& GetHitKnockBackFlag() const
	{
		return m_hitKnockBackFlag;
	}

	/// <summary>
	/// モデルドローフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetModelDrawFlag(bool flag)
	{
		m_modelDrawFlag = flag;
	}

protected:

	/// <summary>
	/// 点滅するか。
	/// </summary>
	/// <returns>trueで非表示、falseで表示</returns>
	bool IsFlashing();

	/// <summary>
	/// 移動時の回転
	/// </summary>
	/// <returns></returns>
	Quaternion Rotation(float rotSpeed, float rotOnlySpeed);

	/// <summary>
	/// ダッシュ回避ステート 使わなくなるかも
	/// </summary>
	enum EnDashInvicibleState
	{
		enDashInvicibleState_None,
		enDashInvicibleState_On,
		enDashInvicibleState_Off
	};

	/// <summary>
	/// ダッシュ回避ステートの設定
	/// </summary>
	/// <param name="enDashInvicibleState"></param>
	void SetInvicibleDashState(EnDashInvicibleState enDashInvicibleState)
	{
		m_enDashInvicibleState = enDashInvicibleState;
	}
	/// <summary>
	/// ダッシュ回避ステートの取得
	/// </summary>
	/// <returns></returns>
	const EnDashInvicibleState& GetInvicibleDashState() const
	{
		return m_enDashInvicibleState;
	}

	/// <summary>
	/// ダッシュしたときの無敵時間の計算
	/// </summary>
	/// <returns></returns>
	bool CalcInvicibleDash();
	
	/// <summary>
	/// ダメージフォント生成
	/// </summary>
	/// <param name="damage"></param>
	void CreateDamageFont(int damage);
	
	/// <summary>
	/// 当たり判定をとるか
	/// </summary>
	/// <returns>当たり判定をとるときはtrue、とらないときはfalse</returns>
	bool IsDecisionCollision();

	
	
protected:

	Status							m_status;
	AIActor*						m_atttackAIActor = nullptr;							//攻撃してきたAIActor(モンスター)
	
	EnDashInvicibleState			m_enDashInvicibleState = enDashInvicibleState_None;	//ダッシュしたときの無敵時間のためのステート

	Vector3							m_forward = { 0.0f,0.0f,1.0f };						//前方向
	Vector3							m_position = Vector3::Zero;							//自身の座標
	Vector3							m_moveSpeed = Vector3::Zero;						//移動方向

	Vector3							m_SaveMoveSpeed = Vector3::Zero;					//回転のみ使用する時に使う
	Vector3							m_rotMove = g_vec3Zero;								//回転の向き

	Quaternion						m_rotation = Quaternion::Identity;
	Vector3							m_scale = Vector3::One;

	bool							m_dashFlag = false;									//ダッシュするかのフラグ

	bool							m_dieFlag = false;									//やられたらtrueにする
	
	bool							m_invicibleFlag = false;							//無敵状態フラグ(アクション時に無敵状態にしたいとき使う)

	bool							m_invincibleTimeFlag = false;						//無敵時間であるかのフラグ(ダメージを受けた後などに使う)
	const float						m_invincbleTime = 1.5f;
	float							m_invincbleTimer = 0.0f;

	const float						m_invincbleDashTime = 0.3f;							//無敵状態ダッシュの時間
	float							m_invincbledDashTimer = 0.0f;

	bool							m_modelDrawFlag = false;							//モデルを描画するかのフラグ

	int								m_hitDamage = 0;									//攻撃されたときのダメージを記憶

	float							m_knockBackPower = 0.0f;							//ノックバックの強さ

	bool							m_hitKnockBackFlag = false;							//自身がノックバックするか


};

