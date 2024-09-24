#include "stdafx.h"
#include "BossEnemyBase.h"
#include "DamageProvider.h"
#include "BossEnemyMovement.h"

#include "Brave.h"

#include "DamageFont.h"

#include "KnockBackInfoManager.h"




void BossEnemyBase::SetDamageInfo(
	int damageId, int attackPower, float knockBackTimeScale,
	EnKnockBackPattern knockBackPattern,
	EnWeaponAttribute weaponAttribute)
{
	m_damageProvider->SetDamageInfo(damageId, attackPower, knockBackTimeScale,
		knockBackPattern, weaponAttribute);
}

void BossEnemyBase::SettingDefaultComponent()
{
	//ボスの移動用コンポーネント
	AddComponent<BossEnemyMovement>();
	m_movement = GetComponent<BossEnemyMovement>();

	//ダメージプロバイダーコンポーネント
	AddComponent<DamageProvider>();
	m_damageProvider = GetComponent<DamageProvider>();
	//自身のインスタンスを保存
	m_damageProvider->SetProviderCharacterInstance(this);


}

void BossEnemyBase::CheckSelfCollision()
{
	//被ダメージを受けない条件
	if (IgnoreCollision()) return;


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
			CreateDamageFont(dp->GetAdjustedDamage(), DamageFont::enDamageActor_Boss);

			//被ダメージ処理を行う引数に情報を入れる
			ProcessHit(dp->GetProviderDamageInfo());

			//今回のIDを前フレームのIDに保存
			oldAttackId = currentAttackId;
		}
	}
}

void BossEnemyBase::ChaseMovement(Vector3 targetPosition)
{
	//アクション中は移動処理しない
	if (IsAction()) return;

	
	//移動速度を計算
	m_moveSpeed = m_movement->CalcChaseCharacterVerocity(
		m_status,
		targetPosition,
		m_position,
		m_moveSpeed
	);

	//プレイヤーに向かう距離を計算
	float toPlayerDistance = CalcDistanceToTargetPosition(targetPosition);
	bool isExecute = true;
	Vector3 tempMoveSpeed = m_moveSpeed;

	//todo 状況によって止まる距離変更するのもあり

	//移動をやめるなら
	if (m_isStopMove)
	{
		isExecute = false;
		m_moveSpeed = g_vec3Zero;
	}
	//プレイヤーとの距離より接近する距離が大きいとき、上限の距離を少し伸ばす
	else if (toPlayerDistance <= m_status.GetApproachDistance())
	{
		isExecute = false;
		m_moveSpeed = g_vec3Zero;
		//接近できる距離を少し伸ばす
		if (m_isArrivedApproachDistance)
		{
			m_status.AddApproachDistance(200.0f);
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

	//移動量があれば前方向を設定
	if (fabsf(tempMoveSpeed.x) >= 0.001f || fabsf(tempMoveSpeed.z) >= 0.001f)
	{
		SetForward(tempMoveSpeed);
	}

	//回転方向を保存
	m_rotateDirection = tempMoveSpeed;

}

void BossEnemyBase::TurnToTarget()
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

void BossEnemyBase::AttackChaseMovement(Vector3 targetPosition, float speed)
{
	//移動速度を計算
	m_moveSpeed = m_movement->CalcChaseCharacterVerocity(
		speed,
		targetPosition,
		m_position,
		m_moveSpeed
	);


	//キャラコンで座標を移動する
	m_position = m_charaCon.get()->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	//回転方向を保存
	m_rotateDirection = m_moveSpeed;
}
