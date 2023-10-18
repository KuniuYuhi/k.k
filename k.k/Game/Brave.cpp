#include "stdafx.h"
#include "Brave.h"
#include "BraveStateIdle.h"
#include "BraveStateSprint.h"

#include "Player.h"

namespace {
	const float ADD_SCALE = 1.2f;

	const float ROT_SPEED = 20.0f;
	const float ROT_ONLY_SPEED = 20.0f;

	const float SWORD_EFFECT_SIZE = 10.0f;
	const Vector3 DASH_EFFECT_SIZE = { 10.0f,20.0f,8.0f };

	int MAXHP = 200;
	int MAXMP = 100;
	int ATK = 40;
	float SPEED = 200.0f;
	const char* NAME = "Brave";

	const int SKILL_ATTACK_POWER = 30;
}

Brave::Brave()
{
}

Brave::~Brave()
{
}

bool Brave::Start()
{
	m_player = FindGO<Player>("player");

	//ステータスの初期化
	m_status.InitStatus(
		MAXHP,
		MAXMP,
		ATK,
		SPEED,
		NAME
	);

	InitModel();

	SetNextAnimationState(enAninationState_Idle);

	//キャラコンの設定
	m_charaCon.Init(12.0f, 33.0f, m_position);

	return true;
}

void Brave::Update()
{
	
	Move();

	PlayAnimation();
	ManageState();

	
	

	m_modelRender.Update();
	UpdateWeapons();
}

void Brave::Move()
{
	m_moveSpeed = calcVelocity(GetStatus());
	m_moveSpeed.y = 0.0f;

	m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);

	Rotation(ROT_SPEED, ROT_ONLY_SPEED);

	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
}

void Brave::ProcessAttack()
{

}

void Brave::Damage(int damage)
{
}

void Brave::SetNextAnimationState(EnAnimationState nextState)
{
	if (m_BraveState != nullptr) {
		// 古いステートを削除する。
		delete m_BraveState;
		m_BraveState = nullptr;
	}
	
	//アニメーションステートを次のステートに変える
	m_enAnimationState = nextState;

	switch (m_enAnimationState)
	{
	case Brave::enAninationState_Idle:
		m_BraveState = new BraveStateIdle(this);
		break;
	case Brave::enAninationState_Sprint:
		m_BraveState = new BraveStateSprint(this);
		break;
	case Brave::enAninationState_DashForward:
		break;
	case Brave::enAnimationState_KnockBack:
		break;
	case Brave::enAnimationState_Hit:
		break;
	case Brave::enAnimationState_Defend:
		break;
	case Brave::enAnimationState_DefendHit:
		break;
	case Brave::enAnimationState_Die:
		break;
	case Brave::enAnimationState_ChangeSwordShield:
		break;
	case Brave::enAnimationState_Win_Start:
		break;
	case Brave::enAnimationState_Win_Main:
		break;
	case Brave::enAnimationState_Attack_1:
		break;
	case Brave::enAnimationState_Attack_2:
		break;
	case Brave::enAnimationState_Attack_3:
		break;
	case Brave::enAnimationState_Attack_4:
		break;
	case Brave::enAnimationState_Attack_5:
		break;
	default:
		break;
	}

}

void Brave::PlayAnimation()
{
	m_BraveState->PlayAnimation();
	//if (m_modelRender.IsPlayingAnimation() == false) {

	//	m_playAnimCount++;
	//	if (m_playAnimCount == enAnimClip_Num)
	//	{
	//		m_playAnimCount = 0;
	//	}

	//	//次のアニメーション再生
	//	m_modelRender.PlayAnimation(m_playAnimCount);
	//}
}

void Brave::ManageState()
{
	m_BraveState->ManageState();
}

bool Brave::RotationOnly()
{
	return false;
}

void Brave::UpdateWeapons()
{
	Matrix swordMt = m_modelRender.GetBone(m_swordBoonId)->GetWorldMatrix();
	Sword.SetWorldMatrix(swordMt);

	Matrix shieldMt = m_modelRender.GetBone(m_shieldBoonId)->GetWorldMatrix();
	Shield.SetWorldMatrix(shieldMt);

	Sword.Update();
	Shield.Update();
}

void Brave::InitModel()
{
	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/Player/NewHero/Idle.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Sprint].Load("Assets/animData/character/Player/NewHero/Sprint.tka");
	m_animationClip[enAnimClip_Sprint].SetLoopFlag(true);
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

	m_modelRender.Init("Assets/modelData/character/Player/NewHero/Hero_Smile.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);
	m_modelRender.Update();

	Sword.Init("Assets/modelData/character/Player/NewHero/OneHandSword.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ);
	m_swordBoonId = m_modelRender.FindBoneID(L"weaponShield_r");

	Shield.Init("Assets/modelData/character/Player/NewHero/Shield.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ);
	m_shieldBoonId = m_modelRender.FindBoneID(L"weaponShield_l");
}

void Brave::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	Sword.Draw(rc);
	Shield.Draw(rc);
}

