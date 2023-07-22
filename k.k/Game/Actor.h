#pragma once
#include "Status.h"

//class Player;

class Actor:public IGameObject,public Noncopyable
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
	/// アニメーションステートを設定する
	/// </summary>
	virtual void SetAnimationState() = 0;

	/// <summary>
	/// キャラコンの座標設定
	/// </summary>
	/// <param name="position"></param>
	void SetCharaConPosition(Vector3 position)
	{
		//m_charaCon.SetPosition(position);
	}

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="status">ステータス</param>
	/// <returns>moveSpeed</returns>
	Vector3 calcVelocity(Status status);

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

	/// <summary>
	/// ステータスの取得
	/// </summary>
	/// <returns></returns>
	const Status GetStatus()
	{
		return m_status;
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
	Quaternion Rotation();

	

protected:

	Status m_status;
	//Player* m_player = nullptr;

	
	EnComboState m_enNowComboState = enNowCombo_None;		//現在のコンボ
	EnComboState m_enDamagedComboState = enDamageCombo_None;		//ダメージを受けた時のコンボ

	Vector3 m_position = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;

	Vector3 m_SaveMoveSpeed = Vector3::Zero;		//回転のみ使用する時に使う

	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;

	int m_skillAttackPower = 0;

	bool m_dashFlag = false;		//ダッシュするかのフラグ

	//enumにするかも
	bool m_dieFlag = false;			//やられたらtrueにする
	bool m_dieToChangeFlag = false;		//やられてからキャラクター切り替えに移るためのフラグ

	bool m_recoveryMpFlag = false;	//スキルを打ち終わったあとにtrueにする。打つ前はfalse

	bool m_createAttackCollisionFlag = false;		//攻撃時に当たり判定を生成するかのフラグ
};

