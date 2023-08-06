#include "stdafx.h"
#include "DarkMeteorite.h"
#include "Meteo.h"
#include "Player.h"

DarkMeteorite::DarkMeteorite()
{
}

DarkMeteorite::~DarkMeteorite()
{
}

//衝突したときに呼ばれる関数オブジェクト(壁用)
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

bool DarkMeteorite::Start()
{
	m_model.Init("Assets/modelData/character/Slime/slime.tkm");


	m_model.SetTransform(m_position, m_rotation, g_vec3One);
	m_model.Update();

	m_player = FindGO<Player>("player");

	return true;
}

void DarkMeteorite::Update()
{
	//一定のサイズになったら処理をしない
	SizeUp();
	
	Shot();


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
	//ショットフラグが立ったら
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
			if (IsHitGround(HitPosition) == true)
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
		m_shotEndFlag = true;
	}
	
}

Vector3 DarkMeteorite::SetMeteoTargetPosition()
{
	//ターゲットを決めてメテオを生成
	//現在のフレームのターゲットの座標を取得
	m_targetPosition = m_player->GetPosition();

	Vector3 createPositon = m_targetPosition;
	float X = rand() % 601 - 300;
	float Z = rand() % 601 - 300;
	createPositon.x += X;
	createPositon.z += Z;

	return createPositon;
}

void DarkMeteorite::CreateMeteo(Vector3 targetposition)
{
	Meteo* meteo = NewGO<Meteo>(0, "meteo");
	meteo->SetPosition(m_position);
	meteo->SetRotation(m_rotation);
	meteo->SetTargetPosition(targetposition);
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

bool DarkMeteorite::IsHitGround(Vector3 targetposition)
{
	BoxCollider m_boxCollider; 
	m_boxCollider.Create(Vector3(5.0f, 5.0f, 5.0f));

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//始点はエネミーの座標。
	start.setOrigin(btVector3(targetposition.x, targetposition.y + 20.0f, targetposition.z));
	//終点はプレイヤーの座標。
	end.setOrigin(btVector3(targetposition.x, targetposition.y - 20.0f, targetposition.z));

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

void DarkMeteorite::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}
