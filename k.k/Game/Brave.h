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

	/// <summary>
	/// アクションする時に使うフラグをまとめている
	/// </summary>
	struct InfoAboutActionFlag
	{
		bool isActionFlag = false;		//アクションフラグ
		bool dashAttackFlag = false;	//前進攻撃フラグ
		bool nextComboFlag = false;		//次のコンボ攻撃をするかのフラグ
		bool isComboReceptionFlag = false;	//コンボ受付可能フラグ
	};

	Brave();
	~Brave();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	/// <summary>
	/// モデルの初期化
	/// </summary>
	void InitModel();
	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();
	/// <summary>
	/// 回転処理
	/// </summary>
	void ProcessRotation();
	/// <summary>
	/// 攻撃処理
	/// </summary>
	void ProcessAttack();
	/// <summary>
	/// 防御処理
	/// </summary>
	void ProcessDefend();
	/// <summary>
	/// ダメージを受けた時の処理
	/// </summary>
	/// <param name="damage">受けるダメージ</param>
	void Damage(int damage);
	/// <summary>
	/// 行動不可能かどうかの判定可能
	/// </summary>
	/// <returns>trueで不可能、falseで可能</returns>
	const bool& IsInaction() const;

	/// <summary>
	/// 攻撃時に前進する時に使う
	/// </summary>
	/// <param name="Speed">前進する速さ</param>
	void CalcAttackDirection(float Speed);

	void ChangeWeapon();



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
		return m_enAnimationState != enAnimationState_Attack_1 &&
			m_enAnimationState != enAnimationState_Attack_2 &&
			m_enAnimationState != enAnimationState_Attack_3 &&
			m_enAnimationState != enAnimationState_Skill_start &&
			m_enAnimationState != enAnimationState_Skill_Main &&
			m_enAnimationState != enAnimationState_Defend &&
			m_enAnimationState != enAnimationState_Hit &&
			m_enAnimationState != enAnimationState_Defend &&
			m_enAnimationState != enAnimationState_Die;
	}

	/// <summary>
	/// 当たり判定可能なアニメーションか
	/// </summary>
	/// <returns></returns>
	bool isCollisionEntable() const
	{
		return m_enAnimationState != enAnimationState_Hit &&
			m_enAnimationState != enAnimationState_Defend;
	}

	/// <summary>
	/// 回転可能なアニメーションが再生中か
	/// </summary>
	/// <returns></returns>
	bool isRotationEntable() const
	{
		return m_enAnimationState == enAnimationState_Defend;
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

	

	/// <summary>
	/// 武器が剣盾の時のスキルの処理
	/// </summary>
	/// <param name="UpOrDownFlag">trueでUp、falseDown</param>
	void ProcessSwordShieldSkill(bool UpOrDownFlag);

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
		enAnimClip_Skill_Start,
		enAnimClip_Skill_Main,

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
		enAnimationState_Skill_start,
		enAnimationState_Skill_Main,
	};

	//攻撃パターンステート。自分がコンボのタイミングを図るため
	enum EnAttackPattern {
		enAttackPattern_None = static_cast<EnAttackPattern>(enAnimationState_Attack_1 - 1),
		enAttackPattern_1 = enAnimationState_Attack_1,
		enAttackPattern_2,
		enAttackPattern_3,
		enAttackPattern_End
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
	void SetNextAnimationState(int nextState);

	/// <summary>
	/// ステート共通の状態遷移処理
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// 通常攻撃ステートの状態遷移処理
	/// </summary>
	void ProcessNormalAttackStateTransition();
	/// <summary>
	/// スキルのスタートステートの状態遷移処理
	/// </summary>
	void ProcessSkillStartStateTransition();
	/// <summary>
	/// スキルのメインステートの状態遷移処理
	/// </summary>
	void ProcessSkillMainStateTransition();
	/// <summary>
	/// 攻撃を受けた時のステートの状態遷移処理
	/// </summary>
	void ProcessHitStateTransition();
	/// <summary>
	/// やられた時のステートの状態遷移処理
	/// </summary>
	void ProcessDieStateTransition();

	/// <summary>
	/// アクションフラグ構造体の全てのフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetAllInfoAboutActionFlag(bool flag)
	{
		m_infoAboutActionFlag.isActionFlag = false;
		m_infoAboutActionFlag.dashAttackFlag = false;
		m_infoAboutActionFlag.nextComboFlag = false;
		m_infoAboutActionFlag.isComboReceptionFlag = false;
	}
	/// <summary>
	/// アクションフラグの設定
	/// </summary>
	/// <param name="flag"></param>
	void SetIsActionFlag(bool flag)
	{
		m_infoAboutActionFlag.isActionFlag = flag;
	}
	/// <summary>
	/// アクションフラグの取得
	/// </summary>
	/// <returns></returns>
	const bool& GetIsActionFlag() const
	{
		return m_infoAboutActionFlag.isActionFlag;
	}
	/// <summary>
	/// 前進する攻撃フラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetDashAttackFlag(bool flag)
	{
		m_infoAboutActionFlag.dashAttackFlag = flag;
	}
	/// <summary>
	/// 前進する攻撃フラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& GetDashAttackFlag() const
	{
		return m_infoAboutActionFlag.dashAttackFlag;
	}
	/// <summary>
	/// 次のコンボ攻撃をするかのフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetNextComboFlagFlag(bool flag)
	{
		m_infoAboutActionFlag.nextComboFlag = flag;
	}
	/// <summary>
	/// 次のコンボ攻撃をするかのフラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& GetNextComboFlagFlag() const
	{
		return m_infoAboutActionFlag.nextComboFlag;
	}
	/// <summary>
	/// コンボ受付可能フラグの設定
	/// </summary>
	/// <param name="flag"></param>
	void SetIsComboReceptionFlagFlag(bool flag)
	{
		m_infoAboutActionFlag.isComboReceptionFlag = flag;
	}
	/// <summary>
	/// コンボ受付可能フラグの取得
	/// </summary>
	/// <returns></returns>
	const bool& GetIsComboReceptionFlagFlag() const
	{
		return m_infoAboutActionFlag.isComboReceptionFlag;
	}


	/// <summary>
	/// 前進する攻撃のスピードの取得
	/// </summary>
	/// <returns></returns>
	float GetNormalAttackSpeed()
	{
		return m_normalAttackSpeed;
	}

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
	/// コンボ攻撃のコンボの処理
	/// </summary>
	void ProcessComboAttack();


	/// <summary>
	/// スキルの使用時などの移動はしないが回転はしたいときに使う
	/// </summary>
	bool RotationOnly();


	void UpdateWeapons();


private:

	Player* m_player = nullptr;
	IBraveState* m_BraveState = nullptr;

	EnAnimationState m_enAnimationState = enAninationState_Idle;			//アニメーションステート
	EnAttackPattern m_attackPatternState = enAttackPattern_None;
	CharacterController m_charaCon;

	Animation	m_animation;				// アニメーション
	AnimationClip	m_animationClip[enAnimClip_Num];// アニメーションクリップ 

	ModelRender m_modelRender;

	InfoAboutActionFlag m_infoAboutActionFlag;

	int m_charaCenterBoonId = -1;

	//bool m_dashAttackFlag = false;		//攻撃時に前進するかのフラグ

	//bool m_isActionFlag = false;			//攻撃や防御などのアクションを起こしたかのフラグ
	//bool m_nextComboFlag = false;		//次のコンボ攻撃をするかのフラグ

	//bool m_isComboReceptionFlag = false;	//コンボ受付可能フラグ

	float m_mulYPos = 0.0f;

	const float m_normalAttackSpeed = 160.0f;

	//なくなる
	ModelRender Sword;
	int m_swordBoonId = -1;

	ModelRender Shield;
	int m_shieldBoonId = -1;

	int m_playAnimCount = 0;

};

