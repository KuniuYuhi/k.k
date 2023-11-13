#include "stdafx.h"
#include "SwordShield.h"
#include "Brave.h"


namespace {
	//武器が収納状態の時の座標
	const Vector3 STOWEDS_POSITION = { 0.0f,-500.0f,0.0f };

	const Vector3 SWORD_COLLISION_SIZE = { 12.0f,100.0f,5.0f };
	const Vector3 SHIELD_COLLISION_SIZE = { 22.0f,40.0f,16.0f };

	const float SKILL_RADIUS = 60.0f;

	const float ADD_FORWARD = 30.0f;

	const int POWER = 30;
}

SwordShield::SwordShield()
{
	SetWeaponPower(POWER);
}

SwordShield::~SwordShield()
{
	DeleteGO(m_swordCollision);
	DeleteGO(m_shieldCollision);
}

bool SwordShield::Start()
{
	//勇者のインスタンスを探す
	m_brave = FindGO<Brave>("brave");

	InitModel();
	InitCollision();

	//防御タイプの設定
	SetEnDefendTipe(enDefendTipe_Defence);

	return true;
}

void SwordShield::Update()
{
	//収納状態なら
	if (GetStowedFlag() == true)
	{
		return;
	}

	MoveWeapon();

	m_modelSword.Update();
	m_modelShield.Update();
	m_swordCollision->Update();
	m_shieldCollision->Update();
}

bool SwordShield::IsHitCollision()
{
	//ダークボールの当たり判定
	const auto& DarkBallCollisions = g_collisionObjectManager->FindCollisionObjects("darkball");
	//コリジョンの配列をfor文で回す
	for (auto collision : DarkBallCollisions)
	{
		//自身のキャラコンと衝突したら
		if (collision->IsHit(m_shieldCollision) == true)
		{	
			return true;
		}
	}

	//ダークウォールの当たり判定
	const auto& DarkWallCollisions = g_collisionObjectManager->FindCollisionObjects("DarkWall");
	//コリジョンの配列をfor文で回す
	for (auto collision : DarkWallCollisions)
	{
		//自身のキャラコンと衝突したら
		if (collision->IsHit(m_shieldCollision) == true)
		{	
			return true;
		}
	}

	//メテオの当たり判定
	const auto& MeteoCollisions = g_collisionObjectManager->FindCollisionObjects("meteo");
	//コリジョンの配列をfor文で回す
	for (auto collision : MeteoCollisions)
	{
		//自身のキャラコンと衝突したら
		if (collision->IsHit(m_shieldCollision) == true)
		{
			return true;
		}
	}
	//メテオの爆発の当たり判定
	const auto& MeteoExplosionCollisions = g_collisionObjectManager->FindCollisionObjects("explosion");
	//コリジョンの配列をfor文で回す
	for (auto collision : MeteoExplosionCollisions)
	{
		//自身のキャラコンと衝突したら
		if (collision->IsHit(m_shieldCollision) == true)
		{	
			return true;
		}
	}

	//モンスターの攻撃の当たり判定
	const auto& MonsterCollisions = g_collisionObjectManager->FindCollisionObjects("monsterattack");
	//コリジョンの配列をfor文で回す
	for (auto collision : MonsterCollisions)
	{
		//自身のキャラコンと衝突したら
		if (collision->IsHit(m_shieldCollision) == true)
		{
			return true;
		}
	}

	//ダークメテオの攻撃の当たり判定
	const auto& DarkMeteoCollisions = g_collisionObjectManager->FindCollisionObjects("bigmeteo");
	//コリジョンの配列をfor文で回す
	for (auto collision : DarkMeteoCollisions)
	{
		//自身のキャラコンと衝突したら
		if (collision->IsHit(m_shieldCollision) == true)
		{	
			return true;
		}
	}

	return false;
}

void SwordShield::ProcessSkillAttack()
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

	//ノックバックの設定
	m_brave->SetKnockBackInfo(
		m_skillAttackPosition,
		m_knockBackPower,
		true
	);
}

void SwordShield::InitModel()
{
	//剣モデルの初期化
	m_modelSword.Init("Assets/modelData/character/Player/NewHero/OneHandSword.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ);
	
	//盾モデルの初期化
	m_modelShield.Init("Assets/modelData/character/Player/NewHero/Shield.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ);
	//剣と盾の座標に対応するボーンIDを取得
	//装備状態の時のボーンID
	m_armedSwordBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_r");
	m_armedShieldBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_l");
}

void SwordShield::InitCollision()
{
	//剣の当たり判定
	m_swordCollision = NewGO<CollisionObject>(0, "Attack");
	m_swordCollision->CreateBox(
		STOWEDS_POSITION,
		Quaternion(0.0f, 90.0f, 180.0f, 1.0f),
		SWORD_COLLISION_SIZE
	);
	m_swordCollision->SetIsEnableAutoDelete(false);
	m_swordCollision->SetIsEnable(false);
	//盾の当たり判定
	m_shieldCollision = NewGO<CollisionObject>(0, "defence");
	m_shieldCollision->CreateBox(
		STOWEDS_POSITION,
		Quaternion(0.0f, 90.0f, 180.0f, 1.0f),
		SHIELD_COLLISION_SIZE
	);
	m_shieldCollision->SetIsEnableAutoDelete(false);
	m_shieldCollision->SetIsEnable(false);
}

void SwordShield::MoveWeapon()
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

void SwordShield::MoveArmed()
{
	//剣と盾のワールド座標を設定
	m_swordMatrix =
		m_brave->GetModelRender().GetBone(m_armedSwordBoonId)->GetWorldMatrix();
	m_modelSword.SetWorldMatrix(m_swordMatrix);
	m_shieldMatrix =
		m_brave->GetModelRender().GetBone(m_armedShieldBoonId)->GetWorldMatrix();
	m_modelShield.SetWorldMatrix(m_shieldMatrix);

	//当たり判定の有効化無効化の処理
	if (m_brave->GetIsCollisionPossibleFlag() == true)
	{
		m_swordCollision->SetIsEnable(true);
	}
	else if (m_swordCollision->IsEnable() != false)
	{
		m_swordCollision->SetIsEnable(false);
	}

	m_swordCollision->SetWorldMatrix(m_swordMatrix);
	m_shieldCollision->SetWorldMatrix(m_shieldMatrix);
}

void SwordShield::MoveStowed()
{
	m_swordPos = STOWEDS_POSITION;
	m_shieldPos = STOWEDS_POSITION;

	m_modelSword.SetPosition(m_swordPos);
	m_modelShield.SetPosition(m_shieldPos);
	//当たり判定の座標の設定
	m_swordCollision->SetPosition(m_swordPos);
	m_shieldCollision->SetPosition(m_shieldPos);
	//当たり判定の無効化
	m_swordCollision->SetIsEnable(false);
	m_shieldCollision->SetIsEnable(false);
	SetStowedFlag(true);
}

void SwordShield::Render(RenderContext& rc)
{
	if (GetWeaponState() == enWeaponState_Stowed)
	{
		return;
	}

	m_modelSword.Draw(rc);
	m_modelShield.Draw(rc);
}
