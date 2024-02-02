#include "stdafx.h"
#include "Brave.h"
#include "BraveStateIdle.h"
#include "BraveStateSprint.h"
#include "BraveStateAttack_1.h"
#include "BraveStateAttack_2.h"
#include "BraveStateAttack_3.h"
#include "BraveStateSkill_Start.h"
#include "BraveStateSkill_Main.h"
#include "BraveStateDefend.h"
#include "BraveStateDefendHit.h"
#include "BraveStateHit.h"
#include "BraveStateDie.h"
#include "BraveStateChangeSwordShield.h"
#include "BraveStateWin_Start.h"
#include "BraveStateWin_Main.h"
#include "BraveStateKnockBack.h"

#include "Player.h"

#include "SwordShield.h"
#include "BigSword.h"
#include "Bow.h"
#include "ManagerPreCompile.h"

#include "CalcCharacterForward.h"

#include "CharactersInfoManager.h"

namespace {
	const float ADD_SCALE = 1.2f;

	const float ROT_SPEED = 20.0f;
	const float ROT_ONLY_SPEED = 20.0f;

	const float SWORD_EFFECT_SIZE = 10.0f;
	const Vector3 DASH_EFFECT_SIZE = { 10.0f,20.0f,8.0f };

	const float KNOCKBACK_TIMER = 1.0f;

	const float GRAVITY = 50.0f;

	const int SKILL_ATTACK_POWER = 30;
}

Brave::Brave()
{
}

Brave::~Brave()
{
	DeleteGO(m_mainUseWeapon.weapon);
	DeleteGO(m_subUseWeapon.weapon);
}

bool Brave::Start()
{
	m_player = FindGO<Player>("player");

	//ステータスの初期化
	m_status.Init(GetName());

	//装備する武器の設定
	SettingWeapons();
	//モデルの初期化
	InitModel();
	//キャラコンの設定
	m_charaCon.Init(12.0f, 33.0f, m_position);
	//初期ステートを設定
	SetNextAnimationState(enAninationState_Idle);

	//前方向計算クラス生成
	m_calcCharacterForward = std::make_unique<CalcCharacterForward>();

	return true;
}

void Brave::Update()
{
	//ポーズ画面なら処理をしない
	if (GameManager::GetInstance()->GetGameSeenState() ==
		GameManager::enGameSeenState_Pause)
	{
		return;
	}

	//行動不可能な状態でないなら
	if (IsInaction() != true)
	{
		//攻撃処理
		ProcessAttack();
		//防御処理
		ProcessDefend();
		//武器の切り替え処理
		ChangeWeapon();
		//移動処理
		Move();
		//回転処理
		ProcessRotation();
		//無敵時間の計算
		CalcInvincibleTime();
		//当たり判定
		DamageCollision(m_charaCon);
	}

	ManageState();
	PlayAnimation();
	//モデルの設定と更新
	SetTransFormModel(m_modelRender);
	m_modelRender.Update();
}

void Brave::Move()
{
	//移動方向を計算
	m_moveSpeed = calcVelocity(GetStatus());

	//特定のアニメーションが再生中なら移動なし
	if (isAnimationEntable() != true)
	{
		//現在の武器が回転可能なら
		if (m_mainUseWeapon.weapon->GetRotationDelectionFlag() == true)
		{
			//前方向を計算する
			CalcForward(m_moveSpeed);
		}
		//移動量を0にする
		m_moveSpeed = g_vec3Zero;
	}
	else
	{
		//前方向を計算
		CalcForward(m_moveSpeed);
	}

	//重力をかける
	m_moveSpeed.y = 0.0f;
	
	//移動方向に座標を移動
	m_position = m_charaCon.Execute(
		m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	//キャラコンが地面に着いているなら、Y座標を0にする
	if (m_charaCon.IsOnGround() == true)
	{
		m_moveSpeed.y = 0.0f;
	}
}

void Brave::ProcessRotation()
{
	Rotation(ROT_SPEED, ROT_ONLY_SPEED);
}

void Brave::ProcessAttack()
{
	//アクションフラグがtrueまたは、現在の武器の攻撃可能フラグがfalseなら攻撃処理をしない
	if (GetIsActionFlag() == true||m_mainUseWeapon.weapon->GetIsAttackEnableFlag()!=true)
	{
		return;
	}
	//通常攻撃
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		//向いている方向に前進するために前方向を計算
		CalcAttackMoveSpeed();
		//コンボ攻撃の処理
		ProcessComboAttack();
	}
	//スキル
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		//アクションフラグをセット
		SetIsActionFlag(true);
		//ノックバックパワーを設定
		SetKnockBackPower(m_mainUseWeapon.weapon->GetKnockBackPower(4));
		//スキルのスタートアニメーションステート
		SetNextAnimationState(enAnimationState_Skill_start);
	}

	//モンスターにダメージを与えられるようにフラグをリセット
	CharactersInfoManager::GetInstance()->SetAllMonsterDamgeHitFlag(true);
}

