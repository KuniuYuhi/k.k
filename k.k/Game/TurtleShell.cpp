#include "stdafx.h"
#include "TurtleShell.h"
#include "ITurtleShellState.h"
#include "TurtleShellStateIdle.h"
#include "TurtleShellStateWalk.h"
#include "TurtleShellStateAttack_1.h"
#include "TurtleShellStateAttack_2.h"
#include "TurtleShellStateDifence.h"
#include "TurtleShellStateDifenceDamage.h"
#include "TurtleShellStateDamage.h"
#include "TurtleShellStateDie.h"
#include "TurtleShellStateVictory.h"

namespace {
	const float ANGLE = 45.0f;				//視野角

	//ステータス
	int MAXHP = 200;
	int MAXMP = 500;
	int ATK = 15;
	float SPEED = 70.0f;
	const char* NAME = "TurtleShell";
}

TurtleShell::TurtleShell()
{
	m_angle = ANGLE;
}

TurtleShell::~TurtleShell()
{
}

//衝突したときに呼ばれる関数オブジェクト(壁用)
struct IsForestResult :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//衝突フラグ。
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//地面とぶつかってなかったら。
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_forest) {
			//衝突したのは壁ではない。
			isHit = false;
			return 0.0f;
		}
		else
		{
			//地面とぶつかったら。
		//フラグをtrueに。
			isHit = true;
			return 0.0f;
		}

	}
};

bool TurtleShell::Start()
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



	//　乱数を初期化。
	srand((unsigned)time(NULL));

	//6から8の範囲のインターバル
	m_angleChangeTime = rand() % 3 + 6;
	return true;
}

