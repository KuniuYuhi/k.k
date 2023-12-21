#include "stdafx.h"
#include "TurtleShell.h"
#include "ITurtleShellState.h"
#include "TurtleShellStateIdle.h"
#include "TurtleShellStateAttack_1.h"
#include "TurtleShellStateDamage.h"
#include "TurtleShellStateDie.h"
#include "TurtleShellStateVictory.h"
#include "TurtleShellStateAppear.h"
#include "TurtleShellStateChase.h"
#include "TurtleShellStatePatrol.h"
#include "TurtleShellStateSkill.h"

#include "CharactersInfoManager.h"
#include "GameManager.h"
#include "Lich.h"

#include "IMobStateMachine.h"

namespace {
	const float ANGLE = 65.0f;				//視野角
	const float DISTANCE_TO_PLAYER = 300.0f;			//プレイヤーとの距離
	const float ATTACK_RANGE = 50.0f;					//攻撃できる距離
	const float SKILL_ATTACK_RANGE = 60.0f;				//スキル攻撃できる距離
	const float STAY_RANGR = 45.0f;						//停止する距離
	const float ATTACK_INTAERVALE_TIME = 2.5f;			//攻撃する間隔
	const float PLAYER_NEARBY_RANGE = 150.0f;			//攻撃した後のプレイヤーを索敵できる範囲
	const float ANGLE_RANGE = 2.0f;						//移動するアングルの範囲
	const float POS2_LENGTH = 30.0f;
	const float ROT_SPEED = 7.0f;
	const float SKILL_TIMER_LIMMIT = 10.0f;
	const float DEFENCE_RANGE = 1200.0f;

	//ステータス
	int MAXHP = 200;
	int MAXMP = 500;
	int ATK = 15;
	float SPEED = 80.0f;
	const char* NAME = "TurtleShell";
}

TurtleShell::TurtleShell()
{
	m_angle = ANGLE;

	m_distanceToPlayer = DISTANCE_TO_PLAYER;
	m_attackRange = ATTACK_RANGE;
	m_stayRange = STAY_RANGR;

	m_attackIntervalTime = ATTACK_INTAERVALE_TIME;

	m_angleRange = ANGLE_RANGE;

	m_pos2Length = POS2_LENGTH;

	m_scale *= 1.3f;

	m_skillUsableLimmit = SKILL_TIMER_LIMMIT;
	m_skillAttackRange = SKILL_ATTACK_RANGE;
}

TurtleShell::~TurtleShell()
{
	delete m_stateMachine;
	DeleteGO(m_headCollision);
}

bool TurtleShell::Start()
{
	//ステータスの初期化
	m_status.InitCharacterStatus(
		MAXHP,
		MAXMP,
		ATK,
		SPEED,
		NAME
	);
	//モデルの初期化
	InitModel();
	//ステートマシンの生成
	m_stateMachine = new IMobStateMachine(this);
	//まず召喚アニメーション。その後行動
	SetNextAnimationState(enAnimationState_Appear);

	//6から8の範囲のインターバル
	m_angleChangeTime = rand() % 3 + 6;
	return true;
}

void TurtleShell::InitModel()
{
	m_animationClip[enAnimationClip_Idle].Load("Assets/animData/character/TurtleShell/Idle_Normal.tka");
	m_animationClip[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Patrol].Load("Assets/animData/character/TurtleShell/Walk.tka");
	m_animationClip[enAnimationClip_Patrol].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Chase].Load("Assets/animData/character/TurtleShell/Run.tka");
	m_animationClip[enAnimationClip_Chase].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Attack].Load("Assets/animData/character/TurtleShell/Attack1.tka");
	m_animationClip[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Skill].Load("Assets/animData/character/TurtleShell/Defence.tka");
	m_animationClip[enAnimationClip_Skill].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Hit].Load("Assets/animData/character/TurtleShell/Damage.tka");
	m_animationClip[enAnimationClip_Hit].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Die].Load("Assets/animData/character/TurtleShell/Die.tka");
	m_animationClip[enAnimationClip_Die].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Victory].Load("Assets/animData/character/TurtleShell/Victory.tka");
	m_animationClip[enAnimationClip_Victory].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Appear].Load("Assets/animData/character/TurtleShell/Appear.tka");
	m_animationClip[enAnimationClip_Appear].SetLoopFlag(false);

	m_modelRender.Init(
		"Assets/modelData/character/TurtleShell/TurtleShell.tkm",
		L"Assets/shader/ToonTextrue/lamp_TurltleShell.DDS",
		m_animationClip,
		enAnimationClip_Num,
		enModelUpAxisZ
	);
	
	m_charaCon.Init(
		23.6f,
		9.0f,
		m_position
	);

	//座標の設定
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();

	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	m_attackBoonId = m_modelRender.FindBoneID(L"Head");
}

