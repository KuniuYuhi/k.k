#include "stdafx.h"
#include "Lich.h"
#include "LichStateIdle.h"
#include "LichStateWalk.h"
#include "LichStateAttack_1.h"
#include "LichStateAttack_2.h"
#include "LichStateDie.h"

namespace {
	const float SCALE_UP = 3.0f;									//キャラクターのサイズ
	const Vector3 FIRST_POSITION = Vector3(0.0f, 0.0f, -250.0f);	//最初の座標
	const float DISTANCE = 1500.0f;									//プレイヤーを発見できる距離


	//ステータス
	int MAXHP = 500;
	int MAXMP = 500;
	int ATK = 20;
	float SPEED = 50.0f;
	const char* NAME = "Lich";
}

Lich::Lich()
{
	//最初の座標を設定する
	m_position = FIRST_POSITION;
	//プレイヤーを発見できる距離を設定する
	m_distance = DISTANCE;
	//リッチのサイズを設定
	m_scale *= SCALE_UP;
}

Lich::~Lich()
{
}

bool Lich::Start()
{
	//初期のアニメーションステートを待機状態にする。
	SetNextAnimationState(enAninationState_Idle);


	//ステータスの初期化
	m_status.InitStatus(
		MAXHP,
		MAXMP,
		ATK,
		SPEED,
		NAME
	);

	InitModel();

	return true;
}

void Lich::InitModel()
{
	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/Lich/Idle.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Walk].Load("Assets/animData/character/Lich/Walk.tka");
	m_animationClip[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClip[enAnimClip_Attack_1].Load("Assets/animData/character/Lich/Attack1.tka");
	m_animationClip[enAnimClip_Attack_1].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_2].Load("Assets/animData/character/Lich/Attack2.tka");
	m_animationClip[enAnimClip_Attack_2].SetLoopFlag(false);
	m_animationClip[enAnimClip_Die].Load("Assets/animData/character/Lich/Die.tka");
	m_animationClip[enAnimClip_Die].SetLoopFlag(false);

	m_modelRender.Init("Assets/modelData/character/Lich/Lich.tkm",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);

	//モデルのTRSの設定
	m_modelRender.SetTransform(
		m_position,
		m_rotation,
		m_scale
	);
	m_modelRender.Update();

	//キャラコンの設定
	m_charaCon.Init(
		70.0f,
		50.0f,
		m_position
	);

}

void Lich::Update()
{
	//倒されたら他の処理を実行しないようにする
	if (m_dieFlag==true)
	{
		ManageState();
		PlayAnimation();
		m_modelRender.Update();
		return;
	}

	AttackInterval();

	DecideNextAction();

	
	

	ManageState();
	PlayAnimation();

	DamageCollision();

	SetTransFormModel(m_modelRender);
	m_modelRender.Update();

	m_oldMoveSpeed = m_moveSpeed;
}

void Lich::Move()
{
	SetTargetPosition();

	if (m_attackRangeFlag == true)
	{
		//移動しないようにする
		m_moveSpeed = calcVelocity(m_status);
		m_moveSpeed = Vector3::Zero;
	}
	//プレイヤーを見つけたら
	else if (IsFindPlayer(m_distance) == true)
	{
		//移動
		m_moveSpeed = calcVelocity(m_status);
		m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	}
	else
	{
		//移動しないようにする
		m_moveSpeed = Vector3::Zero;
	}

	
	Rotation();

}

void Lich::Damage()
{
	if (m_status.hp > 0)
	{
		m_status.hp -= m_player->GetAtk();
	}
	else if(m_status.hp <= 0)
	{
		//Dieフラグをtrueにする
		m_dieFlag = true;
		m_status.hp = 0;
		SetNextAnimationState(enAnimationState_Die);
	}

}

bool Lich::RotationOnly()
{
	//特定のアニメーションが再生中のとき
	if (isRotationEntable() != true)
	{
		//xかzの移動速度があったら(スティックの入力があったら)。
		if (fabsf(m_SaveMoveSpeed.x) >= 0.001f || fabsf(m_SaveMoveSpeed.z) >= 0.001f)
		{
			m_rotation.SetRotationYFromDirectionXZ(m_SaveMoveSpeed);
		}
		return true;
	}

	return false;
}


bool Lich::Attack()
{
	//アタック１の攻撃範囲にターゲットがいたら
	if (IsFindPlayer(m_Attack_1Distance) == true)
	{
		//インターバルがないなら攻撃可能
		if (m_attackFlag == false)
		{
			SetNextAnimationState(enAnimationState_Attack_1);
			m_attackFlag = true;
			//攻撃範囲にターゲットがいる
			m_attackRangeFlag = true;

			return true;
		}
	}
	else
	{
		//攻撃範囲にターゲットがいない
		m_attackRangeFlag = false;
	}

	return false;
}

