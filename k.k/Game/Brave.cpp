#include "stdafx.h"
#include "Brave.h"
#include "BraveStateIdle.h"
#include "BraveStateSprint.h"
#include "BraveStateAttack_1.h"
#include "BraveStateAttack_2.h"
#include "BraveStateAttack_3.h"
#include "BraveStateSkill_Start.h"
#include "BraveStateSkill_Main.h"
#include "BraveStateDefend.h"
#include "BraveStateDefendHit.h"
#include "BraveStateHit.h"
#include "BraveStateDie.h"
#include "BraveStateChangeSwordShield.h"
#include "BraveStateWin_Start.h"
#include "BraveStateWin_Main.h"

#include "Player.h"

//todo
//攻撃力は武器

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

	//m_modelRender.SetAnimationSpeed(0.4f);

	//キャラコンの設定
	m_charaCon.Init(12.0f, 33.0f, m_position);

	return true;
}

void Brave::Update()
{
	//行動不可能な状態でないなら
	if (IsInaction() != true)
	{
		//MPの回復
		RecoveryMP();

		//移動処理
		Move();
		//回転処理
		ProcessRotation();
		//攻撃処理
		ProcessAttack();
		//防御処理
		ProcessDefend();
	}
	

	ManageState();
	PlayAnimation();
	

	UpdateWeapons();
	

	m_modelRender.Update();
	
}

void Brave::Move()
{
	if (isAnimationEntable() != true)
	{
		return;
	}
	//todo 移動しない時せ-ぶするやつの計算
	m_moveSpeed = calcVelocity(GetStatus());
	m_moveSpeed.y = 0.0f;

	m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);

	m_modelRender.SetPosition(m_position);
}

void Brave::ProcessRotation()
{
	Rotation(ROT_SPEED, ROT_ONLY_SPEED);
	m_modelRender.SetRotation(m_rotation);
}

void Brave::ProcessAttack()
{
	//アクションフラグがtrueなら攻撃処理をしない
	if (GetIsActionFlag() == true)
	{
		return;
	}

	//通常攻撃
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		ProcessComboAttack();
		//3コンボ以上ならパターンステートリセット
		if (m_attackPatternState >= enAttackPattern_3)
		{
			m_attackPatternState = enAttackPattern_None;
		}
		return;
	}

	//スキル
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		//アクションフラグをセット
		SetIsActionFlag(true);
		SetNextAnimationState(enAnimationState_Skill_start);
	}

}

void Brave::ProcessDefend()
{
	//アクションフラグがtrueなら攻撃処理をしない
	if (GetIsActionFlag() == true)
	{
		return;
	}
	//防御
	if (g_pad[0]->IsTrigger(enButtonY) == true)
	{
		//アクションフラグをセット
		SetIsActionFlag(true);
		SetNextAnimationState(enAnimationState_Defend);
	}
}

void Brave::Damage(int damage)
{
	//HPが0より大きいなら被ダメージ処理
	if (GetStatus().hp > 0)
	{
		//ガード中なら
		if (m_enAnimationState == enAnimationState_Defend)
		{
			//ダメージを1/3に減らす
			damage /= 3;
			//どれだけダメージを減らしても１以下にはならない
			if (damage < 1){damage = 1;}
			//盾ヒットステートに遷移
			SetNextAnimationState(enAnimationState_DefendHit);
		}
		else
		{
			//ヒットステートに遷移
			SetNextAnimationState(enAnimationState_Hit);
		}

		m_status.CalcHp(damage, false);

		
	}
	//HPが0以下なら
	if (GetStatus().hp <= 0)
	{
		//やられたのでdieFlagをtrueにする
		SetDieFlag(true);
		//HPを0に固定する
		m_status.SetHp(0);
		//死亡ステートに遷移
		SetNextAnimationState(enAnimationState_Die);
	}
}

const bool& Brave::IsInaction() const
{
	//行動出来なくなる条件
	//やられたなら
	if (GetDieFlag() == true)
	{
		return true;
	}

	/*if (m_player->IsWinnerDecision() == true)
	{
		return true;
	}*/

	//ここまできたら行動可能
	return false;
}