void TurtleShell::Update()
{
	if (IsStopProcessing() != true)
	{
		//スキルのインターバルの計算
		CalcSkillAttackIntarval();
		//攻撃間隔インターバル
		AttackInterval(m_attackIntervalTime);
		//アングル切り替えインターバル
		AngleChangeTimeIntarval(m_angleChangeTime);

		//毎フレーム行う処理
		m_stateMachine->Execute();
		
		//回転処理
		Rotation(ROT_SPEED, ROT_SPEED);

		//当たり判定
		DamageCollision(m_charaCon);
	}

	ManageState();
	PlayAnimation();

	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void TurtleShell::CreateCollision()
{
	m_headCollision = NewGO<CollisionObject>(0, "monsterattack");
	m_headCollision->SetCreatorName(GetName());
	m_headCollision->CreateSphere(
		m_position,
		m_rotation,
		17.0f
	);
	//ワールド座標取得
	Matrix HeadMatrix = m_modelRender.GetBone(m_attackBoonId)->GetWorldMatrix();
	m_headCollision->SetWorldMatrix(HeadMatrix);
}

bool TurtleShell::IsStopProcessing()
{
	//ゲームステート以外なら
	if (GameManager::GetInstance()->GetGameSeenState() !=
		GameManager::enGameSeenState_Game)
	{
		return true;
	}

	//勝利したら
	if (GameManager::GetInstance()->GetOutComeState()
		== GameManager::enOutComeState_PlayerLose)
	{
		SetWinFlag(true);
		//攻撃中でなければ
		SetNextAnimationState(enAnimationState_Victory);
		return true;
	}
	//負けた時
	if (GameManager::GetInstance()->GetOutComeState()
		== GameManager::enOutComeState_PlayerWin)
	{
		SetNextAnimationState(enAninationState_Idle);
		return true;
	}

	//召喚された時のアニメーションステートなら	
	if (m_enAnimationState == enAnimationState_Appear)
	{
		return true;
	}

	//ノックバック中なら
	if (GetKnockBackFlag() == true)
	{
		//ノックバックの処理をするなら
		if (IsProcessKnockBack(
			m_moveSpeed, m_knockBackTimer) == true)
		{
			//座標を移動
			m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
			return true;
		}
		else
		{
			SetKnockBackFlag(false);
		}
	}

	//それ以外なら
	return false;
}

bool TurtleShell::IsDifenceEnd()
{
	if (m_difenceTime < m_difenceTimer)
	{
		//防御可能フラグをリセット
		m_difenceEnableFlag = false;
		m_difenceTimer = 0.0f;
		return true;
	}
	else
	{
		//タイマーを加算
		m_difenceTimer += g_gameTime->GetFrameDeltaTime();
		return false;
	}
}

void TurtleShell::HitNormalAttack()
{
	m_damage = m_player->GetAtk();
	//ダメージを受ける処理
	Damage(m_damage);
	//スキルを使っていなかったらダメージとエフェクト表示
	if (m_enAnimationState != enAnimationState_Skill)
	{
		//ダメージを表示
		CreateDamageFont(m_damage);
		//エフェクトを再生
		CreateHitEffect();
	}

	m_player->SetAttackHitFlag(true);
}

void TurtleShell::HitSkillAttack()
{
	m_damage = m_player->GetAtk();
	//ダメージを受ける処理
	Damage(m_damage);
	//スキルを使っていなかったらダメージとエフェクト表示
	if (m_enAnimationState != enAnimationState_Skill)
	{
		//ダメージを表示
		CreateDamageFont(m_damage);
		//エフェクトを再生
		CreateHitEffect();
	}
	
	m_player->SetAttackHitFlag(true);
}

bool TurtleShell::IsSkillUsable()
{
	//スキル攻撃可能フラグがセットされていたらスキル攻撃可能
	//まだプレイヤーが近くにいるなら
	if (m_skillUsableFlag == true&& 
		m_difenceEnableFlag == true/*&& 
		IsFindPlayer(DEFENCE_RANGE) == true*/)
	{
		//スキル攻撃可能
		return true;
	}
	//スキル攻撃不可能
	return false;
}

//bool TurtleShell::IsFoundPlayerFlag()
//{
//	//まずプレイヤーとの距離が近いか
//	if (IsFindPlayer(m_distanceToPlayer) == true)
//	{
//		//追いかけている時は視野角判定なし
//		if (m_chasePlayerFlag == true)
//		{
//			return true;
//		}
//
//		//攻撃後で、まだ近くにプレイヤーがいるなら
//		if (GetPlayerNearbyFlag() == true)
//		{
//			SetPlayerNearbyFlag(false);
//			return true;
//		}
//
//		Vector3 toPlayerDir = m_toTarget;
//		//視野角内にプレイヤーがいるなら
//		if (IsInFieldOfView(toPlayerDir, m_forward, m_angle) == true)
//		{
//			return true;
//		}
//	}
//	
//
//	return false;
//}

void TurtleShell::Damage(int attack)
{
	//防御中は処理しない
	if (m_enAnimationState == enAnimationState_Skill)
	{
		//SE再生

		return;
	}

	//攻撃中かもしれないので当たり判定を生成しないようにする
	m_createAttackCollisionFlag = false;

	//HPを減らす
	m_status.CalcHp(attack, false);

	//ノックバックフラグをセット
	//todo 強さをこんぼやスキルによって変える
	SetKnockBackFlag(true);
	m_moveSpeed = SetKnockBackDirection(
		m_position,
		m_player->GetPosition(),
		m_player->GetKnockBackPower()
	);

	//HPが0以下なら
	if (m_status.GetHp() <= 0)
	{
		//やられアニメーションステート
		m_status.SetHp(0);
		SetNextAnimationState(enAnimationState_Die);
		ProcessDead();
		return;
	}

	//被ダメージアニメーションステート
	SetNextAnimationState(enAnimationState_Hit);
	
}

bool TurtleShell::RotationOnly()
{
	if (m_enAnimationState == enAnimationState_Skill)
	{
		return true;
	}
	return false;
}

void TurtleShell::ManageState()
{
	m_state->ManageState();
}

void TurtleShell::PlayAnimation()
{
	m_state->PlayAnimation();
}

void TurtleShell::SetNextAnimationState(EnAnimationState nextState)
{
	if (m_state != nullptr) {
		// 古いステートを削除する。
		delete m_state;
		m_state = nullptr;
	}

	//アニメーションステートを次のステートに変える
	m_enAnimationState = nextState;

	switch (m_enAnimationState)
	{
	case enAninationState_Idle:
		m_state = new TurtleShellStateIdle(this);
		break;
	case enAninationState_Patrol:
		m_state = new TurtleShellStatePatrol(this);
		//プレイヤーを追いかけていないのでフラグをリセット
		m_chasePlayerFlag = false;
		break;
	case enAninationState_Chase:
		m_state = new TurtleShellStateChase(this);
		//プレイヤーを追いかけているので、フラグをセット
		m_chasePlayerFlag = true;
		break;
	case enAnimationState_Attack:
		m_state = new TurtleShellStateAttack_1(this);
		//攻撃したので、フラグをリセット
		m_attackEnableFlag = false;
		break;
	case enAnimationState_Skill:
		m_state = new TurtleShellStateSkill(this);
		//スキル攻撃したので、スキル攻撃使用可能フラグをリセット
		m_skillUsableFlag = false;
		m_attackEnableFlag = false;
		break;
	case enAnimationState_Hit:
		m_state = new TurtleShellStateDamage(this);
		break;
	case enAnimationState_Die:
		m_state = new TurtleShellStateDie(this);
		break;
	case enAnimationState_Victory:
		m_state = new TurtleShellStateVictory(this);
		break;
	case enAnimationState_Appear:
		m_state = new TurtleShellStateAppear(this);
		break;
	default:
		// ここに来たらステートのインスタンス作成処理の追加忘れ。
		std::abort();
		break;
	}
}

void TurtleShell::ProcessCommonStateTransition()
{
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		SetNextAnimationState(enAninationState_Patrol);
	}
	else
	{
		SetNextAnimationState(enAninationState_Idle);
	}
}

