#pragma once
#include "MobEnemyBase.h"

#include "BeholderEyeStateContext.h"
#include "BeholderEyeInfo.h"


class BeholderEyeStateContext;

using namespace BeholderEyeStates;
using namespace BeholderEyeAnimationClips;
using namespace KnockBackInfo;


/// <summary>
/// モブエネミー：目玉クラス
/// </summary>
class BeholderEye : public MobEnemyBase
{
public:
	~BeholderEye();

	bool Start() override;

	void Update() override;

	void Render(RenderContext& rc) override;

	void InitModel() override;

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);


	/// <summary>
	/// 共通ステートの処理
	/// </summary>
	void ProcessCommonTranstion();

	/// <summary>
	/// プレイヤーの方を向く
	/// </summary>
	void TurnToPlayer();

	/// <summary>
	/// 攻撃アクションを始めるときの処理
	/// </summary>
	void EntryAttackActionProcess();
	/// <summary>
	/// 攻撃アクションの更新処理
	/// </summary>
	void UpdateAttackActionProcess();
	/// <summary>
	/// 攻撃アクションを終わる時の処理
	/// </summary>
	void ExitAttackActionProcess();


	/// <summary>
	/// ヒットアクションに入った時の処理
	/// </summary>
	void EntryHitActionProcess();
	/// <summary>
	/// ヒットアクション中の処理
	/// </summary>
	void UpdateHitActionProcess();
	/// <summary>
	/// ヒットアクション終了時の処理
	/// </summary>
	void ExitHitActionProcess();

	/// <summary>
	/// 死亡時の処理
	/// </summary>
	void DieProcess();

	/// <summary>
	/// 勝利時の処理
	/// </summary>
	void WinProcess() override;

	/// <summary>
	/// 外部から削除
	/// </summary>
	void DieFlomOutside(bool isPlayEffect = false) override;

private:

	/// <summary>
	/// アニメーションクリップを読み込む
	/// </summary>
	void LoadAnimationClip();

	/// <summary>
	/// 全てのコンポーネントを初期化
	/// </summary>
	void InitComponents();


	/// <summary>
	/// 自身をオブジェクトプールに戻す
	/// </summary>
	void ReleaseThis() override;

	/// <summary>
	/// さらに追加するコンポーネントをセッティング
	/// </summary>
	void AddMoreComponent() override;

	/// <summary>
	/// コリジョンを作成する
	/// </summary>
	void CreateCollisionObject() override;

	/// <summary>
	/// ダメージを受けた時の処理
	/// </summary>
	void ProcessHit(DamageInfo damageInfo) override;



	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Attack();

	/// <summary>
	/// 魔法球を放つ
	/// </summary>
	void ShotNormalBall();

private:

	std::unique_ptr<BeholderEyeStateContext> m_beholderEyeContext = nullptr;	//ステートコンテキスト

	AnimationClip m_animationClip[enBeholderEyeAnimClip_num];

	CollisionObject* m_attackCollision = nullptr;		//当たり判定用コリジョン





	bool m_isCreateAttackCollision = false;

	int m_headBoonId = -1;

	bool m_isSettingComponents = false;


	int m_eyeBallCount = 0;

	float count = 0.0f;

};

