#include "stdafx.h"
#include "Actor.h"
#include "AIActor.h"
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
	//前方向と右方向のベクトルを足す
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
		m_forward.y = 0.0f;
		m_forward.Normalize();
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

Quaternion Actor::Rotation(float rotSpeed,float rotOnlySpeed)
{
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_forward.x) >= 0.001f || fabsf(m_forward.z) >= 0.001f)
	{
		m_rotation.SetRotationYFromDirectionXZ(m_forward);
	}
	return m_rotation;
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
