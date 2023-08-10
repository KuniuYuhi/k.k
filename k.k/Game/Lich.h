#pragma once
#include "AIActor.h"
#include "Level3DRender.h"

class ILichState;
class LichAction;
class DarkMeteorite;

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

	void SetStageLevelPosition();

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
	/// ダークウォールの生成
	/// </summary>
	void CreateDarkWall();

	/// <summary>
	/// ダークメテオの生成
	/// </summary>
	void CreateDarkMeteorite();

	bool IsRisingDarkMeteorite();

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
		return m_enAnimationState != enAnimationState_Damage &&
			m_enAnimationState != enAnimationState_Die;
	}

	/// <summary>
	/// 攻撃アニメーションが再生中か
	/// </summary>
	/// <returns></returns>
	bool IsAttackEntable() const
	{
		return m_enAnimationState != enAnimationState_Attack_1 &&
			m_enAnimationState != enAnimationState_Attack_2 &&
			m_enAnimationState != enAnimationState_Attack_DarkMeteorite_start &&
			m_enAnimationState != enAnimationState_Attack_DarkMeteorite_main &&
			m_enAnimationState != enAnimationState_Attack_DarkMeteorite_end &&
			m_enAnimationState != enAninationState_Summon;
	}

	/// <summary>
	/// 回転可能かどうか
	/// </summary>
	/// <returns></returns>
	bool isRotationEntable() const
	{
		return m_enAnimationState != enAnimationState_Attack_1 &&
			m_enAnimationState != enAninationState_Idle&&
			m_enAnimationState!=enAnimationState_Attack_DarkMeteorite_start&&
			m_enAnimationState != enAnimationState_Attack_DarkMeteorite_main;
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
		enAnimClip_Attack_DarkMeteorite_start,
		enAnimClip_Attack_DarkMeteorite_main,
		enAnimClip_Attack_DarkMeteorite_end,
		enAnimClip_Summon,
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
	/// <summary>
	/// 
	/// </summary>
	void OnProcessDarkMeteorite_StartStateTransition();
	/// <summary>
	/// 
	/// </summary>
	void OnProcessDarkMeteorite_MainStateTransition();
	/// <summary>
	/// 
	/// </summary>
	void OnProcessDarkMeteorite_EndStateTransition();
	/// <summary>
	/// 
	/// </summary>
	void OnProcessSummonStateTransition();
	/// <summary>
	/// 勝利遷移処理を実行
	/// </summary>
	void OnProcessVictoryStateTransition();

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
		enAnimationState_Attack_DarkMeteorite_start,
		enAnimationState_Attack_DarkMeteorite_main,
		enAnimationState_Attack_DarkMeteorite_end,
		enAninationState_Summon,
		enAnimationState_Damage,
		enAnimationState_Die,
		enAnimationState_Victory
	};

	/// <summary>
	/// 次のアニメーションステートを作成する。
	/// </summary>
	/// <param name="nextState"></param>
	void SetNextAnimationState(EnAnimationState nextState);

	//
	enum EnSpecialActionState
	{
		enSpecialActionState_Normal,
		enSpecialActionState_Warp,
		enSpecialActionState_CenterWarp,
		SpecialActionState
	};

	void SetSpecialActionState(EnSpecialActionState SpecialActionState)
	{
		m_enSpecialActionState = SpecialActionState;
	}

	const EnSpecialActionState& GetSpecialActionState() const
	{
		return m_enSpecialActionState;
	}

	/// <summary>
	/// ターゲットから一番遠いところに座標を移動させる
	/// </summary>
	void Warp(EnSpecialActionState SpecialActionState);


	/// <summary>
	/// 無敵状態フラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetInvincibleFlag(bool flag)
	{
		m_invincibleFlag = flag;
	}
	/// <summary>
	/// 無敵状態フラグを取得
	/// </summary>
	/// <returns></returns>
	bool GetInvincibleFlag()
	{
		return m_invincibleFlag;
	}

	/// <summary>
	/// ヒットカウントと蓄積ダメージを設定
	/// </summary>
	/// <param name="count">カウント</param>
	/// <param name="damage">受けたダメージ</param>
	void SetHitCountAndDamage(int count, int damage)
	{
		AddHitCount(count);
		AddAccumulationDamage(damage);
	}

	/// <summary>
	/// ヒットカウントを設定
	/// </summary>
	/// <param name="count"></param>
	void AddHitCount(int count)
	{
		m_hitCount += count;
	}

	/// <summary>
	/// ヒットカウントを取得
	/// </summary>
	/// <returns></returns>
	const int& GetHitCount() const
	{
		return m_hitCount;
	}

	/// <summary>
	/// 蓄積ダメージを設定
	/// </summary>
	/// <param name="count"></param>
	void AddAccumulationDamage(int damage)
	{
		m_accumulationDamage += damage;
	}

	/// <summary>
	/// 蓄積ダメージを取得
	/// </summary>
	/// <returns></returns>
	const int& GetAccumulationDamage() const
	{
		return m_accumulationDamage;
	}


	/// <summary>
	/// AIActor(モンスター)の情報をリストに追加
	/// </summary>
	/// <param name="monster"></param>
	void AddAIActorFromList(AIActor* monster)
	{
		m_monsters.emplace_back(monster);
	}
	/// <summary>
	/// AIActor(モンスター)の情報をリストから削除する
	/// </summary>
	/// <param name="monster"></param>
	void RemoveAIActorFromList(AIActor* monster)
	{
		std::vector<AIActor*>::iterator it = std::find(
			m_monsters.begin(), // アクターのリストの最初
			m_monsters.end(),   // アクターのリストの最後
			monster                     // 消したいアクター
		);
		if (it != m_monsters.end()) {
			m_monsters.erase(it);
		}
	}


private:

	bool RotationOnly();


	Level3DRender m_stageLevel;
	std::vector<Vector3> m_WarpPosition;
	
	LichAction* m_lichAction = nullptr;
	ILichState* m_state = nullptr;
	DarkMeteorite* m_darkMeteorite = nullptr;

	std::vector<AIActor*> m_monsters;

	Animation m_animation;	// アニメーション
	AnimationClip m_animationClip[enAnimClip_Num];	// アニメーションクリップ 

	EnAnimationState m_enAnimationState = enAninationState_Idle;	//アニメーションステート

	EnSpecialActionState m_enSpecialActionState = enSpecialActionState_Normal;

	ModelRender m_modelRender;

	CharacterController m_charaCon;

	FontRender m_hpFont;

	//被ダメージ時にカウントを増やす
	int m_hitCount = 0;
	//被ダメージ時に受けたダメージ分増やす
	int m_accumulationDamage = 0;

	const float m_distanceToPlayer = 300.0f;

	int m_darkWallBoonId = -1;					//ダークウォールで使うボーンID

	bool m_attackRangeFlag = false;				//攻撃範囲にいるかのフラグ

	bool m_CreateDarkWallFlag = false;			//ダークウォール生成フラグ

	const float m_attackIntervalTime = 4.0f;
	const float m_damageIntervalTime = 0.5f;
	
	InfoAboutAttack m_InfoAboutAttack;


	bool m_halfHpFlag = false;

	bool m_dieFlag = false;
	bool m_winFlag = false;

	bool m_invincibleFlag = false;


	const float m_RisingLimit = 200.0f;

	bool m_createDarkMeteoriteFlag = false;

};

