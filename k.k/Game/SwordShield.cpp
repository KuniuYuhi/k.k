#include "stdafx.h"
#include "SwordShield.h"
#include "Brave.h"
#include "PlayerController.h"
#include "PlayerMovement.h"


namespace {

	const Vector3 SWORD_COLLISION_SIZE = { 90.0f,20.0f,10.0f };
	const Vector3 SHIELD_COLLISION_SIZE = { 28.0f,50.0f,16.0f };
}

SwordShield::SwordShield()
{
}

SwordShield::~SwordShield()
{
	DeleteGO(m_swordCollision);
	DeleteGO(m_shieldCollision);


}

bool SwordShield::Start()
{

	m_brave = FindGO<Brave>("Brave");
	//プレイヤーコントローラーコンポーネントを取得
	m_playerController = m_brave->GetComponent<PlayerController>();
	//共通ステータスを初期化
	m_status.InitWeaponCommonStatus("SwordShield");
	//固有ステータスの初期化
	m_uniqueStatus.InitUniqueStatus();
	
	//初期化処理
	Init();

	return true;
}

void SwordShield::Update()
{


	if (m_enWeaponState == enArmed)
	{
		MoveArmed();
	}



	m_swordModelRender.Update();
	m_shieldModelRender.Update();

	m_swordCollision->Update();
	m_shieldCollision->Update();

}


