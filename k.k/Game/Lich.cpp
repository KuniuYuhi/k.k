#include "stdafx.h"

#include "Lich.h"
#include "LichStateIdle.h"
#include "LichStateWalk.h"
#include "LichStateAttack_1.h"
#include "LichStateAttack_2.h"
#include "LichStateDie.h"
#include "Game.h"
#include "DarkWall.h"
#include "DarkBall.h"
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

#include "CharactersInfoManager.h"
#include "GameManager.h"

#include "InitEffect.h"



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

	const float HULF_HP_ATK_INTERVAL = 3;						//HPが半分になった時の攻撃間隔時間

	const char* DARL_BALL_LIGHT = "darkball_light";
	const char* DARL_BALL_LEFT = "darkball_left";

	const float ROT_SPEED = 8.0f;
	const float ROT_ONLY_SPEED = 5.0f;

	//ステータス
	int MAXHP = 200;
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
	//
	if (m_lichAction != nullptr)
	{
		delete m_lichAction;
	}
	//勝敗が決まったら場合でないなら処理しない。終わる時にエフェクトが再生されるためエラーがでるから
	if (GameManager::GetInstance()->GetOutComeState()==GameManager::enOutComeState_None)
	{
		return;
	}

	//死んだときにモブモンスターを消しているので消す必要なし
	if (m_dieFlag == true)
	{
		return;
	}

	//モブモンスターが0体でないならリスト内のモブモンスターを死亡
	int mobMonsterNum = CharactersInfoManager::GetInstance()->GetMobMonsters().size();
	if (mobMonsterNum != 0)
	{
		for (auto mob : CharactersInfoManager::GetInstance()->GetMobMonsters())
		{
			mob->ProcessDead(false);
			mob->Dead();
			//リストから削除
			//CharactersInfoManager::GetInstance()->RemoveMobMonsterFormList(mob);
		}
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
	m_status.InitCharacterStatus(
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
	
	//todo　newする必要ない
	m_lichAction = new LichAction(this);
	//優先度設定する
	//m_lichAction->SettingPriority();
	
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
		L"Assets/shader/ToonTextrue/lamp_Lich.DDS",
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
	m_darkWallBoonId = m_modelRender.FindBoneID(L"Staff");
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
	//DamageInterval(m_damageIntervalTime);

	//怒りモードなら怒りモードタイマーの計算
	CalcAngryTime();
	
	Move();
	Rotation(ROT_SPEED, ROT_ONLY_SPEED);

	DecideNextAction();

	ManageState();
	PlayAnimation();

	SetTransFormModel(m_modelRender);
	m_modelRender.Update();
}

bool Lich::IsStopProcessing()
{
	//ゲームステート以外なら
	if (GameManager::GetInstance()->GetGameSeenState() !=
		GameManager::enGameSeenState_Game)
	{
		return true;
	}

	//勝敗が決まったら
	if (m_enOutCome != enOutCome_None)
	{
		return true;
	}

	switch (GameManager::GetInstance()->GetOutComeState())
	{
		//負けた
	case GameManager::enOutComeState_PlayerWin:
		//勝敗ステートの設定
		SetEnOutCome(enOutCome_Lose);
		return true;
		break;

		//勝った
	case GameManager::enOutComeState_PlayerLose:
		//勝敗ステートの設定
		SetEnOutCome(enOutCome_Win);
		//ダークメテオの削除
		DeleteDarkMeteo();
		//勝利アニメーション再生
		SetNextAnimationState(enAnimationState_Victory);
		return true;
		break;
	default:
		break;
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
	if (isAnimationEnable() != true)
	{
		return;
	}
	//怒りモードでないなら処理しない
	if (m_enSpecialActionState != enSpecialActionState_AngryMode)
	{
		//移動しないようにする
		m_moveSpeed = Vector3::Zero;
		return;
	}
	//攻撃中なら移動しない
	if (IsAttackEnable() != true)
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
}

void Lich::Damage(int attack)
{
	//ヒットカウントを増やす、蓄積ダメージを増やす
	SetHitCountAndDamage(1, attack);
	//怒りモードカウントを増やす
	m_angryModeCount++;

	if (m_status.GetHp() > 0)
	{
		//一定確率で怯む。怒りモードの時はひるまない
		if (m_enAnimationState != enAnimationState_Angry && Isflinch() == true)
		{
			//技の途中かもしれない
			if (m_darkWall != nullptr)
			{
				DeleteGO(m_darkWall);
			}
			//クリティカルヒット音再生
			g_soundManager->InitAndPlaySoundSource(enSoundName_CriticalHit);
			//被ダメージアニメーションステートに遷移
			SetNextAnimationState(enAnimationState_Damage);
		}

		//HPを減らす
		m_status.CalcHp(attack, false);
		//HPが半分になったら
		if (m_status.GetHp() <= m_status.GetMaxHp() / 2)
		{
			m_halfHpFlag = true;
			//攻撃間隔を短くする
			m_attackIntervalTime = HULF_HP_ATK_INTERVAL;
		}
	}
	//やられたとき
	if(m_status.GetHp() <= 0)
	{
		//やられるところをゆっくりにする
		//フレームレートを落とす
		g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Variable, 30);
		m_modelRender.SetAnimationSpeed(0.4f);
		//ゲームマネージャーのプレイヤーの勝ちフラグを設定
		GameManager::GetInstance()->SetPlayerWinFlag(true);
		m_status.SetHp(0);
		//技の途中でやられたかもしれない
		if (m_darkWall != nullptr)
		{
			DeleteGO(m_darkWall);
		}

		//やられステート
		SetNextAnimationState(enAnimationState_Die);
		m_modelRender.SetAnimationSpeed(0.8f);
	}
}

bool Lich::Isflinch()
{
	//怒りモードの時はひるまない
	if (m_enSpecialActionState == enSpecialActionState_AngryMode)
	{
		//怯まない
		return false;
	}

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
	//自身からターゲットに向かうベクトルを求める
	Vector3 diff = m_targetPosition - m_position;
	//プレイヤーとの距離
	if (diff.Length() < m_distanceToPlayer)
	{
		//距離内にいる
		return true;
	}
	//距離内にいない
	return false;
}

bool Lich::CalcAngryTime()
{
	//怒りモードでないときは処理しない
	if (m_enSpecialActionState != enSpecialActionState_AngryMode)
	{
		return false;
	}

	if (IsAttackEnable() != true)
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
		//ワープ音再生
		g_soundManager->InitAndPlaySoundSource(enSoundName_Boss_Warp, g_soundManager->GetSEVolume());
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
	if (isRotationEnable() != true)
	{
		return true;
	}
	return false;
}

void Lich::DecideNextAction()
{
	//被ダメージ時は処理をしない
	if (isAnimationEnable() != true)
	{
		return;
	}

	//攻撃中は処理をしない
	if (IsAttackEnable() != true)
	{
		return;
	}

	//攻撃可能なら
	if (m_attackEnableFlag == false)
	{
		SetTargetPosition();
		m_toPlayerDir =  m_targetPosition - m_position;
		//プレイヤーが視野角内にいるなら
		if (IsInFieldOfView(m_toPlayerDir, m_forward, 10.0f))
		{
			//次の行動を選ぶ
			m_lichAction->NextAction();

			m_attackEnableFlag = true;
		}
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
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Lich::OnProcessAttack_2StateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		if (m_darkWall != nullptr)
		{
			DeleteGO(m_darkWall);
		}
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Lich::OnProcessDieStateTransition()
{
	if (m_isdeadEffectPlayFlag==true && m_deadEffect->IsPlay() != true)
	{
		//全ての処理が終わりもう削除されてもよい
		GameManager::GetInstance()->SetBossDeleteOkFlag(true);
		return;
	}

	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		if (m_deadEffect == nullptr)
		{
			//死亡エフェクト再生
			//死亡時エフェクトの再生
			m_deadEffect = NewGO<EffectEmitter>(0);
			m_deadEffect->Init(InitEffect::enEffect_Mob_Dead);
			m_deadEffect->Play();
			m_deadEffect->SetPosition(m_position);
			m_deadEffect->SetScale(g_vec3One * 8.0f);
			m_deadEffect->Update();
			//フレームレートを落とす
			g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Variable, 60);
		}
		m_isdeadEffectPlayFlag = true;
		//やられたのでモデルを表示しないようにする
		m_dieFlag = true;

		//モブモンスターを削除
		DeleteMobMonster();
		//DeleteGO(this);
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
		m_darkMeteorite->SetGameEndFlag(true);
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
			CreateDarkMeteorite(m_halfHpFlag);
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
		DeleteGO(m_darkMeteorite);
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
	//アニメーションがループするので特に処理しない
}

void Lich::OnProcessAngryStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		g_soundManager->StopSound(enSoundName_Boss_Angry);
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
	//上のほうに行ったら
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
	//地上についたら
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
	m_darkWall = NewGO<DarkWall>(0, "darkwall");
	m_darkWall->SetLich(this);
}

void Lich::CreateDarkBall(bool AddBallFlag)
{
	DarkBall* darkBall = NewGO<DarkBall>(0, "darkball");
	darkBall->SetAtk(m_status.GetAtk());
	darkBall->Setting(m_position, m_rotation);
	//更にダークボールを生成しないなら
	if (AddBallFlag != true)
	{
		return;
	}
	Quaternion right = m_rotation;
	right.AddRotationDegY(ADD_CREATE_DARK_BALL_1_Y);
	DarkBall* darkBall2 = NewGO<DarkBall>(0, "darkball");
	darkBall2->SetAtk(m_status.GetAtk());
	darkBall2->Setting(m_position, right);

	right = m_rotation;
	right.AddRotationDegY(ADD_CREATE_DARK_BALL_2_Y);
	DarkBall* darkBall3 = NewGO<DarkBall>(0, "darkball");
	darkBall3->SetAtk(m_status.GetAtk());
	darkBall3->Setting(m_position, right);

	//あと二つ生成する
	//AddCreateDarkBall(m_darkBall_left, DARL_BALL_LEFT,ADD_CREATE_DARK_BALL_1_Y);
	//AddCreateDarkBall(m_darkBall_light, DARL_BALL_LIGHT,ADD_CREATE_DARK_BALL_2_Y);
}

void Lich::AddCreateDarkBall(DarkBall* darkBall, const char* name, float degY)
{
	Quaternion right = m_rotation;
	right.AddRotationDegY(degY);
	darkBall = NewGO<DarkBall>(0, name);
	darkBall->SetAtk(m_status.GetAtk());
	darkBall->Setting(m_position, right);
}

void Lich::CreateDarkMeteorite(bool lastMeteoFlag)
{
	//大きなメテオを作成
	m_darkMeteorite = NewGO<DarkMeteorite>(0, "darkmeteorite");
	m_darkMeteorite->SetPosition(m_position);
	m_darkMeteorite->SetRotation(m_rotation);
	m_darkMeteorite->SetmLastBigMeteoShotFlag(lastMeteoFlag);
}

void Lich::DeleteDarkMeteo()
{
	//ダークメテオが生成されていたら削除する
	m_darkMeteorite = FindGO<DarkMeteorite>("darkmeteorite");
	if (m_darkMeteorite != nullptr)
	{
		m_darkMeteorite->SetGameEndFlag(true);
		DeleteGO(m_darkMeteorite);
	}
}

void Lich::CreateSummon()
{
	//モンスターを召喚する
	m_summon = NewGO<Summon>(0, "summon");
	m_summon->SetLich(this);
	//召喚前の詠唱音再生
	//g_soundManager->InitAndPlaySoundSource(enSoundName_Boss_Summon_charge, g_soundManager->GetSEVolume());
	//最初の召喚だけ
	if (m_firstSummonFlag == true)
	{
		m_summon->SetFirstSummonFlag(m_firstSummonFlag);
		m_firstSummonFlag = false;
	}
}

bool Lich::IsCollisionDetection()
{
	//無敵時間の間は処理をしない
	if (GetInvincibleFlag() == true)
	{
		return true;
	}
	
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
		//攻撃が自身にヒットしたので、プレイヤーのattackHitFlagをtrueにする
		m_player->SetAttackHitFlag(true);
	}
}