void Brave::ProcessDefend()
{
	//アクションフラグがtrue、または現在の武器の防御可能かフラグがfalseなら攻撃処理をしない
	if (GetIsActionFlag() == true|| m_mainUseWeapon.weapon->GetIsDefendEnableFlag() == false)
	{
		return;
	}
	//防御
	if (g_pad[0]->IsTrigger(enButtonY) == true)
	{
		//アクションフラグをセット
		SetIsActionFlag(true);
		SetNextAnimationState(enAnimationState_Defend);
	}
}

void Brave::Damage(int damage)
{
	//HPが0より大きいなら被ダメージ処理
	if (GetStatus().GetHp() > 0)
	{
		//武器切り替え中にダメージを受けたら、武器切り替え前の情報に戻す
		BeforeWeaponInfo();

		//盾にヒットしたなら
		if (IsDefendHit() == true)
		{
			//ダメージを1/3に減らす
			damage /= 3;
			//どれだけダメージを減らしても１以下にはならない
			if (damage < 1) { damage = 1; }
			//盾ヒットステートに遷移
			SetNextAnimationState(enAnimationState_DefendHit);
			//耐久値を減らす。耐久値が0なら防御不可能になる
			m_mainUseWeapon.weapon->CalcEndurance(damage, false);
		}
		else
		{
			//普通にダメージを受ける
			//ヒットステートに遷移
			SetNextAnimationState(enAnimationState_Hit);
		}
		//受けるダメージを決定
		m_hitDamage = damage;
		m_status.CalcHp(m_hitDamage, false);
		//ダメージを受けたので無敵時間に入る
		SetInvicibleTimeFlag(true);
	}
	//HPが0以下なら
	if (GetStatus().GetHp() <= 0)
	{
		//やられたのでdieFlagをtrueにする
		SetDieFlag(true);
		//プレイヤーがやられたので負けたことをゲームマネージャーに教える
		GameManager::GetInstance()->SetPlayerLoseFlag(true);
		//点滅しないようにする
		SetInvicibleTimeFlag(false);
		//HPを0に固定する
		m_status.SetHp(0);
		//アニメーションのスピードを遅くする
		m_modelRender.SetAnimationSpeed(0.5f);
		//死亡ステートに遷移
		SetNextAnimationState(enAnimationState_Die);
	}
}

bool Brave::IsInaction()
{
	//行動出来なくなる条件
	//プレイヤークラスの関数の動けない条件がtrueなら
	if (m_player->IsInaction() == true)
	{
		return true;
	}
	//敵を倒したら
	if (GameManager::GetInstance()->GetPlayerWinFlag() == true)
	{
		return true;
	}
	//やられたなら
	if (GetDieFlag() == true)
	{
		return true;
	}
	//ノックバックステートなら
	if (m_enAnimationState == enAnimationState_KnockBack)
	{
		return true;
	}
	//ここまできたら行動可能
	return false;
}

void Brave::MoveForward(float Speed)
{
	//前進する方向
	Vector3 Direction = m_forward;
	//移動する速度
	Vector3 MoveSpeed = Direction * Speed;
	MoveSpeed.y = 0.0f;
	m_position = m_charaCon.Execute(MoveSpeed, 1.0f / 60.0f);
	m_modelRender.SetPosition(m_position);
}

void Brave::MoveBack(float backSpeed)
{
	//後退する方向
	Vector3 Direction = m_forward;
	Direction *= -1.0f;
	//移動する速度
	Vector3 MoveSpeed = Direction * backSpeed;
	MoveSpeed.y = 0.0f;
	m_position = m_charaCon.Execute(MoveSpeed, 1.0f / 60.0f);
	m_modelRender.SetPosition(m_position);
}