void Brave::CalcAttackDirection(float Speed)
{
	//攻撃する方向
	Vector3 attackDirection = m_forward;
	//移動する速度
	Vector3 MoveSpeed = attackDirection * Speed;
	MoveSpeed.y = 0.0f;
	m_position = m_charaCon.Execute(MoveSpeed, 1.0f / 60.0f);
	m_modelRender.SetPosition(m_position);
}

void Brave::ProcessSwordShieldSkill(bool UpOrDownFlag)
{
	Vector3 Y = g_vec3AxisY;
	float mulYPos = 0.0f;
	if (UpOrDownFlag == true)
	{
		//Up処理
		mulYPos += g_gameTime->GetFrameDeltaTime() * 1000.0f;
		Y.y += mulYPos;
	}
	else
	{
		//Down処理
		mulYPos += g_gameTime->GetFrameDeltaTime() * 1300.0f;
		Y.y -= mulYPos;
	}
	
	m_position = m_charaCon.Execute(Y, 1.0f / 30.0f);
	//地面についているなら
	if (m_charaCon.IsOnGround() == true)
	{
		m_position.y = 0.0f;
	}

	m_modelRender.SetPosition(m_position);
}

void Brave::SetNextAnimationState(int nextState)
{
	if (m_BraveState != nullptr) {
		// 古いステートを削除する。
		delete m_BraveState;
		m_BraveState = nullptr;
	}
	
	//アニメーションステートを次のステートに変える
	m_enAnimationState = (EnAnimationState)nextState;

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
		m_BraveState = new BraveStateHit(this);
		break;
	case Brave::enAnimationState_Defend:
		m_BraveState = new BraveStateDefend(this);
		break;
	case Brave::enAnimationState_DefendHit:
		m_BraveState = new BraveStateDefendHit(this);
		break;
	case Brave::enAnimationState_Die:
		m_BraveState = new BraveStateDie(this);
		break;
	case Brave::enAnimationState_ChangeSwordShield:
		m_BraveState = new BraveStateChangeSwordShield(this);
		break;
	case Brave::enAnimationState_Win_Start:
		m_BraveState = new BraveStateWin_Start(this);
		break;
	case Brave::enAnimationState_Win_Main:
		m_BraveState = new BraveStateWin_Main(this);
		break;
	case Brave::enAnimationState_Attack_1:
		m_BraveState = new BraveStateAttack_1(this);
		break;
	case Brave::enAnimationState_Attack_2:
		m_BraveState = new BraveStateAttack_2(this);
		break;
	case Brave::enAnimationState_Attack_3:
		m_BraveState = new BraveStateAttack_3(this);
		break;
	case Brave::enAnimationState_Skill_start:
		m_BraveState = new BraveStateSkill_Start(this);
		break;
	case Brave::enAnimationState_Skill_Main:
		m_BraveState = new BraveStateSkill_Main(this);
		break;
	default:
		break;
	}
}

void Brave::PlayAnimation()
{
	//派生クラスのPlayAnimation関数を実行
	m_BraveState->PlayAnimation();
}

void Brave::ManageState()
{
	//派生クラスのManageState関数を実行
	m_BraveState->ManageState();
}

void Brave::ProcessComboAttack()
{
	//パターンステートを一つ進める
	m_attackPatternState =
		static_cast<EnAttackPattern>(m_attackPatternState + 1);
	//通常攻撃ステート設定
	SetNextAnimationState(m_attackPatternState);
	//アクションフラグをセット
	SetIsActionFlag(true);
}

void Brave::ProcessCommonStateTransition()
{
	if (fabsf(GetMoveSpeed().x) >= 0.001f ||
		fabsf(GetMoveSpeed().z) >= 0.001f
		)
	{
		//走る
		SetNextAnimationState(enAninationState_Sprint);
	}
	else
	{
		//歩く
		SetNextAnimationState(enAninationState_Idle);
	}
}

