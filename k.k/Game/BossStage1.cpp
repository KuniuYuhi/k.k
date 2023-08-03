#include "stdafx.h"
#include "BossStage1.h"
#include "Player.h"

BossStage1::BossStage1()
{
}

BossStage1::~BossStage1()
{
	
}

bool BossStage1::Start()
{
	m_player = FindGO<Player>("player");

	//レベル
	levelbg.Init(
		"Assets/level3D/FieldLevel.tkl",
		[&](LevelObjectData& objData)
		{
			if (objData.EqualObjectName(L"grass") == true) {
				//地面
				backGround.InitShadow("Assets/modelData/BackGround/ground.tkm");
				backGround.SetPosition(objData.position);
				backGround.SetRotation(objData.rotation);
				backGround.SetScale(objData.scale);
				backGround.Update();
				BGPhysicsStaticObject.CreateFromModel(
					backGround.GetModel(),
					backGround.GetModel().GetWorldMatrix()
				);
				//コリジョン属性をgroundにする
				BGPhysicsStaticObject.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Ground);

				//森
				m_forest.InitShadow("Assets/modelData/BackGround/forest.tkm");
				m_forest.SetPosition(objData.position);
				m_forest.SetRotation(objData.rotation);
				m_forest.SetScale(objData.scale);
				m_forest.Update();
				m_forestPhysicsStaticObject.CreateFromModel(
					m_forest.GetModel(),
					m_forest.GetModel().GetWorldMatrix()
				);
				//コリジョン属性をforestにする
				BGPhysicsStaticObject.GetbtCollisionObject()->setUserIndex(enCollisionAttr_forest);

				return true;


			}
			return false;
		});

	backGround.Update();


	


	/*Tree.Init("Assets/modelData/BGobject/tree0.tkm", nullptr, 0, enModelUpAxisZ, true, true, false);
	Tree.SetPosition(100.0f, 0.0f, 0.0f);
	Tree.Update();

	m_tree_Collision = NewGO<CollisionObject>(0);
	m_tree_Collision->CreateCapsule(
		Vector3(100.0f, 0.0f, 0.0f),
		Quaternion::Identity,
		float(10.0f),
		float(50.0f)
	);
	m_tree_Collision->SetIsEnable(true);
	m_tree_Collision->SetIsEnableAutoDelete(false);
	m_tree_Collision->SetName("tree");
	m_tree_Collision->SetPosition(Vector3(100.0f, 0.0f, 0.0f));
	m_tree_Collision->Update();*/

	

	return true;
}

void BossStage1::Update()
{
	if (m_tree_Collision->IsHit(m_player->GetCharacterController())==true)
	{
		DeleteGO(m_tree_Collision);
	}

}

void BossStage1::Render(RenderContext& rc)
{
	backGround.Draw(rc);
	m_forest.Draw(rc);
}
