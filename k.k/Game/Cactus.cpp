#include "stdafx.h"
#include "Cactus.h"
#include "ICactusState.h"
#include "CactusStateIdle.h"
#include "CactusStateRun.h"
#include "CactusStateAttack_1.h"
#include "CactusStateDamage.h"
#include "CactusStateDie.h"
#include "CactusStateVictory.h"
#include "CactusStateAppear.h"
#include "CactusStateSkill.h"
#include "CactusStatePatrol.h"
#include "CactusStateChase.h"

#include "CharactersInfoManager.h"
#include "GameManager.h"

#include "IMobStateMachine.h"

namespace {
	const float ANGLE = 90.0f;				//視野角
	const float DISTANCE_TO_PLAYER = 280.0f;			//プレイヤーとの距離
	const float ATTACK_RANGE = 50.0f;					//攻撃できる距離
	const float SKILL_ATTACK_RANGE = 60.0f;				//スキル攻撃できる距離
	const float STAY_RANGR = 50.0f;						//停止する距離
	const float ATTACK_INTAERVALE_TIME = 1.7f;			//攻撃する間隔
	const float ANGLE_RANGE = 2.0f;						//移動するアングルの範囲
	const float POS2_LENGTH = 30.0f;
	const float ROT_SPEED = 7.5f;						//回転速度
	const float PLAYER_NEARBY_RANGE = 120.0f;			//攻撃した後のプレイヤーを索敵できる範囲
	const float SKILL_TIMER_LIMMIT = 6.0f;

	//ステータス
	int MAXHP = 120;
	int MAXMP = 500;
	int ATK = 15;
	float SPEED = 120.0f;
	const char* NAME = "Cactus";
}

Cactus::Cactus()
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

Cactus::~Cactus()
{
	DeleteGO(m_headCollision);
}

bool Cactus::Start()
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
	
	//　乱数を初期化。
	srand((unsigned)time(NULL));

	//4から5の範囲のインターバル
	m_angleChangeTime = rand() % 2 + 4;

	return true;
}

