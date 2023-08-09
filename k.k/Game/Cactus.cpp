#include "stdafx.h"
#include "Cactus.h"

namespace {
	const float ANGLE = 140.0f;				//視野角

	//ステータス
	int MAXHP = 170;
	int MAXMP = 500;
	int ATK = 10;
	float SPEED = 100.0f;
	const char* NAME = "Cactus";
}

Cactus::Cactus()
{
	m_angle = ANGLE;
}

Cactus::~Cactus()
{
}

bool Cactus::Start()
{
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

	//4から5の範囲のインターバル
	m_angleChangeTime = rand() % 2 + 4;

	return true;
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

void Cactus::InitModel()
{
	m_modelRender.Init("Assets/modelData/character/Cactus/Cactus.tkm");


	m_charaCon.Init(
		20.0f,
		5.0f,
		m_position
	);

	//座標の設定
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void Cactus::Update()
{
	//プレイヤーかボスがやられたら消える

	AngleChangeTimeIntarval(m_angleChangeTime);

	Move();
	Rotation();


	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void Cactus::Move()
{
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

Vector3 Cactus::SetDirection()
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

bool Cactus::IsBumpedForest()
{
	Vector3 pos1 = m_position;
	Vector3 pos2 = m_position;
	pos1.Normalize();
	pos2 += pos1 * 20.0f;

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

void Cactus::Damage(int attack)
{
}

bool Cactus::RotationOnly()
{
	return false;
}

void Cactus::ManageState()
{
}

void Cactus::PlayAnimation()
{

}

void Cactus::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}