void SwordShield::Init()
{
	//剣モデルの読み込み
	m_swordModelRender.Init("Assets/modelData/character/Player/NewHero/OneHandSword2.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,0,enModelUpAxisZ);
	//剣の中心のボーンIDを取得
	m_swordCenterBoonId = m_swordModelRender.FindBoneID(L"Center");


	//盾モデルの読み込み
	m_shieldModelRender.Init("Assets/modelData/character/Player/NewHero/Shield.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0, 0, enModelUpAxisZ);


	//剣と盾の座標に対応するボーンIDを取得
	//装備状態の時のボーンID
	m_armedSwordBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_r");
	m_armedShieldBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_l");


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

void SwordShield::ChangeStowedState()
{
	SetCurrentWeaponState(enStowed);

	m_swordModelRender.SetPosition(m_stowedPosition);
	m_shieldModelRender.SetPosition(m_stowedPosition);

	if(m_swordCollision != nullptr) m_swordCollision->SetPosition(m_stowedPosition);

	if (m_shieldCollision != nullptr) m_shieldCollision->SetPosition(m_stowedPosition);

}

void SwordShield::ChangeArmedState()
{
	SetCurrentWeaponState(enArmed);
}

void SwordShield::InitCollision()
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


	//盾の当たり判定
	m_shieldCollision = NewGO<CollisionObject>(0, "defence");
	m_shieldCollision->CreateBox(
		m_stowedPosition,
		Quaternion(0.0f, 90.0f, 180.0f, 1.0f),
		SHIELD_COLLISION_SIZE
	);
	m_shieldCollision->SetIsEnableAutoDelete(false);
	m_shieldCollision->SetIsEnable(false);
}

void SwordShield::AttackAction()
{



}

void SwordShield::ProceedComboAttack()
{
	//３コンボ以上なら
	if(m_enComboState >= enCombo_Third)
	{
		//コンボステートをなしにリセットする
		m_enComboState = enCombo_None;
	}

	//コンボを一つ進める
	m_enComboState= static_cast<EnComboState>(m_enComboState + 1);

	
}

void SwordShield::ResetComboAttack()
{
	//コンボステートをなしにリセットする
	m_enComboState = enCombo_None;
}

bool SwordShield::IsEndDefensiveAction()
{
	//防御ボタンを話したら
	if (!m_playerController->IsPressDefensiveActionButton())
	{
		//防御アクションを終わる
		return true;
	}



	return false;
}

void SwordShield::EntryDefensiveActionProcess()
{
	//エフェクト生成
}

void SwordShield::UpdateDefensiveActionProcess()
{
}

void SwordShield::ExitDefensiveActionProcess()
{
	//エフェクト削除
}

void SwordShield::EntryNormalAttackProcess(EnComboState comboState)
{
	//移動方向を前方向か入力方向か計算する
	m_normalAttackMoveDirection = 
	m_brave->GetPlayerMovement()->CalcMoveDirection(
		m_brave->GetForward(),
		m_playerController->GetLStickInput(),
		m_brave->GetMoveSpeed()
	);

	//敵の位置も踏まえて向く方向を決める
	m_normalAttackMoveDirection = CalcAutoAimAtTarget(
		m_brave->GetPosition(),
		m_normalAttackMoveDirection,
		m_uniqueStatus.GetNormalAttackSearchRadius(),
		m_uniqueStatus.GetNormalAttackComparisonDot()
	);

	//プレイヤーの回転方向に移動方向を設定する
	m_brave->SetRotateDirection(m_normalAttackMoveDirection);

	int comboNum = 0;
	switch (comboState)
	{
	case WeaponBase::enCombo_First:
		comboNum = 0;
		break;
	case WeaponBase::enCombo_Second:
		comboNum = 1;
		break;
	case WeaponBase::enCombo_Third:
		comboNum = 2;
		break;
	default:
		std::abort();
		break;
	}

	//武器ステータスから攻撃スピードを取得して方向にかける
	m_normalAttackMoveDirection *= m_uniqueStatus.GetNormalAttackSpeed(comboNum);
}

void SwordShield::UpdateNormalAttackProcess(EnComboState comboState)
{
	//移動フラグが立っている間は移動
	if (IsAttackActionMove())
	{
		m_brave->CharaConExecute(m_normalAttackMoveDirection);
	}

}

void SwordShield::ExitNormalAttackProcess(EnComboState comboState)
{
	//
	SetAttackActionMove(false);
}

void SwordShield::EntrySkillAttackProcess(EnSkillProcessState skillProcessState)
{
	switch (skillProcessState)
	{
	case WeaponBase::enStart:
		EntrySkillStartProcess();
		break;
	case WeaponBase::enMain:
		EntrySkillMainProcess();
		break;
	default:
		break;
	}
}

void SwordShield::UpdateSkillAttackProcess(EnSkillProcessState skillProcessState)
{
	switch (skillProcessState)
	{
	case WeaponBase::enStart:
		UpdateSkillStartProcess();
		break;
	case WeaponBase::enMain:
		UpdateSkillMainProcess();
		break;
	default:
		break;
	}
}

void SwordShield::ExitSkillAttackProcess(EnSkillProcessState skillProcessState)
{
	switch (skillProcessState)
	{
	case WeaponBase::enStart:
		ExitSkillStartProcess();
		break;
	case WeaponBase::enMain:
		ExitSkillMainProcess();
		break;
	default:
		break;
	}
}

void SwordShield::EntrySkillStartProcess()
{
}

void SwordShield::UpdateSkillStartProcess()
{
	//アニメーションが終わったら
	if (m_brave->GetModelRender().IsPlayingAnimation() == false)
	{
		//メインステートに遷移
		m_brave->ChangeBraveState(BraveState::enBraveState_SkillMain);
	}
}

void SwordShield::ExitSkillStartProcess()
{
}

void SwordShield::EntrySkillMainProcess()
{
}

void SwordShield::UpdateSkillMainProcess()
{
}

void SwordShield::ExitSkillMainProcess()
{
}

void SwordShield::MoveArmed()
{
	Vector3 swordPos = g_vec3Zero;
	//剣のワールド座標を設定
	m_swordMatrix =
		m_brave->GetModelRender().GetBone(m_armedSwordBoonId)->GetWorldMatrix();
	m_swordModelRender.SetWorldMatrix(m_swordMatrix);

	//盾のワールド座標を設定
	m_shieldMatrix =
		m_brave->GetModelRender().GetBone(m_armedShieldBoonId)->GetWorldMatrix();
	m_shieldModelRender.SetWorldMatrix(m_shieldMatrix);

	//コリジョンのワールド座標を設定
	//剣の中心のボーンからワールド座標を取得してコリジョンのワールド座標に設定
	Matrix matrix;
	matrix = m_swordModelRender.GetBone(m_swordCenterBoonId)->GetWorldMatrix();
	m_swordCollision->SetWorldMatrix(matrix);
	//盾のワールド座標を設定
	m_shieldCollision->SetWorldMatrix(m_shieldMatrix);
}


void SwordShield::Render(RenderContext& rc)
{
	m_swordModelRender.Draw(rc);
	m_shieldModelRender.Draw(rc);

}