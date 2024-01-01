#pragma once
#include "BossBase.h"
#include "Level3DRender.h"

class IBossStateMachine;
class ISummonerState;
class DarkWall;

class Summoner:public BossBase
{
public:
	Summoner();
	~Summoner();

	bool Start();
	void Update();
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	void Render(RenderContext& rc);

	/////////////////////////////////////////////////////////////////////
	//構造体、列挙型の宣言
	/////////////////////////////////////////////////////////////////////
	/// <summary>
	/// 攻撃に関する情報
	/// </summary>
	struct InfoAboutAttack
	{
		const float m_Attack_1Distance = 1200.0f;	//遠距離攻撃
		const float m_Attack_2Distance = 200.0f;	//近距離攻撃
		const float m_Attack_DarkMeteoDistance = 1200.0f;	//ダークメテオ
	};
	/// <summary>
	/// アニメーションクリップの番号を表す列挙型。
	/// </summary>
	enum EnAnimationClip {
		enAnimClip_Idle,
		enAnimClip_Walk,
		enAnimClip_DarkBall,
		enAnimClip_DarkWall,
		enAnimClip_DarkSpear_Start,
		enAnimClip_DarkSpear_Main,
		enAnimClip_DarkSpear_End,
		enAnimClip_NormalAttack_1,
		enAnimClip_NormalAttack_2,
		enAnimClip_NormalAttack_3,
		enAnimClip_Attack_DarkMeteorite_start,
		enAnimClip_Attack_DarkMeteorite_main,
		enAnimClip_Summon,
		enAnimClip_CriticalHit,
		enAnimClip_Die,
		enAnimClip_Victory,
		enAnimClip_Angry,
		enAnimClip_Warp,
		enAnimClip_Command,
		enAnimClip_KnockBack,
		enAnimClip_Num,				// 7 :アニメーションクリップの数
	};
	/// <summary>
	/// アニメーションステート
	/// </summary>
	enum EnAnimationState {
		enAninationState_Idle,
		enAninationState_Walk,
		enAnimationState_DarkBall,
		enAnimationState_DarkWall,
		enAnimationState_DarkSpear_Start,
		enAnimationState_DarkSpear_Main,
		enAnimationState_DarkSpear_End,
		enAnimationState_NormalAttack_1,
		enAnimationState_NormalAttack_2,
		enAnimationState_NormalAttack_3,
		enAnimationState_Attack_DarkMeteorite_start,
		enAnimationState_Attack_DarkMeteorite_main,
		enAninationState_Summon,
		enAnimationState_CriticalHit,
		enAnimationState_Die,
		enAnimationState_Victory,
		enAnimationState_Angry,
		enAnimationState_Warp,
		enAnimationState_Command,
		enAnimationState_KnockBack,
		enAnimationState_Num,
	};
	/// <summary>
	/// ボスの状態ステート
	/// </summary>
	enum EnSpecialActionState
	{
		enSpecialActionState_Normal,			//通常。移動しない
		enSpecialActionState_AngryMode,			//怒りモード。この時だけ移動する
		SpecialActionState_Num
	};
	/// <summary>
	/// ワープする時の処理順番のステップ
	/// </summary>
	enum EnWarpStepState
	{
		enWarpStep_Up,
		enWarpStep_Warp,
		enWarpStep_Down,
		enWarpStep_End
	};

	/// <summary>
	/// ステートマシンステート
	/// </summary>
	enum EnStateMachineState
	{
		enStateMachineState_Vigilance,	//警戒ステートマシン
		enStateMachineState_Attack		//攻撃ステートマシン
	};

	/////////////////////////////////////////////////////////////////////
	//仮想関数、純粋仮想関数の宣言
	/////////////////////////////////////////////////////////////////////
	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Attack() override {}
	/// <summary>
	/// 処理を止めるか
	/// </summary>
	/// <returns></returns>
	bool IsStopProcessing() override;

