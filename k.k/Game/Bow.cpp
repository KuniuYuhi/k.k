#include "stdafx.h"
#include "Bow.h"
#include "Brave.h"
#include "PlayerMovement.h"
#include "PlayerController.h"

#include "Arrow.h"

#include "DamageProvider.h"
#include "KnockBackInfoManager.h"

namespace {
	


}

Bow::Bow()
{
}

Bow::~Bow()
{
	if (m_arrow != nullptr)
	{
		DeleteGO(m_arrow);
	}

}

bool Bow::Start()
{
	m_brave = FindGO<Brave>("Brave");
	//プレイヤーコントローラーコンポーネントを取得
	m_playerController = m_brave->GetComponent<PlayerController>();

	m_playerMovement = m_brave->GetComponent<PlayerMovement>();
	//共通ステータスの初期化
	m_status.InitWeaponCommonStatus("BowArrow");
	//固有ステータスの初期化
	m_uniqueStatus.InitUniqueStatus();

	//初期化処理
	Init();

	

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

	//矢を生成
	CreateArrow(m_enWeaponState);

	//アニメーションイベント用の関数を設定する。
	m_brave->GetModelRender().AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
	});

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

}

void Bow::ChangeStowedState()
{
	SetCurrentWeaponState(enStowed);
	//収納時の座標に変更
	m_bowModelRender.SetPosition(m_stowedPosition);
	//矢も収納状態に変更
	if (m_arrow == nullptr) return;
	m_arrow->ChangeStowed();
}

void Bow::ChangeArmedState()
{
	SetCurrentWeaponState(enArmed);
	//矢も装備状態に変更
	if (m_arrow == nullptr) return;
	m_arrow->ChangeArmed();
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
	m_defensiveActionDirection = m_playerMovement->CalcMoveDirection(
		m_brave->GetForward(),
		m_playerController->GetLStickInput(),
		m_brave->GetMoveSpeed()
	);

	//プレイヤーの回転方向に移動方向を設定する
	m_brave->SetRotateDirection(m_defensiveActionDirection);
	m_brave->SetForward(m_defensiveActionDirection);

	m_defensiveActionDirection.y = 0.0f;

	//回避速度をかける
	m_defensiveActionDirection *= m_uniqueStatus.GetDefenciveMoveSpeed();

	//回避中は無敵
	m_brave->EnableInvincible();
}

void Bow::UpdateDefensiveActionProcess()
{
	//回避中の移動可能フラグが立っていたら
	if (IsDefensiveActionMove())
	{
		m_brave->CharaConExecute(m_defensiveActionDirection);
	}	

	//回避アクションを終わるなら
	if (IsEndDefensiveAction())
	{
		//ステートの共通処理
		m_brave->ProcessCommonStateTransition();
	}
}

void Bow::ExitDefensiveActionProcess()
{
	//無敵を解除
	m_brave->DisableInvincible();
}

bool Bow::CanDefensiveAction()
{
	//回避に必要なスタミナを消費できるなら
	if (m_brave->GetStatus().TryConsumeStamina(m_status.GetDefensiveStaminaCost()))
	{
		//回避可能
		return true;
	}
	//不可能
	return false;
}

bool Bow::CanSkillAttack()
{
	//スキルに必要なスタミナを消費できるなら
	//チャージするタイプなので攻撃した後に消費する
	if (m_brave->GetStatus().CheckConsumeStamina(m_status.GetSkillStaminaCost()))
	{
		//スキル攻撃可能
		return true;
	}
	//不可能
	return false;
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

void Bow::EntryNormalAttackProcess(EnComboState comboState)
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
	m_brave->SetForward(m_normalAttackMoveDirection);

	//コンボステートを番号に変換する
	m_comboNumber = ConvertComboStateToNumber(comboState);

	//武器ステータスから攻撃スピードを取得して方向にかける
	m_normalAttackMoveDirection *= m_uniqueStatus.GetNormalAttackSpeed(m_comboNumber);

	//通常攻撃待機区間フラグをリセット
	SetStandbyPeriodFlag(false);
	//キャンセルアクションフラグを立てる。(キャンセルアクションできる)
	m_isImpossibleancelAction = true;

	
}

