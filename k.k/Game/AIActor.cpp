#include "stdafx.h"
#include "AIActor.h"

AIActor::AIActor()
{
	m_player = FindGO<Player>("player");
}

AIActor::~AIActor()
{
}

void AIActor::SetTransFormModel(ModelRender& modelRender)
{
	modelRender.SetTransform(m_position, m_rotation, m_scale);
}

void AIActor::SetTransForm(Vector3 position, Quaternion rotation, Vector3 scale)
{
	m_position = position;
	m_rotation = rotation;
	m_scale = scale;
}

Vector3 AIActor::CalcVelocity(Status status,Vector3 targetposition)
{
	Vector3 moveSpeed = Vector3::Zero;

	//自身からターゲットに向かうベクトルを計算する
	Vector3 diff = targetposition - m_position;
	
	//正規化
	diff.Normalize();
	//前方向
	m_forward = diff;

	diff.y = 0.0f;
	//速度を設定
	moveSpeed = diff * status.defaultSpeed;
	
	//値をセーブしておく
	m_SaveMoveSpeed = moveSpeed;

	//特定のアニメーションが再生中のとき
	/*if (isAnimationEntable() != true)
	{
		return moveSpeed = Vector3::Zero;
	}
	else
	{
		return moveSpeed;
	}*/

	return moveSpeed;
}

bool AIActor::IsInFieldOfView(Vector3 toPlayerDir, Vector3 forward, float angle)
{
	//ベクトル正規化
	toPlayerDir.Normalize();
	//ターゲットに向かうベクトルと前方向の内積を計算する
	float t = toPlayerDir.Dot(forward);
	//内積の結果をacos関数に渡して、m_enemyFowradとtoPlayerDirのなす角度を求める。
	float calcangle = acos(t);
	//視野角判定
	if (fabsf(calcangle) < Math::DegToRad(angle))
	{
		//範囲内なら
		return true;
	}
	else
	{
		return false;
	}
}

void AIActor::Attack()
{
}

bool AIActor::IsStopProcessing()
{
	return false;
}

void AIActor::SetTargetPosition()
{
	//ターゲット(プレイヤー)の座標を取得
	m_targetPosition = m_player->GetPosition();
}

void AIActor::CreateDamageFont(int damage)
{
	//受けるダメージを表示するオブジェクトを生成する
	DamageFont* damagefont = NewGO<DamageFont>(0, "damagefont");
	damagefont->Setting(
		DamageFont::enDamageActor_Monster, 
		damage,
		m_position
	);
}

void AIActor::DamageCollision(CharacterController& characon)
{
	//抜け出す処理
	if (IsCollisionDetection() == true)
	{
		return;
	}

	//通常攻撃の当たり判定
	const auto& Attack_1Collisions = g_collisionObjectManager->FindCollisionObjects("Attack");
	//コリジョンの配列をfor文で回す
	for (auto collision : Attack_1Collisions)
	{
		//自身のキャラコンと衝突したら
		if (collision->IsHit(characon) == true)
		{
			CreateHitEffect();
			HitNormalAttack();
			return;
		}
	}

	//ヒーローのスキルの当たり判定
	const auto& SkillCollisions = g_collisionObjectManager->FindCollisionObjects("SkillAttack");
	//コリジョンの配列をfor文で回す
	for (auto collision : SkillCollisions)
	{
		//自身のキャラコンと衝突したら
		if (collision->IsHit(characon) == true)
		{
			CreateHitEffect();
			HitHeroSkillAttack();
			return;
		}
	}

	//ウィザードのファイヤーボールの当たり判定
	const auto& FireBallCollisions = g_collisionObjectManager->FindCollisionObjects("fireball");
	//コリジョンの配列をfor文で回す
	for (auto collision : FireBallCollisions)
	{
		//自身のキャラコンと衝突したら
		if (collision->IsHit(characon) == true)
		{
			FireBall* fireball = FindGO<FireBall>("fireball");
			m_damage = fireball->GetAtk();
			HitFireBall();
			//ぶつかったのでファイヤーボールを消すフラグを立てる
			fireball->SetHitFlag(true);
			return;
		}
	}

	//ウィザードのフレイムピラーの当たり判定
	const auto& FlamePillarCollisions = g_collisionObjectManager->FindCollisionObjects("flamepillar");
	//コリジョンの配列をfor文で回す
	for (auto collision : FlamePillarCollisions)
	{
		//自身のキャラコンと衝突したら
		if (collision->IsHit(characon) == true)
		{
			FlamePillar* flamepillar = FindGO<FlamePillar>("flamepillar");

			bool damageFlag = flamepillar->GetCanDamageFlag();
			m_damage = flamepillar->GetAtk();
			HitFlamePillar(damageFlag);
			//ダメージフラグが立っていなかったら
			if (flamepillar->GetCanDamageFlag() != true)
			{
				//フラグを立てる(ダメージ受けた)
				flamepillar->SetCanDamageFlag(true);
			}
			return;
		}
	}
}

