#pragma once
#include "MobEnemyBase.h"



/// <summary>
/// ���u�G�l�~�[�F�X���C���N���X
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

