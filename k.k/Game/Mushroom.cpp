#include "stdafx.h"
#include "Mushroom.h"
#include "IMushroomState.h"
#include "MushroomStateIdle.h"
#include "MushroomStateWalk.h"
#include "MushroomStateAttack_1.h"
#include "MushroomStateAttack_2.h"
#include "MushroomStateDamage.h"
#include "MushroomStateDie.h"
#include "MushroomStateVictory.h"
#include "MushroomStateAppear.h"

#include "Lich.h"

namespace {
	const float ANGLE = 60.0f;				//視野角
	const float DISTANCE_TO_PLAYER = 800.0f;			//プレイヤーとの距離
	const float ATTACK_RANGE = 45.0f;					//攻撃できる距離
	const float STAY_RANGR = 45.0f;						//停止する距離
	const float ATTACK_INTAERVALE_TIME = 1.5f;			//攻撃する間隔
	const float ANGLE_RANGE = 2.0f;						//移動するアングルの範囲
	const float POS2_LENGTH = 27.0f;
	const float ROT_SPEED = 4.7f;

	//ステータス
	int MAXHP = 100;
	int MAXMP = 500;
	int ATK = 7;
	float SPEED = 130.0f;
	const char* NAME = "Mushroom";
}

Mushroom::Mushroom()
{
	m_angle = ANGLE;

	m_distanceToPlayer = DISTANCE_TO_PLAYER;
	m_attackRange = ATTACK_RANGE;
	m_stayRange = STAY_RANGR;

	m_attackIntervalTime = ATTACK_INTAERVALE_TIME;

	m_angleRange = ANGLE_RANGE;

	m_pos2Length = POS2_LENGTH;
}

Mushroom::~Mushroom()
{
	//if (m_lich != nullptr)
	//{
	//	//リストから自身を消す
	//	m_lich->RemoveAIActorFromList(this);
	//}
}

bool Mushroom::Start()
{
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

	//3から5の範囲のインターバル
	m_angleChangeTime = rand() % 3 + 3;

	return true;
}

void Mushroom::InitModel()
{
	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/Mushroom/Idle_normal.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Walk].Load("Assets/animData/character/Mushroom/Walk.tka");
	m_animationClip[enAnimClip_Walk].SetLoopFlag(true);
	/*m_animationClip[enAnimClip_Walk].Load("Assets/animData/character/Mushroom/Run.tka");
	m_animationClip[enAnimClip_Walk].SetLoopFlag(true);*/
	m_animationClip[enAnimClip_Attack_1].Load("Assets/animData/character/Mushroom/Attack1.tka");
	m_animationClip[enAnimClip_Attack_1].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_2].Load("Assets/animData/character/Mushroom/Attack2.tka");
	m_animationClip[enAnimClip_Attack_2].SetLoopFlag(false);
	m_animationClip[enAnimClip_Damage].Load("Assets/animData/character/Mushroom/Damage.tka");
	m_animationClip[enAnimClip_Damage].SetLoopFlag(false);
	m_animationClip[enAnimClip_Die].Load("Assets/animData/character/Mushroom/Die.tka");
	m_animationClip[enAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enAnimClip_Victory].Load("Assets/animData/character/Mushroom/Victory.tka");
	m_animationClip[enAnimClip_Victory].SetLoopFlag(true);
	m_animationClip[enAnimClip_Appear].Load("Assets/animData/character/Mushroom/Appear.tka");
	m_animationClip[enAnimClip_Appear].SetLoopFlag(false);

	m_modelRender.Init(
		"Assets/modelData/character/Mushroom/Mushroom.tkm",
		L"Assets/shader/ToonTextrue/lamp_Mushroom.DDS",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);
	//モデルの静的オブジェクト作成
	//m_monsterStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	////コリジョン属性を付ける
	//m_monsterStaticObject.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Monster);

	m_charaCon.Init(
		20.0f,
		5.0f,
		m_position
	);

	//座標の設定
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();

	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	m_attackBoonId = m_modelRender.FindBoneID(L"mushroom_spine03");
}

