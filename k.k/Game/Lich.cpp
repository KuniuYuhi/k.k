#include "stdafx.h"

#include "Lich.h"
#include "LichStateIdle.h"
#include "LichStateWalk.h"
#include "LichStateAttack_1.h"
#include "LichStateAttack_2.h"
#include "LichStateDie.h"
#include "Game.h"
#include "DarkWall.h"
#include "LichStateDamage.h"
#include "LichStateDarkMeteorite_Start.h"
#include "LichStateDarkMeteorite_Main.h"
#include "LichStateDarkMeteorite_End.h"
#include "DarkMeteorite.h"
#include "LichStateSummon.h"
#include "LichStateVictory.h"

#include "LichAction.h"
#include "Summon.h"
#include "LichStateAngry.h"
#include "LichStateWarp.h"

//todo ターゲットがしばらく近くにいたら逃げる

namespace {
	const float SCALE_UP = 4.0f;									//キャラクターのサイズ
	const Vector3 FIRST_POSITION = Vector3(0.0f, 0.0f, -250.0f);	//最初の座標
	const float DISTANCE = 4000.0f;									//プレイヤーを発見できる距離
	const float NON_WARP_DISTANCE = 400.0f;							//ワープ不要な距離

	const float FARST_ATTACK_INTERVAL = 3.0f;

	const Vector3 WARP_UP = { 0.0f,800.0f,0.0f };
	const Vector3 WARP_DOWN = { 0.0f,-800.0f,0.0f };

	const float WARP_POS_Y_UP = 2500.0f;

	const float ADD_CREATE_DARK_BALL_1_Y = 30.0f;
	const float ADD_CREATE_DARK_BALL_2_Y = -30.0f;

	const float HULF_HP_ATK_INTERVAL = 3;

	//ステータス
	int MAXHP = 1000;
	int MAXMP = 500;
	int ATK = 20;
	float SPEED = 160.0f;
	const char* NAME = "Lich";
}

Lich::Lich()
{
	//最初の座標を設定する
	m_position = FIRST_POSITION;
	//プレイヤーを発見できる距離を設定する
	m_distance = DISTANCE;
	//リッチのサイズを設定
	m_scale *= SCALE_UP;

	//最初の攻撃のインターバルを早める
	m_attackIntervalTimer = FARST_ATTACK_INTERVAL;
}

Lich::~Lich()
{
	if (m_monsters.size() != 0)
	{
		//生き残っているモンスターを死亡させる
		for (auto monster : m_monsters)
		{
			monster->Dead();
		}
	}

	if (m_lichAction != nullptr)
	{
		delete m_lichAction;
	}
}

bool Lich::Start()
{
	//乱数を初期化。
	srand((unsigned)time(NULL));

	m_game = FindGO<Game>("game");

	//初期のアニメーションステートを待機状態にする。
	SetNextAnimationState(enAninationState_Idle);

	//ステータスの初期化
	m_status.InitStatus(
		MAXHP,
		MAXMP,
		ATK,
		SPEED,
		NAME
	);

	InitModel();
	//
	SetStageLevelPosition();
	//状態ステート
	SetSpecialActionState(enSpecialActionState_Normal);
	
	m_lichAction = new LichAction(this);
	//todo 優先度設定する
	m_lichAction->SettingPriority();

	return true;
}

