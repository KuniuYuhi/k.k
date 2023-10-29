#include "stdafx.h"
#include "DarkMeteorite.h"
#include "Meteo.h"
#include "Player.h"
#include "Lich.h"
#include "InitEffect.h"

namespace {
	const int MAX_LENGTH = 10;
	const int MIN_LENGTH = -10;

	const float BIGMETEO_SPEED = 200.0f;

	const float Y_DOWN = -200.0f;

	const float GROUND_CHECK_Y_UP = 20.0f;
	const float GROUND_CHECK_Y_DOWN = -20.0f;

	const float BIGMETEO_GROUND_CHECK_Y_UP = -100.0f;
	const float BIGMETEO_GROUND_CHECK_Y_DOWN = -130.0f;

	const float WIND_SCALE = 30.0f;

	const float DARKMETEO_SHOT_TIMER = 5.0f;

	const float NEAR_LENGTH = 150.0f;
}

DarkMeteorite::DarkMeteorite()
{

	
}

DarkMeteorite::~DarkMeteorite()
{
	DeleteGO(m_collision);

	if (m_darkMeteoriteEffect != nullptr)
	{
		m_darkMeteoriteEffect->Stop();
	}
	if (m_windEffect != nullptr)
	{
		m_windEffect->Stop();
	}

	if (m_gameEndFlag == true)
	{
		if (m_meteos.size() != 0)
		{
			QueryGOs<Meteo>("meteo", [&](Meteo* meteo_) {
				DeleteGO(meteo_);
				return true;
				});
		}
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
	m_model.Init(
		"Assets/modelData/character/Lich/Effect/Meteo.tkm", 
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		nullptr, 
		0, enModelUpAxisZ, 
		false, 
		false 
		);

	m_model.SetTransform(m_position, m_rotation, g_vec3One);
	m_model.Update();

	m_player = FindGO<Player>("player");

	//ビッグメテオの当たり判定生成
	CreateCollision();

	m_darkMeteoriteEffect = NewGO<EffectEmitter>(0);
	m_darkMeteoriteEffect->Init(InitEffect::enEffect_DarkMeteorite);
	m_darkMeteoriteEffect->Play();
	m_darkMeteoriteEffect->SetPosition(m_position);
	

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
	Vector3 effectScale = m_scale * 1.8f;
	m_darkMeteoriteEffect->SetScale(effectScale);
	m_darkMeteoriteEffect->Update();

	//一定のサイズになったら処理をしない
	if (m_maxScale.x <= m_scale.x)
	{
		m_sizeUpFlag = true;
	}
}

void DarkMeteorite::ProcessShot()
{
	//ショットフラグが立っていないなら処理しない
	if (m_ShotStartFlag != true)
	{
		return;
	}

	//タイマーが生成時間に達していないなら抜け出す
	if (CreateMeteoTimer() != true)
	{
		return;
	}

	//生成する数
	if (m_meteoCounter < m_createMeteoCount)
	{
		while (true)
		{
			if (ShotMeteo() == true)
			{
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
			
		}
		else
		{
			//それ以外ならダークメテオアクションの終わり
			m_shotEndFlag = true;
			//m_enShotState = enShotState_End;

		}
	}
}

bool DarkMeteorite::ShotMeteo()
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

		//もし最後のメテオなら
		if (m_meteoCounter >= m_createMeteoCount && m_lastBigMeteoShotFlag == true)
		{
			//ダークメテオを落とす時のために次のタイマーを長くする
			m_createTime = DARKMETEO_SHOT_TIMER;
		}
		return true;
	}
	return false;
}

Vector3 DarkMeteorite::SetMeteoTargetPosition()
{
	//　乱数を初期化。
	srand((unsigned)time(NULL));

	//ターゲットを決めてメテオを生成
	//現在のフレームのターゲットの座標を取得
	m_targetPosition = m_player->GetPosition();
	/*Vector3 forward = m_player->GetForward();
	forward.Normalize();
	forward *= 100.0f;
	*/
	Vector3 createPositon = m_targetPosition;
	float X = (rand() % (MAX_LENGTH - (MIN_LENGTH) + 1)) + (MIN_LENGTH);
	float Z = (rand() % (MAX_LENGTH - (MIN_LENGTH) + 1)) + (MIN_LENGTH);
	createPositon.x += X;
	createPositon.z += Z;

	return createPositon;
}

void DarkMeteorite::CreateCollision()
{
	//本体の当たり判定の生成
	m_collision = NewGO<CollisionObject>(0, "bigmeteo");
	m_collision->CreateSphere(
		m_position,
		g_quatIdentity,
		120.0f
	);
	m_collision->SetIsEnableAutoDelete(false);
	m_collision->SetPosition(m_position);
	m_collision->Update();
}

void DarkMeteorite::CreateMeteo(Vector3 targetposition)
{
	//メテオの生成とリストに格納
	Vector3 createPos = m_position;
	createPos.y += 70.0f;
	Meteo* meteo = NewGO<Meteo>(0, "meteo");
	meteo->SetPosition(createPos);
	meteo->SetRotation(m_rotation);
	meteo->SetTargetPosition(targetposition);
	//リストに追加。
	m_meteos.emplace_back(meteo);
	//メテオを撃つ音再生
	g_soundManager->InitAndPlaySoundSource(enSoundName_Boss_Meteo_Shot, g_soundManager->GetSEVolume());
}

bool DarkMeteorite::CreateMeteoTimer()
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

