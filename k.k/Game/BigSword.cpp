#include "stdafx.h"
#include "BigSword.h"
#include "Brave.h"

namespace {
	//武器が収納状態の時の座標
	const Vector3 STOWEDS_POSITION = { 0.0f,-500.0f,0.0f };

	const Vector3 BIG_SWORD_COLLISION_SIZE = { 18.0f,100.0f,8.0f };

	const float SKILL_RADIUS = 80.0f;

	const float ADD_FORWARD = 8.0f;

	//ステータス
	const int POWER = 40;
	const int ENDURANCE = -1;		//武器の耐久力(耐久力概念なし)

	const float HITTABLE_TIME = 0.5f;

	const float MOVE_FORWARD_SPEED = 150.0f;
}

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

BigSword::BigSword()
{
	SetMoveForwardSpeed(MOVE_FORWARD_SPEED);
	SetWeaponPower(POWER);
}

BigSword::~BigSword()
{
	DeleteGO(m_bigSwordCollision);
}

bool BigSword::Start()
{
	//武器のステータス初期化
	m_status.InitWeaponStatus(
		POWER, ENDURANCE
	);

	//勇者のインスタンスを探す
	m_brave = FindGO<Brave>("brave");

	InitModel();
	InitCollision();

	//防御タイプの設定
	SetEnDefendTipe(enDefendTipe_avoid);

	//アニメーションイベント用の関数を設定する。
	m_brave->GetModelRender().AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	return true;
}

void BigSword::Update()
{
	//収納状態の時は処理しない
	if (GetStowedFlag() == true)
	{
		return;
	}

	//ヒット可能か判断する
	m_hitDelection.IsHittable(HITTABLE_TIME);

	MoveWeapon();

	m_modelBigSword.Update();
	m_bigSwordCollision->Update();
}

void BigSword::ProcessSkillAttack()
{
	//当たり判定を生成する座標を設定
	m_skillAttackPosition = g_vec3Zero;
	//剣のワールド座標をベクトルに乗算
	m_swordMatrix.Apply(m_skillAttackPosition);
	m_skillAttackPosition.y = 0.0f;
	Vector3 forward;
	forward = m_brave->GetForward();
	forward *= ADD_FORWARD;
	//前方向分を足す
	m_skillAttackPosition += forward;

	//スキル攻撃時の当たり判定の生成
	auto skillCollision = NewGO<CollisionObject>(0, "skillAttack");
	skillCollision->CreateSphere(
		m_skillAttackPosition,
		g_quatIdentity,
		SKILL_RADIUS
	);
}

void BigSword::InitModel()
{
	//剣モデルの初期化
	m_modelBigSword.Init("Assets/modelData/character/Player/NewHero/BigSowrd.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ);

	//剣と盾の座標に対応するボーンIDを取得
	//装備状態の時のボーンID
	m_armedSwordBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_l");
}

void BigSword::InitCollision()
{
	m_bigSwordCollision = NewGO<CollisionObject>(0, "Attack");
	m_bigSwordCollision->CreateBox(
		STOWEDS_POSITION,
		Quaternion(0.0f, 90.0f, 180.0f, 1.0f),
		BIG_SWORD_COLLISION_SIZE
	);
	m_bigSwordCollision->SetIsEnableAutoDelete(false);
	m_bigSwordCollision->SetIsEnable(false);
}

void BigSword::MoveWeapon()
{
	switch (m_enWeaponState)
	{
	case IWeapon::enWeaponState_Stowed://収納状態
		MoveStowed();
		break;
	case IWeapon::enWeaponState_Armed://装備状態
		MoveArmed();
		break;
	case IWeapon::enWeaponState_None://なし
		break;
	default:
		break;
	}
}

void BigSword::MoveArmed()
{
	//剣のワールド座標を設定
	m_swordMatrix =
		m_brave->GetModelRender().GetBone(m_armedSwordBoonId)->GetWorldMatrix();
	m_modelBigSword.SetWorldMatrix(m_swordMatrix);

	
	//当たり判定の有効化無効化の処理
	if (m_brave->GetIsCollisionPossibleFlag() == true)
	{
		m_bigSwordCollision->SetIsEnable(true);
	}
	else if (m_bigSwordCollision->IsEnable() != false)
	{
		m_bigSwordCollision->SetIsEnable(false);
	}

	Matrix collisionMatrix = m_swordMatrix;

	//collisionMatrix.

	m_bigSwordCollision->SetWorldMatrix(m_swordMatrix);

}

void BigSword::MoveStowed()
{
	m_swordPos = STOWEDS_POSITION;
	m_modelBigSword.SetPosition(m_swordPos);
	//当たり判定の座標の設定
	m_bigSwordCollision->SetPosition(m_swordPos);
	//当たり判定の無効化
	m_bigSwordCollision->SetIsEnable(false);
	SetStowedFlag(true);
}

void BigSword::ProcessRising()
{
	m_skillMovePos = g_vec3AxisY;
	float addYPos = 0.0f;
	//上昇処理
	addYPos +=
		g_gameTime->GetFrameDeltaTime() * GetJampSpeed();
	m_skillMovePos.y += addYPos;
	//プレイヤーの座標を更新
	m_brave->ExecutePosition(m_skillMovePos);
	//上昇した時の座標を設定
	m_maxRisingPosition = m_brave->GetPosition();
}

void BigSword::ProcessFall()
{
	m_skillMovePos = g_vec3AxisY;
	float addYPos = 0.0f;
	//上昇処理
	addYPos +=
		g_gameTime->GetFrameDeltaTime() * GetJampSpeed()*1.2f;
	m_skillMovePos.y -= addYPos;

	//プレイヤーの座標を更新
	m_brave->ExecutePosition(m_skillMovePos);

	//地面との衝突判定をとる
	if (IsGround() == true)
	{
		//座標を地面に合わせる
		Vector3 Zero = m_brave->GetPosition();
		Zero.y = 0.0f;
		m_brave->SetPosition(Zero);
	}
}

bool BigSword::IsGround()
{
	Vector3 endPosition = m_brave->GetPosition();

	CCapsuleCollider m_capsuleCollider;
	m_capsuleCollider.Init(20.0f, 30.0f);
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//始点はエネミーの座標。
	start.setOrigin(btVector3(
		m_maxRisingPosition.x, m_maxRisingPosition.y, m_maxRisingPosition.z));
	//終点はプレイヤーの座標。
	end.setOrigin(btVector3(
		endPosition.x*1.1f, endPosition.y * 1.1f, endPosition.z * 1.1f));
	//壁の判定を返す
	IsGroundResult callback_Ground;
	//コライダーを始点から終点まで動かして。
	//壁と衝突するかどうかを調べる。
	PhysicsWorld::GetInstance()->ConvexSweepTest(
		(const btConvexShape*)m_capsuleCollider.GetBody(),
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

void BigSword::Render(RenderContext& rc)
{
	if (GetWeaponState() == enWeaponState_Stowed)
	{
		return;
	}
	m_modelBigSword.Draw(rc);
}

void BigSword::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//スキルのジャンプ処理
	if (wcscmp(eventName, L"RisingGreatSword") == 0)
	{
		//キーフレームがJampの間処理し続ける
		ProcessRising();
	}
	//スキルのジャンプ処理
	if (wcscmp(eventName, L"FallGreatSword") == 0)
	{
		//キーフレームがJampの間処理し続ける
		ProcessFall();
	}
}


