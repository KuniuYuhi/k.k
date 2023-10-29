#include "stdafx.h"
#include "AIActor.h"
#include "FireBall.h"
#include "FlamePillar.h"

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

Vector3 AIActor::CalcVelocity(Status status,Vector3 targetposition,bool dashFlag)
{
	Vector3 moveSpeed = Vector3::Zero;

	//自身からターゲットに向かうベクトルを計算する
	Vector3 diff = targetposition - m_position;
	diff.y = 0.0f;
	//正規化
	diff.Normalize();

	//ダッシュフラグがtrueなら
	if (dashFlag == true)
	{
		//速度を設定
		moveSpeed = diff * (status.defaultSpeed * 1.5f);
	}
	else
	{
		//速度を設定
		moveSpeed = diff * status.defaultSpeed;
	}

	
	//前方向を設定
	m_forward = diff;
	//値をセーブしておく
	m_SaveMoveSpeed = moveSpeed;

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
			if (fireball != nullptr)
			{
				m_damage = fireball->GetAtk();
				HitFireBall();
				//ぶつかったのでファイヤーボールを消すフラグを立てる
				fireball->SetHitFlag(true);
			}
			
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
	m_damage = m_player->GetAtk();
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

void AIActor::PlayAttackSound()
{
	g_soundManager->InitAndPlaySoundSource(enSoundName_Attack1, g_soundManager->GetSEVolume());
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
	
	//計算したベクトルが上限の距離より小さかったら
	if (diff.Length() < distance)
	{
		//見つけた
		return true;
	}
	else
	{
		//見つからなかった
		m_targetPosition = Vector3::Zero;
		return false;
	}


	return false;
}

Quaternion AIActor::Rotation(float rotSpeed,float rotOnlySpeed)
{
	if (RotationOnly() == true)
	{
		//xかzの移動速度があったら(スティックの入力があったら)。
		if (fabsf(m_SaveMoveSpeed.x) >= 0.001f || fabsf(m_SaveMoveSpeed.z) >= 0.001f)
		{
			m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * rotOnlySpeed, m_rotMove, m_SaveMoveSpeed);
			m_rotation.SetRotationYFromDirectionXZ(m_rotMove);
		}
		//前方向を設定
		/*m_forward = Vector3::AxisZ;
		m_rotation.Apply(m_forward);*/
		m_forward = m_rotMove;
		m_forward.Normalize();
		return m_rotation;
	}

	//xかzの移動速度があったら
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//緩やかに回転させる
		m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * rotSpeed, m_rotMove, m_moveSpeed);
		m_rotation.SetRotationYFromDirectionXZ(m_rotMove);
	}
	//前方向を設定
	/*m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);*/
	m_forward = m_rotMove;
	m_forward.Normalize();
	return m_rotation;
}
