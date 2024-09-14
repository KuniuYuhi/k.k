#pragma once
#include "IComponent.h"

class IMagicBall;

/// <summary>
/// 魔法球移動用コンポーネント
/// </summary>
class MagicBallMovement : public IComponent
{
public:
	MagicBallMovement();
	~MagicBallMovement();

	void Start() override;

	/// <summary>
	/// 更新処理。自動で行ってくれる。消したいならGameObjectManagerへ
	/// </summary>
	void UpdateComponent() override;



	/// <summary>
	/// 魔法球インスタンスを設定
	/// </summary>
	/// <param name="magicBall"></param>
	void SetMagicBallInstance(IMagicBall* magicBall)
	{
		m_magicBall = magicBall;
	}

	/// <summary>
	/// 直進移動
	/// </summary>
	void MoveStraight();

	/// <summary>
	/// 落ちる移動
	/// </summary>
	void MoveFall();


	/// <summary>
	/// 追いかける移動
	/// </summary>
	/// <param name="targetPosition">ターゲットの座標</param>
	/// <param name="radius">半径。(ターゲットに近すぎる時においかけないようにする)</param>
	void MoveChase(
		Vector3 targetPosition, float radius,
		float yUp, bool setYUp = false
	);

private:

	IMagicBall* m_magicBall = nullptr;

	Vector3 m_direction = g_vec3Zero;
};

