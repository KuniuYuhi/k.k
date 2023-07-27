#pragma once
#include "AIActor.h"
class ILichState;
class LichAction;
class Lich:public AIActor
{
public:
	Lich();
	~Lich();

	/// <summary>
	/// 攻撃に関する情報
	/// </summary>
	struct InfoAboutAttack
	{
		const float m_Attack_1Distance = 600.0f;	//遠距離攻撃
		const float m_Attack_2Distance = 200.0f;	//近距離攻撃
	};

	bool Start();
	void Update();

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	void Render(RenderContext& rc);

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

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 被ダメージ時処理
	/// </summary>
	/// <param name="attack">くらう攻撃力</param>
	void Damage(int attack);

	/// <summary>
	/// 被ダメージ用当たり判定
	/// </summary>
	void DamageCollision(CharacterController& characon)override;

	/// <summary>
	/// アタック１の攻撃処理
	/// </summary>
	bool Attack();
	/// <summary>
	/// アタック２の攻撃処理
	/// </summary>
	/// <returns></returns>
	bool Attack2();

	/// <summary>
	/// ダークウォールの生成
	/// </summary>
	void CreateDarkWall();

	/// <summary>
	/// 次の行動を決定
	/// </summary>
	void DecideNextAction();

	/// <summary>
	/// 怯むかどうか
	/// </summary>
	/// <returns></returns>
	bool Isflinch();

	bool IsDistanceToPlayer();

	/// <summary>
	/// 特定のアニメーションが再生中か
	/// </summary>
	/// <returns></returns>
	bool isAnimationEntable() const
	{
		return m_enAnimationState != enAnimationState_Damage&&
			m_enAnimationState != enAnimationState_Die;
	}

	/// <summary>
	/// 攻撃アニメーションが再生中か
	/// </summary>
	/// <returns></returns>
	bool IsAttackEntable() const
	{
		return m_enAnimationState != enAnimationState_Attack_1 &&
			m_enAnimationState != enAnimationState_Attack_2;
	}

	/// <summary>
	/// 回転可能かどうか
	/// </summary>
	/// <returns></returns>
	bool isRotationEntable() const
	{
		return m_enAnimationState != enAnimationState_Attack_1 &&
			m_enAnimationState != enAninationState_Idle;
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
	/// ダークウォールのボーンIdを取得
	/// </summary>
	/// <returns></returns>
	const int& GetDarkWallBoonId() const
	{
		return m_darkWallBoonId;
	}

	const InfoAboutAttack& GetInfoAboutAttack()
	{
		return m_InfoAboutAttack;
	}


	// アニメーションクリップの番号を表す列挙型。
	enum EnAnimationClip {
		enAnimClip_Idle,			// 0 : 待機アニメーション
		enAnimClip_Walk,			// 1 : 歩きアニメーション
		enAnimClip_Run,				// 2 : 走りアニメーション
		enAnimClip_Attack_1,		// 3 : 
		enAnimClip_Attack_2,		// 4 : 
		enAnimClip_Attack_3_start,		// 5 : 
		enAnimClip_Attack_3_main,
		enAnimClip_Attack_4,		// 6 : 
		enAnimClip_Damage,
		enAnimClip_Die,
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
	/// アタック2ステート遷移処理を実行
	/// </summary>
	void OnProcessAttack_2StateTransition();
	/// <summary>
	/// Dieステート遷移処理を実行
	/// </summary>
	void OnProcessDieStateTransition();
	/// <summary>
	/// 被ダメージ遷移処理を実行
	/// </summary>
	void OnProcessDamageStateTransition();

	//アニメーションステート
	enum EnAnimationState {
		enAninationState_Idle,
		enAninationState_Walk,
		enAninationState_Run,
		enAnimationState_Attack_1,
		enAnimationState_Attack_2,
		enAnimationState_Attack_3_start,
		enAnimationState_Attack_3_main,
		enAnimationState_Attack_4,
		enAnimationState_Damage,
		enAnimationState_Die
	};

	/// <summary>
	/// 次のアニメーションステートを作成する。
	/// </summary>
	/// <param name="nextState"></param>
	void SetNextAnimationState(EnAnimationState nextState);

private:

	bool RotationOnly();

	
	LichAction* m_lichAction = nullptr;

	ILichState* m_state = nullptr;

	Animation m_animation;	// アニメーション
	AnimationClip m_animationClip[enAnimClip_Num];	// アニメーションクリップ 

	EnAnimationState m_enAnimationState = enAninationState_Idle;	//アニメーションステート

	ModelRender m_modelRender;

	CharacterController m_charaCon;

	FontRender m_hpFont;

	const float m_distanceToPlayer = 300.0f;

	int m_darkWallBoonId = -1;					//ダークウォールで使うボーンID

	bool m_attackRangeFlag = false;				//攻撃範囲にいるかのフラグ

	bool m_CreateDarkWallFlag = false;			//ダークウォール生成フラグ

	const float m_attackIntervalTime = 4.0f;
	const float m_damageIntervalTime = 0.5f;
	
	InfoAboutAttack m_InfoAboutAttack;



	bool m_dieFlag = false;

};

