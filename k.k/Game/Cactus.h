#pragma once
#include "MobEnemyBase.h"


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


private:

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



private:

	bool m_isSettingComponents = false;

};