void Lich::InitModel()
{
	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/Lich/Idle.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Walk].Load("Assets/animData/character/Lich/Walk.tka");
	m_animationClip[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClip[enAnimClip_Attack_1].Load("Assets/animData/character/Lich/Attack1.tka");
	m_animationClip[enAnimClip_Attack_1].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_2].Load("Assets/animData/character/Lich/Attack2.tka");
	m_animationClip[enAnimClip_Attack_2].SetLoopFlag(false);
	m_animationClip[enAnimClip_Die].Load("Assets/animData/character/Lich/Die.tka");
	m_animationClip[enAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enAnimClip_Damage].Load("Assets/animData/character/Lich/Damage.tka");
	m_animationClip[enAnimClip_Damage].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_DarkMeteorite_start].Load("Assets/animData/character/Lich/DarkMeteorite_Start.tka");
	m_animationClip[enAnimClip_Attack_DarkMeteorite_start].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_DarkMeteorite_main].Load("Assets/animData/character/Lich/DarkMeteorite.tka");
	m_animationClip[enAnimClip_Attack_DarkMeteorite_main].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_DarkMeteorite_end].Load("Assets/animData/character/Lich/DarkMeteorite_End.tka");
	m_animationClip[enAnimClip_Attack_DarkMeteorite_end].SetLoopFlag(false);
	m_animationClip[enAnimClip_Summon].Load("Assets/animData/character/Lich/Summon.tka");
	m_animationClip[enAnimClip_Summon].SetLoopFlag(false);
	m_animationClip[enAnimClip_Victory].Load("Assets/animData/character/Lich/Victory.tka");
	m_animationClip[enAnimClip_Victory].SetLoopFlag(true);
	m_animationClip[enAnimClip_Angry].Load("Assets/animData/character/Lich/Angry.tka");
	m_animationClip[enAnimClip_Angry].SetLoopFlag(false);


	m_modelRender.Init("Assets/modelData/character/Lich/Lich_real.tkm",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);

	//モデルのTRSの設定
	m_modelRender.SetTransform(
		m_position,
		m_rotation,
		m_scale
	);
	m_modelRender.Update();

	//キャラコンの設定
	m_charaCon.Init(
		70.0f,
		50.0f,
		m_position
	);

	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//ダークウォールに使うボーンID取得
	m_darkWallBoonId = m_modelRender.FindBoneID(L"Index_Proximal_L");

}

void Lich::Update()
{	
	//勝敗が決まったら他の処理を実行しないようにする
	if (IsStopProcessing()==true)
	{
		ManageState();
		PlayAnimation();
		m_modelRender.Update();
		return;
	}

	//被ダメージの当たり判定
	DamageCollision(m_charaCon);
	//インターバルの計算
	AttackInterval(m_attackIntervalTime);
	DamageInterval(m_damageIntervalTime);

	//怒りモードなら怒りモードタイマーの計算
	CalcAngryTime();

	Move();
	Rotation();

	DecideNextAction();

	ManageState();
	PlayAnimation();

	SetTransFormModel(m_modelRender);
	m_modelRender.Update();

	m_oldMoveSpeed = m_moveSpeed;
}

bool Lich::IsStopProcessing()
{
	//タイムアップしたら
	if (m_game->IsTimeUp() == true)
	{
		SetNextAnimationState(enAninationState_Idle);
		//タイムアップで終わったのでフラグをtrueにする
		m_timeUpEndFlag = true;
		//ダークメテオの削除
		DeleteDarkMeteo();
		return true;
	}
	//プレイヤーが全滅したら勝利アニメーション設定
	if (m_player->IsAnnihilation() == true && m_enAnimationState != enAnimationState_Victory)
	{
		SetWinFlag(true);
		//攻撃中じゃないなら
		if (IsAttackEntable() == true)
		{
			SetNextAnimationState(enAnimationState_Victory);
		}
		//ダークメテオの削除
		DeleteDarkMeteo();
		return true;
	}
	//勝ったフラグがtrueなら
	if (GetWinFlag() == true)
	{
		//ダークメテオの削除
		DeleteDarkMeteo();
		return true;
	}
	//自身がやられたら
	if (m_dieFlag == true)
	{
		return true;
	}
	//全て違うなら
	return false;
}

void Lich::Move()
{
	//プレイヤーの座標を取得
	SetTargetPosition();

	//移動処理
	m_moveSpeed = CalcVelocity(m_status, m_targetPosition);

	//被ダメージ時は処理をしない
	if (isAnimationEntable() != true)
	{
		return;
	}
	//怒りモードでないなら抜け出す
	if (m_enSpecialActionState != enSpecialActionState_AngryMode)
	{
		//移動しないようにする
		m_moveSpeed = Vector3::Zero;
		return;
	}

	//攻撃中なら移動しない
	if (IsAttackEntable() != true)
	{
		//移動しないようにする
		m_moveSpeed = Vector3::Zero;
	}
	//プレイヤーを見つけたら
	else if (IsFindPlayer(m_distance) == true)
	{
		//範囲内でもちょびちょび動かないようにしたい
		//一定の距離になったらそれ以上動かない(ターゲットに近づかないようにする)
		if (IsDistanceToPlayer() == true)
		{
			//移動しないようにする
			m_moveSpeed = Vector3::Zero;
		}
		//移動する
		m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	}
	else
	{
		//移動しないようにする
		m_moveSpeed = Vector3::Zero;
	}
}