void Bow::UpdateNormalAttackProcess(EnComboState comboState)
{
	//キャンセルアクションできる状態なら回避も可能
	if (m_isImpossibleancelAction && 
		IsStandbyPeriod() &&
		m_playerController->IsPressDefensiveActionButton())
	{
		//防御ステートに切り替える
		m_brave->ChangeBraveState(enBraveState_DefensiveActions);
		return;
	}

	//移動フラグが立っている間は移動
	if (IsAttackActionMove())
	{
		//矢を放ったときの反動の移動
		m_brave->CharaConExecute(m_normalAttackMoveDirection);
	}
}

void Bow::ExitNormalAttackProcess(EnComboState comboState)
{
	//攻撃中の移動フラグをリセット
	SetAttackActionMove(false);
	//通常攻撃待機区間フラグをリセット
	SetStandbyPeriodFlag(false);

	if (m_arrow == nullptr)
	{
		//矢を生成
		CreateArrow(m_enWeaponState);
	}
}

void Bow::EntrySkillStartProcess()
{
	m_skillChargeTimer = 0.0f;
	//チャージ段階をリセット
	m_uniqueStatus.SetSkillChargeStage(BowArrowStatus::enStage_1);
	//スキル攻撃可能かフラグをリセット
	m_ispossibleSkillAttack = false;
}

void Bow::UpdateSkillStartProcess()
{
	//ボタンを押している間はチャージする
	if (m_playerController->IsPressSkillAttackButton())
	{
		//チャージ中の処理をする
		SkillChargeTimeProcess();

		
		//移動はできないけど回転だけできるようにする
		Vector3 moveSpeed = m_playerMovement->CalcSimpleMovementVerocity(
			100.0f,
			m_brave->GetMoveSpeed(),
			m_playerController->GetLStickInput()
		);
		//回転方向を設定する
		m_brave->SetRotateDirection(moveSpeed);

	}
	else
	{
		//一段階以上チャージしていたら
		if (m_uniqueStatus.GetCurrentSkillChargeStage() >= BowArrowStatus::enStage_2)
		{
			//メインステートに遷移
			m_brave->ChangeBraveState(BraveState::enBraveState_SkillMain);
			//スキル攻撃できるのでフラグを立てる
			m_ispossibleSkillAttack = true;
			return;
		}

		//スキルを終わる(攻撃は不発)
		m_brave->ProcessCommonStateTransition();
		//スキル攻撃できないのでフラグを下げる
		m_ispossibleSkillAttack = false;
	}
}

void Bow::ExitSkillStartProcess()
{
	//スキル攻撃しなかった場合は
	if (!m_ispossibleSkillAttack)
	{
		//アクションを終わる
		m_brave->ActionDeactive();
	}

}

void Bow::EntrySkillMainProcess()
{
	//攻撃すること確定なのでスタミナを消費する
	m_brave->GetStatus().TryConsumeStamina(m_status.GetSkillStaminaCost());

	//メインに進んだので無敵にする
	m_brave->EnableInvincible();
}

void Bow::UpdateSkillMainProcess()
{
}

void Bow::ExitSkillMainProcess()
{
	//攻撃時の移動フラグをリセット
	SetAttackActionMove(false);

	if (m_arrow == nullptr)
	{
		//矢を生成
		CreateArrow(m_enWeaponState);
	}

	//無敵を無効化する
	m_brave->DisableInvincible();
}

