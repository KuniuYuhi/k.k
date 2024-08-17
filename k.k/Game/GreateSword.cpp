#include "stdafx.h"
#include "GreateSword.h"
#include "Brave.h"
#include "PlayerMovement.h"
#include "PlayerController.h"

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

	m_playerController = m_brave->GetComponent<PlayerController>();
	m_playerMovement = m_brave->GetComponent<PlayerMovement>();

	//共通ステータスを初期化
	m_status.InitWeaponCommonStatus("GreateSword");
	//固有ステータスを初期化
	m_uniqueStatus.InitUniqueStatus();

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

void GreateSword::ProceedComboAttack()
{
	//３コンボ以上なら
	if (m_enComboState >= enCombo_Third)
	{
		//コンボステートをなしにリセットする
		m_enComboState = enCombo_None;
	}

	//コンボを一つ進める
	m_enComboState = static_cast<EnComboState>(m_enComboState + 1);
}

void GreateSword::ResetComboAttack()
{
	//コンボステートをなしにリセットする
	m_enComboState = enCombo_None;
}

bool GreateSword::IsEndDefensiveAction()
{
	//アニメーションが終わったら
	if (m_brave->GetModelRender().IsPlayingAnimation() == false)
	{
		//回避アクションを終わる
		return true;
	}

	return false;
}

void GreateSword::EntryDefensiveActionProcess()
{
	//回避時に移動する方向を決める
	m_defensiveActionDirection = 
		m_playerMovement->CalcForwardDirection(
			m_brave->GetForward(),
			m_brave->GetMoveSpeed()
		);

	//
	m_defensiveActionDirection.x *= m_uniqueStatus.GetDefenciveMoveSpeed();
	m_defensiveActionDirection.z *= m_uniqueStatus.GetDefenciveMoveSpeed();
}

void GreateSword::UpdateDefensiveActionProcess()
{
	
	m_brave->CharaConExecute(m_defensiveActionDirection);
}

void GreateSword::EntryNormalAttackProcess(EnComboState comboState)
{
	//移動方向を前方向か入力方向か計算する
	m_normalAttackMoveDirection =
		m_playerMovement->CalcMoveDirection(
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

void GreateSword::UpdateNormalAttackProcess(EnComboState comboState)
{
	//移動フラグが立っている間は移動
	if (IsAttackActionMove())
	{
		m_brave->CharaConExecute(m_normalAttackMoveDirection);
	}
}

void GreateSword::ExitNormalAttackProcess(EnComboState comboState)
{
	//
	SetAttackActionMove(false);
}

void GreateSword::EntrySkillAttackProcess(EnSkillProcessState skillProcessState)
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

void GreateSword::UpdateSkillAttackProcess(EnSkillProcessState skillProcessState)
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

void GreateSword::ExitSkillAttackProcess(EnSkillProcessState skillProcessState)
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

void GreateSword::EntrySkillStartProcess()
{
	//滞空時間タイマーをリセット
	m_skillFlightTimer = 0.0f;
}

void GreateSword::UpdateSkillStartProcess()
{
	//少し時間が経ったらメインステートに遷移
	if (IsSkillFlightTimeOver())
	{
		//アニメーションが終わったら
		if (m_brave->GetModelRender().IsPlayingAnimation() == false)
		{
			//メインステートに遷移
			m_brave->ChangeBraveState(BraveState::enBraveState_SkillMain);
		}
	}
	else
	{
		//移動できる
		Vector3 moveSpeed = m_playerMovement->CalcSimpleMovementVerocity(
			100.0f,
			m_brave->GetMoveSpeed(),
			m_playerController->GetLStickInput()
		);
		//キャラコンで移動
		m_brave->CharaConExecute(moveSpeed);
		//回転方向を設定する
		m_brave->SetRotateDirection(moveSpeed);
	}
}

void GreateSword::ExitSkillStartProcess()
{
}

void GreateSword::EntrySkillMainProcess()
{
}

void GreateSword::UpdateSkillMainProcess()
{
}

void GreateSword::ExitSkillMainProcess()
{
}

bool GreateSword::IsSkillFlightTimeOver()
{
	if (m_skillFlightTimer >= m_uniqueStatus.GetSkillFlightTimeLimit())
	{
		return true;
	}
	//タイマーを加算
	m_skillFlightTimer += g_gameTime->GetFrameDeltaTime();
	return false;
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
