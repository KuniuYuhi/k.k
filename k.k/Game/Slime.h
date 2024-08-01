#pragma once
#include "MobEnemyBase.h"



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


	/// <summary>
	/// 更新処理
	/// </summary>
	void UpdateCharacter() override;



private:

	/// <summary>
	/// 基本的なコンポーネントをセッティング
	/// </summary>
	void SettingDefaultComponent() override;



private:



};

