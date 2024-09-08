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
	/// 追いかける
	/// </summary>
	/// <param name="targetPosition"></param>
	void MoveChase(Vector3 targetPosition);

private:

	IMagicBall* m_magicBall = nullptr;


};

