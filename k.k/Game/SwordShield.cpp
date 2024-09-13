#include "stdafx.h"
#include "SwordShield.h"
#include "Brave.h"
#include "PlayerController.h"
#include "PlayerMovement.h"
#include "DamageProvider.h"

#include "KnockBackInfoManager.h"


#include "UseEffect.h"


namespace {

	const Vector3 SWORD_COLLISION_SIZE = { 90.0f,20.0f,10.0f };
	const Vector3 SHIELD_COLLISION_SIZE = { 28.0f,50.0f,16.0f };

	const float SKILL_ATTACK_RADIUS = 100.0f;		//スキル攻撃用コリジョンの半径


	const float SHIELD_BARRIER_EFFECT_SCALE = 8.0f;
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

	//アニメーションイベント用の関数を設定する。
	m_brave->GetModelRender().AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
	});

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

void SwordShield::DeleteThis()
{
	DeleteGO(this);
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


	//盾の当たり判定
	m_shieldCollision = NewGO<CollisionObject>(0, "defence");
	//コリジョンの制作者をプレイヤーに設定
	m_shieldCollision->SetCreatorName(m_brave->GetName());
	//コリジョンの形状はボックス
	m_shieldCollision->CreateSphere(
		m_stowedPosition,
		Quaternion::Identity,
		50.0f
	);
	//当たり判定を自動で削除しないようにする
	m_shieldCollision->SetIsEnableAutoDelete(false);
	m_shieldCollision->SetIsEnable(false);
}

void SwordShield::CreateSkillAttackCollision()
{
	Vector3 hitPosition = g_vec3Zero;
	//剣のワールド座標をベクトルに乗算
	m_swordCenterMatrix.Apply(hitPosition);
	hitPosition.y = 0.0f;

	//スキル攻撃時の当たり判定の生成
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
	//防御ボタンを離したら、またはシールドにヒットしたら
	if (!m_playerController->IsPressDefensiveActionButton()|| m_isHitShield)
	{
		//防御アクションを終わる
		return true;
	}

	return false;
}

void SwordShield::EntryDefensiveActionProcess()
{
	Vector3 position = g_vec3Zero;
	m_shieldMatrix.Apply(position);
	//エフェクト生成
	m_shieldEffect = NewGO<UseEffect>(0, "ShieldBarrierEffect");
	m_shieldEffect->PlayEffect(enEffect_SwordShieldDefendBarrier,
		position, g_vec3One * SHIELD_BARRIER_EFFECT_SCALE, Quaternion::Identity, false);


	//盾の当たり判定を使うのでプレイヤー本体は無敵にする
	m_brave->EnableInvincible();
	//盾の当たり判定を有効化する
	m_shieldCollision->SetIsEnable(true);
	//シールドヒットフラグをリセット
	m_isHitShield = false;
}

void SwordShield::UpdateDefensiveActionProcess()
{
	//盾の当たり判定をチェック
	CheckShieldCollision();

	//回避、防御アクションを終わるなら
	if (IsEndDefensiveAction())
	{
		if (m_isHitShield)
		{
			//シールドにヒットしていたらシールドヒットステートに遷移
			m_brave->ChangeBraveState(enBraveState_DefensiveHit);
		}
		else
		//ステートの共通処理
		m_brave->ProcessCommonStateTransition();
	}
}

void SwordShield::ExitDefensiveActionProcess()
{
	//エフェクト削除
	if (m_shieldEffect != nullptr)
	{
		m_shieldEffect->Delete();
		m_shieldEffect = nullptr;
	}

	//シールドにヒットせずにステートが終わったら
	if (!m_isHitShield)
	{
		//無敵を解除
		m_brave->DisableInvincible();
	}

	//盾の当たり判定を無効化する
	m_shieldCollision->SetIsEnable(false);
}

void SwordShield::EntryDefensiveHitProcess()
{
	//盾の当たり判定を使うのでプレイヤー本体は無敵にする
	m_brave->EnableInvincible();

}

void SwordShield::UpdateDefensiveHitProcess()
{
}

