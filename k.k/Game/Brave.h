#pragma once
#include "Actor.h"

class Player;
class IBraveState;

/// <summary>
/// 勇者クラス
/// </summary>
class Brave:public Actor
{
public:
	Brave();
	~Brave();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// モデルの初期化
	/// </summary>
	void InitModel();
	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();
	/// <summary>
	/// 攻撃処理
	/// </summary>
	void ProcessAttack();
	/// <summary>
	/// ダメージを受けた時の処理
	/// </summary>
	/// <param name="damage">受けるダメージ</param>
	void Damage(int damage);

	/// <summary>
	/// キャラクターがチェンジ可能か
	/// </summary>
	/// <returns></returns>
	bool isAnimationSwappable() const
	{
		return true;
	}

	/// <summary>
	/// 特定のアニメーションが再生中か
	/// </summary>
	/// <returns></returns>
	bool isAnimationEntable() const
	{
		return true;
	}

	/// <summary>
	/// 当たり判定可能なアニメーションか
	/// </summary>
	/// <returns></returns>
	bool isCollisionEntable() const
	{
		return true;
	}

	/// <summary>
	/// 回転可能なアニメーションが再生中か
	/// </summary>
	/// <returns></returns>
	bool isRotationEntable() const
	{
		return true;
	}

	/// <summary>
	/// 勝利ステートを設定する
	/// </summary>
	void SetVictoryAnimationState()
	{

	}
	/// <summary>
	/// アイドルステートを設定する
	/// </summary>
	void SetIdleAnimationState()
	{

	}

	// アニメーションクリップの番号を表す列挙型。
	enum EnAnimationClip {
		enAnimClip_Idle,
		enAnimClip_Sprint,
		enAnimClip_DashForward,
		enAnimClip_KnockBack,
		enAnimClip_Hit,
		enAnimClip_Defend,
		enAnimClip_DefendHit,
		enAnimClip_Die,
		enAnimClip_ChangeSwordShield,
		enAnimClip_Win_Start,
		enAnimClip_Win_Main,
		enAnimClip_attack1,
		enAnimClip_attack2,
		enAnimClip_attack3,
		enAnimClip_attack4,
		enAnimClip_attack5,

		enAnimClip_Num
	};

	//アニメーションステート
	enum EnAnimationState {
		enAninationState_Idle,
		enAninationState_Sprint,
		enAninationState_DashForward,
		enAnimationState_KnockBack,
		enAnimationState_Hit,
		enAnimationState_Defend,
		enAnimationState_DefendHit,
		enAnimationState_Die,
		enAnimationState_ChangeSwordShield,
		enAnimationState_Win_Start,
		enAnimationState_Win_Main,
		enAnimationState_Attack_1,
		enAnimationState_Attack_2,
		enAnimationState_Attack_3,
		enAnimationState_Attack_4,
		enAnimationState_Attack_5,
	};

	/// <summary>
	/// モデルレンダーの取得
	/// </summary>
	/// <returns></returns>
	ModelRender& GetModelRender()
	{
		return m_modelRender;
	}

	/// <summary>
	/// 次のアニメーションステートを作成する。
	/// </summary>
	/// <param name="nextState"></param>
	void SetNextAnimationState(EnAnimationState nextState);

private:
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// ステート管理
	/// </summary>
	void ManageState();

	/// <summary>
	/// スキルの使用時などの移動はしないが回転はしたいときに使う
	/// </summary>
	bool RotationOnly();


	void UpdateWeapons();


private:

	Player* m_player = nullptr;
	IBraveState* m_BraveState = nullptr;

	EnAnimationState m_enAnimationState = enAninationState_Idle;			//アニメーションステート

	CharacterController m_charaCon;

	Animation	m_animation;				// アニメーション
	AnimationClip	m_animationClip[enAnimClip_Num];// アニメーションクリップ 

	ModelRender m_modelRender;



	ModelRender Sword;
	int m_swordBoonId = -1;

	ModelRender Shield;
	int m_shieldBoonId = -1;

	int m_playAnimCount = 0;

};