void Lich::HitSkillAttack()
{
	//スキル攻撃を受けられないなら
	if (m_player->GetHittableFlag() != true)
	{
		return;
	}

	m_damageFlag = true;
	Damage(m_player->GetAtk());
	CreateDamageFont(m_player->GetAtk());
	//多段ヒットしたのでフラグをリセット。多段ヒットでなくとも
	m_player->SetHittableFlag(false);
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

		CreateDarkWall();
	}
	//ダークウォール生成終わり
	if (wcscmp(eventName, L"CreateEnd_DarkWall") == 0)
	{
		if (m_darkWall != nullptr)
		{
			DeleteGO(m_darkWall);
		}
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
		//モンスターの召喚
		//詠唱の音を消して召喚の音を再生する
		//g_soundManager->StopSound(enSoundName_Boss_Summon_charge);
		g_soundManager->InitAndPlaySoundSource(enSoundName_Boss_Summon_start, g_soundManager->GetSEVolume());
		//召喚始め
		m_summon->SetSummonStartFlag(true);
	}

	if (wcscmp(eventName, L"AngrySound") == 0)
	{
		g_soundManager->InitAndPlaySoundSource(enSoundName_Boss_Angry, g_soundManager->GetSEVolume());
	}
}

void Lich::Render(RenderContext& rc)
{
	if (m_dieFlag == true)
	{
		return;
	}
	m_modelRender.Draw(rc);
}

void Lich::DeleteMobMonster()
{
	//モブモンスターが0体でないならリスト内のモブモンスターを死亡
	int mobMonsterNum = CharactersInfoManager::GetInstance()->GetMobMonsters().size();
	if (mobMonsterNum != 0)
	{
		for (auto mob : CharactersInfoManager::GetInstance()->GetMobMonsters())
		{
			mob->ProcessDead(false);
			mob->Dead();
			//リストから削除
			CharactersInfoManager::GetInstance()->RemoveMobMonsterFormList(mob);
		}
	}
}
