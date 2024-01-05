#include "stdafx.h"
#include "SwordShield.h"
#include "Brave.h"

//todo アニメーションイベント追加
//ジャンプ処理ｈ作る

namespace {
	//武器が収納状態の時の座標
	const Vector3 STOWEDS_POSITION = { 0.0f,-500.0f,0.0f };

	const Vector3 SWORD_COLLISION_SIZE = { 12.0f,100.0f,5.0f };
	const Vector3 SHIELD_COLLISION_SIZE = { 22.0f,40.0f,16.0f };

	const float SKILL_RADIUS = 60.0f;

	const float ADD_FORWARD = 30.0f;

	//ステータス
	const int POWER = 30;
	const int ENDURANCE = 70;		//武器の耐久力(盾の耐久力)。

	const float MOVE_FORWARD_SPEED = 200.0f;

	const float ATTACK_1COMBO_KNOCKBACK_POWER = 120.0f;
	const float ATTACK_2COMBO_KNOCKBACK_POWER = 130.0f;
	const float ATTACK_3COMBO_KNOCKBACK_POWER = 140.0f;
	const float SKILL_KNOCKBACK_POWER = 180.0f;
}

SwordShield::SwordShield()
{
	SetMoveForwardSpeed(MOVE_FORWARD_SPEED);
	SetWeaponPower(POWER);

	m_knockPower_1combo = ATTACK_1COMBO_KNOCKBACK_POWER;
	m_knockPower_2combo = ATTACK_2COMBO_KNOCKBACK_POWER;
	m_knockPower_3combo = ATTACK_3COMBO_KNOCKBACK_POWER;
	m_knockPower_Skill = SKILL_KNOCKBACK_POWER;
}

SwordShield::~SwordShield()
{
	DeleteGO(m_swordCollision);
	DeleteGO(m_shieldCollision);
}

bool SwordShield::Start()
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
	SetEnDefendTipe(enDefendTipe_Defence);

	//アニメーションイベント用の関数を設定する。
	m_brave->GetModelRender().AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

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
	InitSwordCollision();
	InitShieldCollision();
}

void SwordShield::InitSwordCollision()
{
	//剣の当たり判定
	m_swordCollision = NewGO<CollisionObject>(0, "Attack");
	/*m_swordCollision->CreateMesh(
		g_vec3Zero,
		g_quatIdentity,
		m_modelSword.GetModel(),
		m_brave->GetModelRender().GetBone(m_armedSwordBoonId)->GetWorldMatrix()
	);*/
	m_swordCollision->CreateBox(
		STOWEDS_POSITION,
		Quaternion(0.0f, 90.0f, 180.0f, 1.0f),
		SWORD_COLLISION_SIZE
	);
	m_swordCollision->SetIsEnableAutoDelete(false);
	m_swordCollision->SetIsEnable(false);
}

void SwordShield::InitShieldCollision()
{
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
	Vector3 swordPos = g_vec3Zero;
	//剣と盾のワールド座標を設定
	m_swordMatrix =
		m_brave->GetModelRender().GetBone(m_armedSwordBoonId)->GetWorldMatrix();
	m_modelSword.SetWorldMatrix(m_swordMatrix);


	m_shieldMatrix =
		m_brave->GetModelRender().GetBone(m_armedShieldBoonId)->GetWorldMatrix();
	m_modelShield.SetWorldMatrix(m_shieldMatrix);


	//防御可能かつYボタンを押している間は防御
	if (GetIsDefendEnableFlag()!=false && 
		g_pad[0]->IsPress(enButtonY) == true)
	{
		//防御している間は回転可能
		SetRotationDelectionFlag(true);
	}
	else
	{
		//防御していないのでフラグをリセット
		SetRotationDelectionFlag(false);
	}



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

void SwordShield::ProcessRising()
{
	m_skillMovePos = g_vec3AxisY;
	float addYPos = 0.0f;
	//上昇処理
	addYPos +=
		g_gameTime->GetFrameDeltaTime() * GetJampSpeed();
	m_skillMovePos.y += addYPos;
	//プレイヤーの座標を更新
	m_brave->ExecutePosition(m_skillMovePos);
}

void SwordShield::ProcessFall()
{
	m_skillMovePos = g_vec3AxisY;
	float addYPos = 0.0f;
	//上昇処理
	addYPos +=
		g_gameTime->GetFrameDeltaTime() * GetJampSpeed() * 1.2f;
	m_skillMovePos.y -= addYPos;

	//プレイヤーの座標を更新
	m_brave->ExecutePosition(m_skillMovePos);
}

void SwordShield::Render(RenderContext& rc)
{
	if (GetWeaponState() == enWeaponState_Stowed)
	{
		return;
	}

	m_modelSword.Draw(rc);

	//防御可能でないなら表示しない
	if (GetIsDefendEnableFlag() != true)
	{
		return;
	}
	m_modelShield.Draw(rc);
}

void SwordShield::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//スキルのジャンプ処理
	if (wcscmp(eventName, L"RisingSword") == 0)
	{
		//キーフレームがJampの間処理し続ける
		ProcessRising();
	}
	//スキルのジャンプ処理
	if (wcscmp(eventName, L"FallSword") == 0)
	{
		//キーフレームがJampの間処理し続ける
		ProcessFall();
	}
}
