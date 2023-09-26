#pragma once
#include "MobMonster.h"

class Lich;
class ITurtleShellState;

class TurtleShell:public MobMonster
{
public:
	TurtleShell();
	~TurtleShell();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	void Attack();

	bool Difence();

	/// <summary>
	/// 次の行動を決める
	/// </summary>
	void DecideNextAction();

	void CreateCollision();

	/// <summary>
	/// 処理を止めるか
	/// </summary>
	/// <returns></returns>
	bool IsStopProcessing();

	void SetLich(Lich* lich)
	{
		m_lich = lich;
	}

	/// <summary>
	/// 防御タイマーの処理
	/// </summary>
	bool IsDifenceTime();

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

		return //m_enAnimationState != enAnimationState_Difence &&
			m_enAnimationState != enAnimationState_DifenceDamage &&
			m_enAnimationState != enAnimationState_Damage &&
			m_enAnimationState != enAnimationState_Die;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool isRotationEntable() const
	{
		return m_enAnimationState != enAnimationState_Difence &&
			m_enAnimationState != enAninationState_Idle;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool IsAttackEntable() const
	{
		return /*m_enAnimationState != enAnimationState_Difence &&*/
			m_enAnimationState != enAnimationState_Attack_1 &&
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
		enAnimClip_Difence,
		enAnimClip_DefenceDamage,
		enAnimClip_Damage,
		enAnimClip_Die,
		enAnimClip_Victory,
		enAnimClip_Appear,
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
	/// 防御ステート遷移処理を実行
	/// </summary>
	void OnProcessDifenceStateTransition();
	/// <summary>
	/// 防御被ダメージステート遷移処理を実行
	/// </summary>
	void OnProcessDifenceDamageStateTransition();
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
	/// <summary>
	/// 召喚された時のステート遷移処理を実行
	/// </summary>
	void OnProcessAppearStateTransition();

	//アニメーションステート
	enum EnAnimationState {
		enAninationState_Idle,
		enAninationState_Walk,
		enAninationState_Run,
		enAnimationState_Attack_1,
		enAnimationState_Attack_2,
		enAnimationState_Difence,
		enAnimationState_DifenceDamage,
		enAnimationState_Damage,
		enAnimationState_Die,
		enAnimationState_Victory,
		enAnimationState_Appear
	};

	/// <summary>
	/// 次のアニメーションステートを作成する。
	/// </summary>
	/// <param name="nextState"></param>
	void SetNextAnimationState(EnAnimationState nextState);

	enum EnDefenceState
	{
		enDefenceState_None,
		enDefenceState_damaged,
		enDefenceState_Defence,
		enDefenceState_DefenceDamaged
	};

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


	//Lich* m_lich = nullptr;
	ITurtleShellState* m_state = nullptr;

	Animation m_animation;	// アニメーション
	AnimationClip m_animationClip[enAnimClip_Num];	// アニメーションクリップ 

	EnAnimationState m_enAnimationState = enAninationState_Idle;	//アニメーションステート

	//float m_angleChangeTime = 0.0f;		//ベクトルを計算するタイマー。生成時に決める

	ModelRender m_modelRender;

	CharacterController m_charaCon;

	//Vector3 m_direction = Vector3::Zero;

	//const float m_attackIntervalTime = 2.0f;	//攻撃した後のインターバル

	//const float m_distanceToPlayer = 400.0f;
	//const float m_attackRange = 60.0f;
	//const float m_stayDistance = 50.0f;

	int m_attackBoonId = -1;					//攻撃で使うボーンID

	bool m_createAttackCollisionFlag = false;


	bool m_damagedFlag = false;			//自身が攻撃を受けたかのフラグ
	bool m_difenceFlag = false;			//防御しているかのフラグ
	const float m_difenceTime = 3.0f;
	float m_difenceTimer = 0.0f;

	EnDefenceState m_defenceState = enDefenceState_None;

};

