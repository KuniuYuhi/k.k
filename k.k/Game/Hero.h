#pragma once
#include "Actor.h"
class IHeroState;
class Player;

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

	/// <summary>
	/// スキル使用時の当たり判定の作成
	/// </summary>
	void CreateSkillCollision();

	void Damage(int attack);

	void PowerUpTimer();

	bool CalcDash();

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
			m_enAnimationState != enAninationState_Walk /*&&
			m_enAnimationState != enAninationState_Dash*/;
	}

	/// <summary>
	/// 特定のアニメーションが再生中なら移動できない
	/// </summary>
	/// <returns></returns>
	bool isAnimationEntable() const
	{
		return m_enAnimationState != enAninationState_Dash &&
			m_enAnimationState != enAnimationState_Attack_1 &&
			m_enAnimationState != enAnimationState_Attack_2 &&
			m_enAnimationState != enAnimationState_Attack_3 &&
			m_enAnimationState != enAnimationState_Attack_Skill_Charge&&
			m_enAnimationState != enAnimationState_PowerUp &&
			m_enAnimationState != enAnimationState_Damage&&
			m_enAnimationState != enAnimationState_Die;
	}

	/// <summary>
	/// 特定のアニメーションが再生中のとき当たり判定を取らない
	/// </summary>
	/// <returns></returns>
	bool isCollisionEntable() const
	{
		return m_enAnimationState != enAninationState_Dash &&
			m_enAnimationState != enAnimationState_Damage &&
			m_enAnimationState != enAnimationState_PowerUp &&
			m_enAnimationState != enAnimationState_Die;
	}

	bool isRotationEntable() const
	{
		return m_enAnimationState != enAninationState_Dash &&
			m_enAnimationState != enAnimationState_Attack_1 &&
			m_enAnimationState != enAnimationState_Attack_2 &&
			m_enAnimationState != enAnimationState_Attack_Skill_Charge;
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
	/// ダッシュのステート遷移処理を実行
	/// </summary>
	void OnProcessDashStateTransition();
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
	/// アタックスキルチャージのステート遷移処理を実行
	/// </summary>
	void OnProcessAttack_Skill_ChargeStateTransition();
	/// <summary>
	/// アタックスキルメインのステート遷移処理を実行
	/// </summary>
	void OnProcessAttack_Skill_MainStateTransition();
	/// <summary>
	/// 倒されたときのステート遷移処理を実行
	/// </summary>
	void OnProcessDieStateTransition();
	/// <summary>
	/// ダメージを受けた時のステート遷移処理を実行
	/// </summary>
	void OnProcessDamageStateTransition();
	/// <summary>
	/// パワーアップのステート遷移処理を実行
	/// </summary>
	void OnProcessPowerUpStateTransition();
	/// <summary>
	/// 勝利のステート遷移処理を実行
	/// </summary>
	void OnProcessVictoryStateTransition();

public:
	// アニメーションクリップの番号を表す列挙型。
	enum EnAnimationClip {
		enAnimClip_Idle,	// 0 : 待機アニメーション
		enAnimClip_Walk,	// 1 : 歩きアニメーション
		enAnimClip_Dash,		// 2 : 走りアニメーション
		enAnimClip_Attack_1,// 3 : 
		enAnimClip_Attack_2,// 4 : 
		enAnimClip_Attack_3,// 5 : 
		enAnimClip_Attack_Skill_Charge,
		enAnimClip_Attack_Skill_Main,// 6 : 
		enAnimClip_PowerUp,
		enAnimClip_Victory,
		enAnimClip_Die,
		enAnimClip_Damage,
		enAnimClip_Num,		// 7 :アニメーションクリップの数
	};

private:
	//アニメーションステート
	enum EnAnimationState {
		enAninationState_Idle,
		enAninationState_Walk,
		enAninationState_Dash,
		enAnimationState_Attack_1,
		enAnimationState_Attack_2,
		enAnimationState_Attack_3,
		enAnimationState_Attack_Skill_Charge,
		enAnimationState_Attack_Skill_Main,
		enAnimationState_PowerUp,
		enAnimationState_Victory,
		enAnimationState_Die,
		enAnimationState_Damage
	};

	//攻撃パターンステート
	enum EnAttackPattern {
		enAttackPattern_None,
		enAttackPattern_1,
		enAttackPattern_1to2,
		enAttackPattern_2,
		enAttackPattern_2to3,
		enAttackPattern_3,
		enAttackPattern_Skill_Charge,
		enAttackPattern_Skill_Main,
		enAttackPattern_Skill_PowerUp,
		enAttackPattern_End
	};

	/// <summary>
	/// 勝利ステートを設定する
	/// </summary>
	void SetVictoryAnimationState()
	{
		SetNextAnimationState(enAnimationState_Victory);
	}

private:
	
	/// <summary>
	/// 次のアニメーションステートを作成する。
	/// </summary>
	/// <param name="nextState"></param>
	void SetNextAnimationState(EnAnimationState nextState);

	bool RotationOnly();



	ModelRender m_modelRender;

	Animation m_animation;	// アニメーション
	AnimationClip m_animationClip[enAnimClip_Num];	// アニメーションクリップ 

	EnAnimationState m_enAnimationState = enAninationState_Idle;	//アニメーションステート

	EnAttackPattern m_enAttackPatternState = enAttackPattern_None;	//現在の攻撃タイミングを知るためのステート

	Player* m_player = nullptr;

	IHeroState* m_state = nullptr;					//ステートクラス

	const int m_skillMp = 30;	//スキル発動に必要なMP
	const int m_skillPowerUpMp = 80;		//パワーアップに必要な

	bool m_powerUpTimeFlag = false;
	const float m_powerUpTime = 15.0f;				//パワーアップする時間
	float m_powerUpTimer = 0.0f;					//パワーアップの時間を計るタイマー
	const int m_powerUpPower = 10;					//パワーアップした時の上がる攻撃力

	int m_swordBoonId = -1;		//剣のボーンID取得用変数
	int m_skillBoonId = -1;		//スキル使用時のボーン取得用変数

	bool m_createSkillCollisionFlag = false;		//スキル使用時に当たり判定を生成するかのフラグ
	float m_ChargeTimer = 0.0f;
	const float m_MaxChargeTime = 3.0f;				//スキル１の溜めの最大時間


	float m_dashTimer = 1.0f;

};

