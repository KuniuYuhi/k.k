#include "stdafx.h"
#include "MobEnemyBase.h"

#include "GameSceneManager.h"

////////////////////////////////////////////////////
//コンポーネント
#include "MobEnemyMovement.h"
#include "DamageProvider.h"

/////////////////////////////////////////////////////
#include "DamageFont.h"

#include "KnockBackInfoManager.h"
#include "Brave.h"

#include "UseEffect.h"


//ドロップアイテム
#include "AttackEffect.h"
#include "RecoveryEffect.h"
#include "StaminaEffect.h"


float MobEnemyBase::GetDistanceToTargetPositionValue(Vector3 target)
{
	return CalcDistanceToTargetPosition(target);
}

bool MobEnemyBase::IsStopRequested()
{
	//勝敗が着いたら
	if (GameSceneManager::GetInstance()->IsGameOutcome())return true;

	//死亡したら
	if (IsDie()) return true;

	//ここまで来たら処理は止まらない
	return false;
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


void MobEnemyBase::DieFromDamage()
{
	//自身を返す
	ReleaseThis();

	//死亡音を再生
	g_soundManager->InitAndPlaySoundSource(
		enSoundName_Mob_Die, g_soundManager->GetSEVolume());

	//死亡エフェクト生成
	UseEffect* effect = NewGO<UseEffect>(0, "DieEffect");
	effect->PlayEffect(enEffect_Mob_Dead,
		m_position, g_vec3One * 3.0f, Quaternion::Identity, false);

}

bool MobEnemyBase::IsDropBuffItem()
{
	int ram = rand() % 10;

	if (ram > 3)
	{
		return true;
	}

	return false;
}

void MobEnemyBase::DropBuffItem()
{
	int ram = rand() % 2;

	if (ram > 0)
	{
		//攻撃アイテムを落とす
		AttackEffect* at = NewGO<AttackEffect>(0, "AttackEffect");
		at->SetStartPosition(m_position);
		return;
	}
	
	ram = rand() % 2;

	//スタミナはプレイヤーのスタミナが減っている時だけ落とす
	if (ram > 0 && m_player->GetStatus().GetCurrentStamina() <= 100)
	{
		//スタミナ回復アイテムを落とす
		StaminaEffect* se = NewGO<StaminaEffect>(0, "StaminaEffect");
		se->SetStartPosition(m_position);
	}
	else
	{
		//回復アイテムを落とす
		RecoveryEffect* re = NewGO<RecoveryEffect>(0, "RecoveryEffect");
		re->SetStartPosition(m_position);

	}

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

	direction = m_player->GetPosition() - m_position;

	//回転方向を設定
	SetRotateDirection(direction);
	//前方向を設定
	SetForward(direction);
}

void MobEnemyBase::ChaseMovement(Vector3 targetPosition, bool isBossPosCheck)
{
	//アクション中は移動処理しない
	if (IsAction()) return;

	//もし逃げるようコンポーネントを持っているならそっちの移動処理を優先

	m_moveSpeed = m_movement->CalcChaseCharacterVerocity(
		m_status,
		targetPosition,
		m_position,
		m_moveSpeed,
		isBossPosCheck
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

	//線形補間で回転する方向を計算。徐々に回転させる
	/*m_currentRotDirection = Math::Lerp(
		g_gameTime->GetFrameDeltaTime() * 3.0f, m_currentRotDirection, m_rotateDirection
	);*/

	m_rotation.SetRotationYFromDirectionXZ(m_rotateDirection);

	//前方向を回転方向に合わせる
	//m_forward = m_currentRotDirection;
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

void MobEnemyBase::PlayHitSound()
{
	if (IsDie())
	{
		//やられた場合は強めの音を出す
		//音再生
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_Monster_DieHit,
			g_soundManager->GetSEVolume()
		);

		return;
	}

	//音再生
	g_soundManager->InitAndPlaySoundSource(
		enSoundName_Monster_Hit,
		g_soundManager->GetSEVolume()
	);

}

