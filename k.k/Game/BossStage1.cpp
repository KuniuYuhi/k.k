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

				//��
				m_wall.InitShadow("Assets/modelData/BackGround/wall2.tkm");
				m_wall.SetPosition(objData.position);
				m_wall.SetRotation(objData.rotation);
				m_wall.SetScale(objData.scale);
				m_wall.Update();
				m_wallPhysicsStaticObject.CreateFromModel(
					m_wall.GetModel(),
					m_wall.GetModel().GetWorldMatrix()
				);
				//�R���W����������forest�ɂ���
				m_wallPhysicsStaticObject.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);

				//�X
				m_forest.Init(
					"Assets/modelData/BackGround/Tree2.tkm",
					L"Assets/shader/ToonTextrue/lamp_glay.DDS",
					0,
					0,
					enModelUpAxisZ,
					true,
					true,
					false
				);
				return true;


			}
			return false;
		});

	return true;
}

void BossStage1::Update()
{

}

void BossStage1::Render(RenderContext& rc)
{
	backGround.Draw(rc);
	m_wall.Draw(rc);
	m_forest.Draw(rc);
}
