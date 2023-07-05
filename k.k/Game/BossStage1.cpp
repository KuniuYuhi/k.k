#include "stdafx.h"
#include "BossStage1.h"

BossStage1::BossStage1()
{
}

BossStage1::~BossStage1()
{
}

bool BossStage1::Start()
{
	//ƒŒƒxƒ‹
	levelbg.Init(
		"Assets/level3D/FieldLevel.tkl",
		[&](LevelObjectData& objData)
		{
			if (objData.EqualObjectName(L"grass") == true) {
				backGround.InitShadow("Assets/modelData/BackGround/grass01.tkm");
				backGround.SetPosition(objData.position);
				backGround.SetRotation(objData.rotation);
				backGround.SetScale(objData.scale);
				backGround.Update();
				BGPhysicsStaticObject.CreateFromModel(
					backGround.GetModel(),
					backGround.GetModel().GetWorldMatrix()
				);
				return true;
			}
			return false;
		});

	backGround.Update();


	Tree.Init("Assets/modelData/BGobject/tree0.tkm", nullptr, 0, enModelUpAxisZ, true, true, false);
	Tree.SetPosition(100.0f, 0.0f, 0.0f);
	Tree.Update();

	return true;
}

void BossStage1::Update()
{
}

void BossStage1::Render(RenderContext& rc)
{
	backGround.Draw(rc);
	Tree.Draw(rc);
}
