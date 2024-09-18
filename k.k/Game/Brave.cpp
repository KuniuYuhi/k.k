#include "stdafx.h"
#include "Brave.h"

////////////////////////////////////////
//コンポーネント
///////////////////////////////////////
#include "PlayerMovement.h"
#include "PlayerController.h"
#include "DamageProvider.h"
///////////////////////////////////////
#include "WeaponManager.h"

#include "SwordShield.h"
#include "GreateSword.h"
#include "Bow.h"


#include "KnockBackStatus.h"
#include "KnockBackInfoManager.h"

#include "GameSceneManager.h"

#include "EnemyManager.h"



namespace {
	const Vector3 START_POSITION = { 0.0f,0.0f,-600.0f };

	float STARK_TIME_LIMIT = 0.2f;
}

Brave::~Brave()
{
	m_braveAnimClip.release();
	WeaponManager::GetInstance()->DeleteAllWeaponInstance();
	WeaponManager::DeleteInstance();
}

bool Brave::Start()
{
	EnWeaponType firstWeaponType = enSwordShield;

	//アニメーションクリップロードクラスを生成
	m_braveAnimClip = std::make_unique<LoadBraveAnimationClips>();
	//アニメーションクリップをロードする
	m_braveAnimClip.get()->RoadWeaponsAnimClips();
	//アニメーションの最初の番号を設定
	m_braveAnimClip.get()->SetCurrentWeaponAnimationStartIndexNo(firstWeaponType);

	//モデル読み込み
	InitModel();

	//キャラコンを生成
	CreateCharacterController();
	m_charaCon.get()->Init(17.0f, 40.0f, m_position);

	//ステータスを初期化
	m_status.InitPlayerStatus("Brave");

	//ステート管理クラスを生成
	m_braveStateCotext = std::make_unique<BraveStateContext>();
	//初期化
	m_braveStateCotext.get()->Init(this, enBraveState_Idle);
	

	//各種コンポーネントのセッティング
	SettingDefaultComponent();


	//武器管理マネージャーを生成
	WeaponManager::CreateInstance();
	//初期化処理。武器の役割を設定する
	WeaponManager::GetInstance()->Init(enSwordShield,enGreateSword,enBowArrow);
	//装備している武器を取得
	m_armedWeapon = WeaponManager::GetInstance()->GetArmedWeapon();

	//プレイヤーの攻撃力に武器の攻撃力を加算
	m_status.ChangeWeaponCalcCurrentPower(m_armedWeapon->GetWeaponCurrentPower());
	
	return true;
}