void Lich::Damage(int attack)
{
	//ヒットカウントを増やす、蓄積ダメージを増やす
	SetHitCountAndDamage(1, attack);
	//怒りモードカウントを増やす
	m_angryModeCount++;

	if (m_status.hp > 0)
	{
		//一定確率で怯む
		if (Isflinch() == true)
		{
			//技の途中かもしれない
			m_CreateDarkWallFlag = false;
			SetNextAnimationState(enAnimationState_Damage);
		}
		//HPを減らす
		m_status.hp -= attack;

		//HPが半分になったら
		if (m_status.hp <= m_status.maxHp / 2)
		{
			m_halfHpFlag = true;
			//攻撃間隔を短くする
			m_attackIntervalTime = HULF_HP_ATK_INTERVAL;
		}
	}
	//やられたとき
	if(m_status.hp <= 0)
	{
		//やられるところをゆっくりにする
		//フレームレートを落とす
		g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Variable, 30);
		//自身が倒されたらことをゲームに伝える
		m_game->SetDeathBossFlag(true);
		//カメラがリッチを追うようにする
		m_game->SetClearCameraState(Game::enClearCameraState_Lich);
		//Dieフラグをtrueにする
		m_dieFlag = true;
		m_status.hp = 0;
		//技の途中でやられたかもしれない
		m_CreateDarkWallFlag = false;
		//やられステート
		SetNextAnimationState(enAnimationState_Die);
		m_modelRender.SetAnimationSpeed(0.8f);
	}
}

void Lich::CreateDamageFont(int damage)
{
	DamageFont* damagefont = NewGO<DamageFont>(0, "damagefont");
	damagefont->Setting(
		DamageFont::enDamageActor_Boss,
		damage,
		m_position
	);
}

bool Lich::Isflinch()
{
	int value = rand() % 10;
	if (value > 7)
	{
		//怯む
		return true;
	}
	else
	{
		//怯まない
		return false;
	}
}

bool Lich::IsDistanceToPlayer()
{
	Vector3 diff = m_targetPosition - m_position;
	//プレイヤーとの距離
	if (diff.Length() < m_distanceToPlayer)
	{
		//距離内にいる
		return true;
	}
	else
	{
		//距離内にいない
		return false;
	}
	
}

bool Lich::CalcAngryTime()
{
	//怒りモードでないときは処理しない
	if (m_enSpecialActionState != enSpecialActionState_AngryMode)
	{
		return false;
	}

	if (IsAttackEntable() != true)
	{
		return false;
	}

	if (m_angryLimitTime < m_angryLimitTimer)
	{
		m_angryLimitTimer = 0.0f;
		//通常に戻す
		SetSpecialActionState(enSpecialActionState_Normal);
		//怒りモードカウントをリセットする
		m_angryModeCount = 0;
		//怒りモードから戻るときにプレイヤーから一番遠いところにワープする
		SetNextAnimationState(enAnimationState_Warp);
		//無敵時間にする
		SetInvincibleFlag(true);
		return false;
	}
	else
	{
		m_angryLimitTimer += g_gameTime->GetFrameDeltaTime();
	}

	return true;
}

bool Lich::RotationOnly()
{
	//特定のアニメーションが再生中のとき
	if (isRotationEntable() != true)
	{
		//xかzの移動速度があったら(スティックの入力があったら)。
		if (fabsf(m_SaveMoveSpeed.x) >= 0.001f || fabsf(m_SaveMoveSpeed.z) >= 0.001f)
		{
			m_rotation.SetRotationYFromDirectionXZ(m_SaveMoveSpeed);
		}
		return true;
	}

	return false;
}

void Lich::DecideNextAction()
{
	//todo 通常と怒りモードで攻撃範囲や速度を変えたい


	//被ダメージ時は処理をしない
	if (isAnimationEntable() != true)
	{
		return;
	}

	//攻撃中は処理をしない
	if (IsAttackEntable() != true)
	{
		return;
	}

	//攻撃可能なら
	if (m_attackFlag == false)
	{
		//次の行動を選ぶ
		m_lichAction->NextAction();

		m_attackFlag = true;
	}
}

void Lich::PlayAnimation()
{
	m_state->ManageState();
}

void Lich::ManageState()
{
	m_state->PlayAnimation();
}

