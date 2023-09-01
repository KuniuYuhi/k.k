#include "stdafx.h"
#include "DarkMeteorite.h"
#include "Meteo.h"
#include "Player.h"
#include "Lich.h"

namespace {
	const int MAX_LENGTH = 50;
	const int MIN_LENGTH = -50;

	const float BIGMETEO_SPEED = 200.0f;

	const float Y_DOWN = -150.0f;

	const float GROUND_CHECK_Y_UP = 20.0f;
	const float GROUND_CHECK_Y_DOWN = -20.0f;

	const float BIGMETEO_GROUND_CHECK_Y_UP = 90.0f;
	const float BIGMETEO_GROUND_CHECK_Y_DOWN = 120.0f;
}

DarkMeteorite::DarkMeteorite()
{
}

DarkMeteorite::~DarkMeteorite()
{
	DeleteGO(m_collision);

	if (m_meteos.size() != 0)
	{
		QueryGOs<Meteo>("meteo", [&](Meteo* meteo_) {
			DeleteGO(meteo_);
			return true;
			});
	}
}

//衝突したときに呼ばれる関数オブジェクト(地面用)
struct IsGroundResult :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//衝突フラグ。
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//地面とぶつかってなかったら。
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Ground) {
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

//衝突したときに呼ばれる関数オブジェクト(壁用)
struct IsWallResult :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//衝突フラグ。
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//地面とぶつかってなかったら。
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall) {
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


bool DarkMeteorite::Start()
{
	m_model.Init("Assets/modelData/character/Lich/Effect/DarkBall.tkm", nullptr, 0, enModelUpAxisZ, false, false, false);


	m_model.SetTransform(m_position, m_rotation, g_vec3One);
	m_model.Update();

	m_player = FindGO<Player>("player");

	return true;
}

void DarkMeteorite::Update()
{

	ShotManageState();

	


	m_model.Update();
}

void DarkMeteorite::SizeUp()
{
	//一定のサイズになったら処理をしない
	if (m_sizeUpFlag != false)
	{
		return;
	}

	m_scale *= 1.0f + g_gameTime->GetFrameDeltaTime() * 1.5f;
	m_model.SetScale(m_scale);
	//一定のサイズになったら処理をしない
	if (m_maxScale.x <= m_scale.x)
	{
		m_sizeUpFlag = true;
	}
}

void DarkMeteorite::Shot()
{
	//ショットフラグが立っていないなら処理しない
	if (m_ShotStartFlag != true)
	{
		return;
	}

	//タイマーが生成時間に達していないなら抜け出す
	if (CreateTimer() != true)
	{
		return;
	}

	//生成する数
	if (m_meteoCounter < m_createMeteoCount)
	{
		//
		while (true)
		{
			//メテオが衝突する座標
			Vector3 HitPosition = g_vec3Zero;
			HitPosition = SetMeteoTargetPosition();
			//衝突する座標が地面なら
			if (IsHitGround(HitPosition, GROUND_CHECK_Y_UP, GROUND_CHECK_Y_DOWN) == true)
			{
				//メテオを生成する
				CreateMeteo(HitPosition);
				//カウンターを増やす
				m_meteoCounter++;
				break;
			}
		}	
	}
	//全てのメテオを生成したら
	else
	{
		//最後に大きなメテオを撃つフラグが立っていたら
		if (m_lastBigMeteoShotFlag == true)
		{
			//ビッグメテオを撃つときのターゲットを決める
			m_targetPosition = m_player->GetPosition();
			m_moveSpeed = m_targetPosition - m_position;
			//正規化
			m_moveSpeed.Normalize();
			m_moveSpeed *= BIGMETEO_SPEED;
			//ステートを変える
			m_enShotState = enShotState_BigMeteo;
			//ビッグメテオの当たり判定生成
			CreateCollision();
		}
		else
		{
			//それ以外ならダークメテオ終わり
			m_shotEndFlag = true;

		}
	}


	
}

Vector3 DarkMeteorite::SetMeteoTargetPosition()
{
	//　乱数を初期化。
	srand((unsigned)time(NULL));

	//ターゲットを決めてメテオを生成
	//現在のフレームのターゲットの座標を取得
	m_targetPosition = m_player->GetPosition();

	Vector3 createPositon = m_targetPosition;
	/*float X = rand() % 601 - 300;
	float Z = rand() % 601 - 300;*/
	float X = (rand() % (MAX_LENGTH - (MIN_LENGTH) + 1)) + (MIN_LENGTH);
	float Z = (rand() % (MAX_LENGTH - (MIN_LENGTH) + 1)) + (MIN_LENGTH);
	createPositon.x += X;
	createPositon.z += Z;

	return createPositon;
}

void DarkMeteorite::CreateCollision()
{
	m_collision = NewGO<CollisionObject>(0, "bigmeteo");
	m_collision->CreateSphere(
		m_position,
		g_quatIdentity,
		100.0f
	);
	m_collision->SetIsEnableAutoDelete(false);
	m_collision->SetPosition(m_position);
	m_collision->Update();
}

void DarkMeteorite::CreateMeteo(Vector3 targetposition)
{
	Vector3 createPos = m_position;
	createPos.y += 70.0f;
	Meteo* meteo = NewGO<Meteo>(0, "meteo");
	meteo->SetPosition(createPos);
	meteo->SetRotation(m_rotation);
	meteo->SetTargetPosition(targetposition);
	//リストに追加。
	m_meteos.emplace_back(meteo);
}

bool DarkMeteorite::CreateTimer()
{
	if (m_createTime < m_createTimer)
	{
		m_createTimer = 0.0f;
		return true;
	}
	else
	{
		m_createTimer += g_gameTime->GetFrameDeltaTime();
		return false;
	}
}

bool DarkMeteorite::IsHitGround(Vector3 targetposition,float up, float down)
{
	BoxCollider m_boxCollider; 
	m_boxCollider.Create(Vector3(5.0f, 5.0f, 5.0f));
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//始点はエネミーの座標。
	start.setOrigin(btVector3(targetposition.x, targetposition.y + up, targetposition.z));
	//終点はプレイヤーの座標。
	end.setOrigin(btVector3(targetposition.x, targetposition.y + down, targetposition.z));
	//壁の判定を返す
	IsGroundResult callback_Ground;
	//コライダーを始点から終点まで動かして。
	//壁と衝突するかどうかを調べる。
	PhysicsWorld::GetInstance()->ConvexSweepTest(
		(const btConvexShape*)m_boxCollider.GetBody(), 
		start, end, callback_Ground);
	//地面に衝突した
	if (callback_Ground.isHit == true)
	{
		return true;
	}
	else
	{
		//地面ではなかった
		return false;
	}
}

bool DarkMeteorite::IsHitWall(Vector3 pos1, Vector3 pos2)
{
	BoxCollider m_boxCollider;
	m_boxCollider.Create(Vector3(10.0f, 10.0f, 10.0f));

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//始点はエネミーの座標。
	start.setOrigin(btVector3(pos1.x, pos1.y, pos1.z));
	//終点はプレイヤーの座標。
	end.setOrigin(btVector3(pos2.x, pos2.y, pos2.z));

	//壁の判定を返す
	IsGroundResult callback_Ground;
	//コライダーを始点から終点まで動かして。
	//壁と衝突するかどうかを調べる。
	PhysicsWorld::GetInstance()->ConvexSweepTest(
		(const btConvexShape*)m_boxCollider.GetBody(),
		start, end, callback_Ground);
	//地面に衝突した
	if (callback_Ground.isHit == true)
	{
		return true;
	}
	else
	{
		//地面ではなかった
		return false;
	}
}

void DarkMeteorite::ShotManageState()
{
	switch (m_enShotState)
	{
	case DarkMeteorite::enShotState_Meteo:
		//一定のサイズになったら処理をしない
		SizeUp();
		//メテオを撃つ処理
		Shot();
		break;
	case DarkMeteorite::enShotState_BigMeteo:

		move();
		break;
	default:
		break;
	}
}

void DarkMeteorite::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}