void Brave::ChangeWeapon()
{
	//アクションフラグがtrueなら処理をしない
	if (GetIsActionFlag() == true)
	{
		return;
	}
	//サブ１と武器切り替え
	if (g_pad[0]->IsTrigger(enButtonLB1))
	{
		//武器切り替えのための前準備
		SettingChangeWeapon(
			m_subUseWeapon.weaponAnimationStartIndexNo, enWeapon_Sub);
	}
	//サブ２と武器切り替え
	if (g_pad[0]->IsTrigger(enButtonRB1))
	{
		//武器切り替えのための前準備
		SettingChangeWeapon(
			m_subUseWeapon2.weaponAnimationStartIndexNo, enWeapon_Sub2);
	}
}

void Brave::ProcessWin()
{
	//無敵時間で点滅しないようにする
	SetInvicibleTimeFlag(false);
	//勝利スタートのステート
	SetNextAnimationState(enAnimationState_Win_Start);
}

bool Brave::IsKnockBack()
{
	if (IsKnockingBack(m_moveSpeed, m_knockbackTimer, m_KnockBackTimerLimmit) == true)
	{
		//座標を移動
		m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
		return true;
	}
	else
	{
		//ノックバック終了
		SetHitKnockBackFlag(false);
		return false;
	}
	
}

void Brave::SetNextAnimationState(int nextState)
{
	if (m_BraveState != nullptr) {
		// 古いステートを削除する。
		delete m_BraveState;
		m_BraveState = nullptr;
	}
	
	//アニメーションステートを次のステートに変える
	m_enAnimationState = (EnAnimationState)nextState;

	switch (m_enAnimationState)
	{
	case Brave::enAninationState_Idle:
		m_BraveState = new BraveStateIdle(this);
		break;
	case Brave::enAninationState_Sprint:
		m_BraveState = new BraveStateSprint(this);
		break;
	case Brave::enAnimationState_KnockBack:
		m_BraveState = new BraveStateKnockBack(this);
		break;
	case Brave::enAnimationState_Hit:
		m_BraveState = new BraveStateHit(this);
		break;
	case Brave::enAnimationState_Defend:
		m_BraveState = new BraveStateDefend(this);
		break;
	case Brave::enAnimationState_DefendHit:
		m_BraveState = new BraveStateDefendHit(this);
		break;
	case Brave::enAnimationState_Die:
		m_BraveState = new BraveStateDie(this);
		break;
	case Brave::enAnimationState_ChangeWeapon:
		m_BraveState = new BraveStateChangeSwordShield(this);
		break;
	case Brave::enAnimationState_Win_Start:
		m_BraveState = new BraveStateWin_Start(this);
		break;
	case Brave::enAnimationState_Win_Main:
		m_BraveState = new BraveStateWin_Main(this);
		break;
	case Brave::enAnimationState_Attack_1:
		m_BraveState = new BraveStateAttack_1(this);
		break;
	case Brave::enAnimationState_Attack_2:
		m_BraveState = new BraveStateAttack_2(this);
		break;
	case Brave::enAnimationState_Attack_3:
		m_BraveState = new BraveStateAttack_3(this);
		break;
	case Brave::enAnimationState_Skill_start:
		m_BraveState = new BraveStateSkill_Start(this);
		break;
	case Brave::enAnimationState_Skill_Main:
		m_BraveState = new BraveStateSkill_Main(this);
		break;
	default:
		break;
	}
}

void Brave::PlayAnimation()
{
	//派生クラスのPlayAnimation関数を実行
	m_BraveState->PlayAnimation();
}

void Brave::ManageState()
{
	//派生クラスのManageState関数を実行
	m_BraveState->ManageState();
}

void Brave::SettingKnockBackInfo(
	Vector3 endPos, float knockBackPower, float knockBackLimmit)
{
	//武器切り替え中にダメージを受けたら、武器切り替え前の情報に戻す
	BeforeWeaponInfo();
	//ノックバックする移動方向を設定
	m_moveSpeed = SetKnockBackDirection(m_position, endPos, knockBackPower);
	//ノックバックする制限時間を設定
	m_KnockBackTimerLimmit = knockBackLimmit;
	//前方向の計算
	CalcForward(m_moveSpeed * -1.0f);
	//回転処理
	m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed * -1.0f);
	//ステートをノックバックステートに遷移
	SetNextAnimationState(enAnimationState_KnockBack);
}

void Brave::ExecutePosition(Vector3& executePosition)
{
	m_position = m_charaCon.Execute(executePosition, 1.0f / 30.0f);
	m_modelRender.SetPosition(m_position);
}

