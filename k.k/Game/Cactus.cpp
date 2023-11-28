#include "stdafx.h"
#include "Cactus.h"
#include "ICactusState.h"
#include "CactusStateIdle.h"
#include "CactusStateRun.h"
#include "CactusStateAttack_1.h"
#include "CactusStateAttack_2.h"
#include "CactusStatePlant.h"
#include "CactusStatePlantToBattle.h"
#include "CactusStateDamage.h"
#include "CactusStateDie.h"
#include "CactusStateVictory.h"
#include "CactusStateAppear.h"

#include "CharactersInfoManager.h"

#include "Lich.h"

namespace {
	const float ANGLE = 80.0f;				//視野角
	const float DISTANCE_TO_PLAYER = 200.0f;			//プレイヤーとの距離
	const float ATTACK_RANGE = 50.0f;					//攻撃できる距離
	const float STAY_RANGR = 45.0f;						//停止する距離
	const float ATTACK_INTAERVALE_TIME = 1.7f;			//攻撃する間隔
	const float ANGLE_RANGE = 2.0f;						//移動するアングルの範囲
	const float POS2_LENGTH = 30.0f;
	const float ROT_SPEED = 7.5f;						//回転速度

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
}

Cactus::~Cactus()
{
	//if (m_lich != nullptr)
	//{
	//	//リストから自身を消す
	//	m_lich->RemoveAIActorFromList(this);
	//}
}

bool Cactus::Start()
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

	//4から5の範囲のインターバル
	m_angleChangeTime = rand() % 2 + 4;

	return true;
}

void Cactus::InitModel()
{
	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/Cactus/Idle.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Run].Load("Assets/animData/character/Cactus/Run.tka");
	m_animationClip[enAnimClip_Run].SetLoopFlag(true);
	m_animationClip[enAnimClip_Attack_1].Load("Assets/animData/character/Cactus/Attack1.tka");
	m_animationClip[enAnimClip_Attack_1].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_2].Load("Assets/animData/character/Cactus/Attack2.tka");
	m_animationClip[enAnimClip_Attack_2].SetLoopFlag(false);
	m_animationClip[enAnimClip_Plant].Load("Assets/animData/character/Cactus/Plant.tka");
	m_animationClip[enAnimClip_Plant].SetLoopFlag(false);
	m_animationClip[enAnimClip_PlantToBattle].Load("Assets/animData/character/Cactus/PlantToBattle.tka");
	m_animationClip[enAnimClip_PlantToBattle].SetLoopFlag(false);
	m_animationClip[enAnimClip_Damage].Load("Assets/animData/character/Cactus/Damage.tka");
	m_animationClip[enAnimClip_Damage].SetLoopFlag(false);
	m_animationClip[enAnimClip_Die].Load("Assets/animData/character/Cactus/Die.tka");
	m_animationClip[enAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enAnimClip_Victory].Load("Assets/animData/character/Cactus/Victory.tka");
	m_animationClip[enAnimClip_Victory].SetLoopFlag(true);
	m_animationClip[enAnimClip_Appear].Load("Assets/animData/character/Cactus/Appear.tka");
	m_animationClip[enAnimClip_Appear].SetLoopFlag(false);

	m_modelRender.Init(
		"Assets/modelData/character/Cactus/Cactus.tkm",
		L"Assets/shader/ToonTextrue/lamp_Cactus.DDS",
		m_animationClip,
		enAnimClip_Num,
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
		//攻撃間隔インターバル
		AttackInterval(m_attackIntervalTime);
		//当たり判定
		DamageCollision(m_charaCon);
		//アングル切り替えインターバル
		AngleChangeTimeIntarval(m_angleChangeTime);
		//移動処理
		Move(m_charaCon);
		//回転処理
		Rotation(ROT_SPEED, ROT_SPEED);
		//攻撃処理
		Attack();
		//当たり判定の生成
		if (m_createAttackCollisionFlag == true)
		{
			CreateCollision();
		}
	}

	ManageState();
	PlayAnimation();

	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void Cactus::Attack()
{
	//被ダメージ、デス時は処理をしない
	if (isAnimationEnable() != true)
	{
		return;
	}
	//攻撃中は処理しない
	if (IsAttackEnable() != true)
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
				//攻撃1
				SetNextAnimationState(enAnimationState_Attack_1);
				//攻撃したのでフラグをtrueにしてインターバルに入る
				m_attackFlag = true;
				m_enAttackName = enAttackName_1;
				break;
			case enAttackName_2:
				//攻撃2
				SetNextAnimationState(enAnimationState_Attack_2);
				//攻撃したのでフラグをtrueにしてインターバルに入る
				m_attackFlag = true;
				m_enAttackName = enAttackName_2;
				break;
			default:
				break;
			}
		}
	}
}

