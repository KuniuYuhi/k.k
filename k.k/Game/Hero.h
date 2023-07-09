#pragma once
#include "Actor.h"
class IHeroState;
class Hero :public Actor
{
public:
	Hero();
	~Hero();

	bool Start();

	/// <summary>
	/// モデルの初期化
	/// </summary>
	void InitModel();

	void Update();
	void Render(RenderContext& rc);

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();
	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Attack();
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// ステート管理
	/// </summary>
	void ManageState();

	/// <summary>
	/// 攻撃時の当たり判定の作成
	/// </summary>
	void CreateCollision();


	bool GetAtkCollsionCreateFlag() const
	{
		return m_createAttackCollisionFlag;
	}

	/// <summary>
	/// 特定のアニメーションが再生中なら入れ替え可能。?
	/// </summary>
	/// <returns></returns>
	bool isAnimationSwappable() const
	{
		return m_enAnimationState != enAninationState_Idle &&
			m_enAnimationState != enAninationState_Walk &&
			m_enAnimationState != enAninationState_Run;
	}

	/// <summary>
	/// 特定のアニメーションが再生中なら移動できない
	/// </summary>
	/// <returns></returns>
	bool isAnimationEntable() const
	{
		return m_enAnimationState != enAnimationState_Attack_1 &&
			m_enAnimationState != enAnimationState_Attack_2 &&
			m_enAnimationState != enAnimationState_Attack_3 /*&&
			m_enAnimationState != enAnimationState_Attack_4*/;
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
	/// 共通のステート遷移処理を実行
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// アタック１のステート遷移処理を実行
	/// </summary>
	void OnProcessAttack_1StateTransition();
	/// <summary>
	/// アタック2のステート遷移処理を実行
	/// </summary>
	void OnProcessAttack_2StateTransition();
	/// <summary>
	/// アタック3のステート遷移処理を実行
	/// </summary>
	void OnProcessAttack_3StateTransition();
	/// <summary>
	/// アタック4のステート遷移処理を実行
	/// </summary>
	void OnProcessAttack_4StateTransition();

public:
	// アニメーションクリップの番号を表す列挙型。
	enum EnAnimationClip {
		enAnimClip_Idle,	// 0 : 待機アニメーション
		enAnimClip_Walk,	// 1 : 歩きアニメーション
		enAnimClip_Run,		// 2 : 走りアニメーション
		enAnimClip_Attack_1,// 3 : 
		enAnimClip_Attack_2,// 4 : 
		enAnimClip_Attack_3,// 5 : 
		enAnimClip_Attack_4,// 6 : 
		enAnimClip_Num,		// 7 :アニメーションクリップの数
	};

private:
	//アニメーションステート
	enum EnAnimationState {
		enAninationState_Idle,
		enAninationState_Walk,
		enAninationState_Run,
		enAnimationState_Attack_1,
		enAnimationState_Attack_2,
		enAnimationState_Attack_3,
		enAnimationState_Attack_4
	};

	//攻撃パターンステート
	enum EnAttackPattern {
		enAttackPattern_None,
		enAttackPattern_1,
		enAttackPattern_2,
		enAttackPattern_3,
		enAttackPattern_4,
		enAttackPattern_End
	};

public:
	//アニメーションステートを設定する
	void SetAnimationState()
	{
		//m_enAnimationState= enAninationState_Idle;
	}

private:

	
	
	/// <summary>
	/// 次のアニメーションステートを作成する。
	/// </summary>
	/// <param name="nextState"></param>
	void SetNextAnimationState(EnAnimationState nextState);


	ModelRender m_modelRender;

	Animation m_animation;	// アニメーション
	AnimationClip m_animationClip[enAnimClip_Num];	// アニメーションクリップ 

	EnAnimationState m_enAnimationState = enAninationState_Idle;	//アニメーションステート

	EnAttackPattern m_enAttackPatternState = enAttackPattern_None;

	IHeroState* m_state = nullptr;					//ステートクラス


	int m_swordBoonId = -1;		//剣のボーンID取得用変数
	bool m_createAttackCollisionFlag = false;		//攻撃時に当たり判定を生成するかのフラグ

};

