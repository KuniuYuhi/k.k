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
	g_renderingEngine->SetAmbient(g_vec3One * 1.3f);

	////シャドウキャスター
	//tea.Init("Assets/modelData/cascadeShadowModel/testModel.tkm",
	//	L"Assets/shader/ToonTextrue/lamp_glay.DDS",
	//	0, 0, enModelUpAxisZ, true, true, true);


	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/Player/NewHero/Idle.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(false);
	m_animationClip[enAnimClip_Sprint].Load("Assets/animData/character/Player/NewHero/Sprint.tka");
	m_animationClip[enAnimClip_Sprint].SetLoopFlag(false);
	m_animationClip[enAnimClip_DashForward].Load("Assets/animData/character/Player/NewHero/DashForward.tka");
	m_animationClip[enAnimClip_DashForward].SetLoopFlag(false);
	m_animationClip[enAnimClip_KnockBack].Load("Assets/animData/character/Player/NewHero/KnockBack.tka");
	m_animationClip[enAnimClip_KnockBack].SetLoopFlag(false);
	m_animationClip[enAnimClip_Hit].Load("Assets/animData/character/Player/NewHero/Hit.tka");
	m_animationClip[enAnimClip_Hit].SetLoopFlag(false);
	m_animationClip[enAnimClip_Defend].Load("Assets/animData/character/Player/NewHero/Defend.tka");
	m_animationClip[enAnimClip_Defend].SetLoopFlag(false);
	m_animationClip[enAnimClip_DefendHit].Load("Assets/animData/character/Player/NewHero/DefendHit.tka");
	m_animationClip[enAnimClip_DefendHit].SetLoopFlag(false);
	m_animationClip[enAnimClip_Die].Load("Assets/animData/character/Player/NewHero/Die.tka");
	m_animationClip[enAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enAnimClip_ChangeSwordShield].Load("Assets/animData/character/Player/NewHero/ChangeSwordShield.tka");
	m_animationClip[enAnimClip_ChangeSwordShield].SetLoopFlag(false);
	m_animationClip[enAnimClip_Win_Start].Load("Assets/animData/character/Player/NewHero/Win_start.tka");
	m_animationClip[enAnimClip_Win_Start].SetLoopFlag(false);
	m_animationClip[enAnimClip_Win_Main].Load("Assets/animData/character/Player/NewHero/Win_main.tka");
	m_animationClip[enAnimClip_Win_Main].SetLoopFlag(false);
	m_animationClip[enAnimClip_attack1].Load("Assets/animData/character/Player/NewHero/Attack_1.tka");
	m_animationClip[enAnimClip_attack1].SetLoopFlag(false);
	m_animationClip[enAnimClip_attack2].Load("Assets/animData/character/Player/NewHero/Attack_2.tka");
	m_animationClip[enAnimClip_attack2].SetLoopFlag(false);
	m_animationClip[enAnimClip_attack3].Load("Assets/animData/character/Player/NewHero/Attack_3.tka");
	m_animationClip[enAnimClip_attack3].SetLoopFlag(false);
	m_animationClip[enAnimClip_attack4].Load("Assets/animData/character/Player/NewHero/Attack_4.tka");
	m_animationClip[enAnimClip_attack4].SetLoopFlag(false);
	m_animationClip[enAnimClip_attack5].Load("Assets/animData/character/Player/NewHero/Attack_5.tka");
	m_animationClip[enAnimClip_attack5].SetLoopFlag(false);

	Player.Init("Assets/modelData/character/Player/NewHero/Hero_Smile.tkm", 
		L"Assets/shader/ToonTextrue/lamp_glay.DDS", 
		m_animationClip, 
		enAnimClip_Num, 
		enModelUpAxisZ);

	//Player.PlayAnimation(enAnimClip_Idle);
	Player.SetAnimationSpeed(0.5f);

	Sword.Init("Assets/modelData/character/Player/NewHero/OneHandSword.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ);
	m_swordBoonId = Player.FindBoneID(L"weaponShield_r");

	Shield.Init("Assets/modelData/character/Player/NewHero/Shield.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ);
	m_shieldBoonId = Player.FindBoneID(L"weaponShield_l");


	////シャドウレシーバー
	bg.Init("Assets/modelData/cascadeShadowModel/bg/bg.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0, 0, enModelUpAxisZ, false, false, false);

	g_camera3D->SetPosition(0, 100.0f, 350.0f);
	g_camera3D->SetTarget(0, 0.0f, 0);


	return true;
}

void CascadModel::Update()
{
	MoveCamera();

	
	
	if (Player.IsPlayingAnimation() == false) {

		m_playAnimCount++;
		if (m_playAnimCount == enAnimClip_Num)
		{
			m_playAnimCount = 0;
		}

		//次のアニメーション再生
		Player.PlayAnimation(m_playAnimCount);
	}




	Player.Update();

	UpDateWeapon();

}

void CascadModel::Render(RenderContext& rc)
{
	Player.Draw(rc);
	Sword.Draw(rc);
	Shield.Draw(rc);
	//tea.Draw(rc);
	bg.Draw(rc);
}

void CascadModel::UpDateWeapon()
{
	Matrix swordMt = Player.GetBone(m_swordBoonId)->GetWorldMatrix();
	Sword.SetWorldMatrix(swordMt);

	Matrix shieldMt = Player.GetBone(m_shieldBoonId)->GetWorldMatrix();
	Shield.SetWorldMatrix(shieldMt);

	Sword.Update();
	Shield.Update();
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
