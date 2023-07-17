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

	void SetTargetPosition();

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

	Vector3 m_SaveMoveSpeed = Vector3::Zero;		//回転のみ使用する時に使う

	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;

	bool m_dashFlag = false;		//ダッシュするかのフラグ

	float m_distance = 0.0f;	//プレイヤーを発見できる距離

};