void Lich::SetNextAnimationState(EnAnimationState nextState)
{
	if (m_state != nullptr) {
		// 古いステートを削除する。
		delete m_state;
		m_state = nullptr;
	}

	//アニメーションステートを次のステートに変える
	m_enAnimationState = nextState;

	switch (m_enAnimationState)
	{
	case Lich::enAninationState_Idle:
		// 待機ステートを作成する。
		m_state = new LichStateIdle(this);
		break;
	case Lich::enAninationState_Walk:
		// 待機ステートを作成する。
		m_state = new LichStateWalk(this);
		break;
	case Lich::enAninationState_Run:
		break;
	case Lich::enAnimationState_Attack_1:
		// アタック１ステートを作成する。
		m_state = new LichStateAttack_1(this);
		break;
	case Lich::enAnimationState_Attack_2:
		// アタック２ステートを作成する。
		m_state = new LichStateAttack_2(this);
		break;
	case Lich::enAnimationState_Damage:
		//被ダメージステートを作成する
		m_state = new LichStateDamage(this);
		break;
	case Lich::enAnimationState_Die:
		//Dieステートを作成する
		m_state = new LichStateDie(this);
		break;
	case Lich::enAnimationState_Attack_DarkMeteorite_start:
		//ダークメテオスタートステートを作成する
		m_state = new LichStateDarkMeteorite_Start(this);
		break;
	case Lich::enAnimationState_Attack_DarkMeteorite_main:
		//ダークメテオメインステートを作成する
		m_state = new LichStateDarkMeteorite_Main(this);
		break;
	case Lich::enAnimationState_Attack_DarkMeteorite_end:
		//ダークメテオエンドステートを作成する
		m_state = new LichStateDarkMeteorite_End(this);
		break;
	case Lich::enAninationState_Summon:
		//ダークメテオエンドステートを作成する
		m_state = new LichStateSummon(this);
		break;
	case Lich::enAnimationState_Victory:
		//勝利ステートを作成する
		m_state = new LichStateVictory(this);
		break;
	case Lich::enAnimationState_Angry:
		//怒りモードステートを作成する
		m_state = new LichStateAngry(this);
		break;
	case Lich::enAnimationState_Warp:
		//ワープステートを作成する
		m_state = new LichStateWarp(this);
		break;
	default:
		// ここに来たらステートのインスタンス作成処理の追加忘れ。
		std::abort();
		break;
	}
}

void Lich::SetStageLevelPosition()
{
	m_stageLevel.Init(
		"Assets/level3D/BossStage1.tkl",
		[&](LevelObjectData& objData)
		{
			if (objData.ForwardMatchName(L"Pos") == true) {
				//ワープする座標を配列に格納する
				Vector3 warpPos = objData.position;
				m_warpPositions.emplace_back(warpPos);
				return true;
			}
			return false;
		});
}

void Lich::DecideWarpPosition()
{
	float MaxLength = 0.0f;
	//ターゲットから一番遠いところ座標を調べる
	for (int amount = 0; amount < m_warpPositions.size(); amount++)
	{
		//ターゲットから自身へのベクトル
		Vector3 diff = m_warpPositions[amount] - m_targetPosition;
		//ターゲットから一番遠いところの座標にする
		if (MaxLength < diff.Length())
		{
			MaxLength = diff.Length();
			//一番遠いところの座標
			m_warpPosition = m_warpPositions[amount];
		}
	}
}

void Lich::MoveWarpPosition()
{
	m_warpPosition.y += WARP_POS_Y_UP;
	m_position = m_warpPosition;
	m_charaCon.SetPosition(m_position);
	Vector3 zero = g_vec3Zero;
	m_charaCon.Execute(zero, 1.0f / 60.0f);
}

void Lich::ProcessCommonStateTransition()
{
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		SetNextAnimationState(enAninationState_Walk);
	}
	else
	{
		SetNextAnimationState(enAninationState_Idle);
	}
	
}

void Lich::OnProcessAttack_1StateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//攻撃パターンをなし状態にする
		//m_enAttackPatternState = enAttackPattern_None;
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Lich::OnProcessAttack_2StateTransition()
{
	//ダークウォールの生成
	if (m_CreateDarkWallFlag == true)
	{
		CreateDarkWall();
	}

	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//攻撃パターンをなし状態にする
		//m_enAttackPatternState = enAttackPattern_None;
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Lich::OnProcessDieStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//
		//m_game->SetClearCameraState(Game::enClearCameraState_Player);
		DeleteGO(this);
	}
}

