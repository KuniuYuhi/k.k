#pragma once
#include "Level3DRender.h"

class Player;

class BossStage1:public IGameObject
{
public:
	BossStage1();
	~BossStage1();

	bool Start();
	void Update();

	void Render(RenderContext& rc);

private:
	Player* m_player = nullptr;

	Level3DRender levelbg;
	ModelRender backGround;
	ModelRender Castle;
	ModelRender m_forest;

	PhysicsStaticObject BGPhysicsStaticObject;
	PhysicsStaticObject m_forestPhysicsStaticObject;

	CollisionObject* m_tree_Collision;

};