void Brave::Update()
{
	//コントローラーの入力を受け付けている間は
	if (!IsStopRequested())
	{
		//スタミナの自動回復
		AutoRecoveryStamina();

		//ボタンを押して行うアクション
		ButtonAction();

		//移動処理
		Movement();

		//回転
		Rotation();

		//当たり判定
		CheckSelfCollision();
	}
	
	UpdateDamagedInvisibleTimer();

	//現在のステートの処理
	m_braveStateCotext.get()->UpdateCurrentState();
	m_braveStateCotext.get()->PlayAnimationCurrentState();


	//モデルの更新処理
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void Brave::SetCurrentAnimationStartIndexNoForMainWeaponType()
{
	m_braveAnimClip.get()->SetCurrentWeaponAnimationStartIndexNo(
		WeaponManager::GetInstance()->GetMainWeaponType()
	);
}

void Brave::CharaConExecute(Vector3 moveSpeed)
{
	//キャラコンを使って座標を移動
	m_position = m_charaCon.get()->Execute(moveSpeed, g_gameTime->GetFrameDeltaTime());

}

bool Brave::IsButtonAction()
{
	////アクション中は他の処理をしない
	//if (IsAction() == true)
	//{
	//	return true;
	//}


	//Attack();


	//ChangeWeapon();

	return false;
}

void Brave::ProcessCommonStateTransition()
{
	if (fabsf(GetMoveSpeed().x) >= 0.001f ||
		fabsf(GetMoveSpeed().z) >= 0.001f)
	{
		m_braveStateCotext.get()->ChangeBraveState(enBraveState_Sprint);
	}
	else
	{
		m_braveStateCotext.get()->ChangeBraveState(enBraveState_Idle);
	}
}

void Brave::SettingDefaultComponent()
{
	//プレイヤー移動コンポーネントを取得
	AddComponent<PlayerMovement>();
	m_playerMovement = GetComponent<PlayerMovement>();

	//コントローラーコンポーネントを取得
	AddComponent<PlayerController>();
	m_playerContoller = GetComponent<PlayerController>();

	AddComponent<DamageProvider>();
	m_damageProvider = GetComponent<DamageProvider>();
}

void Brave::Movement()
{
	//アクション中は他の処理をしない
	if (IsAction() == true)
	{
		return;
	}

	//コントローラーの入力を受け付けないなら移動処理しない
	if (!m_playerContoller->IsControllerInputEnabledFlag()) return;

	//移動量を取得
	m_moveSpeed = m_playerMovement->CalcSimpleMovementVerocity(
		m_status.GetDefaultSpeed(), m_moveSpeed, m_playerContoller->GetLStickInput()
	);

	//仮のジャンプ処理
	/*if (m_playerContoller->IsButtonTrigger(enButtonX) && m_charaCon.get()->IsOnGround())
	{
		m_moveSpeed.y = 400.0f;
	}*/

	//重力の計算
	m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();

	//キャラコンを使って座標を移動
	m_position = m_charaCon.get()->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	if (m_charaCon.get()->IsOnGround()) {
		//地面についた。
		m_moveSpeed.y = 0.0f;
	}
	

	//回転用ベクトルに移動量を保存
	m_rotateDirection = m_moveSpeed;
}

void Brave::Rotation()
{
	
	if (fabsf(m_rotateDirection.x) < 0.001f
		&& fabsf(m_rotateDirection.z) < 0.001f) {
		//m_moveSpeed.xとm_moveSpeed.zの絶対値がともに0.001以下ということは
		//このフレームではキャラは移動していないので旋回する必要はない。
		return;
	}

	m_rotation.SetRotationYFromDirectionXZ(m_rotateDirection);

}

void Brave::ButtonAction()
{
	//アクション中は他の処理をしない
	if (IsAction() == true)
	{
		return;
	}

	//攻撃処理
	AttackAction();

	//回避、防御アクション
	DefensiveAction();

	//武器切り替え
	ChangeWeaponAction();

}

void Brave::AttackAction()
{
	//アクション中は他の処理をしない
	if (IsAction() == true)
	{
		return;
	}

	//ほんの少し硬直させる
	if (IsStarkTime() == true) return;


	//通常攻撃ボタンを押したなら
	if (m_playerContoller->IsTriggerNromalAttackButton())
	{
		//プレイヤーの攻撃力に武器の攻撃力を加算
		m_status.ChangeWeaponCalcCurrentPower(m_armedWeapon->GetWeaponCurrentPower());

		NormalAttackProcess();
		//アクション中にする
		ActionActive();
		return;
	}

	//スキル攻撃ボタンを押したかつ
	//武器側がスキル攻撃を行える状態なら
	if (m_playerContoller->IsTriggerSkillAttackButton() &&
		m_armedWeapon->CanSkillAttack())
	{
		//プレイヤーの攻撃力に武器のスキル攻撃力を加算
		m_status.ChangeWeaponCalcCurrentPower(m_armedWeapon->GetWeaponSkillPower());

		m_braveStateCotext.get()->ChangeBraveState(enBraveState_SkillStart);
		//アクション中にする
		ActionActive();
	}


}

void Brave::DefensiveAction()
{
	//アクション中は他の処理をしない
	if (IsAction() == true)
	{
		return;
	}
	//回避、防御ボタンを押したかつ
	//武器側が回避、防御アクションが行える状態なら
	if (m_playerContoller->IsTriggerDefensiveActionButton() && 
		m_armedWeapon->CanDefensiveAction())
	{
		//ステートを切り替える
		m_braveStateCotext.get()->ChangeBraveState(enBraveState_DefensiveActions);
		//アクション中にする
		ActionActive();

	}


}

void Brave::NormalAttackProcess()
{
	//コンボ処理をする
	m_armedWeapon->ProceedComboAttack();
	//通常攻撃ステートに切り替える。コンボによって変わる
	m_braveStateCotext.get()->ChangeBraveState(m_armedWeapon->GetCurrentComboState());
}

void Brave::ExitAttackAction()
{
	//コンボ処理のリセット
	m_armedWeapon->ResetComboAttack();
	//アクションを終わる
	ActionDeactive();
	//硬直タイマーをリセット
	m_starkTimer = 0.0f;
}

void Brave::ChangeBraveState(BraveState::EnBraveState nextState)
{
	m_braveStateCotext.get()->ChangeBraveState(nextState);
}

void Brave::EntryHitActionProcess()
{
	//アクション中にする
	ActionActive();
	//ノックバックするための情報を設定
	SettingKnockBackProcess();
	//攻撃中かもしれないのでインパクト時の処理をリセット
	m_armedWeapon->AttackImpactProcess(false);

	count = 0.0f;

	//無敵にする
	EnableInvincible();
}

void Brave::UpdateHitActionProcess()
{
	//まずカーブデータを元に移動
	if (count < m_curvePointList.size())
	{
		//ノックバックによる移動
		KnockBackMove(count);
		//タイムスケールを加算
		count += m_knockBackTimeScale;
	}
	//次に空中に浮いていたら地面に降りる
	else if (m_charaCon.get()->IsJump())
	{
		KnockBackGravityFall();

	}
	//最後に少し硬直させて共通ステート処理に移行
	else
	{
		//アニメーションが終わったら
		if (GetModelRender().IsPlayingAnimation() == false)
		{
			if (IsDie())
			{
				//死亡しているときはステート遷移
				m_braveStateCotext.get()->ChangeBraveState(enBraveState_Die);
				return;
			}

			//少し硬直して共通ステート処理に移行
			if (m_starkTimer >= 0.1f)
			{
				//共通ステートに移行
				ProcessCommonStateTransition();
			}
			m_starkTimer += g_gameTime->GetFrameDeltaTime();
		}
	}
}

void Brave::ExitHitActionProcess()
{
	//アクションを終わる
	ActionDeactive();

	//保存していた前方向を取得
	m_forward = m_saveForward;

	//数フレームだけ無敵時間にするためにタイマーをスタートする
	m_isDamagedInvincible = true;

	//無敵解除
	//DisableInvincible();
}



void Brave::ChangeWeaponAction()
{
	//アクション中は他の処理をしない
	if (IsAction() == true)
	{
		return;
	}

	bool isTrigger = false;

	//サブ武器１に切り替える
	if (m_playerContoller->IsButtonTrigger(enButtonRB1))
	{
		//サブ武器１を切り替え武器にする
		WeaponManager::GetInstance()->ChangeSubWeaponTypeToChangeWeaponType();
		isTrigger = true;
	}
	//サブ武器２に切り替える
	if (m_playerContoller->IsButtonTrigger(enButtonLB1))
	{
		//サブ武器２を切り替え武器にする
		WeaponManager::GetInstance()->ChangeSubWeaponType2ToChangeWeaponType();
		isTrigger = true;
	}

	//ボタンを押していたら
	if (isTrigger)
	{
		//切り替える武器のアニメーションの最初の番号を切り替え対象武器の番号にする
		m_braveAnimClip.get()->SetCurrentWeaponAnimationStartIndexNo(
			WeaponManager::GetInstance()->GetChangeTargetWeaponType()
		);
		//ステートを切り替える
		m_braveStateCotext.get()->ChangeBraveState(enBraveState_ChangeWeapon);


		//アクション中にする
		ActionActive();
	}

}

bool Brave::IsStarkTime()
{
	if (m_starkTimer >= STARK_TIME_LIMIT)
	{
		//硬直時間が終わったのでtrue
		return false;
	}
	else
	{
		//タイマー加算
		m_starkTimer += g_gameTime->GetFrameDeltaTime();
	}
	//硬直中なのでtrue
	return true;
}

void Brave::AutoRecoveryStamina()
{
	//アクション中は回復しない
	//if (IsAction()) return;

	//スタミナを回復する量を計算
	float recoveryValue = g_gameTime->GetFrameDeltaTime() * m_status.GetStaminaRecoveryRate();
	//回復
	m_status.RecoveryStamina(recoveryValue);
}

void Brave::CheckSelfCollision()
{
	//当たり判定無効条件
	//無敵なら処理しない
	if (IsInvincible()) {
		return; 
	}

	//エネミーの攻撃コリジョンを取得
	const auto& Collisions =
		g_collisionObjectManager->FindCollisionObjects(
			g_collisionObjectManager->m_enemyAttackCollisionName
		);

	int nazenaze = 0;

	//コリジョンの配列をfor文で回す
	for (auto collision : Collisions)
	{
		//当たり判定が有効でないなら飛ばす
		if (!collision->IsEnable()) continue;

		//自身のキャラコンと衝突したら
		if (collision->IsHit(*m_charaCon) == true)
		{
			//コリジョンを持っているキャラのダメージプロバイダーコンポーネントを取得
			DamageProvider* dp = FindGOComponent<DamageProvider>(collision->GetCreatorName());

			if (dp == nullptr)
			{
				return;
			}

			//攻撃がヒットしたことをコリジョンを持っているDamageProviderクラスに伝える
			dp->Hit();

			//ダメージプロバイダーの座標を取得
			m_damageProviderPosition = dp->GetProviderPostion();

			//攻撃IDを取得
			int currentAttackId = dp->GetAttackId();

			//前回とIDが同じなら処理しない
			if (currentAttackId == oldAttackId) return;

			nazenaze++;

			//ダメージをランダムに調整して、フォントとして表示
			CreateDamageFont(dp->GetAdjustedDamage(),DamageFont::enDamageActor_Player);

			//被ダメージ処理を行う引数に情報を入れる
			ProcessHit(dp->GetProviderDamageInfo());

			//今回のIDを前フレームのIDに保存
			oldAttackId = currentAttackId;


			if (nazenaze >= 5)
			{
				nazenaze = 0;
			}

			//todo 無敵時間に入る
			EnableInvincible();
		}
	}

}

void Brave::SettingKnockBackProcess()
{
	m_curvePointList.clear();
	//ノックバックカーブのポイントリストを取得
	m_curvePointList = KnockBackInfoManager::GetInstance()->GetCurvePoint(
		m_hitKnockBackPattern
	);

	//今回のパターンのノックバックのスピードを取得
	m_knockBackSpeed = KnockBackInfoManager::
		GetInstance()->GetStatus().GetKnockBackSpeed(m_hitKnockBackPattern);

	//前方向を変更する前に今の方向を保存しておく
	m_saveForward = m_forward;

	//前方向を正規化して方向を反転させる
	Vector3 diff = m_position - m_damageProviderPosition;
	m_forward = diff;
	m_forward.Normalize();

	//硬直タイマーリセット
	m_starkTimer = 0.0f;
}

void Brave::KnockBackMove(int listNum)
{
	//正規化
	m_curvePointList[listNum].curvePosition.Normalize();
	//移動速度を計算
	m_moveSpeed.x = m_forward.x * m_curvePointList[listNum].curvePosition.x * m_knockBackSpeed.x;
	m_moveSpeed.y = m_curvePointList[listNum].curvePosition.y * m_knockBackSpeed.y;
	m_moveSpeed.z = m_forward.z * m_curvePointList[listNum].curvePosition.z * m_knockBackSpeed.x;
	//キャラコンを使って移動
	m_position = m_charaCon.get()->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
}

void Brave::KnockBackGravityFall()
{
	//重力の計算
	m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();
	//キャラコンを使って座標を移動
	m_position = m_charaCon.get()->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.get()->IsOnGround()) {
		//地面についた。
		m_moveSpeed.y = 0.0f;
	}
}

