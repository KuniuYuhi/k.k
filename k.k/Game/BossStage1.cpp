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

	//���x��
	levelbg.Init(
		"Assets/level3D/FieldLevel.tkl",
		[&](LevelObjectData& objData)
		{
			if (objData.EqualObjectName(L"grass") == true) {
				//�n��
				backGround.InitShadow("Assets/modelData/BackGround/ground.tkm");
				backGround.SetPosition(objData.position);
				backGround.SetRotation(objData.rotation);
				backGround.SetScale(objData.scale);
				backGround.Update();
				BGPhysicsStaticObject.CreateFromModel(
					backGround.GetModel(),
					backGround.GetModel().GetWorldMatrix()
				);
				//�R���W����������ground�ɂ���
				BGPhysicsStaticObject.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Ground);

				//�X
				m_forest.InitShadow("Assets/modelData/BackGround/forest.tkm");
				m_forest.SetPosition(objData.position);
				m_forest.SetRotation(objData.rotation);
				m_forest.SetScale(objData.scale);
				m_forest.Update();
				m_forestPhysicsStaticObject.CreateFromModel(
					m_forest.GetModel(),
					m_forest.GetModel().GetWorldMatrix()
				);
				//�R���W����������forest�ɂ���
				m_forestPhysicsStaticObject.GetbtCollisionObject()->setUserIndex(enCollisionAttr_forest);

				return true;


			}
			return false;
		});

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
