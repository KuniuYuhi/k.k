#pragma once
#include "IMagicBall.h"


class MagicBallMovement;

/// <summary>
/// サモナーが放つ魔法球：ダークメテオ
/// </summary>
class DarkMeteorite : public IMagicBall
{
public:
	DarkMeteorite();
	~DarkMeteorite();

	bool Start() override;

	void Update() override;

	void Render(RenderContext& rc) override;

	void InitModel() override;


private:

	/// <summary>
	/// 追加で設定するコンポーネント
	/// </summary>
	void AddSettingComponents() override;

	/// <summary>
	/// 当たり判定生成
	/// </summary>
	void CreateCollision();

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 消去するか？
	/// </summary>
	/// <returns></returns>
	bool IsDelete();

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool IsGroundHit();

	/// <summary>
	/// 爆発する
	/// </summary>
	void Explosion();


private:

	MagicBallMovement* m_magicBallMovement = nullptr;

	CollisionObject* m_collision = nullptr;


	

};

