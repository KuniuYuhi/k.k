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

#include "Player.h"
#include "KnockBack.h"

#include "SwordShield.h"
#include "BigSword.h"
#include "Bow.h"
#include "ManagerPreCompile.h"


namespace {
	const float ADD_SCALE = 1.2f;

	const float ROT_SPEED = 20.0f;
	const float ROT_ONLY_SPEED = 20.0f;

	const float SWORD_EFFECT_SIZE = 10.0f;
	const Vector3 DASH_EFFECT_SIZE = { 10.0f,20.0f,8.0f };

	int MAXHP = 200;
	int MAXMP = 100;
	int ATK = 40;
	float SPEED = 200.0f;
	const char* NAME = "Brave";

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
	m_status.InitStatus(
		MAXHP,
		MAXMP,
		ATK,
		SPEED,
		NAME
	);

	//装備する武器の設定
	SettingWeapons();

	InitModel();

	//キャラコンの設定
	m_charaCon.Init(12.0f, 33.0f, m_position);

	

	SetNextAnimationState(enAninationState_Idle);


	


	return true;
}

void Brave::Update()
{
	//行動不可能な状態でないなら
	if (IsInaction() != true)
	{
		//MPの回復
		RecoveryMP();
		//武器の切り替え処理
		ChangeWeapon();
		//移動処理
		Move();
		//回転処理
		ProcessRotation();
		//攻撃処理
		ProcessAttack();
		//防御処理
		ProcessDefend();
		//無敵時間の計算
		CalcInvincibleTime();
		//当たり判定
		DamageCollision(m_charaCon);
	}

	ManageState();
	PlayAnimation();
	
	SetTransFormModel(m_modelRender);

	m_modelRender.Update();
	
}

void Brave::Move()
{
	m_moveSpeed = calcVelocity(GetStatus());
	m_moveSpeed.y = 0.0f;

	//特定のアニメーションが再生中なら移動なし
	if (isAnimationEntable() != true)
	{
		if (m_mainUseWeapon.weapon->GetRotationDelectionFlag() == true)
		{
			//前方向の計算
			CalcForward(m_moveSpeed);
		}
		m_moveSpeed = g_vec3Zero;
	}
	else
	{
		CalcForward(m_moveSpeed);
	}


	m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
}

void Brave::ProcessRotation()
{
	Rotation(ROT_SPEED, ROT_ONLY_SPEED);
}

void Brave::ProcessAttack()
{
	//アクションフラグがtrueなら攻撃処理をしない
	if (GetIsActionFlag() == true)
	{
		return;
	}
	//通常攻撃
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		ProcessComboAttack();
		return;
	}
	//スキル
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		//アクションフラグをセット
		SetIsActionFlag(true);
		SetNextAnimationState(enAnimationState_Skill_start);
	}
}