void Brave::CalcAttackMoveSpeed()
{
	//moveSpeed(移動方向)の取得
	m_moveSpeed = calcVelocity(GetStatus());
	m_moveSpeed.y = 0.0f;
	//前方向を設定
	m_calcCharacterForward.get()->CalcForwardOfNearMonster(
		m_position, m_forward, m_moveSpeed, 150.0f);
}

void Brave::BeforeWeaponInfo()
{
	//コンボが繋がっている時にダメージを受けたかもしれないのでリセット
	m_attackPatternState = enAttackPattern_None;
	//武器切り替え中にダメージを受けたら
	if (m_enAnimationState == enAnimationState_ChangeWeapon)
	{
		//武器切り替え前の武器のアニメーションが再生されるようにする
		m_currentAnimationStartIndexNo
			= m_mainUseWeapon.weaponAnimationStartIndexNo;
	}
}

void Brave::ProcessComboAttack()
{
	//パターンステートを一つ進める
	m_attackPatternState =
		static_cast<EnAttackPattern>(m_attackPatternState + 1);
	//通常攻撃ステート設定
	SetNextAnimationState(m_attackPatternState);

	//モンスターにダメージを与えられるようにフラグをリセット
	CharactersInfoManager::GetInstance()->SetAllMonsterDamgeHitFlag(true);

	//敵のためのコンボステートを設定
	switch (m_attackPatternState)
	{
	case Brave::enAttackPattern_None:
		break;
	case Brave::enAttackPattern_1:
		//ノックバックパワーを設定
		SetKnockBackPower(m_mainUseWeapon.weapon->GetKnockBackPower(1));
		break;
	case Brave::enAttackPattern_2:
		//ノックバックパワーを設定
		SetKnockBackPower(m_mainUseWeapon.weapon->GetKnockBackPower(2));
		break;
	case Brave::enAttackPattern_3:
		//ノックバックパワーを設定
		SetKnockBackPower(m_mainUseWeapon.weapon->GetKnockBackPower(3));
		break;
	case Brave::enAttackPattern_End:
		break;
	default:
		break;
	}
	//3コンボ以上ならパターンステートリセット
	if (m_attackPatternState >= enAttackPattern_3)
	{
		m_attackPatternState = enAttackPattern_None;
	}
	//アクションフラグをセット
	SetIsActionFlag(true);
}

void Brave::ProcessCommonStateTransition()
{
	if (fabsf(GetMoveSpeed().x) >= 0.001f ||
		fabsf(GetMoveSpeed().z) >= 0.001f
		)
	{
		//走るアニメーションステート
		SetNextAnimationState(enAninationState_Sprint);
	}
	else
	{
		//待機アニメーションステート
		SetNextAnimationState(enAninationState_Idle);
	}
}

void Brave::ProcessCommonWeaponChangeStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//無敵状態フラグのリセット
		SetInvicibleFlag(false);
		//現在の武器のアニメーションクリップの最初の番号を変更
		m_currentAnimationStartIndexNo
			= m_mainUseWeapon.weaponAnimationStartIndexNo;
		//切り替えアニメーションが終わったのでアクションフラグをfalseにする
		SetIsActionFlag(false);
		//ステート共通の状態遷移処理に遷移
		ProcessCommonStateTransition();
	}
}

void Brave::ProcessNormalAttackStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//次のコンボに繋げない、または3コンボ目ならリセット
		if (GetNextComboFlagFlag() == false ||
			m_attackPatternState == enAttackPattern_None)
		{
			m_position.y = 0.0f;
			m_attackPatternState = enAttackPattern_None;
			//攻撃アニメーションが終わったのでアクション構造体のフラグを全てリセット
			SetAllInfoAboutActionFlag(false);
			//ステート共通の状態遷移処理に遷移
			ProcessCommonStateTransition();
		}
		//次のコンボの攻撃ステート設定
		else
		{
			//向いている方向に前進するために前方向を計算
			CalcAttackMoveSpeed();
			//次のコンボの処理
			ProcessComboAttack();
		}
		//必ずリセット
		{
			//コンボ受付フラグをリセット
			SetIsComboReceptionFlagFlag(false);
			//次のコンボフラグをリセット
			SetNextComboFlagFlag(false);
		}
	}
	//コンボ受付フラグがtrueかつAボタンを押したら
	else if (GetIsComboReceptionFlagFlag() == true &&
		g_pad[0]->IsTrigger(enButtonA))
	{
		//次のコンボに進むフラグセット
		SetNextComboFlagFlag(true);
	}
}

