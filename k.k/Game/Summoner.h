#pragma once
#include "BossEnemyBase.h"


#include "SummonerInfo.h"
#include "CommonEnemyStatus.h"
#include "SummonerSkillStatus.h"

#include "SummonerSMContext.h"
#include "SummonerStateContext.h"



class SummonerSkillStatus;

class SummonerSMContext;
class SummonerStateContext;
class SummonerAIController;

class Brave;

using namespace SummonerStates;
using namespace SummonerAnimationClips;
using namespace SummonerSkillType;

/// <summary>
/// ボスエネミー：サモナークラス
/// </summary>
class Summoner :public BossEnemyBase
{
public:
	~Summoner();

	bool Start() override;

	void Update() override;

	void Render(RenderContext& rc) override;

	void InitModel() override;

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);


	Vector3 GetForwardYZero()
	{
		m_forward.y = 0.0f;
		m_forward.Normalize();
		return m_forward;
	}

	/// <summary>
	/// ショックウェーブのダメージ情報を設定する
	/// </summary>
	void SetShockWaveDamageInfo();

	/// <summary>
	/// ダメージ情報を設定する
	/// </summary>
	/// <param name="skillType">技のタイプ</param>
	void SettingDamageInfo(EnSkillSType skillType);

	/// <summary>
	/// 接近できる距離に到達した
	/// </summary>
	void ActiveArrivedApproachDistance()
	{
		m_isArrivedApproachDistance = true;
	}
	/// <summary>
	/// 接近できる距離に到達していない
	/// </summary>
	void DeactiveArrivedApproachDistance()
	{
		m_isArrivedApproachDistance = false;
	}
	/// <summary>
	/// 接近できる距離をリセットする
	/// </summary>
	void ResetApproachDistanceValue()
	{
		m_status.ResetCurrentApproachDistance();
	}

	/// <summary>
	/// 移動ストップフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetStopMoveFlag(bool flag)
	{
		m_isStopMove = flag;
	}
	/// <summary>
	/// 杖のボーンIDを取得
	/// </summary>
	/// <returns></returns>
	const int GetStaffBoonId()
	{
		return m_staffBoonId;
	}
	/// <summary>
	/// 右手のボーンIDを取得
	/// </summary>
	/// <returns></returns>
	const int GetRightHandBoonId()
	{
		return m_rightHandBoonId;
	}
	/// <summary>
	/// 左手のボーンIDを取得
	/// </summary>
	/// <returns></returns>
	const int GetLeftHandBoonId()
	{
		return m_leftHandBoonId;
	}


	/// <summary>
	/// ステート切り替え
	/// </summary>
	/// <param name="changeState"></param>
	void ChangeState(EnSummonerState changeState);
	/// <summary>
	/// ステートマシンを切り替えるかチェック
	/// </summary>
	bool CheckAndTransitionStateMachine();


	/// <summary>
	/// 共通ステートの処理
	/// </summary>
	void ProcessCommonTranstion();

	/// <summary>
	/// 自身からプレイヤーに向かう距離を取得
	/// </summary>
	float GetDistanceToPlayerPositionValue();


	/// <summary>
	/// コンボ攻撃の処理
	/// </summary>
	void UpdateComboAttackProcess();
	/// <summary>
	/// 攻撃中にターゲットのほうに回転する
	/// </summary>
	void UpdateAttackTurnToTargetProcess();
	/// <summary>
	/// ショックウェーブ攻撃の処理
	/// </summary>
	void UpdateShockWaveProcess();

	/// <summary>
	/// 死亡後の処理
	/// </summary>
	void AfterDieProcess();

	/// <summary>
	/// 外部から自身を削除
	/// </summary>
	void DieFlomOutside();


private:

	/// <summary>
	/// ダメージを受けた時の処理
	/// </summary>
	void ProcessHit(DamageInfo damageInfo) override;

	/// <summary>
	/// アニメーションクリップを読み込む
	/// </summary>
	void LoadAnimationClip();

	/// <summary>
	/// 当たり判定の処理をしない条件
	/// </summary>
	/// <returns></returns>
	bool IgnoreCollision() override;

	/// <summary>
	/// 自身をオブジェクトプールに戻す
	/// </summary>
	void ReleaseThis() override;

	/// <summary>
	/// さらに追加するコンポーネントをセッティング
	/// </summary>
	void AddMoreComponent() override;

	/// <summary>
	/// 全てのコンポーネントを初期化
	/// </summary>
	void InitComponents();


	/// <summary>
	/// 回転処理
	/// </summary>
	void Rotation();


private:

	SummonerSkillStatus m_skillStatus;
	

	std::unique_ptr<SummonerSMContext> m_StateMachineCotext = nullptr;	//ステートマシン管理

	std::unique_ptr<SummonerStateContext> m_stateContext = nullptr;		//ステート管理

	SummonerAIController* m_aiController = nullptr;


	AnimationClip m_animationClip[enSummonerAnimClip_Num];

	Vector3 m_currentRotDirection = g_vec3Zero;



	bool m_isSettingComponents = false;

	bool m_isAttackMove = false;



	int m_staffBoonId = -1;					//杖のボーンID
	int m_rightHandBoonId = -1;				//左手のボーンID
	int m_leftHandBoonId = -1;				//右手のボーンID

};

