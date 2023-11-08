#pragma once
#include "Actor.h"
#include "IWeapon.h"

class Player;
class IBraveState;
class IWeapon;

class SwordShield;

/// <summary>
/// 勇者クラス
/// </summary>
class Brave:public Actor
{
public:

	enum EnWepons
	{
		enWeapon_Main,
		enWeapon_Sub,
		enWeapon_num
	};

	/// <summary>
	/// アクションする時に使うフラグをまとめている
	/// </summary>
	struct InfoAboutActionFlag
	{
		bool isActionFlag = false;		//アクションフラグ
		bool moveforwardFlag = false;	//前進攻撃フラグ
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
		IWeapon* weapon = nullptr;	//武器オブジェクトの変数
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
			m_enAnimationState != enAnimationState_ChangeSwordShield;
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

public:
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
	/// 行動不可能かどうかの判定可能
	/// </summary>
	/// <returns>trueで不可能、falseで可能</returns>
	const bool& IsInaction() const;

	/// <summary>
	/// 攻撃時に前進する時に使う
	/// </summary>
	/// <param name="Speed">前進する速さ</param>
	void MoveForward(float Speed);

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
	/// <param name="subOrMain">サブかメインのステート</param>
	/// <returns></returns>
	IWeapon* GetWeapon(EnWepons subOrMain) const
	{
		return m_useWeapon[subOrMain].weapon;
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
		return m_useWeapon[enWeapon_Main].weapon->GetEnDefendTipe();
	}

	/// <summary>
	/// 攻撃の中心座標を設定
	/// </summary>
	/// <param name="attackPosition"></param>
	void SetAttackPosition(Vector3 attackPosition);
	/// <summary>
	/// ノックバックする時の設定をまとめたもの
	/// </summary>
	/// <param name="attackPosition">攻撃の中心座標</param>
	/// <param name="power">ノックバックパワー</param>
	/// <param name="flag">ノックバックするかのフラグ</param>
	void SetKnockBackInfo(
		Vector3 attackPosition, float power, bool flag)
	{
		//攻撃の中心座標を設定
		SetAttackPosition(attackPosition);
		//ノックバックパワーを設定
		SetKnockBackPower(power);
		//ノックバック攻撃フラグをセット
		SetKnockBackAttackFalg(flag);
	}

private:
	/// <summary>
	/// コンボ攻撃のコンボの処理
	/// </summary>
	void ProcessComboAttack();
	/// <summary>
	/// メイン武器とサブ武器を入れ替える
	/// </summary>
	void ReverseWeapon();
	/// <summary>
	/// UseWeapon構造体の中身を入れ替える
	/// </summary>
	void ChangeUseWeapon();

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


		//回転のみ可能なアニメーションではないなら
		return 
			GetMoveForwardFlag() != true;
	}

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
	//片手剣の最初のアニメーションクリップの番号
	const int OneHandSwordAnimationStartIndexNo = AnimationClipGroup_OneHandedSword;
	//両手剣の最初のアニメーションクリップの番号
	const int TwoHandSwordAnimationStartIndexNo = enAnimClip_Num * AnimationClipGroup_TwoHandedSword;
	//弓の最初のアニメーションクリップの番号
	const int BowAnimationStartIndexNo = enAnimClip_Num * AnimationClipGroup_Bow;

	//現在の武器のアニメーションの最初の番号
	int m_currentAnimationStartIndexNo = OneHandSwordAnimationStartIndexNo;

	UseWeapon					m_useWeapon[enWeapon_num];	//使う武器

	IWeapon*					m_weapon[enWeapon_num];	//武器の数
	IWeapon*					m_mainWeapon = nullptr;	//メイン武器
	IWeapon*					m_subWeapon = nullptr;		//サブ武器

	Player*						m_player = nullptr;
	IBraveState*				m_BraveState = nullptr;
	SwordShield*				m_swordShield = nullptr;

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

	

};

