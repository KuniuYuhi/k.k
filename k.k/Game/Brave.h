#pragma once
#include "Actor.h"
#include "WeaponBase.h"
#include "MyWeapon.h"

class Player;
class IBraveState;
class WeaponBase;

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
		bool moveforwardFlag = false;	//前進攻撃フラグ
		bool moveBackFlag = false;		//後退フラグ。ノックバック時も使う
		bool nextComboFlag = false;		//次のコンボ攻撃をするかのフラグ
		bool isComboReceptionFlag = false;	//コンボ受付可能フラグ
		bool isCollisionPossibleFlag = false;	//当たり判定が有効かの判定フラグ。
		bool attackHitFlag = false;		//攻撃が相手にヒットしたかのフラグ
	};

	/// <summary>
	/// 使用する武器の構造体
	/// </summary>
	struct UseWeapon
	{
		WeaponBase* weapon = nullptr;	//武器オブジェクト
		int weaponAnimationStartIndexNo = 0;	//武器のアニメーションクリップの最初の番号
	};

	Brave();
	~Brave();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	//////////////////////////////////////////////////////////////
	// 仮想関数、純粋仮想関数
	//////////////////////////////////////////////////////////////

	/// <summary>
	/// ダメージを受けた時の処理
	/// </summary>
	/// <param name="damage">受けるダメージ</param>
	void Damage(int damage) override;
	/// <summary>
	/// 特定のアニメーションが再生中か
	/// </summary>
	/// <returns></returns>
	bool isAnimationEntable() const override
	{
		return m_enAnimationState != enAnimationState_Attack_1 &&
			m_enAnimationState != enAnimationState_Attack_2 &&
			m_enAnimationState != enAnimationState_Attack_3 &&
			m_enAnimationState != enAnimationState_Skill_start &&
			m_enAnimationState != enAnimationState_Skill_Main &&
			m_enAnimationState != enAnimationState_Defend &&
			m_enAnimationState != enAnimationState_DefendHit &&
			m_enAnimationState != enAnimationState_Hit &&
			m_enAnimationState != enAnimationState_Die &&
			m_enAnimationState != enAnimationState_ChangeWeapon;
	}
	/// <summary>
	/// 当たり判定可能なアニメーションか
	/// </summary>
	/// <returns></returns>
	bool isCollisionEntable() const override;
	/// <summary>
	/// 回転可能なアニメーションが再生中か
	/// </summary>
	/// <returns></returns>
	bool isRotationEntable() const override;

	/// <summary>
	/// 勝利時の処理
	/// </summary>
	void ProcessWin() override;

private:
	/// <summary>
	/// モデルの初期化
	/// </summary>
	void InitModel() override;
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	void PlayAnimation() override;
	/// <summary>
	/// ステート管理
	/// </summary>
	void ManageState() override;
	/// <summary>
	/// スキルの使用時などの移動はしないが回転はしたいときに使う
	/// </summary>
	bool RotationOnly() override;

	//////////////////////////////////////////////////////////////
	// その他の関数
	//////////////////////////////////////////////////////////////

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
	/// 防御、回避処理
	/// </summary>
	void ProcessDefend();