void Brave::SettingKnockBackInfoForDamageInfo(DamageInfo damageInfo)
{
	//ノックバックパターンを取得
	//todo レベルによって変更
	m_hitKnockBackPattern = damageInfo.knockBackPattern;

	//ノックバックの時間間隔を取得
	m_knockBackTimeScale = damageInfo.knockBackTimeScale;
}

void Brave::AfterDieProcess()
{
	//エネミーたちに勝利時の処理をさせる
	EnemyManager::GetInstance()->WinProcessAllEnemy();

}

void Brave::GameOutComeProcess()
{
	//ステートを切り替える
	m_braveStateCotext.get()->ChangeBraveState(enBraveState_Idle);
	m_moveSpeed = g_vec3Zero;
}

void Brave::ProcessHit(DamageInfo damageInfo)
{
	//ノックバックの情報を設定
	SettingKnockBackInfoForDamageInfo(damageInfo);

	//攻撃中かもしれないのでインパクト時の処理をリセット
	m_armedWeapon->AttackImpactProcess(false);

	//被ダメージ処理
	TakeDamage(damageInfo.attackPower);

	//ステートを切り替える
	m_braveStateCotext.get()->ChangeBraveState(enBraveState_Hit);
}

void Brave::TakeDamage(int damage)
{
	//ダメージを受ける
	//やられた場合は死亡フラグが立つ
	SetDieFlag(m_status.TakeDamage(damage));

	//やられた場合は入力を受け付けない
	if (IsDie())
	{
		m_playerContoller->SetControllerInputEnabledFlag(false);
	}
}