void TurtleShell::OnProcessAttack_1StateTransition()
{
	//アニメーションが終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//攻撃終了後まだ近くにプレイヤーがいるなら
		if (IsFindPlayer(PLAYER_NEARBY_RANGE) == true)
		{
			//プレイヤーが近くにいるかフラグをセット
			SetPlayerNearbyFlag(true);
		}
		//共通のステート遷移処理実行
		ProcessCommonStateTransition();
	}
}

void TurtleShell::OnProcessDamageStateTransition()
{
	//アニメーションが終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//ダメージを受けたのでスキルを使えるようにする
		m_difenceEnableFlag = true;

		//共通のステート遷移処理実行
		ProcessCommonStateTransition();
	}
}

void TurtleShell::OnProcessDieStateTransition()
{
	//アニメーションが終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//リストから自身を消す
		CharactersInfoManager::GetInstance()->RemoveMobMonsterFormList(this);
		m_elaseListFlag = true;
		//自身を削除する
		DeleteGO(this);
	}
}

void TurtleShell::OnProcessVictoryStateTransition()
{
	//アニメーションが終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//共通のステート遷移処理実行
		ProcessCommonStateTransition();
	}
}

void TurtleShell::OnProcessAppearStateTransition()
{
	//アニメーションが終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//共通のステート遷移処理実行
		ProcessCommonStateTransition();
	}
}

void TurtleShell::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//当たり判定生成タイミング
	if (wcscmp(eventName, L"Collision_Start") == 0)
	{
		m_createAttackCollisionFlag = true;
	}
	//当たり判定生成終わり
	if (wcscmp(eventName, L"Collision_End") == 0)
	{
		m_createAttackCollisionFlag = false;
	}
}

void TurtleShell::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
