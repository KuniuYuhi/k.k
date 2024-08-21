#pragma once
#include "MobEnemyBase.h"
#include "SlimeStateContext.h"
#include "SlimeInfo.h"

class SlimeStateContext;


using namespace SlimeAnimationClips;

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


private:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

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

private:

	std::unique_ptr<SlimeStateContext> m_slimeContext = nullptr;	//ステートコンテキスト

	AnimationClip m_animationClip[enSlimeAnimClip_num];


	bool m_isSettingComponents = false;


};