void Bow::SkillChargeTimeProcess()
{
	//チャージ段階がマックスなら処理しない
	if (m_uniqueStatus.GetCurrentSkillChargeStage() == BowArrowStatus::enStage_max)
	{
		return;
	}

	//タイマーを加算
	m_skillChargeTimer += g_gameTime->GetFrameDeltaTime();

	//チャージ時間に達したら
	if (m_skillChargeTimer >= m_uniqueStatus.GetSkillChargeCompletionTime(m_uniqueStatus.GetCurrentSkillChargeStage()))
	{
		//チャージエフェクト生成


		//次の段階にする
		m_uniqueStatus.SetSkillChargeStage(
			static_cast<BowArrowStatus::EnSkillChargeStage>(m_uniqueStatus.GetCurrentSkillChargeStage() + 1)
		);
	}

}

void Bow::CreateArrow(EnWeaponState weaponState)
{
	//矢の名前を決める。
	//当たり判定を取りやすくするために名前の後にIDを設定
	const char* name = "Arrow" + m_arrowNameId;

	m_arrow = NewGO<Arrow>(0, name);
	//弓のインスタンスを設定
	m_arrow->SetBowInstance(this);

	//矢を弓の状態と同じ状態にする
	if (weaponState == enStowed)
	{
		m_arrow->ChangeStowed();
	}
	else
	{
		m_arrow->ChangeArmed();
	}

	//IDを加算
	m_arrowNameId++;
	//一定の数を超えるとIDをリセット
	if (m_arrowNameId > 100)
	{
		m_arrowNameId = -1;
	}
}

void Bow::ShotNromalAttackArrow()
{
	if (m_arrow == nullptr) return;
	//放つ時のパラメータの設定
	m_arrow->SetShotArrowParameters(Arrow::enNormalShot, m_brave->GetForward());

	//矢を放ったので矢自体にダメージ情報を設定
	m_arrow->GetDamageProvider()->SetDamageInfo(
		KnockBackInfoManager::GetInstance()->GetAddAttackId(), m_brave->GetCurrentPower(),
		m_uniqueStatus.GetAttackTimeScale(m_comboNumber),
		m_status.GetComboKnockBackPattern(static_cast<WeaponStatus::EnCombo>(m_comboNumber)),
		m_status.GetWeaponAttribute()
	);

	//矢を放ったので矢を持っていない状態にする
	m_arrow = nullptr;
}

void Bow::ShotSkillAttackArrow()
{
	if (m_arrow == nullptr) return;
	//放つ時のパラメータの設定
	m_arrow->SetShotArrowParameters(Arrow::enSkillShot, m_brave->GetForward());

	//矢を放ったので矢自体にダメージ情報を設定
	m_arrow->GetDamageProvider()->SetDamageInfo(
		KnockBackInfoManager::GetInstance()->GetAddAttackId(), m_brave->GetCurrentPower(),
		m_uniqueStatus.SkillAttackTimeScale(),
		m_status.GetSkillKnockBackPattern(),
		m_status.GetWeaponAttribute()
	);

	//スキル攻撃に必要な情報を設定
	m_arrow->SetSkillShotInfo(
		m_brave->GetCurrentPower(),m_uniqueStatus.GetAttackInfoUpdateInterval()
	);

	//矢を放ったので矢を持っていない状態にする
	m_arrow = nullptr;
}

void Bow::MoveArmed()
{
	//弓のワールド座標を設定
	m_bowMatrix = m_brave->GetModelRender().GetBone(m_armedBowBoonId)->GetWorldMatrix();
	m_bowModelRender.SetWorldMatrix(m_bowMatrix);

}

void Bow::Render(RenderContext& rc)
{
	m_bowModelRender.Draw(rc);
}

void Bow::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{

	//通常攻撃で矢を放つアニメーションキーフレーム
	if (wcscmp(eventName, L"LongRangeAttack") == 0)
	{
		//矢を放つ
		ShotNromalAttackArrow();
	}

	//スキル攻撃で矢を放つアニメーションキーフレーム
	if (wcscmp(eventName, L"SkillShot") == 0)
	{
		ShotSkillAttackArrow();
	}


}
