#pragma once
#include "AIActor.h"

class TurtleShell:public AIActor
{
public:
	TurtleShell();
	~TurtleShell();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void Move();




	/// <summary>
	/// 特定のアニメーションが再生中か
	/// </summary>
	/// <returns></returns>
	bool isAnimationEntable() const
	{
		return true;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool isRotationEntable() const
	{
		return true;
	}

	/// <summary>
	/// 被ダメージ時処理
	/// </summary>
	void Damage(int attack);

	///// <summary>
	///// 被ダメージ用当たり判定
	///// </summary>
	//virtual void DamageCollision(CharacterController& characon);
	////通常攻撃に当たった時の処理
	//virtual void HitNormalAttack();
	////ヒーローのスキルに当たった時の処理
	//virtual void HitHeroSkillAttack();
	////ウィザードのファイヤーボールに当たった時の処理
	//virtual void HitFireBall();
	////ウィザードのフレイムピラーに当たった時の処理
	//virtual void HitFlamePillar();

	bool RotationOnly();

private:
	/// <summary>
	/// モデルの初期化
	/// </summary>
	void InitModel();

	/// <summary>
	/// アニメーションを再生
	/// </summary>
	void PlayAnimation();

	/// <summary>
	/// ステート管理
	/// </summary>
	void ManageState();



	ModelRender m_modelRender;

	CharacterController m_charaCon;

};