void DarkMeteorite::move()
{
	if (m_bigMeteoMoveCount > 0)
	{
		m_targetPosition = m_player->GetPosition();
		m_moveSpeed = m_targetPosition - m_position;
		//正規化
		m_moveSpeed.Normalize();
		m_moveSpeed *= BIGMETEO_SPEED;
		//カウントを減らす
		m_bigMeteoMoveCount-=g_gameTime->GetFrameDeltaTime();
	}
	

	if (m_isBigMeteoYDownFlag == false)
	{
		//地面についていない間斜め下に移動
		if (IsHitGround(m_position, BIGMETEO_GROUND_CHECK_Y_UP, BIGMETEO_GROUND_CHECK_Y_DOWN) != true)
		{
			m_moveSpeed.y = Y_DOWN;
		}
		else
		{
			m_isBigMeteoYDownFlag = true;
			m_moveSpeed.y = 0.0f;
		}
	}
	else
	{
		Vector3 addPos = m_moveSpeed;
		addPos.Normalize();
		Vector3 pos2 = m_position + (addPos * 0.65f);
		//壁との衝突判定
		if (IsHitWall(m_position, pos2) == true)
		{
			//爆発
			//処理終わり
			m_shotEndFlag = true;
			m_enShotState = enShotState_End;
		}
	}
	
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime()*1.5f;

	m_model.SetPosition(m_position);

	m_collision->SetPosition(m_position);
	m_collision->Update();
}