void Brave::ProcessSkillStartStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//メインステートに遷移
		SetNextAnimationState(enAnimationState_Skill_Main);
	}
}

void Brave::ProcessSkillMainStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//todo 無敵状態フラグのリセット
		SetInvicibleFlag(false);
		//攻撃アニメーションが終わったのでアクション構造体のフラグを全てリセット
		SetAllInfoAboutActionFlag(false);
		//ステート共通の状態遷移処理に遷移
		ProcessCommonStateTransition();
	}
}

void Brave::ProcessHitStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//アクション中にダメージ受けたかもしれないので
		// アクションフラグ関係を全てfalseにする
		SetAllInfoAboutActionFlag(false);
		//被ダメージによって戻せなかった変数をリセット
		m_mainUseWeapon.weapon->ResetVariable();
		//ステート共通の状態遷移処理に遷移
		ProcessCommonStateTransition();
	}
}

void Brave::ProcessDieStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//やられたのでdieFlagをtrueにする
		SetDieFlag(true);
		//バトル終了後の処理終わり
		GameManager::GetInstance()->SetGameFinishProcessEndFlag(true);
	}
}

void Brave::ProcessDefendStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//アクション中にダメージ受けたかもしれないので
		// アクションフラグ関係を全てfalseにする
		SetAllInfoAboutActionFlag(false);
		//被ダメージによって戻せなかった変数をリセット
		m_mainUseWeapon.weapon->ResetVariable();
		//ステート共通の状態遷移処理に遷移
		ProcessCommonStateTransition();
	}
}

void Brave::ProcessKnockBackStateTransition()
{
	//アクション中にダメージ受けたかもしれないので
	// アクションフラグ関係を全てfalseにする
	SetAllInfoAboutActionFlag(false);
	//被ダメージによって戻せなかった変数をリセット
	m_mainUseWeapon.weapon->ResetVariable();
	//ステート共通の状態遷移処理に遷移
	ProcessCommonStateTransition();
}

bool Brave::RotationOnly()
{
	//回転のみ可能なアニメーションまたは、現在の武器の防御タイプが盾で守るタイプなら
	if (isRotationEntable() == false &&
		m_mainUseWeapon.weapon->GetEnDefendTipe()==IWeapon::enDefendTipe_Defence)
	{
		return true;
	}
	//装備中の武器の回転可能フラグがtrueなら
	else if (m_mainUseWeapon.weapon->GetRotationDelectionFlag() == true)
	{
		return true;
	}
		

	return false;
}

void Brave::ReverseWeapon(EnWeapons changeTargetWeaponType)
{
	UseWeapon* ChangeUseSubWeapon = nullptr;

	if (changeTargetWeaponType == enWeapon_Sub)
	{
		//武器のサブとメインの中身を入れ替える
		ChangeUseWeapon(m_subUseWeapon);
		ChangeUseSubWeapon = &m_subUseWeapon;
	}
	else
	{
		//武器のサブ２とメインの中身を入れ替える
		ChangeUseWeapon(m_subUseWeapon2);
		ChangeUseSubWeapon = &m_subUseWeapon2;
	}

	//メイン武器とサブ武器の状態ステートを逆にする
	m_mainUseWeapon.weapon->ReverseWeaponState();
	ChangeUseSubWeapon->weapon->ReverseWeaponState();
	//攻撃力を現在の武器のものに変更。
	m_status.SetAtk(m_mainUseWeapon.weapon->GetStatus().GetAtk());
	//多段ヒット判定フラグをセット
	m_mainUseWeapon.weapon->SetHittableFlag(true);
	//武器を切り替えた
	SetChangeWeaponCompleteFlag(true);
	//切り替えた
}

void Brave::ChangeUseWeapon(UseWeapon& ChangeUseSubWeapon)
{
	UseWeapon temporary;
	temporary = m_mainUseWeapon;
	m_mainUseWeapon = ChangeUseSubWeapon;
	ChangeUseSubWeapon = temporary;
	//現在の武器のアニメーションクリップの最初の番号を変更
	m_currentAnimationStartIndexNo
		= m_mainUseWeapon.weaponAnimationStartIndexNo;
}

