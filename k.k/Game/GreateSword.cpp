#include "stdafx.h"
#include "GreateSword.h"
#include "Brave.h"
#include "PlayerMovement.h"
#include "PlayerController.h"
#include "DamageProvider.h"

#include "KnockBackInfoManager.h"

#include "UseEffect.h"

namespace {


	const Vector3 SWORD_COLLISION_SIZE = { 100.0f,30.0f,10.0f };

	const float SKILL_ATTACK_RADIUS = 100.0f;		//スキル攻撃用コリジョンの半径
	
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

	//アニメーションイベント用の関数を設定する。
	m_brave->GetModelRender().AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
	});

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

void GreateSword::DeleteThis()
{
	DeleteGO(this);
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
		(0, g_collisionObjectManager->m_playerAttackCollisionName
		);
	//コリジョンの制作者をプレイヤーに設定
	m_swordCollision->SetCreatorName(m_brave->GetName());
	//コリジョンの形状はボックス
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

void GreateSword::CreateSkillAttackCollision()
{
	//スキル攻撃用コリジョン作成
	
	Vector3 hitPosition = g_vec3Zero;

	m_swordCenterMatrix.Apply(hitPosition);
	hitPosition.y = 0.0f;

	//スキル攻撃時の当たり判定の生成
	//自動で消える
	auto skillCollision = NewGO<CollisionObject>(0, g_collisionObjectManager->m_playerAttackCollisionName);
	//コリジョンの制作者をプレイヤーに設定
	skillCollision->SetCreatorName(m_brave->GetName());
	//コリジョンの形状は球
	skillCollision->CreateSphere(
		hitPosition,
		g_quatIdentity,
		SKILL_ATTACK_RADIUS
	);
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

void GreateSword::UpdateDefensiveActionProcess()
{
	//回避中の移動可能フラグが立っていたら
	if (IsDefensiveActionMove())
	{
		//回避中の移動処理
		m_brave->CharaConExecute(m_defensiveActionDirection);
	}

	//回避アクションを終わるなら
	if (IsEndDefensiveAction())
	{
		//ステートの共通処理
		m_brave->ProcessCommonStateTransition();
	}

}

void GreateSword::ExitDefensiveActionProcess()
{
	//スタミナを消費する
	m_brave->GetStatus().TryConsumeStamina(m_status.GetDefensiveStaminaCost());
	//無敵を解除
	m_brave->DisableInvincible();
}

bool GreateSword::CanDefensiveAction()
{
	//回避に必要なスタミナを消費できるかチェック
	if (m_brave->GetStatus().CheckConsumeStamina(m_status.GetDefensiveStaminaCost()))
	{
		//回避可能
		return true;
	}

	//スタミナが不足しているのでフラグを立てる
	m_brave->SetStaminaInsufficientFlag(true);
	//不可能
	return false;
}

bool GreateSword::CanSkillAttack()
{
	//スキルに必要なスタミナを消費できるかチェック
	if (m_brave->GetStatus().CheckConsumeStamina(m_status.GetSkillStaminaCost()))
	{
		//スキル攻撃可能
		return true;
	}

	//スタミナが不足しているのでフラグを立てる
	m_brave->SetStaminaInsufficientFlag(true);
	//不可能
	return false;
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
	m_brave->SetForward(m_normalAttackMoveDirection);

	//コンボステートを番号に変換する
	int comboNum = ConvertComboStateToNumber(comboState);

	//武器ステータスから攻撃スピードを取得して方向にかける
	m_normalAttackMoveDirection *= m_uniqueStatus.GetNormalAttackSpeed(comboNum);

	//通常攻撃待機区間フラグをリセット
	SetStandbyPeriodFlag(false);
	//キャンセルアクションフラグを立てる。(キャンセルアクションできる)
	m_isImpossibleancelAction = true;

	//ダメージ情報を設定
	m_brave->GetDamageProvider()->SetDamageInfo(
		KnockBackInfoManager::GetInstance()->GetAddAttackId(), m_brave->GetCurrentPower(),
		m_uniqueStatus.GetAttackTimeScale(comboNum),
		m_status.GetComboKnockBackPattern(static_cast<WeaponStatus::EnCombo>(comboNum)),
		m_status.GetWeaponAttribute()
	);

}

void GreateSword::UpdateNormalAttackProcess(EnComboState comboState)
{
	//キャンセルアクションできる状態なら回避も可能
	//三コンボ目は回避不可能
	if (m_isImpossibleancelAction && 
		IsStandbyPeriod() &&
		m_playerController->IsPressDefensiveActionButton()&&
		comboState!=enCombo_Third)
	{
		//防御ステートに切り替える
		m_brave->ChangeBraveState(enBraveState_DefensiveActions);
		return;
	}

	//移動フラグが立っている間は移動
	if (IsAttackActionMove())
	{
		m_brave->CharaConExecute(m_normalAttackMoveDirection);
	}
}

void GreateSword::ExitNormalAttackProcess(EnComboState comboState)
{
	//攻撃中の移動フラグをリセット
	SetAttackActionMove(false);
	//通常攻撃待機区間フラグをリセット
	SetStandbyPeriodFlag(false);
	//一応当たり判定を無効化しておく
	m_swordCollision->SetIsEnable(false);
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

void GreateSword::AttackImpactProcess(bool startOrEnd)
{
	//キャンセルアクションフラグを設定
	m_isImpossibleancelAction = startOrEnd;

	//当たり判定の有効化、無効化の設定
	m_swordCollision->SetIsEnable(startOrEnd);
}

void GreateSword::EntrySkillStartProcess()
{
	//滞空時間タイマーをリセット
	m_skillFlightTimer = 0.0f;
	//スキル中移動できるかフラグをリセット
	isMoveSkillAttack = false;
	//スキルメインステートに進むかのフラグをリセット
	m_brave->SetProceedSkillMainFlag(false);
}

void GreateSword::UpdateSkillStartProcess()
{
	//少し時間が経ったらメインステートに遷移
	if (IsSkillFlightTimeOver())
	{
		//アニメーションが終わったら
		if (m_brave->GetModelRender().IsPlayingAnimation() == false)
		{
			//スキルメインステートに進むよ
			m_brave->SetProceedSkillMainFlag(true);
			//メインステートに遷移
			m_brave->ChangeBraveState(BraveState::enBraveState_SkillMain);
		}
	}
	else
	{
		//移動できるタイミングでないなら処理しない
		if (!isMoveSkillAttack) return;

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
	//スキル中移動できるかフラグをリセット
	isMoveSkillAttack = false;
}

void GreateSword::EntrySkillMainProcess()
{
	//ダメージ情報を設定
	m_brave->GetDamageProvider()->SetDamageInfo(
		KnockBackInfoManager::GetInstance()->GetAddAttackId(), m_brave->GetCurrentPower(),
		m_uniqueStatus.SkillAttackTimeScale(),
		m_status.GetSkillKnockBackPattern(),
		m_status.GetWeaponAttribute()
	);

	//メインに進んだので無敵にする
	m_brave->EnableInvincible();
}

void GreateSword::UpdateSkillMainProcess()
{
}

void GreateSword::ExitSkillMainProcess()
{
	//スタミナを消費する
	m_brave->GetStatus().TryConsumeStamina(m_status.GetSkillStaminaCost());
	//無敵を無効化する
	m_brave->DisableInvincible();
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
	m_swordCenterMatrix = m_swordModelRender.GetBone(m_swordCenterBoonId)->GetWorldMatrix();
	m_swordCollision->SetWorldMatrix(m_swordCenterMatrix);

}

void GreateSword::Render(RenderContext& rc)
{
	m_swordModelRender.Draw(rc);
}

void GreateSword::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//スキル攻撃のキャラクター上昇アニメーションキーフレーム
	if (wcscmp(eventName, L"GreatSwordSkillRising") == 0)
	{
		//ここから先移動できる
		isMoveSkillAttack = true;

		//空中に浮いてからは無敵にする
		m_brave->EnableInvincible();

		//エフェクトや音の再生
		UseEffect* effect = NewGO<UseEffect>(0, "SkillRisingEffect");
		effect->PlayEffect(enEffect_GreatSwordSkillRising,
			m_brave->GetPosition(), g_vec3One * 11.0f, Quaternion::Identity, false);
		//音再生
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_GreatSwordSkillRising,
			g_soundManager->GetSEVolume()
		);
	}

	//スキル攻撃のダメージ判定出現アニメーションキーフレーム
	if (wcscmp(eventName, L"GreatSwordSkillAttack") == 0)
	{
		//当たり判定作成
		CreateSkillAttackCollision();


		Vector3 hitPosition = g_vec3Zero;
		m_swordCenterMatrix.Apply(hitPosition);
		hitPosition.y = 0.0f;

		//エフェクトや音の再生
		UseEffect* effect = NewGO<UseEffect>(0, "SkillAttackEffect");
		effect->PlayEffect(enEffect_GreatSwordSkillAttack,
			hitPosition, g_vec3One * 10.0f, Quaternion::Identity, false);
		//音再生
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_GreatSwordSkillAttack,
			g_soundManager->GetSEVolume()
		);
	}


	//通常攻撃１のアニメーションキーフレーム
	if (wcscmp(eventName, L"GreatSwordPlayCombo1Effect") == 0)
	{
		Vector3 pos = g_vec3Zero;
		Quaternion rot = Quaternion::Identity;
		Vector3 forward = m_brave->GetForward();
		forward.y = 0;
		rot.SetRotationYFromDirectionXZ(forward);
		rot.AddRotationDegZ(-280.0f);

		m_swordMatrix.Apply(pos);


		UseEffect* effect = NewGO<UseEffect>(0, "SrashEffect");
		effect->PlayEffect(enEffect_GreatSwordCombo1,
			pos, g_vec3One * 11.0f, rot, false);
	}

	//通常攻撃２のアニメーションキーフレーム
	if (wcscmp(eventName, L"GreatSwordPlayCombo2Effect") == 0)
	{
		Vector3 pos = g_vec3Zero;
		Quaternion rot = Quaternion::Identity;
		Vector3 forward = m_brave->GetForward();
		forward.y = 0;
		rot.SetRotationYFromDirectionXZ(forward);
		rot.AddRotationDegZ(80.0f);

		m_swordCenterMatrix.Apply(pos);


		UseEffect* effect = NewGO<UseEffect>(0, "SrashEffect");
		effect->PlayEffect(enEffect_GreatSwordCombo2,
			pos, g_vec3One * 11.0f, rot, false);
	}

	//通常攻撃３のアニメーションキーフレーム
	if (wcscmp(eventName, L"GreatSwordPlayCombo3Effect") == 0)
	{
		Vector3 pos = g_vec3Zero;
		Quaternion rot = Quaternion::Identity;
		Vector3 forward = m_brave->GetForward();
		forward.y = 0;
		rot.SetRotationYFromDirectionXZ(forward);
		//rot.AddRotationDegZ(230.0f);

		m_swordCenterMatrix.Apply(pos);


		UseEffect* effect = NewGO<UseEffect>(0, "SrashEffect");
		effect->PlayEffect(enEffect_GreatSwordCombo3,
			pos, g_vec3One * 18.0f, rot, false);
	}

}
