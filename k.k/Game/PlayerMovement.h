#pragma once
//#include "Component.h"

#include "IComponent.h"


class Brave;
class Status_Player;

/// <summary>
/// キャラクター移動用コンポーネント
/// </summary>
class PlayerMovement : public IComponent
{
public:
	PlayerMovement();
	~PlayerMovement();

	void Start() override;

	void UpdateComponent() override;


	

	/// <summary>
	/// 普段の移動速度の計算。プレイヤーの前方向も設定するよ！
	/// </summary>
	/// <param name="playerStatus">ステータス</param>
	/// <param name="moveSpeed">現在の移動速度</param>
	/// <param name="StickInput">コントローラーの移動スティックの入力量</param>
	/// <returns>移動速度</returns>
	Vector3 CalcSimpleMovementVerocity(
		float speed,
		Vector3 moveSpeed,
		Vector3 StickInput
	);


	
	/// <summary>
	/// 移動方向のみの計算
	/// </summary>
	/// <param name="forward">前方向</param>
	/// <param name="moveSpeed">現在の移動速度</param>
	/// <returns></returns>
	Vector3 CalcForwardDirection(Vector3 forward, Vector3 moveSpeed = g_vec3Zero);


	/// <summary>
	/// 移動方向のみの計算。入力方向も考慮
	/// </summary>
	/// <param name="forward">前方向</param>
	/// <param name="stick">入力方向</param>
	/// <param name="moveSpeed">現在の移動速度</param>
	/// <returns></returns>
	Vector3 CalcMoveDirection(Vector3 forward, Vector3 stick, Vector3 moveSpeed = g_vec3Zero);


private:

	/// <summary>
	/// カメラの前方向を取得。正規化済み
	/// </summary>
	/// <returns></returns>
	Vector3 GetCameraForward();
	/// <summary>
	/// カメラの右方向を取得。正規化済み
	/// </summary>
	/// <returns></returns>
	Vector3 GetCameraRight();



private:

	Brave* m_brave = nullptr;


	Vector3 m_moveDirection = g_vec3Zero;		//移動方向のみ。正規化すること！！


};