bool Brave::IsDefendHit()
{
	//ガード中なら
	if (m_enAnimationState == enAnimationState_Defend)
	{
		//盾に当たっているなら
		if (m_mainUseWeapon.weapon->IsHitCollision() == true)
		{
			return true;
		}
		return false;
	}
	else
	{
		return false;
	}
}

void Brave::SettingWeapons()
{
	//メイン武器はソード＆シールド
	SwordShield* swordShield = NewGO<SwordShield>(0, "swordshield");
	m_mainUseWeapon.weapon = swordShield;
	m_mainWeaponType = enWeaponType_SwordShield;
	//アニメーションクリップの最初の番号を設定
	SetCurrentAnimationStartIndexNo(m_mainUseWeapon, enWeapon_Main);
	//装備状態に設定
	m_mainUseWeapon.weapon->StartSetWeaponState(SwordShield::enWeaponState_Armed);

	//サブ１はグレイトソード
	BigSword* greatSword = NewGO<BigSword>(0, "greatSword");
	m_subUseWeapon.weapon = greatSword;
	m_subWeaponType = enWeaponType_TwoHandSword;
	//アニメーションクリップの最初の番号を設定
	SetCurrentAnimationStartIndexNo(m_subUseWeapon, enWeapon_Sub);
	m_subUseWeapon.weapon->StartSetWeaponState(BigSword::enWeaponState_Stowed);

	//サブ２はボウ＆アロー
	Bow* bowArrow = NewGO<Bow>(0, "bowarrow");
	m_subUseWeapon2.weapon = bowArrow;
	m_subWeapon2Type = enWeaponType_Bow;
	//アニメーションクリップの最初の番号を設定
	SetCurrentAnimationStartIndexNo(m_subUseWeapon2, enWeapon_Sub2);
	m_subUseWeapon2.weapon->StartSetWeaponState(BigSword::enWeaponState_Stowed);

	//現在の武器のアニメーションクリップの最初の番号
	m_currentAnimationStartIndexNo
		= m_mainUseWeapon.weaponAnimationStartIndexNo;

	//武器の攻撃力を自身の攻撃力に設定
	m_status.SetAtk(m_mainUseWeapon.weapon->GetWeaponPower());
}

void Brave::SettingChangeWeapon(
	int animationStartInbexNo, EnWeapons changeTargetWeapon)
{
	//現在の武器のアニメーションクリップの最初の番号をサブに変更
	m_currentAnimationStartIndexNo
		= animationStartInbexNo;
	m_changeTargetUseWeapon = changeTargetWeapon;
	//サブ武器の武器切り替えアニメーションステートを設定
	SetNextAnimationState(enAnimationState_ChangeWeapon);
	SetIsActionFlag(true);
}

void Brave::SetCurrentAnimationStartIndexNo(UseWeapon& useWeapon, EnWeapons mainOrSub)
{
	switch (mainOrSub)
	{
	case enWeapon_Main:
		useWeapon.weaponAnimationStartIndexNo = m_mainWeaponAnimationStartIndexNo;
		break;
	case enWeapon_Sub:
		useWeapon.weaponAnimationStartIndexNo = m_subWeaponAnimationStartIndexNo;
		break;
	case enWeapon_Sub2:
		useWeapon.weaponAnimationStartIndexNo = m_subWeapon2AnimationStartIndexNo;
		break;
	case enWeapon_num:
		std::abort();
		break;
	default:
		break;
	}
}

void Brave::RoadWeaponTypeAnimetionClip(EnWeaponType weaponType, int weaponAnimationStartIndexNo)
{
	switch (weaponType)
	{
	case enWeaponType_SwordShield:
		RoadOneHandSwordAnimationClip(weaponAnimationStartIndexNo);
		break;
	case enWeaponType_TwoHandSword:
		RoadTwoHandSwordAnimationClip(weaponAnimationStartIndexNo);
		break;
	case enWeaponType_Bow:
		RoadBowAnimationClip(weaponAnimationStartIndexNo);
		break;
	case enWeaponType_Num:
		std::abort();
		break;
	default:
		break;
	}
}

