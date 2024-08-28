#pragma once
#include "MobEnemyBase.h"
#include "SlimeStateContext.h"
#include "SlimeInfo.h"

//#include "KnockBackInfo.h"


class SlimeStateContext;

using namespace SlimeStates;
using namespace SlimeAnimationClips;
using namespace KnockBackInfo;


/// <summary>
/// モブエネミー：スライムクラス
/// </summary>
class Slime : public MobEnemyBase
{
public:
	~Slime();

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
	/// ターゲットのほうに振り向く
	/// </summary>
	void TrunToTarget();




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
	/// 
	/// </summary>
	void EntryHitActionProcess();
	/// <summary>
	/// 
	/// </summary>
	void UpdateHitActionProcess();

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
	/// 攻撃可能か？
	/// </summary>
	/// <returns>可能ならtrue</returns>
	bool IsAttackable();

private:

	std::unique_ptr<SlimeStateContext> m_slimeContext = nullptr;	//ステートコンテキスト

	AnimationClip m_animationClip[enSlimeAnimClip_num];

	CollisionObject* m_attackCollision = nullptr;		//当たり判定用コリジョン

	



	bool m_isCreateAttackCollision = false;

	int m_headBoonId = -1;

	bool m_isSettingComponents = false;

	float m_attackIntarvalTimer = 0.0f;




	float count = 0.0f;

	
	
	

	

};

