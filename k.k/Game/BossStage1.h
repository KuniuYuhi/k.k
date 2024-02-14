#pragma once
#include "Level3DRender.h"

class Player;

class BossStage1:public IGameObject
{
public:
	BossStage1();
	~BossStage1();

	bool Start() override;
	void Update() override;

	void Render(RenderContext& rc) override;

private:
	Player* m_player = nullptr;

	Level3DRender levelbg;
	ModelRender backGround;
	ModelRender m_wall;
	ModelRender m_forest;

	PhysicsStaticObject BGPhysicsStaticObject;
	PhysicsStaticObject m_wallPhysicsStaticObject;

	CollisionObject* m_tree_Collision;

};

