#include "stdafx.h"
#include "CascadModel.h"

CascadModel::CascadModel()
{
}

CascadModel::~CascadModel()
{
}

bool CascadModel::Start()
{
	g_renderingEngine->SetAmbient(g_vec3One * 1.1f);

	//シャドウキャスター
	tea.Init("Assets/modelData/cascadeShadowModel/testModel.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0, 0, enModelUpAxisZ, true, true, true);

	////シャドウレシーバー
	bg.Init("Assets/modelData/cascadeShadowModel/bg/bg.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0, 0, enModelUpAxisZ, false, false, false);
	//カスケードシャドウ用カメラ
	g_camera3D->SetPosition(0, 100.0f, 350.0f);
	g_camera3D->SetTarget(0, 0.0f, 0);

	return true;
}

void CascadModel::Update()
{
	MoveCamera();

}

void CascadModel::Render(RenderContext& rc)
{
	tea.Draw(rc);
	bg.Draw(rc);
}

void CascadModel::MoveCamera()
{
	auto pos = g_camera3D->GetPosition();
	auto target = g_camera3D->GetTarget();
	pos.z -= g_pad[0]->GetLStickYF() * 2.0f;
	target.z -= g_pad[0]->GetLStickYF() * 2.0f;
	pos.y += g_pad[0]->GetRStickYF() * 2.0f;
	target.y += g_pad[0]->GetRStickYF() * 2.0f;

	pos.x += g_pad[0]->GetRStickXF() * 2.0f;
	target.x += g_pad[0]->GetRStickXF() * 2.0f;

	g_camera3D->SetPosition(pos);
	g_camera3D->SetTarget(target);
}
