#pragma once
#include "Status.h"
#include "DamageFont.h"

//class Player;
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

	virtual void SetVictoryAnimationState() = 0;

	virtual void SetIdleAnimationState() = 0;

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="status">ステータス</param>
	/// <returns>moveSpeed</returns>
	Vector3 calcVelocity(Status status);

	/// <summary>
	/// 無敵時間の計算
	/// </summary>
	bool CalcInvincibleTime();

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
	/// キャラクターがチェンジ可能か
	/// </summary>
	/// <returns></returns>
	virtual bool isAnimationSwappable() const = 0;

	/// <summary>
	/// 特定のアニメーションが再生中か
	/// </summary>
	/// <returns></returns>
	virtual bool isAnimationEntable() const = 0;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	virtual bool isCollisionEntable() const = 0;

	/// <summary>
	/// 回転可能なアニメーションが再生中か
	/// </summary>
	/// <returns></returns>
	virtual bool isRotationEntable() const = 0;

	/// <summary>
	/// キャラ切り替え直後の無敵時間かどうか
	/// </summary>
	bool IsInvincible();

	//敵のダメージ判定用コンボステート
	enum EnComboState
	{
		enNowCombo_None,
		enDamageCombo_None,
		enNowCombo_1,
		enNowCombo_2,
		enNowCombo_3
	};

	/// <summary>
	/// 現在のコンボステートとダメージを受けた時のコンボステートが違うか
	/// </summary>
	/// <returns></returns>
	bool IsComboStateSame();

	/// <summary>
	/// 現在のコンボ状態を設定
	/// </summary>
	/// <param name="nowcombostate"></param>
	void SetNowComboState(EnComboState nowcombostate)
	{
		m_enNowComboState = nowcombostate;
	}
	/// <summary>
	/// ダメージを受けた時のコンボ状態を設定。
	/// ダメージを受けたキャラが設定する。
	/// またはアニメーションが終わったら設定する。
	/// </summary>
	/// <param name="oldcombostate"></param>
	void SetDamagedComboState(EnComboState damagedcombostate)
	{
		m_enDamagedComboState = damagedcombostate;
	}

	/// <summary>
	///	現在のコンボ状態を取得
	/// </summary>
	/// <returns></returns>
	EnComboState GetNowComboState() const
	{
		return m_enNowComboState;
	}
	/// <summary>
	/// ダメージを受けた時のコンボ状態を取得
	/// </summary>
	/// <returns></returns>
	EnComboState GetDamagedComboState() const
	{
		return m_enDamagedComboState;
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
	/// ステータスの取得
	/// </summary>
	/// <returns></returns>
	const Status& GetStatus() const
	{
		return m_status;
	}

	const Vector3& GetForward() const
	{
		return m_forward;
	}

	/// <summary>
	/// スキルの攻撃力の取得
	/// </summary>
	/// <returns></returns>
	const int GetSkillAttackPower() const
	{
		return m_skillAttackPower;
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
	/// 自身がやられてキャラクターを切り替えるかのフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetDieToChangeFlag(bool flag)
	{
		m_dieToChangeFlag = flag;
	}
	/// <summary>
	/// 自身がやられてキャラクターを切り替えるかのフラグを取得
	/// </summary>
	/// <returns></returns>
	const bool GetDieToChangeFlag() const
	{
		return m_dieToChangeFlag;
	}

	void SetInvicibleTimeFlag(bool flag)
	{
		m_invincibleTimeFlag = flag;
	}

	const bool GetInvincibleTimeFlag()
	{
		return m_invincibleTimeFlag;
	}

	void SetChangeCharacterInvincbleFlag(bool flag)
	{
		m_changeCharacterInvincbleFlag = flag;
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
	/// スキルの使用時などの移動はしないが回転はしたいときに使う
	/// </summary>
	virtual bool RotationOnly() = 0;

	/// <summary>
	/// MPの回復
	/// m_recoveryMpFlagがtrueの時に処理を実行
	/// </summary>
	void RecoveryMP();

	/// <summary>
	/// MPを回復するかのフラグを設定する
	/// </summary>
	/// <param name="flag"></param>
	void SetRecoveryMpFlag(bool flag)
	{
		m_recoveryMpFlag = flag;
	}

	/// <summary>
	/// 移動時の回転
	/// </summary>
	/// <returns></returns>
	Quaternion Rotation(float rotSpeed, float rotOnlySpeed);

	enum EnDashInvicibleState
	{
		enDashInvicibleState_None,
		enDashInvicibleState_On,
		enDashInvicibleState_Off
	};

	/// <summary>
	/// ダッシュした瞬間のステートの設定
	/// </summary>
	/// <param name="enDashInvicibleState"></param>
	void SetInvicibleDashState(EnDashInvicibleState enDashInvicibleState)
	{
		m_enDashInvicibleState = enDashInvicibleState;
	}

	const EnDashInvicibleState GetInvicibleDashState() const
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
	
	
protected:

	Status							m_status;
	AIActor*						m_atttackAIActor = nullptr;							//攻撃してきたAIActor(モンスター)
	
	EnComboState					m_enNowComboState = enNowCombo_None;				//現在のコンボ
	EnComboState					m_enDamagedComboState = enDamageCombo_None;			//ダメージを受けた時のコンボ
	EnDashInvicibleState			m_enDashInvicibleState = enDashInvicibleState_None;	//ダッシュしたときの無敵時間のためのステート

	Vector3							m_forward = { 0.0f,0.0f,1.0f };						//前方向
	Vector3							m_position = Vector3::Zero;							//自身の座標
	Vector3							m_moveSpeed = Vector3::Zero;						//移動方向

	Vector3							m_SaveMoveSpeed = Vector3::Zero;					//回転のみ使用する時に使う
	Vector3							m_rotMove = g_vec3Zero;								//回転の向き

	Quaternion						m_rotation = Quaternion::Identity;
	Vector3							m_scale = Vector3::One;

	int								m_skillAttackPower = 0;

	bool							m_dashFlag = false;									//ダッシュするかのフラグ

	//enumにするかも
	bool							m_dieFlag = false;									//やられたらtrueにする
	bool							m_dieToChangeFlag = false;							//やられてからキャラクター切り替えに移るためのフラグ

	bool							m_recoveryMpFlag = false;							//スキルを打ち終わったあとにtrueにする。打つ前はfalse

	bool							m_createAttackCollisionFlag = false;				//攻撃時に当たり判定を生成するかのフラグ

	bool							m_invincibleTimeFlag = false;						//無敵時間であるかのフラグ
	const float						m_invincbleTime = 2.0f;
	float							m_invincbleTimer = 0.0f;

	const float						m_invincbleDashTime = 0.3f;							//無敵状態ダッシュの時間
	float							m_invincbledDashTimer = 0.0f;

	bool							m_modelDrawFlag = false;							//モデルを描画するかのフラグ

	bool							m_changeCharacterInvincbleFlag = false;				//キャラを切り替えた直後の無敵時間のフラグ
	const float						m_changeCharaInvisibleTime = 1.0f;
	float							m_changeCharaInvisibleTimer = 0.0f;

};