	/// <summary>
	/// 特定のアニメーションが再生中か。当たり判定に使う
	/// </summary>
	/// <returns></returns>
	bool isAnimationEnable() const override
	{
		return m_enAnimationState != enAnimationState_KnockBack &&
			m_enAnimationState != enAnimationState_Attack_DarkMeteorite_start &&
			m_enAnimationState != enAnimationState_Attack_DarkMeteorite_main &&
			m_enAnimationState != enAnimationState_DarkSpear_Main &&
			m_enAnimationState != enAnimationState_DarkSpear_End;
	}
	/// <summary>
	/// 回転可能か
	/// </summary>
	/// <returns></returns>
	bool isRotationEnable() const override
	{
		return m_enAnimationState != enAnimationState_DarkBall &&
			m_enAnimationState != enAnimationState_DarkWall &&
			m_enAnimationState != enAnimationState_KnockBack &&
			m_enAnimationState != enAnimationState_NormalAttack_1 &&
			m_enAnimationState != enAnimationState_NormalAttack_2 &&
			m_enAnimationState != enAnimationState_NormalAttack_3 &&
			m_enAnimationState != enAnimationState_CriticalHit &&
			m_enAnimationState != enAnimationState_DarkSpear_Main &&
			m_enAnimationState != enAnimationState_DarkSpear_End;
	}
	/// <summary>
	/// 攻撃可能か
	/// </summary>
	/// <returns>trueで攻撃可能</returns>
	bool IsAttackEnable() const override
	{
		return m_enAnimationState != enAnimationState_DarkBall &&
			m_enAnimationState != enAnimationState_DarkWall &&
			m_enAnimationState != enAnimationState_NormalAttack_1 &&
			m_enAnimationState != enAnimationState_NormalAttack_2 &&
			m_enAnimationState != enAnimationState_NormalAttack_3 &&
			m_enAnimationState != enAnimationState_KnockBack &&
			m_enAnimationState != enAnimationState_Attack_DarkMeteorite_start &&
			m_enAnimationState != enAnimationState_Attack_DarkMeteorite_main &&
			m_enAnimationState != enAnimationState_DarkSpear_Start &&
			m_enAnimationState != enAnimationState_DarkSpear_Main &&
			m_enAnimationState != enAnimationState_DarkSpear_End &&
			m_enAnimationState != enAnimationState_CriticalHit;

	}

	/// <summary>
	/// 被ダメージ時処理
	/// </summary>
	void Damage(int attack) override;

	/// <summary>
	/// 通常攻撃に当たった時の処理
	/// </summary>
	void HitNormalAttack() override;

	/// <summary>
	/// スキルに当たった時の処理
	/// </summary>
	void HitSkillAttack() override;

	/// <summary>
	/// ウィザードのファイヤーボールに当たった時の処理。派生クラスで実装
	/// </summary>
	void HitFireBall() override {}
	/// <summary>
	/// ウィザードのフレイムピラーに当たった時の処理。派生クラスで実装
	/// </summary>
	void HitFlamePillar(bool damageFlag = false) override {}

	/// <summary>
	/// 回転のみを行う処理条件
	/// </summary>
	/// <returns></returns>
	bool RotationOnly() override
	{
		return false;
	}

	/// <summary>
	/// やられたときの処理
	/// </summary>
	/// <param name="seFlag">やられたときの効果音を再生するかのフラグ</param>
	void ProcessDead(bool seFlag = true) override;

	/////////////////////////////////////////////////////////////////////
	//その他の関数
	/////////////////////////////////////////////////////////////////////

	/// <summary>
	/// 次のステートマシンを生成する
	/// </summary>
	/// <param name="nextStateMachine"></param>
	void SetNextStateMachine(EnStateMachineState nextStateMachine);

	/// <summary>
	/// スタート時のステートマシンの作成
	/// </summary>
	/// <param name="nextStateMachine"></param>
	void SetStartStateMachine(EnStateMachineState nextStateMachine);

	/// <summary>
	/// 攻撃が終わった後の一通りの処理
	/// </summary>
	void ProcessEndAttackState();

	/// <summary>
	/// モブモンスターを削除
	/// </summary>
	void DeleteMobMonsters();

	/// <summary>
	/// 次のアニメーションステートを作成する。
	/// </summary>
	/// <param name="nextState"></param>
	void SetNextAnimationState(EnAnimationState nextState);

	/// <summary>
	/// 共通のステート遷移処理を実行
	/// </summary>
	void ProcessCommonStateTransition();


	/// <summary>
	/// 現在のステートを取得
	/// </summary>
	/// <returns></returns>
	ISummonerState* GetNowSummonerState()
	{
		return m_nowBossState;
	}