void Brave::ProcessDefend()
{
	//アクションフラグがtrueなら攻撃処理をしない
	if (GetIsActionFlag() == true)
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
	if (GetStatus().hp > 0)
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

		if (IsDefendHit() == true)
		{
			//ダメージを1/3に減らす
			damage /= 3;
			//どれだけダメージを減らしても１以下にはならない
			if (damage < 1) { damage = 1; }
			//盾ヒットステートに遷移
			SetNextAnimationState(enAnimationState_DefendHit);
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
	if (GetStatus().hp <= 0)
	{
		//やられたのでdieFlagをtrueにする
		SetDieFlag(true);
		//点滅しないようにする
		SetInvicibleTimeFlag(false);
		//HPを0に固定する
		m_status.SetHp(0);
		//死亡ステートに遷移
		SetNextAnimationState(enAnimationState_Die);
	}
}

const bool& Brave::IsInaction() const
{
	//行動出来なくなる条件
	//プレイヤークラスの関数の動けない条件がtrueなら
	if (m_player->IsInaction() == true)
	{
		return true;
	}
	//やられたなら
	if (GetDieFlag() == true)
	{
		return true;
	}
	//ここまできたら行動可能
	return false;
}

void Brave::MoveForward(float Speed)
{
	//攻撃する方向
	Vector3 attackDirection = m_forward;
	//移動する速度
	Vector3 MoveSpeed = attackDirection * Speed;
	MoveSpeed.y = 0.0f;
	m_position = m_charaCon.Execute(MoveSpeed, 1.0f / 60.0f);
	m_modelRender.SetPosition(m_position);
}

void Brave::ChangeWeapon()
{
	//アクションフラグがtrueなら攻撃処理をしない
	if (GetIsActionFlag() == true)
	{
		return;
	}
	//サブ１と武器切り替え
	if (g_pad[0]->IsTrigger(enButtonUp))
	{
		//武器切り替えのための前準備
		SettingChangeWeapon(
			m_subUseWeapon.weaponAnimationStartIndexNo, enWeapon_Sub);
	}
	//サブ２と武器切り替え
	if (g_pad[0]->IsTrigger(enButtonDown))
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

void Brave::ProcessSwordShieldSkill(bool UpOrDownFlag)
{
	Vector3 Y = g_vec3AxisY;
	float mulYPos = 0.0f;
	if (UpOrDownFlag == true)
	{
		//Up処理
		mulYPos += 
			g_gameTime->GetFrameDeltaTime() * m_mainUseWeapon.weapon->GetJampSpeed();
		Y.y += mulYPos;
	}
	else
	{
		//Down処理
		mulYPos += 
			g_gameTime->GetFrameDeltaTime() * m_mainUseWeapon.weapon->GetJampSpeed() * 1.2f;
		Y.y -= mulYPos;
	}
	
	m_position = m_charaCon.Execute(Y, 1.0f / 30.0f);
	//地面についているなら


	if (m_charaCon.IsOnGround() == true|| m_position.y < 0.0f)
	{
		m_position.y = 0.0f;
	}

	m_modelRender.SetPosition(m_position);
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
	case Brave::enAninationState_DashForward:
		break;
	case Brave::enAnimationState_KnockBack:
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

void Brave::SetAttackPosition(Vector3 attackPosition)
{
	m_player->SetAttackPosition(attackPosition);
}

void Brave::ProcessComboAttack()
{
	//パターンステートを一つ進める
	m_attackPatternState =
		static_cast<EnAttackPattern>(m_attackPatternState + 1);

	//通常攻撃ステート設定
	SetNextAnimationState(m_attackPatternState);
	//敵のためのコンボステートを設定
	switch (m_attackPatternState)
	{
	case Brave::enAttackPattern_None:
		SetNowComboState(enNowCombo_None);
		break;
	case Brave::enAttackPattern_1:
		SetNowComboState(enNowCombo_1);
		break;
	case Brave::enAttackPattern_2:
		SetNowComboState(enNowCombo_2);
		break;
	case Brave::enAttackPattern_3:
		SetNowComboState(enNowCombo_3);
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
		//走る
		SetNextAnimationState(enAninationState_Sprint);
	}
	else
	{
		//歩く
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
			//コンボ状態をリセット
			SetComboStateNone();
			//ステート共通の状態遷移処理に遷移
			ProcessCommonStateTransition();
		}
		//次のコンボの攻撃ステート設定
		else
		{
			//次のコンボに移る前に回転する
			if (fabsf(m_SaveMoveSpeed.x) >= 0.001f || fabsf(m_SaveMoveSpeed.z) >= 0.001f)
			{
				m_rotation.SetRotationYFromDirectionXZ(m_SaveMoveSpeed);
				m_forward = m_SaveMoveSpeed;
				m_forward.Normalize();
			}
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

		//ノックバック攻撃フラグが立っていたらリセット
		if (GetKnockBackAttackFalg() == true)
		{
			SetKnockBackAttackFalg(false);
		}

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
		//コンボ状態をリセット
		SetComboStateNone();
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
	}
}

void Brave::ProcessDefendStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//アクション中にダメージ受けたかもしれないので
		// アクションフラグ関係を全てfalseにする
		SetAllInfoAboutActionFlag(false);
		//コンボ状態をリセット
		SetComboStateNone();
		//被ダメージによって戻せなかった変数をリセット
		m_mainUseWeapon.weapon->ResetVariable();
		//ステート共通の状態遷移処理に遷移
		ProcessCommonStateTransition();
	}
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
	//逆参照あった
	//攻撃力を現在の武器のものに変更。
	m_status.atk = m_mainUseWeapon.weapon->GetWeaponPower();
	//多段ヒット判定フラグをセット
	m_mainUseWeapon.weapon->SetHittableFlag(true);
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
	m_status.atk = m_mainUseWeapon.weapon->GetWeaponPower();


	//メイン武器の生成とインスタンスの代入
	//m_mainWeaponType = WeaponManager::GetInstance()->GetMainWeapon();
	//m_mainUseWeapon.weapon =
	//	WeaponManager::GetInstance()->CreateWeapon(m_mainWeaponType);
	////メイン武器を装備状態にする
	//m_mainUseWeapon.weapon->StartSetWeaponState(WeaponBase::enWeaponState_Armed);
	////アニメーションクリップの最初の番号を設定
	//SetCurrentAnimationStartIndexNo(m_mainUseWeapon, enWeapon_Main);
	////サブ武器の生成とインスタンスの代入
	//m_subWeaponType = WeaponManager::GetInstance()->GetSubWeapon();
	//m_subUseWeapon.weapon =
	//	WeaponManager::GetInstance()->CreateWeapon(m_subWeaponType);
	////サブ武器を収納状態にする
	//m_subUseWeapon.weapon->StartSetWeaponState(WeaponBase::enWeaponState_Stowed);
	////アニメーションクリップの最初の番号を設定
	//SetCurrentAnimationStartIndexNo(m_subUseWeapon, enWeapon_Sub);
	////現在の武器のアニメーションクリップの最初の番号
	//m_currentAnimationStartIndexNo
	//	= m_mainUseWeapon.weaponAnimationStartIndexNo;
	////武器の攻撃力を自身の攻撃力に設定
	//m_status.atk = m_mainUseWeapon.weapon->GetWeaponPower();

	////ウェポンマネージャーの削除
	//WeaponManager::DeleteInstance();
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
	case Brave::enWeapon_Main:
		useWeapon.weaponAnimationStartIndexNo = m_mainWeaponAnimationStartIndexNo;
		break;
	case Brave::enWeapon_Sub:
		useWeapon.weaponAnimationStartIndexNo = m_subWeaponAnimationStartIndexNo;
		break;
	case Brave::enWeapon_Sub2:
		useWeapon.weaponAnimationStartIndexNo = m_subWeapon2AnimationStartIndexNo;
		break;
	case Brave::enWeapon_num:
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
		{ "Assets/animData/character/Player/OneHandSword/Defend.tka", true },
		{ "Assets/animData/character/Player/OneHandSword/DefendHit.tka", false },
		{ "Assets/animData/character/Player/OneHandSword/Die.tka", false },
		{"Assets/animData/character/Player/OneHandSword/ChangeSwordShield.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Win_start.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Win_main.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Attack_1.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Attack_2.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Attack_3.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Attack_4.tka",false},
		{"Assets/animData/character/Player/OneHandSword/Attack_5.tka",false},
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
		{ "Assets/animData/character/Player/TwoHandSword/Rool.tka", false },
		{ "Assets/animData/character/Player/TwoHandSword/Rool.tka", false },
		{ "Assets/animData/character/Player/TwoHandSword/Die.tka", false },
		{"Assets/animData/character/Player/TwoHandSword/ChangeTwoHandSword.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Win_Start.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Win_Main.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Attack_1.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Attack_2.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Attack_3.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Skill_Start.tka",false},
		{"Assets/animData/character/Player/TwoHandSword/Skill_Main.tka",false},
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
		{ "Assets/animData/character/Player/Bow/Rool.tka", false },
		{ "Assets/animData/character/Player/Bow/Rool.tka", false },
		{ "Assets/animData/character/Player/Bow/Die.tka", false },
		{"Assets/animData/character/Player/Bow/ChangeBow.tka",false},
		{"Assets/animData/character/Player/Bow/Win_Start.tka",false},
		{"Assets/animData/character/Player/Bow/Win_Main.tka",false},
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
	m_modelRender.Init("Assets/modelData/character/Player/NewHero/Hero_Smile.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		m_animationClip,
		enAnimClip_Num * AnimationClipGroup_Num,
		enModelUpAxisZ
	);
	
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	m_charaCenterBoonId = m_modelRender.FindBoneID(L"root");

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
		CalcForward(m_moveSpeed);
		SetMoveforwardFlag(true);
	}
	//前進する終わり
	if (wcscmp(eventName, L"MoveForwardEnd") == 0)
	{
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

	//スキル使用時の攻撃処理
	if (wcscmp(eventName, L"SkillAttack") == 0)
	{
		//メイン武器のスキル攻撃処理
		m_mainUseWeapon.weapon->ProcessSkillAttack();
	}
	//スキルのジャンプ処理
	if (wcscmp(eventName, L"Jamp") == 0)
	{
		//キーフレームがJampの間処理し続ける
		ProcessSwordShieldSkill(true);
	}
	//スキルのジャンプ処理
	if (wcscmp(eventName, L"Down") == 0)
	{
		//キーフレームがJampの間処理し続ける
		ProcessSwordShieldSkill(false);
	}

	//武器固有の処理はそれぞれの武器クラスのアニメーションイベント関数にある

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
