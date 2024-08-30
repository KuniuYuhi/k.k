#pragma once
#include "MobEnemyBase.h"
#include "CactusStateContext.h"
#include "CactusInfo.h"


class CactusStateContext;

using namespace CactusStates;
using namespace CactusAnimationClips;
using namespace KnockBackInfo;

/// <summary>
/// モブエネミー：カクタスクラス
/// </summary>
class Cactus : public MobEnemyBase
{
public:
	~Cactus();

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



private:

	std::unique_ptr<CactusStateContext> m_cactusContext = nullptr;	//ステートコンテキスト

	AnimationClip m_animationClip[enCactusAnimClip_num];

	CollisionObject* m_attackCollision = nullptr;		//当たり判定用コリジョン





	bool m_isCreateAttackCollision = false;

	int m_headBoonId = -1;

	bool m_isSettingComponents = false;

	




	float count = 0.0f;

};