void Mushroom::Update()
{
	if (m_lich != nullptr)
	{
		if (m_lich->GetWinFlag() == true)
		{
			SetWinFlag(true);
			//攻撃中でなければ
			SetNextAnimationState(enAnimationState_Victory);
		}
		if (m_lich->GetTimeUpEndFlag() == true)
		{
			SetWinFlag(true);
			SetNextAnimationState(enAninationState_Idle);
		}
	}

	if (IsStopProcessing() == true)
	{
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

void Mushroom::Attack()
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
			int i = rand() % 2;
			switch (i)
			{
			case enAttackName_1:
				//攻撃
				SetNextAnimationState(enAnimationState_Attack_1);
				//攻撃したのでフラグをtrueにしてインターバルに入る
				m_attackFlag = true;
				break;
			case enAttackName_2:
				//攻撃
				SetNextAnimationState(enAnimationState_Attack_2);
				//攻撃したのでフラグをtrueにしてインターバルに入る
				m_attackFlag = true;
				break;
			default:
				break;
			}
		}
	}
}

void Mushroom::CreateCollision()
{
	auto HeadCollision = NewGO<CollisionObject>(0, "monsterattack");
	HeadCollision->SetCreatorName(GetName());
	HeadCollision->CreateSphere(
		m_position,
		m_rotation,
		16.0f
	);
	//ワールド座標取得
	Matrix HeadMatrix = m_modelRender.GetBone(m_attackBoonId)->GetWorldMatrix();
	HeadCollision->SetWorldMatrix(HeadMatrix);
}

bool Mushroom::IsStopProcessing()
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

void Mushroom::Damage(int attack)
{
	//攻撃中かもしれないので当たり判定を生成しないようにする
	m_createAttackCollisionFlag = false;
	//HPを減らす
	m_status.hp -= attack;

	//HPが0以下なら
	if (m_status.hp <= 0)
	{
		m_status.hp = 0;
		SetNextAnimationState(enAnimationState_Die);
		Dead();
		return;
	}

	//もし防御中なら

	SetNextAnimationState(enAnimationState_Damage);
}

void Mushroom::ManageState()
{
	m_state->ManageState();
}

void Mushroom::PlayAnimation()
{
	m_state->PlayAnimation();
}

void Mushroom::SetNextAnimationState(EnAnimationState nextState)
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
	case Mushroom::enAninationState_Idle:
		m_state = new MushroomStateIdle(this);
		break;
	case Mushroom::enAninationState_Walk:
		m_state = new MushroomStateWalk(this);
		break;
	case Mushroom::enAninationState_Run:
		m_state = new MushroomStateWalk(this);
		break;
	case Mushroom::enAnimationState_Attack_1:
		m_state = new MushroomStateAttack_1(this);
		break;
	case Mushroom::enAnimationState_Attack_2:
		m_state = new MushroomStateAttack_2(this);
		break;
	case Mushroom::enAnimationState_Damage:
		m_state = new MushroomStateDamage(this);
		break;
	case Mushroom::enAnimationState_Die:
		m_state = new MushroomStateDie(this);
		break;
	case Mushroom::enAnimationState_Victory:
		m_state = new MushroomStateVictory(this);
		break;
	case Mushroom::enAnimationState_Appear:
		m_state = new MashroomStateAppear(this);
		break;
	default:
		// ここに来たらステートのインスタンス作成処理の追加忘れ。
		std::abort();
		break;
	}
}

void Mushroom::ProcessCommonStateTransition()
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

void Mushroom::OnProcessAttack_1StateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{

		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Mushroom::OnProcessAttack_2StateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{

		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Mushroom::OnProcessDamageStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{

		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Mushroom::OnProcessDieStateTransition()
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

void Mushroom::OnProcessVictoryStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Mushroom::OnProcessAppearStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Mushroom::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
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

void Mushroom::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}