	/// <summary>
	/// 現在のステートマシンを取得
	/// </summary>
	/// <returns></returns>
	IBossStateMachine* GetNowStateMachine()
	{
		return m_stateMachine;
	}

	/// <summary>
	/// 状態の切り替え
	/// </summary>
	/// <param name="normalOrAngry">falseでノーマル、trueで怒りモード</param>
	void ChangeSpecialState(bool normalOrAngry)
	{
		if (normalOrAngry == true)m_enSpecialActionState = enSpecialActionState_AngryMode;
		else m_enSpecialActionState = enSpecialActionState_Normal;
	}

	/// <summary>
	/// 現在の状態を取得
	/// </summary>
	/// <returns></returns>
	const EnSpecialActionState& GetNowSpecialActionState() const
	{
		return m_enSpecialActionState;
	}

	/// <summary>
	/// モデルドローフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetIsDrawModelFlag(bool flag)
	{
		m_isDrawModelFlag = flag;
	}
	/// <summary>
	/// モデルドローフラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& GetIsDrawModelFlag() const
	{
		return m_isDrawModelFlag;
	}

	/// <summary>
	/// 被ダメージカウントを設定
	/// </summary>
	/// <param name="value"></param>
	void SetDamageCount(int value)
	{
		m_damageCount = value;
	}
	/// <summary>
	/// 被ダメージカウントを取得
	/// </summary>
	/// <returns></returns>
	const int& GetDamageCount() const
	{
		return m_damageCount;
	}

	/// <summary>
	/// ダークウォールのボーンIDの取得
	/// </summary>
	/// <returns></returns>
	const int& GetDarkWallBoonId() const
	{
		return m_darkWallBoonId;
	}

private:
	/// <summary>
	/// モデルの初期化
	/// </summary>
	void InitModel()  override;
	/// <summary>
	/// アニメーションを再生
	/// </summary>
	void PlayAnimation()  override;
	/// <summary>
	/// ステート管理
	/// </summary>
	void ManageState()  override;

	/// <summary>
	/// 移動処理
	/// </summary>
	void ProcessMove();
	/// <summary>
	/// 回転処理
	/// </summary>
	void ProcessRotation();

	/// <summary>
	/// 被ダメージ処理
	/// </summary>
	void ProcessHit(int hitDamage);

	/// <summary>
	/// スーパーアーマーの回復。ブレイクしていないと処理しない
	/// </summary>
	void RecoverySuperArmor();
	

	/// <summary>
	/// ダークボール生成
	/// </summary>
	void CreateDarkBall();
	/// <summary>
	/// ダークウォール生成
	/// </summary>
	void CreateDarkWall();

	/// <summary>
	/// ノックバック処理
	/// </summary>
	void ProcessKnockBack();

	/// <summary>
	/// 通常攻撃の当たり判定生成
	/// </summary>
	void CreateNormalAttackCollision();

	/// <summary>
	/// 通常攻撃の当たり判定生成
	/// </summary>
	void NormalComboFinnish();

private:

	EnAnimationState				m_enAnimationState = enAninationState_Idle;				//アニメーションステート
	EnSpecialActionState			m_enSpecialActionState = enSpecialActionState_Normal;	//特別な状態ステート(通常、怒りモード)
	EnWarpStepState					m_enWarpStep = enWarpStep_Up;

	EnStateMachineState	m_stateMachineState = enStateMachineState_Vigilance;

	Level3DRender					m_stageLevel;
	AnimationClip					m_animationClip[enAnimClip_Num];						// アニメーションクリップ 

	DarkWall* m_darkWall = nullptr;

	CharacterController				m_charaCon;												//キャラクターコントローラー

	std::unique_ptr<IBossStateMachine> m_SummonerstateMachine = nullptr;


	IBossStateMachine*				m_stateMachine = nullptr;

	ISummonerState*					m_nowBossState = nullptr;

	bool							m_isDrawModelFlag = true;

	int								m_damageCount = 0;				//被ダメージした時に加算するカウント

	int m_darkWallBoonId = -1;

	bool m_oldBreakSuperArmorFlag = false;	//前フレームのスーパーアーマーブレイクフラグ

};