void SwordShield::ExitDefensiveHitProcess()
{
	////まだ防御ボタンを押していたら防御ステートに切り替える
	//if (m_playerController->IsPressDefensiveActionButton())
	//{
	//	m_brave->ChangeBraveState(enBraveState_DefensiveActions);
	//	return;
	//}

	//無敵を解除
	m_brave->DisableInvincible();
}

bool SwordShield::CanDefensiveAction()
{
	//現在のシールドの耐久値が0より大きければ
	if (m_uniqueStatus.GetCurrentShieldEnduranceValue() > 0)
	{
		//防御可能
		return true;
	}
	//不可能
	return false;
}

bool SwordShield::CanSkillAttack()
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

void SwordShield::UpdateNormalAttackProcess(EnComboState comboState)
{
	//キャンセルアクションできる状態なら防御も可能
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
		m_brave->CharaConExecute(m_normalAttackMoveDirection);
	}

}

void SwordShield::ExitNormalAttackProcess(EnComboState comboState)
{
	//攻撃中の移動フラグをリセット
	SetAttackActionMove(false);
	//通常攻撃待機区間フラグをリセット
	SetStandbyPeriodFlag(false);
	//一応当たり判定を無効化しておく
	m_swordCollision->SetIsEnable(false);
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

void SwordShield::AttackImpactProcess(bool startOrEnd)
{
	//キャンセルアクションフラグを設定
	m_isImpossibleancelAction = startOrEnd;

	//当たり判定の有効化、無効化の設定
	m_swordCollision->SetIsEnable(startOrEnd);
}

void SwordShield::EntrySkillStartProcess()
{
	//スキルメインステートに進むかのフラグをリセット
	m_brave->SetProceedSkillMainFlag(false);
}

void SwordShield::UpdateSkillStartProcess()
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

void SwordShield::ExitSkillStartProcess()
{

}

void SwordShield::EntrySkillMainProcess()
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

void SwordShield::UpdateSkillMainProcess()
{
}

void SwordShield::ExitSkillMainProcess()
{
	//スタミナを消費する
	m_brave->GetStatus().TryConsumeStamina(m_status.GetSkillStaminaCost());
	//無敵を無効化する
	m_brave->DisableInvincible();
}

void SwordShield::CheckShieldCollision()
{
	if (m_isHitShield) return;

	//エネミーの攻撃コリジョンを取得
	const auto& Collisions =
		g_collisionObjectManager->FindCollisionObjects(
			g_collisionObjectManager->m_enemyAttackCollisionName
		);

	//コリジョンの配列をfor文で回す
	for (auto collision : Collisions)
	{
		//当たり判定が有効でないなら飛ばす
		if (!collision->IsEnable()) continue;

		//盾の当たり判定と衝突したら
		if (collision->IsHit(m_shieldCollision) == true)
		{
			//コリジョンを持っているキャラのダメージプロバイダーコンポーネントを取得
			DamageProvider* dp = FindGOComponent<DamageProvider>(collision->GetCreatorName());

			if (dp == nullptr) return;

			dp->Hit();

			//敵の位置保存
			m_brave->SetDamageProviderPosition(
				dp->GetProviderPostion()
			);

			//ノックバックの情報を設定
			m_brave->SettingKnockBackInfoForDamageInfo(dp->GetProviderDamageInfo());

			//盾の耐久値をダメージ分減らす
			m_uniqueStatus.SubShieldEnduranceValue(
				dp->GetAdjustedDamage()
			);

			m_brave->CreateDamageFont(
				dp->GetProviderDamageInfo().attackPower,
				DamageFont::enDamageActor_Player
			);

			//シールドにヒットした！
			m_isHitShield = true;


			//ダメージを盾の耐久値が肩代わり
			ProcessShieldHit();
		}
	}

}

void SwordShield::ProcessShieldHit()
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
	m_swordCenterMatrix = m_swordModelRender.GetBone(m_swordCenterBoonId)->GetWorldMatrix();
	m_swordCollision->SetWorldMatrix(m_swordCenterMatrix);
	//盾のワールド座標を設定
	m_shieldCollision->SetWorldMatrix(m_shieldMatrix);
}


