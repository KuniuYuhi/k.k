#pragma once
#include "MagicBallStatus.h"


class MagicBallStatus;
class CharacterBase;
class DamageProvider;

/// <summary>
/// 魔法球の親クラス
/// </summary>
class IMagicBall:public IGameObject
{
public:

	virtual ~IMagicBall() {}

	/// <summary>
	/// ローカル座標を設定
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector3 position)
	{
		m_position = position;
	}
	/// <summary>
	/// ローカル座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetPosition()
	{
		return m_position;
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
	/// 回転を取得
	/// </summary>
	/// <returns></returns>
	Quaternion GetRotation()
	{
		return m_rotation;
	}

	/// <summary>
	/// 拡大率を設定
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(Vector3 scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// 拡大率を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetScale()
	{
		return m_scale;
	}

	/// <summary>
	/// 前方向を設定
	/// </summary>
	/// <param name="forward"></param>
	void SetForward(Vector3 forward)
	{
		m_forward = forward;
	}
	/// <summary>
	/// 前方向を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetForward()
	{
		return m_forward;
	}

	/// <summary>
	/// 移動速度を設定
	/// </summary>
	/// <param name="moveSpeed"></param>
	void SetMoveSpeed(Vector3 moveSpeed)
	{
		m_moveSpeed = moveSpeed;
	}
	/// <summary>
	/// 移動速度を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetMoveSpeed()
	{
		return m_moveSpeed;
	}
	
	/// <summary>
	/// スピードパラメータの取得
	/// </summary>
	/// <returns></returns>
	float GetSpeedParameter()
	{
		return m_status.GetSpeed();
	}

	
	/// <summary>
	/// 魔法球を放つ時のパラメータの設定
	/// </summary>
	/// <param name="startPosition">開始座標</param>
	/// <param name="forward">撃つ方向（前方向</param>
	/// <param name="chaseCharacter">追いかけるキャラクター。（いないならそれでいい）</param>
	void SetShotMagicBallParameters(
		Vector3 startPosition,
		Vector3 forward,
		CharacterBase* chaseCharacter = nullptr
	);


protected:
	/// <summary>
	/// 基本的なコンポーネントの設定
	/// </summary>
	void DefaultSettingComponents();

	/// <summary>
	/// 追加で設定するコンポーネント
	/// </summary>
	virtual void AddSettingComponents() {}


protected:

	MagicBallStatus m_status;

	DamageProvider* m_damageProvider = nullptr;		//ダメージ提供者インスタンス

	CharacterBase* m_chaseCharacter = nullptr;		//追いかける時のキャラクターインスタンス
	

	Vector3 m_position = g_vec3Zero;				//自身の座標
	Quaternion m_rotation = Quaternion::Identity;	//回転
	Vector3 m_scale = g_vec3One;					//拡大率

	Vector3 m_forward = g_vec3Zero;					//前方向
	Vector3 m_moveSpeed = g_vec3Zero;				//移動速度

	Vector3 m_startPosition = g_vec3Zero;			//開始座標



};