public:
	
	
	/// <summary>
	/// 行動不可能かどうかの判定可能
	/// </summary>
	/// <returns>trueで不可能、falseで可能</returns>
	const bool& IsInaction() const;

	/// <summary>
	/// 前進する
	/// </summary>
	/// <param name="Speed">前進する速さ。武器のスピードによって変える</param>
	void MoveForward(float Speed);

	/// <summary>
	/// 後退する
	/// </summary>
	/// <param name="backSpeed"></param>
	void MoveBack(float backSpeed);

	/// <summary>
	///	武器の切り替え処理
	/// </summary>
	void ChangeWeapon();
	/// <summary>
	/// 武器が剣盾の時のスキルの処理
	/// </summary>
	/// <param name="UpOrDownFlag">trueでUp、falseDown</param>
	void ProcessSwordShieldSkill(bool UpOrDownFlag);

	// アニメーションクリップの番号を表す列挙型。
	enum EnAnimationClip {
		enAnimClip_Idle,
		enAnimClip_Sprint,
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
		enAnimationState_ChangeWeapon,
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
	/// ステート共通の武器切り替え処理
	/// </summary>
	void ProcessCommonWeaponChangeStateTransition();
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
	/// 防御、回避のステートの状態遷移処理
	/// </summary>
	void ProcessDefendStateTransition();
	
	/// <summary>
	/// アクションフラグ構造体の全てのフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetAllInfoAboutActionFlag(bool flag)
	{
		m_infoAboutActionFlag.isActionFlag = flag;
		m_infoAboutActionFlag.moveforwardFlag = flag;
		m_infoAboutActionFlag.moveBackFlag = flag;
		m_infoAboutActionFlag.nextComboFlag = flag;
		m_infoAboutActionFlag.isComboReceptionFlag = flag;
		m_infoAboutActionFlag.isCollisionPossibleFlag = flag;
		m_infoAboutActionFlag.attackHitFlag = flag;
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
	/// 前進するフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetMoveforwardFlag(bool flag)
	{
		m_infoAboutActionFlag.moveforwardFlag = flag;
	}
	/// <summary>
	/// 前進する攻撃フラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& GetMoveForwardFlag() const
	{
		return m_infoAboutActionFlag.moveforwardFlag;
	}
	/// <summary>
	/// 後退するフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetMoveBackFlag(bool flag)
	{
		m_infoAboutActionFlag.moveBackFlag = flag;
	}
	/// <summary>
	/// 後退する攻撃フラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& GetMoveBackFlag() const
	{
		return m_infoAboutActionFlag.moveBackFlag;
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
	/// 当たり判定有効化フラグの設定
	/// </summary>
	/// <param name="flag"></param>
	void SetIsCollisionPossibleFlag(bool flag)
	{
		m_infoAboutActionFlag.isCollisionPossibleFlag = flag;
	}
	/// <summary>
	/// 当たり判定有効化フラグの取得
	/// </summary>
	/// <returns></returns>
	const bool& GetIsCollisionPossibleFlag() const
	{
		return m_infoAboutActionFlag.isCollisionPossibleFlag;
	}
	/// <summary>
	/// 攻撃がヒットしたかのフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetAttackHitFlag(bool flag)
	{
		m_infoAboutActionFlag.attackHitFlag = flag;
	}
	/// <summary>
	/// 攻撃がヒットしたかのフラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& GetAttackHitFlag() const
	{
		return m_infoAboutActionFlag.attackHitFlag;
	}

	/// <summary>
	/// 前進する攻撃のスピードの取得
	/// </summary>
	/// <returns></returns>
	float GetNormalAttackSpeed()
	{
		return m_normalAttackSpeed;
	}
	/// <summary>
	/// 回避するときのスピードの取得
	/// </summary>
	/// <returns></returns>
	const float& GetAvoidSpeed() const
	{
		return m_avoidSpeed;
	}
	
	/// <summary>
	/// 武器の取得
	/// </summary>
	/// <param name="enWeapon">サブかメインのステート</param>
	/// <returns></returns>
	WeaponBase* GetWeapon(EnWeapons enWeapon) const
	{
		if (enWeapon == enWeapon_Main)
		{
			return m_mainUseWeapon.weapon;
		}
		else if (enWeapon == enWeapon_Sub)
		{
			return m_subUseWeapon.weapon;
		}
		else
		{
			return m_subUseWeapon2.weapon;
		}
	}
	/// <summary>
	/// メイン武器のアニメーションクリップの最初の番号を取得
	/// </summary>
	/// <returns></returns>
	const int& GetCurrentMainWeaponAnimationStartIndexNo() const
	{
		return m_currentAnimationStartIndexNo;
	}
	/// <summary>
	/// メイン武器の防御タイプを取得
	/// </summary>
	/// <returns></returns>
	const int& GetMainWeaponDefendTipe() const
	{
		return m_mainUseWeapon.weapon->GetEnDefendTipe();
	}

	/// <summary>
	/// 武器切り替え完了フラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetChangeWeaponCompleteFlag(bool flag)
	{
		m_changeWeaponCompleteFlag = flag;
	}
	/// <summary>
	/// 武器切り替え完了フラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& GetChangeWeaponCompleteFlag() const
	{
		return m_changeWeaponCompleteFlag;
	}

	/// <summary>
	/// 切り替え対象の武器を取得
	/// </summary>
	/// <returns></returns>
	const EnWeapons& GetChangeTargetUseWeapon() const
	{
		return m_changeTargetUseWeapon;
	}

	/// <summary>
	/// キャラを強制的にアイドル状態にする
	/// </summary>
	void ForciblyIdleAnim()
	{
		m_moveSpeed = g_vec3Zero;
	}

private:
	/// <summary>
	/// コンボ攻撃のコンボの処理
	/// </summary>
	void ProcessComboAttack(); 
	
	/// <summary>
	/// メイン武器とサブ武器を入れ替える
	/// </summary>
	/// <param name="ChangeUseWeapon">入れ替えたいサブ武器</param>
	void ReverseWeapon(EnWeapons changeTargetWeapon);
	/// <summary>
	/// UseWeapon構造体の中身を入れ替える
	/// </summary>
	void ChangeUseWeapon(UseWeapon& ChangeUseSubWeapon);
	/// <summary>
	/// 防御中にヒットしたか
	/// </summary>
	/// <returns>ヒットならtrue、ヒットしていないならfalse</returns>
	bool IsDefendHit();

	/// <summary>
	/// 前方向が設定できる条件か
	/// </summary>
	/// <returns>trueで可能、falseで不可能</returns>
	const bool& IsSetForwardCondition() const
	{
		if (isAnimationEntable() == true)
		{
			return true;
		}
		
		if (GetMoveForwardFlag() != true)
		{
			return true;
		}
		//回転のみ可能なアニメーションではないなら
		return false;
	}

	/// <summary>
	/// 装備する武器の設定。スタート時に設定
	/// </summary>
	void SettingWeapons();
	/// <summary>
	/// 
	/// </summary>
	/// <param name="animationStartInbexNo"></param>
	/// <param name="changeTargetWeapon"></param>
	void SettingChangeWeapon(int animationStartInbexNo, EnWeapons changeTargetWeapon);;
	
	/// <summary>
	/// 武器のアニメーションの最初の番号を設定
	/// </summary>
	/// <param name="useWeapon">番号を設定したい武器</param>
	/// <param name="weaponType">武器の種類</param>
	void SetCurrentAnimationStartIndexNo(UseWeapon& useWeapon, EnWeapons mainOrSub);

	/// <summary>
	/// 武器の種類に対応したアニメーションクリップのロード
	/// </summary>
	void RoadWeaponTypeAnimetionClip(EnWeaponType weaponType, int mainWeaponAnimationStartIndexNo);
	void RoadOneHandSwordAnimationClip(int mainWeaponAnimationStartIndexNo);
	void RoadTwoHandSwordAnimationClip(int mainWeaponAnimationStartIndexNo);
	void RoadBowAnimationClip(int mainWeaponAnimationStartIndexNo);
private:
	/// <summary>
	/// 武器それぞれのアニメーションクリップグループ
	/// </summary>
	enum AnimationClipGroup {
		AnimationClipGroup_OneHandedSword,	// 片手剣を装備中のアニメーションクリップグループ
		AnimationClipGroup_TwoHandedSword,	// 両手剣を装備中のアニメーションクリップグループ
		AnimationClipGroup_Bow,				// 弓を装備中のアニメーションクリップグループ
		AnimationClipGroup_Num,
	};
	
	const int m_mainWeaponAnimationStartIndexNo = 0;						//メイン武器のアニメーションクリップの最初の番号
	const int m_subWeaponAnimationStartIndexNo = enAnimClip_Num * 1;		//サブ武器のアニメーションクリップの最初の番号
	const int m_subWeapon2AnimationStartIndexNo = enAnimClip_Num * 2;

	//現在の武器のアニメーションの最初の番号
	int m_currentAnimationStartIndexNo = m_mainWeaponAnimationStartIndexNo;

	EnWeaponType				m_mainWeaponType = enWeaponType_Num;
	EnWeaponType				m_subWeaponType = enWeaponType_Num;
	EnWeaponType				m_subWeapon2Type = enWeaponType_Num;
	EnWeapons				    m_changeTargetUseWeapon = enWeapon_num;		//切り替え対象の武器

	UseWeapon					m_mainUseWeapon;		//メイン武器
	UseWeapon					m_subUseWeapon;			//サブ武器
	UseWeapon					m_subUseWeapon2;		//サブ２


	Player*						m_player = nullptr;
	IBraveState*				m_BraveState = nullptr;

	EnAnimationState			m_enAnimationState = enAninationState_Idle;			//アニメーションステート
	EnAttackPattern				m_attackPatternState = enAttackPattern_None;
	CharacterController			m_charaCon;
	
	AnimationClip				m_animationClip[enAnimClip_Num * AnimationClipGroup_Num];// アニメーションクリップ 
	
	ModelRender					m_modelRender;

	InfoAboutActionFlag			m_infoAboutActionFlag;

	int							m_charaCenterBoonId = -1;

	float						m_mulYPos = 0.0f;

	const float					m_normalAttackSpeed = 160.0f;

	const float					m_avoidSpeed = 230.0f;

	bool						m_changeWeaponCompleteFlag = false;			//武器切り替え完了フラグ

};