void Lich::OnProcessDamageStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Lich::OnProcessDarkMeteorite_StartStateTransition()
{
	//アニメーション中に勝敗が決まったら
	if (IsStopProcessing() == true)
	{
		DeleteGO(m_darkMeteorite);
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
		return;
	}

	//サイズが最大まで大きくなったら
	//自分の変数にしてもいいかも
	if (m_createDarkMeteoriteFlag == true)
	{
		if (m_darkMeteorite->GetSizeUpFlag() == true)
		{
			SetNextAnimationState(enAnimationState_Attack_DarkMeteorite_main);
			return;
		}
	}
	else
	{
		if (m_modelRender.IsPlayingAnimation() == false)
		{
			//一度だけダークメテオを生成
			CreateDarkMeteorite(true);
			m_createDarkMeteoriteFlag = true;
		}
	}
	
}

void Lich::OnProcessDarkMeteorite_MainStateTransition()
{
	//アニメーション中に勝敗が決まったら
	if (IsStopProcessing() == true)
	{
		DeleteGO(m_darkMeteorite);
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
		return;
	}

	//メテオを全て生成したら
	if (m_darkMeteorite->GetShotEndFlag() == true)
	{
		//DeleteGO(m_darkMeteorite);
		//エンドに移る
		SetNextAnimationState(enAnimationState_Attack_DarkMeteorite_end);
	}
}

void Lich::OnProcessDarkMeteorite_EndStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_createDarkMeteoriteFlag = false;
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
		//無敵時間ではない
		SetInvincibleFlag(false);
	}
}

void Lich::OnProcessSummonStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//無敵時間ではない
		SetInvincibleFlag(false);
		//ヒットカウントをリセット
		m_hitCount = 0;
		//蓄積ダメージをリセット
		m_accumulationDamage = 0;
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Lich::OnProcessVictoryStateTransition()
{
	//アニメーシがループするので特に処理しない
}

void Lich::OnProcessAngryStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//怒りモードに移行
		SetSpecialActionState(enSpecialActionState_AngryMode);
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
		//無敵時間ではない
		SetInvincibleFlag(false);
	}
}

void Lich::OnProcessWarpStateTransition()
{
	switch (m_enWarpStep)
	{
	case Lich::enWarpStep_Up:
		OnProcessenWarpStepUp();
		break;
	case Lich::enWarpStep_Warp:
		OnProcessenWarpStepWarp();
		break;
	case Lich::enWarpStep_Down:
		OnProcessenWarpSteDown();
		break;
	case Lich::enWarpStep_End:
		OnProcessenWarpStepEnd();
		break;

	default:
		break;
	}
}

void Lich::OnProcessenWarpStepUp()
{
	DecideWarpPosition();
	Vector3 Up = WARP_UP;
	//ワープする前に上に上昇する
	m_position = m_charaCon.Execute(Up, 1.0f / 30.0f);

	if (m_position.y >= 2500.0f)
	{
		//次のステップに進む
		m_enWarpStep = enWarpStep_Warp;
	}
}

void Lich::OnProcessenWarpStepWarp()
{
	MoveWarpPosition();
	//次のステップに進む
	m_enWarpStep = enWarpStep_Down;
}

void Lich::OnProcessenWarpSteDown()
{
	//
	if (m_charaCon.IsOnGround()==true)
	{
		m_position.y = 0.0f;
		m_charaCon.SetPosition(m_position);
		Vector3 zero = g_vec3Zero;
		m_charaCon.Execute(zero, 1.0f / 60.0f);
		//次のステップに進む
		m_enWarpStep = enWarpStep_End;
	}
	else
	{
		Vector3 Down = WARP_DOWN;
		//ワープ先で下がる
		m_position = m_charaCon.Execute(Down, 1.0f / 30.0f);
	}
}

void Lich::OnProcessenWarpStepEnd()
{
	//共通の状態遷移処理に移行
	ProcessCommonStateTransition();
	//無敵時間ではない
	SetInvincibleFlag(false);
	//ステップ終わり
	m_enWarpStep = enWarpStep_Up;
}


void Lich::CreateDarkWall()
{
	DarkWall* darkball = NewGO<DarkWall>(0, "darkwall");
	darkball->SetLich(this);
}

