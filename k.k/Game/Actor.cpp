#include "stdafx.h"
#include "Actor.h"
#include "DarkWall.h"
#include "DarkBall.h"
#include "Meteo.h"
#include "DarkMeteorite.h"
#include "DarkSpearObj.h"

#include "AIActor.h"
#include "MagicBall.h"

#include "IAttackObject.h"


Actor::Actor()
{
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

bool Actor::IsFlashing()
{
	//無敵時間か
	if (m_invincibleTimeFlag == true)
	{
		//モデルのdrawフラグがtrueなら表示(false)。
		if (m_modelDrawFlag == true)
		{
			m_modelDrawFlag = false;
			return false;
		}
		else
		{
			//モデルのdrawフラグがfalseなので非表示(true)。
			m_modelDrawFlag = !m_modelDrawFlag;
			return true;
		}
	}
	else
	{
		//無敵時間でないなら表示
		return false;
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
	right.Normalize();
	//移動の入力
	Vector3 stickL = Vector3::Zero;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//ダッシュフラグがfalseなら
	if (m_dashFlag == false)
	{
		//移動の入力量とstatusのスピードを乗算。
		right *= stickL.x * status.GetDefaultSpeed();
		forward *= stickL.y * status.GetDefaultSpeed();
	}
	else
	{
		//移動の入力量とstatusのスピードとダッシュの値を乗算。
		right *= stickL.x * (status.GetDefaultSpeed() + status.GetDashSpeed());
		forward *= stickL.y * (status.GetDefaultSpeed() + status.GetDashSpeed());
	}

	moveSpeed += right + forward;
	
	//値をセーブしておく
	m_SaveMoveSpeed = moveSpeed;

	return moveSpeed;
}

void Actor::CalcForward(Vector3 moveSpeed)
{
	//前方向の取得
	//x,yどちらかの入力があったら
	if (fabsf(moveSpeed.x) >= 0.001f || fabsf(moveSpeed.z) >= 0.001f)
	{
		m_forward = moveSpeed;
		m_forward.Normalize();

		//前方向を設定できる条件なら
		if (IsSetForwardCondition() == true)
		{
			
		}
	}
}

bool Actor::CalcInvincibleTime()
{
	//無敵時間フラグがセットされていないなら処理しない
	if (m_invincibleTimeFlag != true)
	{
		return false;
	}

	//無敵時間フラグが立ったら
	if (m_invincbleTime < m_invincbleTimer)
	{
		m_invincbleTimer = 0.0f;
		//フラグを下げる。
		m_invincibleTimeFlag = false;
		return false;
	}
	else
	{
		m_invincbleTimer += g_gameTime->GetFrameDeltaTime();
		//無敵時間なのでtrueをかえす
		return true;

	}
}

void Actor::DamageCollision(CharacterController& characon)
{
	//当たり判定をとるか
	if (IsDecisionCollision() == false)
	{
		return;
	}

	//アタックオブジェクトの当たり判定
	const auto& DarkBallCollisions = 
		g_collisionObjectManager->FindCollisionObjects("attackobject");
	//コリジョンの配列をfor文で回す
	for (auto collision : DarkBallCollisions)
	{
		//自身のキャラコンと衝突したら
		if (collision->IsHit(characon) == true)
		{
			//当たったオブジェクトの製作者のインスタンスを取得
			auto* attackObject = 
				FindGO<IAttackObject>(collision->GetCreatorName());
			int attack = attackObject->GetAttack();
			//被ダメージ処理
			Damage(attack);
			//フォント生成
			CreateDamageFont(m_hitDamage);

			return;
		}
	}




	///////////////////////////////////////////////

	//ダークボールの当たり判定
	//const auto& DarkBallCollisions = g_collisionObjectManager->FindCollisionObjects("darkball");
	////コリジョンの配列をfor文で回す
	//for (auto collision : DarkBallCollisions)
	//{
	//	//自身のキャラコンと衝突したら
	//	if (collision->IsHit(characon) == true)
	//	{
	//		//todo 常に真ん中で判定をとってしまう
	//		DarkBall* darkball = FindGO<DarkBall>("darkball");
	//		//ぶつかったのでフラグを立てる
	//		darkball->SetHitFlag(true);
	//		Damage(darkball->GetAtk());
	//		CreateDamageFont(m_hitDamage);
	//		return;
	//	}
	//}

	////ダークウォールの当たり判定
	//const auto& DarkWallCollisions = g_collisionObjectManager->FindCollisionObjects("DarkWall");
	////コリジョンの配列をfor文で回す
	//for (auto collision : DarkWallCollisions)
	//{
	//	//自身のキャラコンと衝突したら
	//	if (collision->IsHit(characon) == true)
	//	{
	//		DarkWall* darkwall = FindGO<DarkWall>("darkwall");
	//		Damage(darkwall->GetAtk());
	//		CreateDamageFont(m_hitDamage);
	//		return;
	//	}
	//}

	////ダークスピアの当たり判定
	//const auto& DarkSpearCollisions = g_collisionObjectManager->FindCollisionObjects("darkspear");
	////コリジョンの配列をfor文で回す
	//for (auto collision : DarkSpearCollisions)
	//{
	//	//自身のキャラコンと衝突したら
	//	if (collision->IsHit(characon) == true)
	//	{
	//		DarkSpearObj* darkSpearObj = FindGO<DarkSpearObj>("darkspearobj");
	//		Damage(darkSpearObj->GetAtk());
	//		CreateDamageFont(m_hitDamage);
	//		return;
	//	}
	//}

	////メテオの当たり判定
	//const auto& MeteoCollisions = g_collisionObjectManager->FindCollisionObjects("meteo");
	////コリジョンの配列をfor文で回す
	//for (auto collision : MeteoCollisions)
	//{
	//	//自身のキャラコンと衝突したら
	//	if (collision->IsHit(characon) == true)
	//	{
	//		Meteo* meteo = FindGO<Meteo>("meteo");
	//		Damage(meteo->GetAtk());
	//		CreateDamageFont(m_hitDamage);
	//		//メテオに当たったので強制的に爆発させる
	//		meteo->Explosion();
	//		return;
	//	}
	//}
	////メテオの爆発の当たり判定
	//const auto& MeteoExplosionCollisions = g_collisionObjectManager->FindCollisionObjects("explosion");
	////コリジョンの配列をfor文で回す
	//for (auto collision : MeteoExplosionCollisions)
	//{
	//	//自身のキャラコンと衝突したら
	//	if (collision->IsHit(characon) == true)
	//	{
	//		Meteo* meteo = FindGO<Meteo>("meteo");
	//		int damage = meteo->CalcDamageToDistance(m_position);
	//		Damage(damage);
	//		CreateDamageFont(m_hitDamage);
	//		return;
	//	}
	//}

	//ダークメテオの攻撃の当たり判定
	//const auto& DarkMeteoCollisions = g_collisionObjectManager->FindCollisionObjects("bigmeteo");
	////コリジョンの配列をfor文で回す
	//for (auto collision : DarkMeteoCollisions)
	//{
	//	//自身のキャラコンと衝突したら
	//	if (collision->IsHit(characon) == true)
	//	{
	//		DarkMeteorite* darkMeteo = FindGO<DarkMeteorite>("darkmeteorite");
	//		Damage(darkMeteo->GetAtk());
	//		CreateDamageFont(m_hitDamage);
	//		darkMeteo->SetChaseFlag(true);
	//		return;
	//	}
	//}

	//モンスターの攻撃の当たり判定
	const auto& MonsterCollisions = g_collisionObjectManager->FindCollisionObjects("monsterattack");
	//コリジョンの配列をfor文で回す
	for (auto collision : MonsterCollisions)
	{
		//自身のキャラコンと衝突したら
		if (collision->IsHit(characon) == true)
		{
			m_atttackAIActor = FindGO<AIActor>(collision->GetCreatorName());
			//ヒット音を再生
			m_atttackAIActor->PlayAttackSound();
			Damage(m_atttackAIActor->GetStatus().GetAtk());
			CreateDamageFont(m_hitDamage);
			return;
		}
	}

	

}

bool Actor::IsComboStateSame()
{
	//プレイヤーのコンボ状態と
	// 敵が攻撃を受けた時のコンボ状態が違うなら
	if (GetNowComboState() != GetDamagedComboState())
	{
		return true;
	}
	return false;
}

Quaternion Actor::Rotation(float rotSpeed,float rotOnlySpeed)
{
	//回転だけさせたいなら
	if (RotationOnly() == true)
	{
		//xかzの移動速度があったら(スティックの入力があったら)。
		if (fabsf(m_forward.x) >= 0.001f || fabsf(m_forward.z) >= 0.001f)
		{
			//m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * rotOnlySpeed, m_rotMove, m_SaveMoveSpeed);
			m_rotation.SetRotationYFromDirectionXZ(m_forward);

			/*Vector3 forward;
			m_rotation.Apply(forward);
			Vector3 diff = forward - m_position;
			m_forward = diff;
			m_forward.y = 0.0f;
			m_forward.Normalize();*/
		}
		return m_rotation;
	}

	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_forward.x) >= 0.001f || fabsf(m_forward.z) >= 0.001f)
	{
		//m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * rotSpeed, m_rotMove, m_SaveMoveSpeed);
		m_rotation.SetRotationYFromDirectionXZ(m_forward);

		/*Vector3 forward;
		m_rotation.Apply(forward);
		Vector3 diff = forward - m_position;
		m_forward = diff;
		m_forward.y = 0.0f;
		m_forward.Normalize();*/
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
	//ダメージが0か0以下ならフォントを出さない
	if (damage <= 0)
	{
		return;
	}

	DamageFont* damagefont = NewGO<DamageFont>(0, "damagefont");
	damagefont->Setting(
		DamageFont::enDamageActor_Player,
		damage,
		m_position
	);
}

bool Actor::IsDecisionCollision()
{
	//無敵状態なら処理しない
	if (GetInvicibleFlag() == true)
	{
		return false;
	}

	//無敵時間なら処理しない
	if (GetInvincibleTimeFlag() == true)
	{
		return false;
	}
	//当たり判定しないアニメーションなら処理しない
	if (isCollisionEntable() == true)
	{
		return false;
	}
	//判定をとる
	return true;
}