void Brave::RoadOneHandSwordAnimationClip(int mainWeaponAnimationStartIndexNo)
{
	// 片手剣のアニメーションクリップをロードする
	const std::pair<const char*, bool> oneHandedSwordAnimClipFilePaths[] = {
		{"Assets/animData/character/Player/OneHandSword/Idle.tka",true},
		{"Assets/animData/character/Player/OneHandSword/Sprint.tka",true},
		{"Assets/animData/character/Player/OneHandSword/KnockBack.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Hit.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Defend.tka", true },
		{"Assets/animData/character/Player/OneHandSword/DefendHit.tka", false },
		{"Assets/animData/character/Player/OneHandSword/Die.tka", false },
		{"Assets/animData/character/Player/OneHandSword/ChangeSwordShield.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Win_start.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Win_main.tka",true},
		{"Assets/animData/character/Player/OneHandSword/Attack_1.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Attack_2.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Attack_3.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Skill_Start.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Skill_Main.tka",false},
		{"None",false}
	};
	for (int i = 0; i < enAnimClip_Num; i++) {
		m_animationClip[mainWeaponAnimationStartIndexNo + i].Load(oneHandedSwordAnimClipFilePaths[i].first);
		m_animationClip[mainWeaponAnimationStartIndexNo + i].SetLoopFlag(oneHandedSwordAnimClipFilePaths[i].second);
	}
}
void Brave::RoadTwoHandSwordAnimationClip(int mainWeaponAnimationStartIndexNo)
{
	// 両手剣のアニメーションクリップをロードする
	const std::pair<const char*, bool> twoHandedSwordAnimClipFilePaths[] = {
		{"Assets/animData/character/Player/TwoHandSword/idle_BigSword.tka",true},
		{"Assets/animData/character/Player/TwoHandSword/Sprint.tka",true},
		{"Assets/animData/character/Player/TwoHandSword/KnockBack.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Hit.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Rool.tka", false },
		{"Assets/animData/character/Player/TwoHandSword/Rool.tka", false },
		{"Assets/animData/character/Player/TwoHandSword/Die.tka", false },
		{"Assets/animData/character/Player/TwoHandSword/ChangeTwoHandSword.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Win_Start.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Win_Main.tka",true},
		{"Assets/animData/character/Player/TwoHandSword/Attack_1.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Attack_2.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Attack_3.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Skill_Start2.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Skill_Main2.tka",false},
		{"None",false}
	};
	for (int i = 0; i < enAnimClip_Num; i++) {
		m_animationClip[mainWeaponAnimationStartIndexNo + i].Load(twoHandedSwordAnimClipFilePaths[i].first);
		m_animationClip[mainWeaponAnimationStartIndexNo + i].SetLoopFlag(twoHandedSwordAnimClipFilePaths[i].second);
	}
}
void Brave::RoadBowAnimationClip(int mainWeaponAnimationStartIndexNo)
{
	// 弓のアニメーションクリップをロードする
	const std::pair<const char*, bool> bowAnimClipFilePaths[] = {
		{"Assets/animData/character/Player/Bow/Idle.tka",true},
		{"Assets/animData/character/Player/Bow/Sprint.tka",true},
		{"Assets/animData/character/Player/Bow/KnockBack.tka",false},
		{"Assets/animData/character/Player/Bow/Hit.tka",false},
		{"Assets/animData/character/Player/Bow/Rool.tka", false },
		{"Assets/animData/character/Player/Bow/Rool.tka", false },
		{"Assets/animData/character/Player/Bow/Die.tka", false },
		{"Assets/animData/character/Player/Bow/ChangeBow.tka",false},
		{"Assets/animData/character/Player/Bow/Win_Start.tka",false},
		{"Assets/animData/character/Player/Bow/Win_Main.tka",true},
		{"Assets/animData/character/Player/Bow/Attack_1.tka",false},
		{"Assets/animData/character/Player/Bow/Attack_2.tka",false},
		{"Assets/animData/character/Player/Bow/Attack_3.tka",false},
		{"Assets/animData/character/Player/Bow/Skill_Start.tka",true},
		{"Assets/animData/character/Player/Bow/Skill_Main.tka",false},
		{"None",false}
	};
	for (int i = 0; i < enAnimClip_Num; i++) {
		m_animationClip[mainWeaponAnimationStartIndexNo + i].Load(bowAnimClipFilePaths[i].first);
		m_animationClip[mainWeaponAnimationStartIndexNo + i].SetLoopFlag(bowAnimClipFilePaths[i].second);
	}
}