bool AIActor::IsCollisionDetection()
{
	//特定のアニメーションが再生中なら
	if (isAnimationEntable() != true)
	{
		return true;
	}

	return false;
}

void AIActor::HitNormalAttack()
{
	m_damage = m_player->GetAtk();
	//ダメージを受ける
	Damage(m_damage);
	CreateDamageFont(m_damage);
}

void AIActor::HitHeroSkillAttack()
{
	m_damage = m_player->GetSkillAtk();
	//ダメージを受ける
	Damage(m_damage);
	CreateDamageFont(m_damage);
}

void AIActor::HitFireBall()
{
	//ダメージを受ける
	Damage(m_damage);
	CreateDamageFont(m_damage);
}

void AIActor::HitFlamePillar(bool damageFlag)
{
	//ダメージを受ける
	Damage(m_damage);
	CreateDamageFont(m_damage);
}

void AIActor::CreateHitEffect()
{
}

bool AIActor::AttackInterval(const float attackintarvaltime)
{
	//攻撃したら
	if (m_attackFlag == true)
	{
		//タイマーがインターバルを超えたら
		if (attackintarvaltime < m_attackIntervalTimer)
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

bool AIActor::DamageInterval(const float damageintarvaltime)
{
	//ダメージを受けたら
	if (m_damageFlag == true)
	{
		//タイマーがインターバルを超えたら
		if (damageintarvaltime < m_damageIntervalTimer)
		{
			//ダメージを受けられるようにする
			m_damageFlag = false;
			//タイマーをリセット
			m_damageIntervalTimer = 0.0f;
		}
		else
		{
			m_damageIntervalTimer += g_gameTime->GetFrameDeltaTime();
			//ダメージを受けない
			return false;
		}
	}
	//ダメージを受けられるようにする
	return true;
}

bool AIActor::AngleChangeTimeIntarval(float LimitTime)
{
	//ベクトルを計算したら
	if (m_angleChangeTimeFlag == true)
	{
		//タイマーがインターバルを超えたら
		if (LimitTime < m_angleChangeTimer)
		{
			//ベクトルを計算できるようにする
			m_angleChangeTimeFlag = false;
			//タイマーをリセット
			m_angleChangeTimer = 0.0f;
			//ベクトルを計算不可能
			return false;
		}
		else
		{
			m_angleChangeTimer += g_gameTime->GetFrameDeltaTime();
		}
	}
	//ベクトルを計算可能
	return true;
}

bool AIActor::IsFindPlayer(float distance)
{
	//ターゲット(プレイヤー)の座標を取得
	m_targetPosition = m_player->GetPosition();
	//自身からターゲットに向かうベクトルを計算する
	Vector3 diff = m_targetPosition - m_position;
	
	diff.y = 0.0f;
	//ベクトルをセーブ
	m_toTarget = diff;
	//前方向を保存
	/*m_forward = diff;
	m_forward.Normalize();*/

	//計算したベクトルが上限の距離より小さかったら
	if (diff.Length() < distance)
	{
		//見つけた
		return true;
	}
	else
	{
		//diff.Normalize();
		////前方向を保存
		//m_forward = diff;
		//見つからなかった
		m_targetPosition = Vector3::Zero;
		return false;
	}


	return false;
}

Quaternion AIActor::Rotation()
{
	//todo 線形補間で緩やかに回転
	//セーブする値

	if (RotationOnly() == true)
	{
		m_forward = Vector3::AxisZ;
		m_rotation.Apply(m_forward);
		return m_rotation;
	}

	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{

		m_rotTimer += g_gameTime->GetFrameDeltaTime();
		if (m_rotTimer > 1.0f)
		{
			m_rotTimer = 0.0f;
		}

		Vector3 nowMoveSpeed;
		nowMoveSpeed.Lerp(m_rotTimer, m_SaveMoveSpeed, m_moveSpeed);

		m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
	}
	//前方向を設定
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	return m_rotation;
}
