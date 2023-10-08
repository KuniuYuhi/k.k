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

#include "Lich.h"
#include "InitEffect.h"

namespace {
	const float ANGLE = 70.0f;				//視野角
	const float DISTANCE_TO_PLAYER = 200.0f;			//プレイヤーとの距離
	const float ATTACK_RANGE = 50.0f;					//攻撃できる距離
	const float STAY_RANGR = 45.0f;						//停止する距離
	const float ATTACK_INTAERVALE_TIME = 2.0f;			//攻撃する間隔
	const float ANGLE_RANGE = 2.0f;						//移動するアングルの範囲
	const float POS2_LENGTH = 30.0f;
	const float ROT_SPEED = 7.5f;

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
}

Slime::~Slime()
{
	//if (m_lich != nullptr)
	//{
	//	//リストから自身を消す
	//	m_lich->RemoveAIActorFromList(this);
	//}
}

bool Slime::Start()
{
	//初期のアニメーションステートを待機状態にする。
	SetNextAnimationState(enAninationState_Idle);

	m_status.InitStatus(
		MAXHP,
		MAXMP,
		ATK,
		SPEED,
		NAME
	);

	InitModel();

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
	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/Slime/Idle_Normal.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Walk].Load("Assets/animData/character/Slime/Walk.tka");
	m_animationClip[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClip[enAnimClip_Walk].Load("Assets/animData/character/Slime/Run.tka");
	m_animationClip[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClip[enAnimClip_Attack_1].Load("Assets/animData/character/Slime/Attack1.tka");
	m_animationClip[enAnimClip_Attack_1].SetLoopFlag(false);
	m_animationClip[enAnimClip_Damage].Load("Assets/animData/character/Slime/Damege.tka");
	m_animationClip[enAnimClip_Damage].SetLoopFlag(false);
	m_animationClip[enAnimClip_Die].Load("Assets/animData/character/Slime/Die.tka");
	m_animationClip[enAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enAnimClip_Victory].Load("Assets/animData/character/Slime/Victory.tka");
	m_animationClip[enAnimClip_Victory].SetLoopFlag(true);
	m_animationClip[enAnimClip_Appear].Load("Assets/animData/character/Slime/Appear.tka");
	m_animationClip[enAnimClip_Appear].SetLoopFlag(false);

	m_modelRender.Init(
		"Assets/modelData/character/Slime/slime.tkm",
		L"Assets/shader/ToonTextrue/lamp_Slime.DDS",
		m_animationClip, 
		enAnimClip_Num,
		enModelUpAxisZ
	);
	//モデルの静的オブジェクト作成
	//m_monsterStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	////コリジョン属性を付ける
	//m_monsterStaticObject.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Monster);

	m_charaCon.Init(
		16.0f,
		4.0f,
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
	if (m_lich != nullptr)
	{
		//勝利したら
		if (m_lich->GetWinFlag() == true)
		{
			SetWinFlag(true);
			//攻撃中でなければ
			SetNextAnimationState(enAnimationState_Victory);
		}
		//タイムアップフラグがtrueなら
		if (m_lich->GetTimeUpEndFlag() == true)
		{
			//敵の勝利
			SetWinFlag(true);
			SetNextAnimationState(enAninationState_Idle);
		}
	}
	
	if (IsStopProcessing()==true)
	{
		//これより下の処理をしない
		ManageState();
		PlayAnimation();
		m_modelRender.Update();
		return;
	}

	AttackInterval(m_attackIntervalTime);

	DamageCollision(m_charaCon);

	AngleChangeTimeIntarval(m_angleChangeTime);

	Move(m_charaCon);
	Rotation(ROT_SPEED, ROT_SPEED);

	//攻撃
	Attack();

	ManageState();
	PlayAnimation();

	if (m_createAttackCollisionFlag == true)
	{
		CreateCollision();
	}

	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void Slime::Attack()
{
	//被ダメージ、デス時は処理しない
	if (isAnimationEntable() != true)
	{
		return;
	}

	//攻撃中は処理しない
	if (IsAttackEntable() != true)
	{
		return;
	}

	//攻撃した後のインターバルなら抜け出す
	if (m_attackFlag == true)
	{
		return;
	}

	//一定の距離にターゲットがいたら
	if (IsFindPlayer(m_attackRange) == true)
	{
		Vector3 toPlayerDir = m_toTarget;
		//視野角内にターゲットがいたら
		if (IsInFieldOfView(toPlayerDir, m_forward, m_angle) == true)
		{
			//攻撃
			SetNextAnimationState(enAnimationState_Attack_1);
			//攻撃したのでフラグをtrueにしてインターバルに入る
			m_attackFlag = true;
		}
	}


}

bool Slime::IsStopProcessing()
{
	//勝利したら
	if (GetWinFlag() == true)
	{
		return true;
	}
	//召喚された時のアニメーションステートなら	
	if (m_enAnimationState == enAnimationState_Appear)
	{
		return true;
	}

	//それ以外なら
	return false;
}

void Slime::CreateCollision()
{
	auto HeadCollision = NewGO<CollisionObject>(0, "monsterattack");
	HeadCollision->SetCreatorName(GetName());
	HeadCollision->CreateSphere(
		m_position,
		m_rotation,
		17.0f
	);
	//ワールド座標取得
	Matrix HeadMatrix = m_modelRender.GetBone(m_attackBoonId)->GetWorldMatrix();
	HeadCollision->SetWorldMatrix(HeadMatrix);
}

void Slime::Damage(int attack)
{
	//攻撃中かもしれないので当たり判定を生成しないようにする
	m_createAttackCollisionFlag = false;

	//HPを減らす
	m_status.hp -= attack;

	//HPが0以下なら
	if (m_status.hp <= 0)
	{
		//やられアニメーションステート
		m_status.hp = 0;
		SetNextAnimationState(enAnimationState_Die);
		Dead();
		return;
	}
	//被ダメージアニメーションステート
	SetNextAnimationState(enAnimationState_Damage);
	
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
	case Slime::enAninationState_Idle:
		m_state = new SlimeStateIdle(this);
		break;
	case Slime::enAninationState_Walk:
		m_state = new SlimeStateWalk(this);
		break;
	case Slime::enAninationState_Run:
		m_state = new SlimeStateWalk(this);
		break;
	case Slime::enAnimationState_Attack_1:
		m_state = new SlimeStateAttack(this);
		break;
	case Slime::enAnimationState_Damage:
		m_state = new SlimeStateDamage(this);
		break;
	case Slime::enAnimationState_Die:
		m_state = new SlimeStateDie(this);
		break;
	case Slime::enAnimationState_Victory:
		m_state = new SlimeStateVictory(this);
		break;
	case Slime::enAnimationState_Appear:
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
		SetNextAnimationState(enAninationState_Walk);
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
		if (m_lich != nullptr)
		{
			//リストから自身を消す
			m_lich->RemoveAIActorFromList(this);
			m_elaseListFlag = true;
		}
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