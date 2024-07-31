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


private:



};