void Cactus::InitModel()
{
	m_animationClip[enAninationClip_Idle].Load("Assets/animData/character/Cactus/Idle.tka");
	m_animationClip[enAninationClip_Idle].SetLoopFlag(true);
	m_animationClip[enAninationClip_Patrol].Load("Assets/animData/character/Cactus/Walk.tka");
	m_animationClip[enAninationClip_Patrol].SetLoopFlag(true);
	m_animationClip[enAninationClip_Chase].Load("Assets/animData/character/Cactus/Run.tka");
	m_animationClip[enAninationClip_Chase].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Attack].Load("Assets/animData/character/Cactus/Attack1.tka");
	m_animationClip[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Skill].Load("Assets/animData/character/Cactus/Attack2.tka");
	m_animationClip[enAnimationClip_Skill].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Hit].Load("Assets/animData/character/Cactus/Damage.tka");
	m_animationClip[enAnimationClip_Hit].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Die].Load("Assets/animData/character/Cactus/Die.tka");
	m_animationClip[enAnimationClip_Die].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Victory].Load("Assets/animData/character/Cactus/Victory.tka");
	m_animationClip[enAnimationClip_Victory].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Appear].Load("Assets/animData/character/Cactus/Appear.tka");
	m_animationClip[enAnimationClip_Appear].SetLoopFlag(false);

	m_modelRender.Init(
		"Assets/modelData/character/Cactus/Cactus.tkm",
		L"Assets/shader/ToonTextrue/lamp_Cactus.DDS",
		m_animationClip,
		enAnimationClip_Num,
		enModelUpAxisZ
	);
	
	m_charaCon.Init(
		22.0f,
		6.5f,
		m_position
	);

	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	m_attackBoonId = m_modelRender.FindBoneID(L"cactus_head");

	//座標の設定
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void Cactus::Update()
{
	if (IsStopProcessing() != true)
	{
		//スキル攻撃のインターバルの計算
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

//void Cactus::Attack()
//{
//	//被ダメージ、デス時は処理をしない
//	if (isAnimationEnable() != true)
//	{
//		return;
//	}
//	//攻撃中は処理しない
//	if (IsAttackEnable() != true)
//	{
//		return;
//	}
//
//	//攻撃した後のインターバルなら抜け出す
//	if (m_attackEnableFlag == true)
//	{
//		return;
//	}
//
//	//一定の距離にターゲットがいたら
//	if (IsFindPlayer(m_attackRange) == true)
//	{
//		Vector3 toPlayerDir = m_toTarget;
//		//視野角内にターゲットがいたら
//		if (IsInFieldOfView(toPlayerDir, m_forward, m_angle) == true)
//		{
//			int i = rand() % 2;
//			switch (i)
//			{
//			case enAttackName_1:
//				//攻撃1
//				SetNextAnimationState(enAnimationState_Attack_1);
//				//攻撃したのでフラグをtrueにしてインターバルに入る
//				m_attackEnableFlag = true;
//				m_enAttackName = enAttackName_1;
//				break;
//			case enAttackName_2:
//				//攻撃2
//				SetNextAnimationState(enAnimationState_Attack_2);
//				//攻撃したのでフラグをtrueにしてインターバルに入る
//				m_attackEnableFlag = true;
//				m_enAttackName = enAttackName_2;
//				break;
//			default:
//				break;
//			}
//		}
//	}
//}

bool Cactus::IsStopProcessing()
{
	//ゲームステート以外なら
	if (GameManager::GetInstance()->GetGameSeenState() !=
		GameManager::enGameSeenState_Game)
	{
		return true;
	}

	//勝敗が決まったら
	if (m_enOutCome != enOutCome_None)
	{
		return true;
	}

	//勝利したら
	if (GameManager::GetInstance()->GetOutComeState()
		== GameManager::enOutComeState_PlayerLose)
	{
		//勝敗ステートの設定
		SetEnOutCome(enOutCome_Win);
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
			m_knockBackTimer, m_moveSpeed) == true)
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

void Cactus::CreateCollision()
{
	m_headCollision = NewGO<CollisionObject>(0, "monsterattack");
	m_headCollision->SetCreatorName(GetName());
	m_headCollision->CreateSphere(
		m_position,
		m_rotation,
		16.0f
	);
	//ワールド座標取得
	Matrix HeadMatrix = m_modelRender.GetBone(m_attackBoonId)->GetWorldMatrix();
	m_headCollision->SetWorldMatrix(HeadMatrix);
}

void Cactus::Damage(int attack)
{
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
		150.0f
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
	
	SetNextAnimationState(enAnimationState_Hit);
}

void Cactus::ManageState()
{
	m_state->ManageState();
}

void Cactus::PlayAttackSound()
{
	switch (m_enAttackName)
	{
	case Cactus::enAttackName_1:
		g_soundManager->InitAndPlaySoundSource(enSoundName_Cactus_Attack1, g_soundManager->GetSEVolume());
		break;
	case Cactus::enAttackName_2:
		g_soundManager->InitAndPlaySoundSource(enSoundName_Cactus_Attack1, g_soundManager->GetSEVolume());
		break;
	default:
		break;
	}
}

void Cactus::PlayAnimation()
{
	m_state->PlayAnimation();
}

void Cactus::SetNextAnimationState(EnAnimationState nextState)
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
		m_state = new CactusStateIdle(this);
		break;
	case enAninationState_Patrol:
		m_state = new CactusStatePatrol(this);
		//プレイヤーを追いかけていないのでフラグをリセット
		m_chasePlayerFlag = false;
		break;
	case enAninationState_Chase:
		m_state = new CactusStateChase(this);
		//プレイヤーを追いかけているので、フラグをセット
		m_chasePlayerFlag = true;
		break;
	case enAnimationState_Attack:
		m_state = new CactusStateAttack_1(this);
		//攻撃したので、フラグをリセット
		m_attackEnableFlag = false;
		break;
	case enAnimationState_Skill:
		m_state = new CactusStateSkill(this);
		//スキル攻撃したので、スキル攻撃使用可能フラグをリセット
		m_skillUsableFlag = false;
		m_attackEnableFlag = false;
		break;
	case enAnimationState_Hit:
		m_state = new CactusStateDamage(this);
		break;
	case enAnimationState_Die:
		m_state = new CactusStateDie(this);
		break;
	case enAnimationState_Victory:
		m_state = new CactusStateVictory(this);
		break;
	case enAnimationState_Appear:
		m_state = new CactusStateAppear(this);
		break;
	default:
		//強制的にクラッシュさせる
		std::abort();
		break;
	}
}


void Cactus::ProcessCommonStateTransition()
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

void Cactus::OnProcessAttack_1StateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//攻撃終了後まだ近くにプレイヤーがいるなら
		if (IsFindPlayer(PLAYER_NEARBY_RANGE) == true)
		{
			//プレイヤーが近くにいるかフラグをセット
			SetPlayerNearbyFlag(true);
		}

		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Cactus::OnProcessAttack_2StateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{

		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Cactus::OnProcessPlantStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{

		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Cactus::OnProcessPlantToBattleStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{

		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Cactus::OnProcessDamageStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{

		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}


void Cactus::OnProcessDieStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//リストから自身を消す
		CharactersInfoManager::GetInstance()->RemoveMobMonsterFormList(this);
		m_elaseListFlag = true;
		//自身を削除する
		DeleteGO(this);
	}
}

void Cactus::OnProcessVictoryStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Cactus::OnProcessAppearStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Cactus::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
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

void Cactus::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}