bool DarkMeteorite::IsHitWall()
{
	Vector3 addPos = m_moveSpeed;
	addPos.Normalize();
	Vector3 pos2 = m_position;
	pos2.Add(addPos * 3.0f);

	BoxCollider m_boxCollider;
	m_boxCollider.Create(Vector3(1.0f, 1.0f, 1.0f));

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//始点はエネミーの座標。
	start.setOrigin(btVector3(m_position.x, 0.0f, m_position.z));
	//終点はプレイヤーの座標。
	end.setOrigin(btVector3(pos2.x, 0.0f, pos2.z));

	//壁の判定を返す
	IsWallResult isWallResult;
	//コライダーを始点から終点まで動かして。
	//壁と衝突するかどうかを調べる。
	PhysicsWorld::GetInstance()->ConvexSweepTest(
		(const btConvexShape*)m_boxCollider.GetBody(),
		start, end, isWallResult);
	//壁に衝突した
	if (isWallResult.isHit == true)
	{
		return true;
	}
	else
	{
		//壁ではなかった
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
		ProcessShot();
		break;
	case DarkMeteorite::enShotState_BigMeteo:
		DarkMeteoMoveManageState();
		break;
	default:
		DeleteGO(this);
		break;
	}
}

void DarkMeteorite::DarkMeteoMoveManageState()
{
	switch (m_darkMeteoMoveState)
	{
	case DarkMeteorite::enDarkMeteoMoveState_fall:
		OnProcessFallStateTransition();
		break;
	case DarkMeteorite::enDarkMeteoMoveState_Chase:
		OnProcessChaseStateTransition();
		break;
	case DarkMeteorite::enDarkMeteoMoveState_straight:
		OnProcessStraightStateTransition();
		break;
	default:
		break;
	}
}

void DarkMeteorite::OnProcessFallStateTransition()
{
	//ターゲットの向かうベクトルを計算
	SetToTargetDirection();
	//正規化
	m_moveSpeed.Normalize();
	//回転
	m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
	//速度をかける
	m_moveSpeed *= BIGMETEO_SPEED;
	//地面についていないなら
	if (IsHitGround(m_position, BIGMETEO_GROUND_CHECK_Y_UP, BIGMETEO_GROUND_CHECK_Y_DOWN) != true)
	{
		//地面につくまでの処理
		//斜め下に移動
		m_moveSpeed.y = Y_DOWN;
	}
	else
	{
		//地面についた
		//全ての玉を撃ち終わった
		//m_shotEndFlag = true;
		m_moveSpeed.y = 0.0f;
		//風エフェクト生成
		m_windEffect = NewGO<EffectEmitter>(0);
		m_windEffect->Init(InitEffect::enEffect_DarkMeteorite_Wind);
		m_windEffect->Play();
		m_windEffect->SetPosition(m_position);
		m_windEffect->SetScale(g_vec3One * WIND_SCALE);
		m_windEffect->Update();

		//次のステートに遷移
		m_darkMeteoMoveState = enDarkMeteoMoveState_Chase;
	}

	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime() * 1.5f;
	m_model.SetPosition(m_position);
	m_collision->SetPosition(m_position);
	m_collision->Update();
	m_darkMeteoriteEffect->SetPosition(m_position);
	m_darkMeteoriteEffect->Update();
}

void DarkMeteorite::OnProcessChaseStateTransition()
{
	//ターゲットの向かうベクトルを計算
	SetToTargetDirection();
	//正規化
	m_moveSpeed.Normalize();
	//回転
	m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
	//速度をかける
	m_moveSpeed *= BIGMETEO_SPEED;
	//壁との衝突判定
	//todo たまに消えないので制限時間を求める
	m_moveSpeed.y = 0.0f;
	//壁にぶつかったか判定
	if (IsHitWall() == true)
	{
		//ぶつかったので爆発
		//処理終わり
		m_shotEndFlag = true;
		//ダークメテオの移動ステートの処理をしないようにする
		m_darkMeteoMoveState = enDarkMeteoMoveState_None;
		//ショットステート
		m_enShotState = enShotState_End;
		return;
	}

	//プレイヤーとの距離が近かったら処理をやめる
	if (IsNearDistanceToPlayer() == true)
	{
		//次のステートに遷移
		m_darkMeteoMoveState = enDarkMeteoMoveState_straight;
		return;
	}

	SetTRS();
}

void DarkMeteorite::OnProcessStraightStateTransition()
{
	//壁との衝突判定
	//todo たまに消えないので制限時間を求める
	m_moveSpeed.y = 0.0f;
	//壁にぶつかったか判定
	if (IsHitWall() == true)
	{
		//ぶつかったので爆発
		//処理終わり
		m_shotEndFlag = true;
		//ダークメテオの移動ステートの処理をしないようにする
		m_darkMeteoMoveState = enDarkMeteoMoveState_None;
		//ショットステート
		m_enShotState = enShotState_End;
		return;
	}

	SetTRS();
}

void DarkMeteorite::SetToTargetDirection()
{
	//ターゲットの向かうベクトルを計算
	m_targetPosition = m_player->GetPosition();
	m_moveSpeed = m_targetPosition - m_position;
}

bool DarkMeteorite::IsNearDistanceToPlayer()
{
	//プレイヤーに当たったかの判定
	Vector3 diff = m_position - m_player->GetPosition();
	if (diff.Length() < NEAR_LENGTH)
	{
		return true;
	}
	return false;
}

void DarkMeteorite::SetTRS()
{
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime() * 1.5f;
	m_model.SetPosition(m_position);
	m_model.SetRotation(m_rotation);
	m_collision->SetPosition(m_position);
	m_collision->Update();
	m_darkMeteoriteEffect->SetPosition(m_position);
	m_darkMeteoriteEffect->Update();

	if (m_windEffect != nullptr)
	{
		Vector3 pos = m_position;
		pos.y = 0.0f;
		m_windEffect->SetPosition(pos);
		m_windEffect->SetRotation(m_rotation);
		m_windEffect->Update();
	}
}

void DarkMeteorite::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}