bool Brave::IsStopRequested()
{
	//コントローラーの入力を受け付けないなら
	if (m_playerContoller->IsControllerInputEnabledFlag()) true;

	//勝敗が着いたら
	if (GameSceneManager::GetInstance()->IsGameOutcome())return true;

	//死亡したら
	if (IsDie()) return true;

	return false;
}

void Brave::UpdateDamagedInvisibleTimer()
{
	//ダメージ後の無敵フラグがたっていないなら処理しない
	if (!m_isDamagedInvincible) return;
	//制限時間に達したら
	if (m_damagedInvisibleTimer >= m_status.GetDamagedInvisibleTimeLimit())
	{
		m_damagedInvisibleTimer = 0.0f;
		//被ダメージ後無敵フラグをリセット
		m_isDamagedInvincible = false;

		//if(m_braveStateCotext.get())
		//無敵無効化
		DisableInvincible();

		//モデルは絶対表示
		m_isViewModel = true;

		return;
	}
	//タイマーを加算
	m_damagedInvisibleTimer += g_gameTime->GetFrameDeltaTime();


	int a = m_damagedInvisibleTimer * 10.0f;

	if (a % 2 == 0)
	{
		m_isViewModel = !m_isViewModel;
	}

}


void Brave::Render(RenderContext& rc)
{
	if (!m_isViewModel) return;


	m_modelRender.Draw(rc);
}

