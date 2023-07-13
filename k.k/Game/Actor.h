#pragma once
#include "Status.h"
#include "Player.h"

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
	/// 座標の取得
	/// </summary>
	/// <returns></returns>
	const Vector3 GetPosition()
	{
		return m_position;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const Vector3 GetScale()
	{
		return m_scale;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const Quaternion GetRotation()
	{
		return m_rotation;
	}

	const Status GetStatus()
	{
		return m_status;
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
	Player* m_player = nullptr;

	//CharacterController m_charaCon;

	Vector3 m_position = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;

	Vector3 m_SaveMoveSpeed = Vector3::Zero;		//回転のみ使用する時に使う

	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;

	bool m_dashFlag = false;		//ダッシュするかのフラグ

	bool m_recoveryMpFlag = false;	//スキルを打ち終わったあとにtrueにする。打つ前はfalse

};