bool Cactus::IsStopProcessing()
{
	//勝敗が決まったら
	if (m_enOutCome != enOutCome_None)
	{
		return true;
	}

	//勝利したら
	if (m_lich != nullptr)
	{
		if (m_lich->GetEnOutCome() == enOutCome_Win)
		{
			//勝敗ステートの設定
			SetEnOutCome(enOutCome_Win);
			SetWinFlag(true);
			//攻撃中でなければ
			SetNextAnimationState(enAnimationState_Victory);
			return true;
		}
		//負けた時
		if (m_lich->GetEnOutCome() == enOutCome_Lose)
		{
			SetNextAnimationState(enAninationState_Idle);
			return true;
		}
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

void Cactus::Damage(int attack)
{
	//攻撃中かもしれないので当たり判定を生成しないようにする
	m_createAttackCollisionFlag = false;
	//HPを減らす
	m_status.CalcHp(attack, false);

	//HPが0以下なら
	if (m_status.hp <= 0)
	{
		//やられアニメーションステート
		m_status.SetHp(0);
		SetNextAnimationState(enAnimationState_Die);
		ProcessDead();
		return;
	}
	//その攻撃にノックバック効果があるなら
	if (m_player->GetKnockBackAttackFlag() == true)
	{
		//ノックバックフラグをセット
		SetKnockBackFlag(true);
		//ノックバックする方向を決める
		m_moveSpeed = SetKnockBackDirection(
			m_player->GetAttackPosition(),
			m_position,
			m_player->GetKnockBackPower()
		);
	}
	SetNextAnimationState(enAnimationState_Damage);
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
	case Cactus::enAninationState_Idle:
		m_state = new CactusStateIdle(this);
		break;
	case Cactus::enAninationState_Walk:
		m_state = new CactusStateRun(this);
		break;
	case Cactus::enAninationState_Run:
		m_state = new CactusStateRun(this);
		break;
	case Cactus::enAnimationState_Attack_1:
		m_state = new CactusStateAttack_1(this);
		break;
	case Cactus::enAnimationState_Attack_2:
		m_state = new CactusStateAttack_2(this);
		break;
	case Cactus::enAnimationState_Plant:
		m_state = new CactusStatePlant(this);
		break;
	case Cactus::enAnimationState_PlantToBattle:
		m_state = new CactusStatePlantToBattle(this);
		break;
	case Cactus::enAnimationState_Damage:
		m_state = new CactusStateDamage(this);
		break;
	case Cactus::enAnimationState_Die:
		m_state = new CactusStateDie(this);
		break;
	case Cactus::enAnimationState_Victory:
		m_state = new CactusStateVictory(this);
		break;
	case Cactus::enAnimationState_Appear:
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
		SetNextAnimationState(enAninationState_Run);
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
		if (m_lich != nullptr)
		{
			//リストから自身を消す
			CharactersInfoManager::GetInstance()->RemoveMobMonsterFormList(this);
			//m_lich->RemoveAIActorFromList(this);
			m_elaseListFlag = true;
		}
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