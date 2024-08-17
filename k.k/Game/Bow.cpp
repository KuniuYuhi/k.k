#include "stdafx.h"
#include "Bow.h"
#include "Brave.h"
#include "PlayerMovement.h"
#include "PlayerController.h"

namespace {
	


}

Bow::Bow()
{
}

Bow::~Bow()
{
}

bool Bow::Start()
{
	m_brave = FindGO<Brave>("Brave");
	//プレイヤーコントローラーコンポーネントを取得
	m_playerController = m_brave->GetComponent<PlayerController>();
	//共通ステータスの初期化
	m_status.InitWeaponCommonStatus("BowArrow");
	//固有ステータスの初期化
	m_uniqueStatus.InitUniqueStatus();

	//初期化処理
	Init();

	return true;
}

void Bow::Update()
{
	if(m_enWeaponState==enArmed)
	{
		MoveArmed();
	}



	m_bowModelRender.Update();

}

void Bow::Init()
{
	//弓モデルの初期化
	m_bowModelRender.Init("Assets/modelData/character/Player/NewHero/Bow.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,0,enModelUpAxisZ
	);

	//弓と矢の座標に対応するボーンIDを取得
	//装備状態の時のボーンID
	m_armedBowBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_l");
	m_armedArrowBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_r");

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

void Bow::ChangeStowedState()
{
	SetCurrentWeaponState(enStowed);
	//収納時の座標に変更
	m_bowModelRender.SetPosition(m_stowedPosition);

}

void Bow::ChangeArmedState()
{
	SetCurrentWeaponState(enArmed);
}

void Bow::AttackAction()
{
}

void Bow::ProceedComboAttack()
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

void Bow::ResetComboAttack()
{
	//コンボステートをなしにリセットする
	m_enComboState = enCombo_None;
}

bool Bow::IsEndDefensiveAction()
{

	//アニメーションが終わったら
	if (m_brave->GetModelRender().IsPlayingAnimation() == false)
	{
		//回避アクションを終わる
		return true;
	}

	return false;
}

void Bow::EntryDefensiveActionProcess()
{
	//回避時に移動する方向を決める
	m_defensiveActionDirection = m_brave->
		GetPlayerMovement()->CalcForwardDirection(
			m_brave->GetForward(),
			m_brave->GetMoveSpeed()
		);

	//
	m_defensiveActionDirection.x *= m_uniqueStatus.GetDefenciveMoveSpeed();
	m_defensiveActionDirection.z *= m_uniqueStatus.GetDefenciveMoveSpeed();
}

void Bow::UpdateDefensiveActionProcess()
{
	m_brave->CharaConExecute(m_defensiveActionDirection);
}

void Bow::EntryNormalAttackProcess(EnComboState comboState)
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

void Bow::UpdateNormalAttackProcess(EnComboState comboState)
{
	//移動フラグが立っている間は移動
	if (IsAttackActionMove())
	{
		m_brave->CharaConExecute(m_normalAttackMoveDirection);
	}
}

void Bow::ExitNormalAttackProcess(EnComboState comboState)
{
	//
	SetAttackActionMove(false);
}

void Bow::EntrySkillAttackProcess(EnSkillProcessState skillProcessState)
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

void Bow::UpdateSkillAttackProcess(EnSkillProcessState skillProcessState)
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

void Bow::ExitSkillAttackProcess(EnSkillProcessState skillProcessState)
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

void Bow::EntrySkillStartProcess()
{
	m_skillChargeTimer = 0.0f;
}

void Bow::UpdateSkillStartProcess()
{
	//ボタンを押している間はチャージする
	if (m_playerController->IsPressSkillAttackButton())
	{
		//
		SkillChargeTimeProcess();
	}
	else
	{


		//メインステートに遷移
		m_brave->ChangeBraveState(BraveState::enBraveState_SkillMain);
	}
}

void Bow::ExitSkillStartProcess()
{
}

void Bow::EntrySkillMainProcess()
{
}

void Bow::UpdateSkillMainProcess()
{
}

void Bow::ExitSkillMainProcess()
{
}

void Bow::SkillChargeTimeProcess()
{
	//タイマーを加算
	m_skillChargeTimer += g_gameTime->GetFrameDeltaTime();

	//一段階目のチャージが完了したら

}

void Bow::MoveArmed()
{
	Vector3 swordPos = g_vec3Zero;
	//弓のワールド座標を設定
	m_bowMatrix = m_brave->GetModelRender().GetBone(m_armedBowBoonId)->GetWorldMatrix();
	m_bowModelRender.SetWorldMatrix(m_bowMatrix);

}

void Bow::Render(RenderContext& rc)
{
	m_bowModelRender.Draw(rc);
}