void Brave::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{

	//武器入れ替えタイミング
	if (wcscmp(eventName, L"ArmedSwordShield") == 0)
	{
		//武器自体と武器タイプを入れ替える
		WeaponManager::GetInstance()->ChangeArmedWeapon(
			WeaponManager::GetInstance()->GetChangeTargetWeaponType()
		);
		//入れ替え後の現在の武器をインスタンスを取得
		m_armedWeapon = WeaponManager::GetInstance()->GetArmedWeapon();

		//武器のステータス(攻撃力など)を自身のステータスに加算
		m_status.ChangeWeaponCalcCurrentPower(m_armedWeapon->GetWeaponCurrentPower());

		g_soundManager->InitAndPlaySoundSource(
			enSoundName_ChangeWeapon,
			g_soundManager->GetSEVolume()
		);
	}


	//前進する始まり
	if (wcscmp(eventName, L"MoveForwardStart") == 0)
	{
		m_armedWeapon->SetAttackActionMove(true);
	}
	//前進する終わり
	if (wcscmp(eventName, L"MoveForwardEnd") == 0)
	{
		m_armedWeapon->SetAttackActionMove(false);
	}

	//回避時の移動の始まり
	if (wcscmp(eventName, L"AvoidMoveStart") == 0)
	{
		m_armedWeapon->SetDefensiveActionMove(true);
	}
	//回避時の移動の終わり
	if (wcscmp(eventName, L"AvoidMoveEnd") == 0)
	{
		m_armedWeapon->SetDefensiveActionMove(false);
	}


	//次の行動待機区間始まり
	if (wcscmp(eventName, L"StandbyPeriodStart") == 0)
	{
		//待機区間フラグを立てる
		m_armedWeapon->SetStandbyPeriodFlag(true);
	}
	//次の行動待機区間終わり
	if (wcscmp(eventName, L"StandbyPeriodEnd") == 0)
	{
		//待機区間フラグをリセット
		m_armedWeapon->SetStandbyPeriodFlag(false);
	}

	//攻撃(敵に攻撃が届くなど)が始まった瞬間。
	if (wcscmp(eventName, L"ImpactStart") == 0)
	{
		//コリジョンの生成など、キャンセルアクションの設定など
		m_armedWeapon->AttackImpactProcess(true);
	}
	
	//攻撃(敵に攻撃が届くなど)が終わった瞬間。
	if (wcscmp(eventName, L"ImpactEnd") == 0)
	{
		//コリジョンの生成など、キャンセルアクションの設定など
		m_armedWeapon->AttackImpactProcess(false);
	}

}

void Brave::InitModel()
{
	//モデル初期化
	m_modelRender.Init("Assets/modelData/character/Player/NewHero/Hero_Smile_Selllook.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		m_braveAnimClip.get()->GetBraveAnimationClip(),
		m_braveAnimClip.get()->GetNumAnimationClips(),
		enModelUpAxisZ
	);

	m_position = START_POSITION;

	m_modelRender.SetTransform(m_position, m_rotation, m_scale);

	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
	});
}

void Brave::ChangePlayerWinState()
{
	//コントローラーの入力を受け付けないようにする
	m_playerContoller->SetControllerInputEnabledFlag(false);
	//勝利ステートに遷移
	m_braveStateCotext.get()->ChangeBraveState(enBraveState_WinStart);
}

int Brave::GetCurrentPower()
{
	return m_status.GetCurrentPower();
}