bool Lich::Attack2()
{
	//アタック１の攻撃範囲にターゲットがいたら
	if (IsFindPlayer(m_Attack_2Distance) == true)
	{
		//インターバルがないなら攻撃可能
		if (m_attackFlag == false)
		{
			SetNextAnimationState(enAnimationState_Attack_2);
			m_attackFlag = true;
			//攻撃範囲にターゲットがいる
			m_attackRangeFlag = true;

			return true;
		}
	}
	else
	{
		//攻撃範囲にターゲットがいない
		m_attackRangeFlag = false;
	}

	return false;
}

void Lich::DecideNextAction()
{
	Move();

	if (m_attackFlag == false)
	{
		//範囲が狭い順
		if (Attack2() == true)
		{
			return;
		}

		if (Attack() == true)
		{
			return;
		}
	}
}

bool Lich::AttackInterval()
{
	//攻撃したら
	if (m_attackFlag == true)
	{
		//タイマーがインターバルを超えたら
		if (m_attackIntervalTime < m_attackIntervalTimer)
		{
			//攻撃可能にする
			m_attackFlag = false;
			//タイマーをリセット
			m_attackIntervalTimer = 0.0f;
		}
		else
		{
			m_attackIntervalTimer += g_gameTime->GetFrameDeltaTime();
			//攻撃不可能
			return false;

		}
	}
	//攻撃可能
	return true;
}


void Lich::PlayAnimation()
{
	m_state->ManageState();
}

void Lich::ManageState()
{
	m_state->PlayAnimation();
}

void Lich::SetNextAnimationState(EnAnimationState nextState)
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
	case Lich::enAninationState_Idle:
		// 待機ステートを作成する。
		m_state = new LichStateIdle(this);
		break;
	case Lich::enAninationState_Walk:
		// 待機ステートを作成する。
		m_state = new LichStateWalk(this);
		break;
	case Lich::enAninationState_Run:
		break;
	case Lich::enAnimationState_Attack_1:
		// アタック１ステートを作成する。
		m_state = new LichStateAttack_1(this);
		break;
	case Lich::enAnimationState_Attack_2:
		// アタック２ステートを作成する。
		m_state = new LichStateAttack_2(this);
		break;
	case Lich::enAnimationState_Attack_3_start:
		break;
	case Lich::enAnimationState_Attack_3_main:
		break;
	case Lich::enAnimationState_Attack_4:
		break;
	case Lich::enAnimationState_Die:
		//Dieステートを作成する
		m_state = new LichStateDie(this);
		break;
	default:
		// ここに来たらステートのインスタンス作成処理の追加忘れ。
		std::abort();
		break;
		break;
	}


		
	
}

void Lich::ProcessCommonStateTransition()
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

void Lich::OnProcessAttack_1StateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//攻撃パターンをなし状態にする
		//m_enAttackPatternState = enAttackPattern_None;
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Lich::OnProcessAttack_2StateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//攻撃パターンをなし状態にする
		//m_enAttackPatternState = enAttackPattern_None;
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Lich::OnProcessDieStateTransition()
{

}

void Lich::DamageCollision()
{
	//敵の攻撃用のコリジョンを取得する名前一緒にする
	const auto& Attack_1Collisions = g_collisionObjectManager->FindCollisionObjects("Attack");
	//コリジョンの配列をfor文で回す
	for (auto collision : Attack_1Collisions)
	{
		//自身のキャラコンと衝突したら
		if (collision->IsHit(m_charaCon) == true)
		{
			//１コンボの間に1回だけ判定
			//ダメージを受けた時のコンボステートと現在のコンボステートが違うなら
			if (m_player->IsComboStateSame()==true)
			{
				Damage();

				//ダメージを受けた時のコンボステートに現在のコンボステートを代入する
				m_player->SetDamagedComboState(m_player->GetNowComboState());
			}
			else
			{

			}

			
			
		}

		
	}

	//敵の攻撃用のコリジョンを取得する名前一緒にする
	const auto& SkillCollisions = g_collisionObjectManager->FindCollisionObjects("SkillAttack");
	//コリジョンの配列をfor文で回す
	for (auto collision : SkillCollisions)
	{
		//自身のキャラコンと衝突したら
		if (collision->IsHit(m_charaCon) == true)
		{
			
			Damage();
		}
	}

}

void Lich::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}