#include "stdafx.h"
#include "Slime.h"
#include "ISlimeState.h"
#include "SlimeStateIdle.h"
#include "SlimeStateWalk.h"
#include "SlimeStateAttack.h"
#include "SlimeStateDamage.h"
#include "SlimeStateDie.h"
#include "SlimeStateVictory.h"
#include "SlimeStateAppear.h"
#include "SlimeStateChase.h"
#include "SlimeStatePatrol.h"
#include "SlimeStateSkill.h"


#include "CharactersInfoManager.h"
#include "GameManager.h"
#include "InitEffect.h"

#include "IMobStateMachine.h"

namespace {
	const float ANGLE = 70.0f;				//視野角
	const float DISTANCE_TO_PLAYER = 300.0f;			//プレイヤーとの距離
	const float ATTACK_RANGE = 50.0f;					//攻撃できる距離
	const float SKILL_ATTACK_RANGE = 70.0f;				//スキル攻撃できる距離
	const float STAY_RANGR = 45.0f;						//停止する距離
	const float ATTACK_INTAERVALE_TIME = 2.0f;			//攻撃する間隔
	const float PLAYER_NEARBY_RANGE = 140.0f;			//攻撃した後のプレイヤーを索敵できる範囲
	const float ANGLE_RANGE = 2.0f;						//移動するアングルの範囲
	const float POS2_LENGTH = 30.0f;
	const float ROT_SPEED = 7.5f;
	const float SKILL_TIMER_LIMMIT = 8.0f;

	//ステータス
	int MAXHP = 150;
	int MAXMP = 500;
	int ATK = 10;
	float SPEED = 110.0f;
	const char* NAME = "Slime";
}

Slime::Slime()
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

Slime::~Slime()
{
	delete m_stateMachine;
	DeleteGO(m_headCollision);
}

bool Slime::Start()
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


	//4から６の範囲のインターバル
	m_angleChangeTime = rand() % 3 + 4;

	
	return true;
}

void Slime::InitModel()
{
	m_animationClip[enAnimationClip_Idle].Load("Assets/animData/character/Slime/Idle_Normal.tka");
	m_animationClip[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Patrol].Load("Assets/animData/character/Slime/Walk.tka");
	m_animationClip[enAnimationClip_Patrol].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Chase].Load("Assets/animData/character/Slime/Run.tka");
	m_animationClip[enAnimationClip_Chase].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Attack].Load("Assets/animData/character/Slime/Attack1.tka");
	m_animationClip[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Skill].Load("Assets/animData/character/Slime/Skill.tka");
	m_animationClip[enAnimationClip_Skill].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Hit].Load("Assets/animData/character/Slime/Damege.tka");
	m_animationClip[enAnimationClip_Hit].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Die].Load("Assets/animData/character/Slime/Die.tka");
	m_animationClip[enAnimationClip_Die].SetLoopFlag(false);
	m_animationClip[enAnimationClip_Victory].Load("Assets/animData/character/Slime/Victory.tka");
	m_animationClip[enAnimationClip_Victory].SetLoopFlag(true);
	m_animationClip[enAnimationClip_Appear].Load("Assets/animData/character/Slime/Appear.tka");
	m_animationClip[enAnimationClip_Appear].SetLoopFlag(false);

	m_modelRender.Init(
		"Assets/modelData/character/Slime/slime.tkm",
		L"Assets/shader/ToonTextrue/lamp_Slime.DDS",
		m_animationClip, 
		enAnimationClip_Num,
		enModelUpAxisZ
	);
	
	m_charaCon.Init(
		22.0f,
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

	//攻撃用ボーンの取得
	m_attackBoonId = m_modelRender.FindBoneID(L"Head");

}

void Slime::Update()
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

bool Slime::IsStopProcessing()
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

void Slime::CreateCollision()
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

void Slime::Damage(int attack)
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

void Slime::ManageState()
{
	m_state->ManageState();
}

void Slime::PlayAttackSound()
{
	g_soundManager->InitAndPlaySoundSource(enSoundName_Slime_Attack, g_soundManager->GetSEVolume());
}

void Slime::PlayAnimation()
{
	m_state->PlayAnimation();
}

void Slime::SetNextAnimationState(EnAnimationState nextState)
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
		m_state = new SlimeStateIdle(this);
		break;
	case enAninationState_Patrol:
		m_state = new SlimeStatePatrol(this);
		//プレイヤーを追いかけていないのでフラグをリセット
		m_chasePlayerFlag = false;
		break;
	case enAninationState_Chase:
		m_state = new SlimeStateChase(this);
		//プレイヤーを追いかけているので、フラグをセット
		m_chasePlayerFlag = true;
		break;
	case enAnimationState_Attack:
		m_state = new SlimeStateAttack(this);
		//攻撃したので、フラグをリセット
		m_attackEnableFlag = false;
		break;
	case enAnimationState_Skill:
		m_state = new SlimeStateSkill(this);
		//スキル攻撃したので、スキル攻撃使用可能フラグをリセット
		m_skillUsableFlag = false;
		m_attackEnableFlag = false;
		break;
	case enAnimationState_Hit:
		m_state = new SlimeStateDamage(this);
		break;
	case enAnimationState_Die:
		m_state = new SlimeStateDie(this);
		break;
	case enAnimationState_Victory:
		m_state = new SlimeStateVictory(this);
		break;
	case enAnimationState_Appear:
		m_state = new SlimeStateAppear(this);
		break;
	default:
		// ここに来たらステートのインスタンス作成処理の追加忘れ。
		std::abort();
		break;
	}
}

void Slime::ProcessCommonStateTransition()
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

void Slime::OnProcessAttack_1StateTransition()
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

void Slime::OnProcessDamageStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{

		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Slime::OnProcessDieStateTransition()
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

void Slime::OnProcessVictoryStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{

		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Slime::OnProcessAppearStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Slime::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
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

void Slime::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}