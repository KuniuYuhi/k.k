#pragma once
#include "Level3DRender.h"

class BossStage1:public IGameObject
{
public:
	BossStage1();
	~BossStage1();

	bool Start();
	void Update();

	void Render(RenderContext& rc);

private:
	Level3DRender levelbg;
	ModelRender backGround;
	ModelRender Castle;
	ModelRender Tree;

	PhysicsStaticObject BGPhysicsStaticObject;

};