void Brave::InitModel()
{
	//メイン武器に対応するアニメーションクリップを読み込む
	RoadWeaponTypeAnimetionClip(m_mainWeaponType, m_mainWeaponAnimationStartIndexNo);
	//サブ武器に対応するアニメーションクリップを読み込む
	RoadWeaponTypeAnimetionClip(m_subWeaponType, m_subWeaponAnimationStartIndexNo);
	//サブ武器2に対応するアニメーションクリップを読み込む
	RoadWeaponTypeAnimetionClip(m_subWeapon2Type, m_subWeapon2AnimationStartIndexNo);
	//モデルの初期化
	m_modelRender.Init("Assets/modelData/character/Player/NewHero/Hero_Smile_Selllook.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		m_animationClip,
		enAnimClip_Num * AnimationClipGroup_Num,
		enModelUpAxisZ
	);
	//座標の設定と更新
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
	
	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
	});
}

void Brave::Render(RenderContext& rc)
{
	if (IsFlashing() != true)
	{
		m_modelRender.Draw(rc);
	}
}

void Brave::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	///////////////////////////////////////////////////////////
	// 全武器共通処理
	/////////////////////////////////////////////////////////// 
	//コンボ受付が可能なら
	if (wcscmp(eventName, L"ComboAcceptable") == 0)
	{
		SetIsComboReceptionFlagFlag(true);
	}
	//前進する始まり
	if (wcscmp(eventName, L"MoveForwardStart") == 0)
	{
		//向いている方向に前進するために前方向を計算
		//CalcAttackMoveSpeed();
		//前方向に移動フラグをセット
		SetMoveforwardFlag(true);
	}
	//前進する終わり
	if (wcscmp(eventName, L"MoveForwardEnd") == 0)
	{
		//前方向に移動フラグをリセット
		SetMoveforwardFlag(false);
	}

	//後退する始まり
	if (wcscmp(eventName, L"MoveBackStart") == 0)
	{
		//向いている方向に前進するために前方向を計算
		CalcForward(m_moveSpeed);
		SetMoveBackFlag(true);
	}
	//後退する終わり
	if (wcscmp(eventName, L"MoveBackEnd") == 0)
	{
		SetMoveBackFlag(false);
	}

	//回避時の移動の始まり
	if (wcscmp(eventName, L"AvoidMoveStart") == 0)
	{
		//前方向を計算
		CalcForward(m_moveSpeed);
		//前方向に移動フラグをセット
		SetMoveforwardFlag(true);
	}
	//回避時の移動の終わり
	if (wcscmp(eventName, L"AvoidMoveEnd") == 0)
	{
		//前方向に移動フラグをリセット
		SetMoveforwardFlag(false);
	}

	//武器入れ替え
	if (wcscmp(eventName, L"ArmedSwordShield") == 0)
	{
		ReverseWeapon(m_changeTargetUseWeapon);
	}

	//攻撃当たり判定の有効化
	if (wcscmp(eventName, L"CollisionStart") == 0)
	{
		SetIsCollisionPossibleFlag(true);
	}
	//攻撃当たり判定の無効化
	if (wcscmp(eventName, L"CollisionEnd") == 0)
	{
		SetIsCollisionPossibleFlag(false);
	}

	//無敵状態の有効化
	if (wcscmp(eventName, L"InvisibleStart") == 0)
	{
		SetInvicibleFlag(true);
	}
	//無敵状態の無効化
	if (wcscmp(eventName, L"InvisibleEnd") == 0)
	{
		SetInvicibleFlag(false);
	}
}

bool Brave::isCollisionEntable() const
{
	//もし武器の防御タイプが盾などで防ぐタイプなら
	if (m_mainUseWeapon.weapon->GetEnDefendTipe()==IWeapon::enDefendTipe_Defence)
	{
		return m_enAnimationState == enAnimationState_Hit ||
			m_enAnimationState == enAnimationState_DefendHit;
	}
	//それ以外(回避)なら
	else
	{
		return m_enAnimationState == enAnimationState_Hit ||
			m_enAnimationState == enAnimationState_Defend ||
			m_enAnimationState == enAnimationState_DefendHit;
	}
}

bool Brave::isRotationEntable() const
{
	//もし武器の防御タイプが盾などで防ぐタイプなら
	if (m_mainUseWeapon.weapon->GetEnDefendTipe() == IWeapon::enDefendTipe_Defence)
	{
		return m_enAnimationState != enAnimationState_Defend;
	}
	//それ以外(回避)なら
	else
	{
		return m_enAnimationState == enAnimationState_Defend;
	}
}