void Brave::ProcessNormalAttackStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//次のコンボに繋げない、または3コンボ目ならリセット
		if (GetNextComboFlagFlag() == false ||
			m_attackPatternState >= enAttackPattern_3)
		{
			m_attackPatternState = enAttackPattern_None;
			//攻撃アニメーションが終わったのでアクションフラグをfalseにする
			SetIsActionFlag(false);
			//ステート共通の状態遷移処理に遷移
			ProcessCommonStateTransition();
		}
		//次のコンボの攻撃ステート設定
		else
		{
			//次のコンボの処理
			ProcessComboAttack();
		}
		//必ずリセット
		{
			//コンボ受付フラグをリセット
			SetIsComboReceptionFlagFlag(false);
			//次のコンボフラグをリセット
			SetNextComboFlagFlag(false);
		}
	}
	//コンボ受付フラグがtrueかつAボタンを押したら
	else if (GetIsComboReceptionFlagFlag() == true &&
		g_pad[0]->IsTrigger(enButtonA))
	{
		//次のコンボに進むフラグセット
		SetNextComboFlagFlag(true);
	}
}

void Brave::ProcessSkillStartStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//メインステートに遷移
		SetNextAnimationState(enAnimationState_Skill_Main);
	}
}

void Brave::ProcessSkillMainStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//攻撃アニメーションが終わったので攻撃可能
		SetIsActionFlag(false);
		//ステート共通の状態遷移処理に遷移
		ProcessCommonStateTransition();
	}
}

void Brave::ProcessHitStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//アクション中にダメージ受けたかもしれないので
		// アクションフラグ関係を全てfalseにする
		SetAllInfoAboutActionFlag(false);

		//ステート共通の状態遷移処理に遷移
		ProcessCommonStateTransition();
	}
}

void Brave::ProcessDieStateTransition()
{

}



bool Brave::RotationOnly()
{
	//回転可能なアニメーションなら
	if (isRotationEntable() == true)
	{
		m_SaveMoveSpeed = calcVelocity(GetStatus());
		return true;
	}

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
	m_animationClip[enAnimClip_Defend].SetLoopFlag(true);
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
	m_animationClip[enAnimClip_Skill_Start].Load("Assets/animData/character/Player/NewHero/Attack_4.tka");
	m_animationClip[enAnimClip_Skill_Start].SetLoopFlag(false);
	m_animationClip[enAnimClip_Skill_Main].Load("Assets/animData/character/Player/NewHero/Attack_5.tka");
	m_animationClip[enAnimClip_Skill_Main].SetLoopFlag(false);

	m_modelRender.Init("Assets/modelData/character/Player/NewHero/Hero_Smile.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);
	/*m_position = { 0.0f,0.0f,200.0f };
	m_modelRender.SetPosition(m_position);*/
	m_modelRender.Update();

	m_charaCenterBoonId = m_modelRender.FindBoneID(L"root");

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


	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
}

void Brave::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	Sword.Draw(rc);
	Shield.Draw(rc);
}

void Brave::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	///////////////////////////////////////////////////////////
	// 全武器共通処理
	/////////////////////////////////////////////////////////// 
	//コンボ受付が可能なら
	if (wcscmp(eventName, L"ComboAcceptable") == 0)
	{
		SetIsComboReceptionFlagFlag(true);
	}
	//前進する攻撃の始まり
	if (wcscmp(eventName, L"DashAttackStart") == 0)
	{
		SetDashAttackFlag(true);
	}

	//前進する攻撃の終わり
	if (wcscmp(eventName, L"DashAttackEnd") == 0)
	{
		SetDashAttackFlag(false);
	}
	////////////////////////////////////////////////////////////
	// 剣盾の処理
	////////////////////////////////////////////////////////////
	//スキルのノックバックのタイミング
	if (wcscmp(eventName, L"KnockBack") == 0)
	{
		int a = 0;
	}
	//スキルのジャンプ処理
	if (wcscmp(eventName, L"Jamp") == 0)
	{
		ProcessSwordShieldSkill(true);
	}

	//スキルのジャンプ処理
	if (wcscmp(eventName, L"Down") == 0)
	{
		ProcessSwordShieldSkill(false);
	}

	
}

