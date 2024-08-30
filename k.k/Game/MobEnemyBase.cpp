#include "stdafx.h"
#include "MobEnemyBase.h"

////////////////////////////////////////////////////
//コンポーネント
#include "MobEnemyMovement.h"
#include "DamageProvider.h"

/////////////////////////////////////////////////////
#include "DamageFont.h"

#include "KnockBackInfoManager.h"
#include "Brave.h"

float MobEnemyBase::CalcDistanceToTargetPosition(Vector3 target)
{
	Vector3 diff = target - m_position;
	return diff.Length();
}

void MobEnemyBase::SettingDefaultComponent()
{
	//モブエネミー移動コンポーネント
	AddComponent<MobEnemyMovement>();
	m_movement = GetComponent<MobEnemyMovement>();
	//ダメージプロバイダーコンポーネント
	AddComponent<DamageProvider>();
	m_damageProvider = GetComponent<DamageProvider>();
	//自身のインスタンスを保存
	m_damageProvider->SetProviderCharacterInstance(this);
}

void MobEnemyBase::TakeDamage(int damage)
{
	//ダメージを受ける
	//やられた場合は死亡フラグが立つ
	SetDieFlag(m_status.TakeDamage(damage));

}

void MobEnemyBase::DieFromDamage()
{
	//自身を返す
	ReleaseThis();

	//エフェクト生成

}

void MobEnemyBase::CheckSelfCollision()
{
	//被ダメージを受けない条件
	//if (adadadadada) return;



	//プレイヤーの攻撃コリジョンを取得
	const auto& Collisions =
		g_collisionObjectManager->FindCollisionObjects(
			g_collisionObjectManager->m_playerAttackCollisionName
		);



	//コリジョンの配列をfor文で回す
	for (auto collision : Collisions)
	{
		//当たり判定が有効でないなら飛ばす
		if (!collision->IsEnable()) continue;

		//自身のキャラコンと衝突したら
		if (collision->IsHit(*m_charaCon) == true)
		{
			//コリジョンを持っているキャラのダメージプロバイダーコンポーネントを取得
			DamageProvider* dp = FindGOComponent<DamageProvider>(collision->GetCreatorName());

			if (dp == nullptr) return;

			//攻撃がヒットした
			dp->Hit();

			//攻撃IDを取得
			int currentAttackId = dp->GetAttackId();
			
			//前回とIDが同じなら処理しない
			if (currentAttackId == oldAttackId) return;

			//ダメージをランダムに調整して、フォントとして表示
			CreateDamageFont(dp->GetAdjustedDamage(),DamageFont::enDamageActor_Monster);
		
			//被ダメージ処理を行う引数に情報を入れる
			ProcessHit(dp->GetProviderDamageInfo());

			//今回のIDを前フレームのIDに保存
			oldAttackId = currentAttackId;
		}
	}


}

void MobEnemyBase::TurnToTarget()
{
	if (m_player == nullptr) return;

	Vector3 direction = g_vec3Zero;
	//プレイヤーの方を向くベクトルを取得
	direction = m_movement->CalcChaseCharacterVerocity(
		m_status,
		m_player->GetPosition(),
		m_position,
		m_moveSpeed
	);

	//回転方向を設定
	SetRotateDirection(direction);
	//前方向を設定
	SetForward(direction);
}

void MobEnemyBase::ChaseMovement(Vector3 targetPosition)
{
	//アクション中は移動処理しない
	if (IsAction()) return;

	m_moveSpeed = m_movement->CalcChaseCharacterVerocity(
		m_status,
		targetPosition,
		m_position,
		m_moveSpeed
	);

	//移動量があれば前方向を設定
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		SetForward(m_moveSpeed);
	}
	
	//プレイヤーに向かう距離を計算
	float toPlayerDistance = CalcDistanceToTargetPosition(targetPosition);
	bool isExecute = true;

	Vector3 tempMoveSpeed = m_moveSpeed;
	//待機フラグがあるなら
	if (m_isWaitingFlag)
	{
		//プレイヤーとの距離より待機する距離が大きいならキャラコンの処理をしないようにする
		if (toPlayerDistance < m_status.GetWaitingDistance())
		{
			isExecute = false;
			m_moveSpeed = g_vec3Zero;
		}
	}
	else
	{
		//プレイヤーとの距離より接近する距離が大きいならキャラコンの処理をしないようにする
		if (toPlayerDistance < m_status.GetApproachDistance())
		{
			isExecute = false;
			m_moveSpeed = g_vec3Zero;
		}
	}
	

	//実行フラグがtrueなら
	if (m_charaCon != nullptr && isExecute)
	{
		//重力の計算
		m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();


		//キャラコンで座標を移動する
		m_position = m_charaCon.get()->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

		if (m_charaCon.get()->IsOnGround()) {
			//地面についた。
			m_moveSpeed.y = 0.0f;
		}

	}
	
	

	//回転方向を保存
	m_rotateDirection = tempMoveSpeed;
}

void MobEnemyBase::Rotation()
{
	if (fabsf(m_rotateDirection.x) < 0.001f
		&& fabsf(m_rotateDirection.z) < 0.001f) {
		//m_moveSpeed.xとm_moveSpeed.zの絶対値がともに0.001以下ということは
		//このフレームではキャラは移動していないので旋回する必要はない。
		return;
	}

	m_rotation.SetRotationYFromDirectionXZ(m_rotateDirection);

}

void MobEnemyBase::SettingKnockBackProcess()
{
	m_curvePointList.clear();
	//ノックバックカーブのポイントリストを取得
	m_curvePointList = KnockBackInfoManager::GetInstance()->GetCurvePoint(
		m_hitKnockBackPattern
	);

	//今回のパターンのノックバックのスピードを取得
	m_knockBackSpeed = KnockBackInfoManager::
		GetInstance()->GetStatus().GetKnockBackSpeed(m_hitKnockBackPattern);

	//前方向を正規化して方向を反転させる
	Vector3 diff = m_position - m_player->GetPosition();
	m_forward = diff;
	m_forward.Normalize();

	//硬直タイマーリセット
	m_starkTimer = 0.0f;
}

void MobEnemyBase::KnockBackMove(int listNum)
{
	//正規化
	m_curvePointList[listNum].curvePosition.Normalize();
	//移動速度を計算
	m_moveSpeed.x = m_forward.x * m_curvePointList[listNum].curvePosition.x * m_knockBackSpeed.x;
	m_moveSpeed.y = m_curvePointList[listNum].curvePosition.y * m_knockBackSpeed.y;
	m_moveSpeed.z = m_forward.z * m_curvePointList[listNum].curvePosition.z * m_knockBackSpeed.x;

	//キャラコンを使って移動
	m_position = m_charaCon.get()->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
}

void MobEnemyBase::KnockBackGravityFall()
{
	//重力の計算
	m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();

	//キャラコンを使って座標を移動
	m_position = m_charaCon.get()->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	if (m_charaCon.get()->IsOnGround()) {
		//地面についた。
		m_moveSpeed.y = 0.0f;
	}
}

bool MobEnemyBase::IsAttackable()
{
	//タイマーが攻撃インターバルを超えたら
	if (m_attackIntarvalTimer >= m_status.GetAttackIntarval())
	{
		//攻撃可能
		return true;
	}
	//タイマーを加算
	m_attackIntarvalTimer += g_gameTime->GetFrameDeltaTime();

	return false;
}

