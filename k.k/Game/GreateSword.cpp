#include "stdafx.h"
#include "GreateSword.h"
#include "Brave.h"



namespace {


	const Vector3 SWORD_COLLISION_SIZE = { 100.0f,30.0f,10.0f };
	
}

GreateSword::GreateSword()
{
}

GreateSword::~GreateSword()
{
	DeleteGO(m_swordCollision);
}

bool GreateSword::Start()
{
	m_brave = FindGO<Brave>("Brave");

	//初期化処理
	Init();

	return true;
}

void GreateSword::Update()
{
	if (m_enWeaponState == enArmed)
	{
		MoveArmed();
	}


	m_swordModelRender.Update();

	m_swordCollision->Update();

}

void GreateSword::Init()
{
	//剣モデルの読み込み
	m_swordModelRender.Init("Assets/modelData/character/Player/NewHero/GreateSword.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0, 0, enModelUpAxisZ);
	//剣の中心のボーンIDを取得
	m_swordCenterBoonId = m_swordModelRender.FindBoneID(L"Center");

	//剣と盾の座標に対応するボーンIDを取得
	//装備状態の時のボーンID
	m_armedSwordBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_l");



	InitCollision();

	//武器が収納状態なら
	if (m_enWeaponState == enStowed)
	{
		ChangeStowedState();
	}
	//武器が装備状態なら
	else
	{
		ChangeArmedState();
	}

}

void GreateSword::ChangeStowedState()
{
	SetCurrentWeaponState(enStowed);
	m_swordModelRender.SetPosition(m_stowedPosition);

	if (m_swordCollision != nullptr) m_swordCollision->SetPosition(m_stowedPosition);
}

void GreateSword::ChangeArmedState()
{
	SetCurrentWeaponState(enArmed);
}

void GreateSword::InitCollision()
{
	//剣の当たり判定
	m_swordCollision =
		NewGO<CollisionObject>
		(0, g_collisionObjectManager->m_attackCollisionName
		);

	m_swordCollision->CreateBox(
		m_stowedPosition,
		Quaternion::Identity,
		SWORD_COLLISION_SIZE
	);

	//当たり判定を自動で削除しないようにする
	m_swordCollision->SetIsEnableAutoDelete(false);
	//当たり判定を無効化
	m_swordCollision->SetIsEnable(false);

}

void GreateSword::AttackAction()
{
}

void GreateSword::MoveArmed()
{
	Vector3 swordPos = g_vec3Zero;
	//剣のワールド座標を設定
	m_swordMatrix =
		m_brave->GetModelRender().GetBone(m_armedSwordBoonId)->GetWorldMatrix();
	m_swordModelRender.SetWorldMatrix(m_swordMatrix);


	//コリジョンのワールド座標を設定
	//剣の中心のボーンからワールド座標を取得してコリジョンのワールド座標に設定
	Matrix matrix;
	matrix = m_swordModelRender.GetBone(m_swordCenterBoonId)->GetWorldMatrix();
	m_swordCollision->SetWorldMatrix(matrix);

}

void GreateSword::Render(RenderContext& rc)
{
	m_swordModelRender.Draw(rc);
}
