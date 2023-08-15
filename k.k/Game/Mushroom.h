#pragma once
#include "AIActor.h"

class Lich;
class IMushroomState;

class Mushroom :public AIActor
{
public:
	Mushroom();
	~Mushroom();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	void Move();

	void Attack();

	void CreateCollision();

	Vector3 SetDirection();

	bool IsBumpedForest();

	void SetLich(Lich* lich)
	{
		m_lich = lich;
	}

	/// <summary>
	/// モデルレンダーの取得
	/// </summary>
	/// <returns></returns>
	ModelRender& GetModelRender()
	{
		return m_modelRender;
	}

	/// <summary>
	/// 特定のアニメーションが再生中か
	/// </summary>
	/// <returns></returns>
	bool isAnimationEntable() const
	{
		return m_enAnimationState != enAnimationState_Damage &&
			m_enAnimationState != enAnimationState_Die;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool isRotationEntable() const
	{
		return m_enAnimationState != enAninationState_Idle;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool IsAttackEntable() const
	{
		return m_enAnimationState != enAnimationState_Attack_1 &&
			m_enAnimationState != enAnimationState_Attack_2;
	}

	/// <summary>
	/// 被ダメージ時処理
	/// </summary>
	void Damage(int attack);

	bool RotationOnly();

	// アニメーションクリップの番号を表す列挙型。
	enum EnAnimationClip {
		enAnimClip_Idle,			// 0 : 待機アニメーション
		enAnimClip_Walk,			// 1 : 歩きアニメーション
		enAnimClip_Run,				// 2 : 走りアニメーション
		enAnimClip_Attack_1,		// 3 : 
		enAnimClip_Attack_2,
		enAnimClip_Damage,
		enAnimClip_Die,
		enAnimClip_Victory,
		enAnimClip_Num,				// 7 :アニメーションクリップの数
	};

	/// <summary>
	/// 共通のステート遷移処理を実行
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// アタック１ステート遷移処理を実行
	/// </summary>
	void OnProcessAttack_1StateTransition();
	/// <summary>
	/// アタック２ステート遷移処理を実行
	/// </summary>
	void OnProcessAttack_2StateTransition();
	/// <summary>
	/// 被ダメージステート遷移処理を実行
	/// </summary>
	void OnProcessDamageStateTransition();
	/// <summary>
	/// デスステート遷移処理を実行
	/// </summary>
	void OnProcessDieStateTransition();
	/// <summary>
	/// 勝利ステート遷移処理を実行
	/// </summary>
	void OnProcessVictoryStateTransition();

	//アニメーションステート
	enum EnAnimationState {
		enAninationState_Idle,
		enAninationState_Walk,
		enAninationState_Run,
		enAnimationState_Attack_1,
		enAnimationState_Attack_2,
		enAnimationState_Damage,
		enAnimationState_Die,
		enAnimationState_Victory
	};

	/// <summary>
	/// 次のアニメーションステートを作成する。
	/// </summary>
	/// <param name="nextState"></param>
	void SetNextAnimationState(EnAnimationState nextState);

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


	enum EnAttackName
	{
		enAttackName_1,
		enAttackName_2
	};

	Lich* m_lich = nullptr;
	IMushroomState* m_state = nullptr;

	Animation m_animation;	// アニメーション
	AnimationClip m_animationClip[enAnimClip_Num];	// アニメーションクリップ 

	EnAnimationState m_enAnimationState = enAninationState_Idle;	//アニメーションステート

	ModelRender m_modelRender;

	CharacterController m_charaCon;

	float m_angleChangeTime = 6.0f;		//ベクトルを計算するタイマー

	Vector3 m_direction = Vector3::Zero;

	const float m_distanceToPlayer = 400.0f;
	const float m_attackRange = 60.0f;
	const float m_stayDistance = 50.0f;

	int m_attackBoonId = -1;					//攻撃で使うボーンID

	bool m_createAttackCollisionFlag = false;

	const float m_attackIntervalTime = 1.0f;	//攻撃した後のインターバル

};

