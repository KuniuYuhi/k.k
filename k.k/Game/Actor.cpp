#include "stdafx.h"
#include "Actor.h"
#include "FireBall.h"
#include "DarkWall.h"
#include "Meteo.h"
#include "AIActor.h"


Actor::Actor()
{
	//m_player = FindGO<Player>("player");
	//m_charaCon;
}

Actor::~Actor()
{
}

void Actor::SetTransFormModel(ModelRender& modelRender)
{
	modelRender.SetTransform(m_position, m_rotation, m_scale);
}

void Actor::SetTransForm(Vector3 position, Quaternion rotation,Vector3 scale)
{
	m_position = position;
	m_rotation = rotation;
	m_scale = scale;
}

void Actor::RecoveryMP()
{
	//MP回復状態なら
	if (m_recoveryMpFlag == true)
	{
		if (m_status.mp < m_status.maxMp)
		{
			m_status.mp += g_gameTime->GetFrameDeltaTime();

			if (m_status.mp > m_status.maxMp)
			{
				m_status.mp = m_status.maxMp;
				//MP回復状態をなしにする
				m_recoveryMpFlag = false;
			}
		}
	}

	
}

Vector3 Actor::calcVelocity(Status status)
{
	Vector3 moveSpeed = Vector3::Zero;

	//カメラの前方向と右方向のベクトルを持ってくる。
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//y方向には移動させない。
	forward.y = 0.0f;
	right.y = 0.0f;
	forward.Normalize();

	//移動の入力
	Vector3 stickL = Vector3::Zero;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//ダッシュフラグがfalseなら
	if (m_dashFlag == false)
	{
		//移動の入力量とstatusのスピードを乗算。
		right *= stickL.x * status.defaultSpeed;
		forward *= stickL.y * status.defaultSpeed;
	}
	else
	{
		//移動の入力量とstatusのスピードとダッシュの値を乗算。
		right *= stickL.x * (status.defaultSpeed + status.dashSpeed);
		forward *= stickL.y * (status.defaultSpeed + status.dashSpeed);
	}

	moveSpeed += right + forward;
	moveSpeed.y = 0.0f;
	//値をセーブしておく
	m_SaveMoveSpeed = moveSpeed;

	//特定のアニメーションが再生中なら移動なし
	if (isAnimationEntable() != true)
	{
		return moveSpeed = Vector3::Zero;
	}
	else
	{
		return moveSpeed;
	}
	
	
}

bool Actor::CalcInvincibleTime()
{
	//無敵時間フラグが立ったら
	if (m_invincibleTimeFlag == true)
	{
		if (m_invincbleTime < m_invincbleTimer)
		{
			m_invincbleTimer = 0.0f;
			//フラグを
			m_invincibleTimeFlag = false;

			//return false;
		}
		else
		{
			m_invincbleTimer += g_gameTime->GetFrameDeltaTime();
		}

		return true;
	}

	return false;
}

void Actor::DamageCollision(CharacterController& characon)
{
	//抜け出す処理
	if (isCollisionEntable() != true)
	{
		return;
	}

	//ダークボールの当たり判定
	const auto& DarkBallCollisions = g_collisionObjectManager->FindCollisionObjects("darkball");
	//コリジョンの配列をfor文で回す
	for (auto collision : DarkBallCollisions)
	{
		//自身のキャラコンと衝突したら
		if (collision->IsHit(characon) == true)
		{
			FireBall* fireball = FindGO<FireBall>("darkball");
			Damage(fireball->GetAtk());
			CreateDamageFont(fireball->GetAtk());
			return;
		}
	}

	//ダークウォールの当たり判定
	const auto& DarkWallCollisions = g_collisionObjectManager->FindCollisionObjects("DarkWall");
	//コリジョンの配列をfor文で回す
	for (auto collision : DarkWallCollisions)
	{
		//自身のキャラコンと衝突したら
		if (collision->IsHit(characon) == true)
		{
			DarkWall* darkwall = FindGO<DarkWall>("darkwall");
			Damage(darkwall->GetAtk());
			CreateDamageFont(darkwall->GetAtk());
			return;
		}
	}

	//メテオの当たり判定
	const auto& MeteoCollisions = g_collisionObjectManager->FindCollisionObjects("meteo");
	//コリジョンの配列をfor文で回す
	for (auto collision : MeteoCollisions)
	{
		//自身のキャラコンと衝突したら
		if (collision->IsHit(characon) == true)
		{
			Meteo* meteo = FindGO<Meteo>("meteo");
			Damage(meteo->GetAtk());
			CreateDamageFont(meteo->GetAtk());
			return;
		}
	}
	//メテオの爆発の当たり判定
	const auto& MeteoExplosionCollisions = g_collisionObjectManager->FindCollisionObjects("explosion");
	//コリジョンの配列をfor文で回す
	for (auto collision : MeteoExplosionCollisions)
	{
		//自身のキャラコンと衝突したら
		if (collision->IsHit(characon) == true)
		{
			Meteo* meteo = FindGO<Meteo>("meteo");
			Damage(meteo->GetExplosionAttack());
			CreateDamageFont(meteo->GetExplosionAttack());
			return;
		}
	}

	//モンスターの攻撃の当たり判定
	const auto& MonsterCollisions = g_collisionObjectManager->FindCollisionObjects("monsterattack");
	//コリジョンの配列をfor文で回す
	for (auto collision : MonsterCollisions)
	{
		//自身のキャラコンと衝突したら
		if (collision->IsHit(characon) == true)
		{
			m_atttackAIActor = FindGO<AIActor>(collision->GetCreatorName());
			Damage(m_atttackAIActor->GetStatus().atk);
			CreateDamageFont(m_atttackAIActor->GetStatus().atk);
			return;
		}
	}

}

bool Actor::IsComboStateSame()
{
	//現在のコンボステートとダメージを受けた時のコンボステートが違うなら
	if (GetNowComboState() != GetDamagedComboState())
	{
		//違う
		return true;
	}
	else
		//同じ
		return false;
}

Quaternion Actor::Rotation()
{
	//回転だけさせたいなら
	if (RotationOnly() == true)
	{
		return m_rotation;
	}

	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
	}

	return m_rotation;
}

bool Actor::CalcInvicibleDash()
{
	if (m_enDashInvicibleState == enDashInvicibleState_On)
	{
		if (m_invincbleDashTime < m_invincbledDashTimer)
		{
			m_invincbledDashTimer = 0.0f;
			SetInvicibleDashState(enDashInvicibleState_Off);
		}
		else
		{
			m_invincbledDashTimer += g_gameTime->GetFrameDeltaTime();
		}

		return true;
	}

	
	return false;
}

void Actor::CreateDamageFont(int damage)
{
	DamageFont* damagefont = NewGO<DamageFont>(0, "damagefont");
	damagefont->Setting(
		DamageFont::enDamageActor_Player,
		damage,
		m_position
	);
}
