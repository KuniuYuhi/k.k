#include "stdafx.h"
#include "BigSword.h"
#include "Brave.h"

namespace {
	//武器が収納状態の時の座標
	const Vector3 STOWEDS_POSITION = { 0.0f,-500.0f,0.0f };

	const Vector3 BIG_SWORD_COLLISION_SIZE = { 18.0f,100.0f,8.0f };

	const float SKILL_RADIUS = 80.0f;

	const float ADD_FORWARD = 8.0f;

	const float POWER = 40;

	const float HITTABLE_TIME = 0.5f;
}

BigSword::BigSword()
{
	SetWeaponPower(POWER);
}

BigSword::~BigSword()
{
	DeleteGO(m_bigSwordCollision);
}

bool BigSword::Start()
{
	//勇者のインスタンスを探す
	m_brave = FindGO<Brave>("brave");

	InitModel();
	InitCollision();

	//装備
	//SetWeaponState(enWeaponState_Stowed);
	SetWeaponState(enWeaponState_Armed);
	SetStowedFlag(false);
	//防御タイプの設定
	SetEnDefendTipe(enDefendTipe_avoid);

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

	//ノックバックの設定
	m_brave->SetKnockBackInfo(
		m_skillAttackPosition,
		m_knockBackPower,
		true
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

void BigSword::Render(RenderContext& rc)
{
	if (GetWeaponState() == enWeaponState_Stowed)
	{
		return;
	}
	m_modelBigSword.Draw(rc);
}