void TurtleShell::InitModel()
{
	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/TurtleShell/Idle_Normal.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Walk].Load("Assets/animData/character/TurtleShell/Walk.tka");
	m_animationClip[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClip[enAnimClip_Walk].Load("Assets/animData/character/TurtleShell/Run.tka");
	m_animationClip[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClip[enAnimClip_Attack_1].Load("Assets/animData/character/TurtleShell/Attack1.tka");
	m_animationClip[enAnimClip_Attack_1].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_2].Load("Assets/animData/character/TurtleShell/Attack2.tka");
	m_animationClip[enAnimClip_Attack_2].SetLoopFlag(false);
	m_animationClip[enAnimClip_Difence].Load("Assets/animData/character/TurtleShell/Defence.tka");
	m_animationClip[enAnimClip_Difence].SetLoopFlag(false);
	m_animationClip[enAnimClip_DefenceDamage].Load("Assets/animData/character/TurtleShell/DefenceDamage.tka");
	m_animationClip[enAnimClip_DefenceDamage].SetLoopFlag(false);
	m_animationClip[enAnimClip_Damage].Load("Assets/animData/character/TurtleShell/Damage.tka");
	m_animationClip[enAnimClip_Damage].SetLoopFlag(false);
	m_animationClip[enAnimClip_Die].Load("Assets/animData/character/TurtleShell/Die.tka");
	m_animationClip[enAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enAnimClip_Victory].Load("Assets/animData/character/TurtleShell/Victory.tka");
	m_animationClip[enAnimClip_Victory].SetLoopFlag(false);

	m_modelRender.Init(
		"Assets/modelData/character/TurtleShell/TurtleShell.tkm",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);


	m_charaCon.Init(
		22.0f,
		4.0f,
		m_position
	);

	//座標の設定
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void TurtleShell::Update()
{
	//プレイヤーかボスがやられたら消える


	DamageCollision(m_charaCon);

	AngleChangeTimeIntarval(m_angleChangeTime);

	Move();
	Rotation();

	ManageState();
	PlayAnimation();

	m_oldPosition = m_position;

	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void TurtleShell::Move()
{
	//特定のアニメーションが再生中なら抜け出す
	if (isAnimationEntable() != true)
	{
		return;
	}

	//視界にターゲットを見つけたら
	if (IsFindPlayer(m_distanceToPlayer) == true)
	{
		Vector3 toPlayerDir = m_toTarget;
		toPlayerDir.Normalize();
		Vector3 a = m_position;
		a.Normalize();
		//ターゲットに向かうベクトルと前方向の内積を計算する
		float t = toPlayerDir.Dot(m_forward);
		//内積の結果をacos関数に渡して、m_enemyFowradとtoPlayerDirのなす角度を求める。
		float angle = acos(t);

		//視野角判定
		if (fabsf(angle) < Math::DegToRad(m_angle))
		{
			//追いかける
			m_direction = toPlayerDir;
			m_moveSpeed = m_direction * m_status.defaultSpeed;
			//m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);

			//次の座標が決まったので抜け出す
		}

		m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	}
	else
	{
		//数秒間隔で向かうベクトルを変える
		if (m_angleChangeTimeFlag == false)
		{
			m_direction = SetDirection();
			m_angleChangeTimeFlag = true;
		}
		//ランダムな方向に移動
		m_moveSpeed = m_direction * m_status.defaultSpeed;
		m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	}
	
	//壁にぶつかったら反転
	if (IsBumpedForest() == true)
	{
		m_direction *= -1.0f;
		m_moveSpeed = m_direction * m_status.defaultSpeed;
		m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
		return;
	}

}

void TurtleShell::DecideNextAction()
{
}

Vector3 TurtleShell::SetDirection()
{

	Vector3 randomPos = g_vec3Zero;
	randomPos.y = 0.0f;
	float X = (rand() % (2 - (-2) + 1)) + (-2);
	float Z = (rand() % (2 - (-2) + 1)) + (-2);
	randomPos.x += X;
	randomPos.z += Z;
	randomPos.Normalize();

	return randomPos;
}

bool TurtleShell::IsBumpedForest()
{
	Vector3 pos1 = m_position;
	Vector3 pos2 = m_position;
	pos1.Normalize();
	pos2 /**=10.0f*/+= pos1 * 30.0f;

	SphereCollider m_sphereCollider;
	m_sphereCollider.Create(1.0f);

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//始点はエネミーの座標。
	start.setOrigin(btVector3(m_position.x, m_position.y, m_position.z));
	//終点はプレイヤーの座標。
	end.setOrigin(btVector3(
		pos2.x, pos2.y, pos2.z));

	//壁の判定を返す
	IsForestResult callback_Forest;
	//コライダーを始点から終点まで動かして。
	//壁と衝突するかどうかを調べる。
	PhysicsWorld::GetInstance()->ConvexSweepTest(
		(const btConvexShape*)m_sphereCollider.GetBody(),
		start, end, callback_Forest);
	//森に衝突した
	if (callback_Forest.isHit == true)
	{
		return true;
	}
	else
	{
		//衝突しなかった
		return false;
	}
}

void TurtleShell::Damage(int attack)
{
	//HPを減らす
	m_status.hp -= attack;

	//HPが0以下なら
	if (m_status.hp <= 0)
	{
		m_status.hp = 0;
		SetNextAnimationState(enAnimationState_Die);
		return;
	}

	//もし防御中なら

	SetNextAnimationState(enAnimationState_Damage);
	
}

void TurtleShell::HitFireBall()
{
}

void TurtleShell::HitFlamePillar()
{
}

bool TurtleShell::RotationOnly()
{
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
	case TurtleShell::enAninationState_Idle:
		m_state = new TurtleShellStateIdle(this);
		break;
	case TurtleShell::enAninationState_Walk:
		m_state = new TurtleShellStateWalk(this);
		break;
	case TurtleShell::enAninationState_Run:
		m_state = new TurtleShellStateWalk(this);
		break;
	case TurtleShell::enAnimationState_Attack_1:
		m_state = new TurtleShellStateAttack_1(this);
		break;
	case TurtleShell::enAnimationState_Attack_2:
		m_state = new TurtleShellStateAttack_2(this);
		break;
	case TurtleShell::enAnimationState_Difence:
		m_state = new TurtleShellStateDifence(this);
		break;
	case TurtleShell::enAnimationState_DifencDamage:
		m_state = new TurtleShellStateDifenceDamage(this);
		break;
	case TurtleShell::enAnimationState_Damage:
		m_state = new TurtleShellStateDamage(this);
		break;
	case TurtleShell::enAnimationState_Die:
		m_state = new TurtleShellStateDie(this);
		break;
	case TurtleShell::enAnimationState_Victory:
		m_state = new TurtleShellStateVictory(this);
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
		SetNextAnimationState(enAninationState_Walk);
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
		//共通のステート遷移処理実行
		ProcessCommonStateTransition();
	}
}

void TurtleShell::OnProcessAttack_2StateTransition()
{
	//アニメーションが終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//共通のステート遷移処理実行
		ProcessCommonStateTransition();
	}
}

void TurtleShell::OnProcessDifenceStateTransition()
{
	//アニメーションが終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//共通のステート遷移処理実行
		ProcessCommonStateTransition();
	}
}

void TurtleShell::OnProcessDifenceDamageStateTransition()
{
	//アニメーションが終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//共通のステート遷移処理実行
		ProcessCommonStateTransition();
	}
}

void TurtleShell::OnProcessDamageStateTransition()
{
	//アニメーションが終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//共通のステート遷移処理実行
		ProcessCommonStateTransition();
	}
}

void TurtleShell::OnProcessDieStateTransition()
{
	//アニメーションが終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
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

void TurtleShell::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