void Lich::CreateDarkBall(bool AddBallFlag)
{
	FireBall* fireball = NewGO<FireBall>(0, "darkball");
	fireball->SetLich(this);
	fireball->SetLichAtk(m_status.atk);
	fireball->Setting(m_position, m_rotation);
	
	if (AddBallFlag != true)
	{
		return;
	}

	//あと二つ生成する
	AddCreateDarkBall(ADD_CREATE_DARK_BALL_1_Y);
	AddCreateDarkBall(ADD_CREATE_DARK_BALL_2_Y);
}

void Lich::AddCreateDarkBall(float degY)
{
	Quaternion right = m_rotation;
	right.AddRotationDegY(degY);

	FireBall* fireball1 = NewGO<FireBall>(0, "darkball");
	fireball1->SetLich(this);
	fireball1->SetLichAtk(m_status.atk);
	fireball1->Setting(m_position, right);
}

void Lich::CreateDarkMeteorite(bool lastMeteoFlag)
{
	//大きなメテオを作成
	m_darkMeteorite = NewGO<DarkMeteorite>(0, "darkmeteorite");
	Vector3 pos = m_position;
	pos.y += 410.0f;
	m_darkMeteorite->SetPosition(pos);
	m_darkMeteorite->SetRotation(m_rotation);
	m_darkMeteorite->SetmLastBigMeteoShotFlag(lastMeteoFlag);
}

void Lich::DeleteDarkMeteo()
{
	//ダークメテオが生成されていたら削除する
	m_darkMeteorite = FindGO<DarkMeteorite>("darkmeteorite");
	if (m_darkMeteorite != nullptr)
	{
		DeleteGO(m_darkMeteorite);
	}
}

bool Lich::IsCollisionDetection()
{
	//無敵時間の間は処理をしない
	if (GetInvincibleFlag() == true)
	{
		return true;
	}
	////被ダメージ時、デス時は処理をしない
	//if (isAnimationEntable() != true)
	//{
	//	return;
	//}
	////攻撃中は処理をしない
	//if (IsAttackEntable() != true)
	//{
	//	return;
	//}

	return false;
}

void Lich::HitNormalAttack()
{
	//１コンボの間に1回だけ判定
			//ダメージを受けた時のコンボステートと現在のコンボステートが違うなら
	if (m_player->IsComboStateSame() == true)
	{
		Damage(m_player->GetAtk());
		CreateDamageFont(m_player->GetAtk());
		//ダメージを受けた時のコンボステートに現在のコンボステートを代入する
		m_player->SetDamagedComboState(m_player->GetNowComboState());
	}
}

void Lich::HitHeroSkillAttack()
{
	//一定間隔でダメージを受ける
	if (m_damageFlag == false)
	{
		m_damageFlag = true;
		Damage(m_player->GetSkillAtk());
		CreateDamageFont(m_player->GetSkillAtk());
	}
}

void Lich::HitFlamePillar(bool damageFlag)
{
	if (damageFlag == false)
	{
		//ダメージを受ける
		Damage(m_damage);
		CreateDamageFont(m_damage);
	}
}

void Lich::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//ボール生成タイミング
	if (wcscmp(eventName, L"Create_Ball") == 0)
	{
		//ボール生成
		CreateDarkBall(m_halfHpFlag);
	}

	//ダークウォール生成タイミング
	if (wcscmp(eventName, L"Create_DarkWall") == 0)
	{
		m_CreateDarkWallFlag = true;
	}
	//ダークウォール生成終わり
	if (wcscmp(eventName, L"CreateEnd_DarkWall") == 0)
	{
		m_CreateDarkWallFlag = false;
	}

	//メテオ生成タイミング
	if (wcscmp(eventName, L"createMeteo") == 0)
	{
		m_darkMeteorite->SetShotStartFlag(true);
		SetTargetPosition();
		m_darkMeteorite->SetTargetPosition(m_targetPosition);
	}

	if (wcscmp(eventName, L"Summon") == 0)
	{
		//モンスターを召喚する
		Summon* summon = NewGO<Summon>(0, "summon");
		summon->SetLich(this);
		//最初の召喚だけ
		if (m_firstSummonFlag == true)
		{
			summon->SetFirstSummonFlag(m_firstSummonFlag);
			m_firstSummonFlag = false;
		}
	}
}

void Lich::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}