void SwordShield::Render(RenderContext& rc)
{
	m_swordModelRender.Draw(rc);
	m_shieldModelRender.Draw(rc);

}

void SwordShield::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{

	//スキル攻撃のダメージ判定出現アニメーションキーフレーム
	if (wcscmp(eventName, L"SwordShieldSkillAttack") == 0)
	{
		//スキル攻撃用コリジョン生成
		CreateSkillAttackCollision();
		//スキル攻撃の衝撃エフェクト再生

		Vector3 hitPosition = g_vec3Zero;
		//剣のワールド座標をベクトルに乗算
		m_swordCenterMatrix.Apply(hitPosition);
		hitPosition.y = 0.0f;

		UseEffect* effect = NewGO<UseEffect>(0, "SkillShockEffect");
		effect->PlayEffect(enEffect_SwordShieldSkillAttack,
			hitPosition, g_vec3One * 17.0f, Quaternion::Identity, false);

		//スキルの音再生
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_SwordShieldSkillAttack,
			g_soundManager->GetSEVolume()
		);
	}

	//スキルの上昇アニメーションキーフレーム
	if (wcscmp(eventName, L"SwordShieldSkillRising") == 0)
	{
		//エフェクト再生
		UseEffect* effect = NewGO<UseEffect>(0, "SkillRisingEffect");
		effect->PlayEffect(enEffect_SwordShieldSkillRising,
			m_brave->GetPosition(), g_vec3One * 10.0f, Quaternion::Identity, false);
		//音再生
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_SwordShieldSkillRising,
			g_soundManager->GetSEVolume()
		);
	}


	if (wcscmp(eventName, L"PlayCombo1Effect") == 0)
	{
		//エフェクト再生のための座標と回転設定
		
		Vector3 pos = g_vec3Zero;
		Quaternion rot = Quaternion::Identity;
		Vector3 forward = m_brave->GetForward();
		forward.y = 0;
		rot.SetRotationYFromDirectionXZ(forward);
		rot.AddRotationDegZ(230.0f);

		m_swordCenterMatrix.Apply(pos);
		

		UseEffect* effect = NewGO<UseEffect>(0, "SrashEffect");
		effect->PlayEffect(enEffect_SwordShieldCombo12,
			pos, g_vec3One * 15.0f, rot, false);

	}

	//通常攻撃２のアニメーションキーフレーム
	if (wcscmp(eventName, L"PlayCombo2Effect") == 0)
	{
		//エフェクト再生のための座標と回転設定

		Vector3 pos = g_vec3Zero;
		Quaternion rot = Quaternion::Identity;
		Vector3 forward = m_brave->GetForward();
		forward.y = 0;
		rot.SetRotationYFromDirectionXZ(forward);

		m_swordMatrix.Apply(pos);


		UseEffect* effect = NewGO<UseEffect>(0, "SrashEffect");
		effect->PlayEffect(enEffect_SwordShieldCombo12,
			pos, g_vec3One * 15.0f, rot, false);
	}

	//通常攻撃２のアニメーションキーフレーム
	if (wcscmp(eventName, L"PlayCombo3Effect") == 0)
	{
		//エフェクト再生のための座標と回転設定

		Vector3 pos = g_vec3Zero;
		Quaternion rot = Quaternion::Identity;
		Vector3 forward = m_brave->GetForward();
		forward.y = 0;
		rot.SetRotationYFromDirectionXZ(forward);
		m_swordMatrix.Apply(pos);

		UseEffect* effect = NewGO<UseEffect>(0, "SrashEffect");
		effect->PlayEffect(enEffect_SwordShieldCombo3,
			pos, g_vec3One * 15.0f, rot, false);
		
	}


	//防御ヒットのアニメーションキーフレーム
	if (wcscmp(eventName, L"SwordShieldDifendHit") == 0)
	{
		//音再生
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_SwordShieldDefendHit,
			g_soundManager->GetSEVolume()
		);
	}

}
