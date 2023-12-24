#include "stdafx.h"
#include "Summoner.h"
//マネージャー
#include "CharactersInfoManager.h"
#include "GameManager.h"
//ステートマシン
#include "IBossStateMachine.h"
//各種ステート
#include "SummonerState_Idle.h"
#include "SummonerState_DarkBall.h"
#include "SummonerState_DarkWall.h"
#include "SummonerState_KnockBack.h"
#include "SummonerState_NAttack_1.h"
#include "SummonerState_NAttack_2.h"
#include "SummonerState_NAttack_3.h"
#include "SummonerState_DMeteo_Start.h"
#include "SummonerState_DMeteo_Main.h"
#include "SummonerState_Angry.h"
#include "SummonerState_Command.h"
#include "SummonerState_Hit.h"
#include "SummonerState_Die.h"
#include "SummonerState_Summon.h"
#include "SummonerState_Walk.h"
#include "SummonerState_Warp.h"
#include "SummonerState_Victory.h"
#include "SummonerState_DarkSpearMain.h"
#include "SummonerState_DarkSpearEnd.h"
#include "SummonerState_DarkSpearStart.h"

//todo スーパーアーマーがある間はのけぞらない
//壊れたらたまにのけぞる
//回復していく

namespace {
	const float SCALE_UP = 4.0f;		//キャラクターのサイズ

	const float MAX_SUPERARMOR_POINT = 10.0f;

	//ステータス
	int MAXHP = 200;
	int MAXMP = 500;
	int ATK = 20;
	float SPEED = 160.0f;
	const char* NAME = "Summoner";
}

Summoner::Summoner()
{
	m_maxSuperArmorPoint = MAX_SUPERARMOR_POINT;
	m_superArmorPoint = m_maxSuperArmorPoint;
}

Summoner::~Summoner()
{
	//ステートマシンの削除
	IBossStateMachine::DeleteInstance();

	//勝敗が決まったら場合でないなら処理しない。終わる時にエフェクトが再生されるためエラーがでるから
	if (GameManager::GetInstance()->GetOutComeState() == GameManager::enOutComeState_None)
	{
		return;
	}

	//死んだときにモブモンスターを消しているので消す必要なし
	if (GameManager::GetInstance()->GetBossDeleteOkFlag() == true)
	{
		return;
	}

	//モブモンスターを削除
	DeleteMobMonsters();
}

bool Summoner::Start()
{
	//ステータスの初期化
	m_status.InitCharacterStatus(
		MAXHP,
		MAXMP,
		ATK,
		SPEED,
		NAME
	);

	//モデルの初期化
	InitModel();
	//ステートマシンの生成
	IBossStateMachine::CreateInstance(this);

	//使いやすくするために
	//m_stateMachine = IBossStateMachine::GetInstance();


	//最初のアニメーション設定
	SetNextAnimationState(enAninationState_Idle);

	//プレイヤーのインスタンスの代入
	m_player = CharactersInfoManager::GetInstance()->GetPlayerInstance();

	return true;
}

void Summoner::Update()
{
	if (IsStopProcessing() != true)
	{
		//回転処理
		ProcessRotation();
		//移動処理。怒りモードの時のみ
		ProcessMove();

		//ステートマシンの毎フレームの処理
		IBossStateMachine::GetInstance()->Execute();

		//当たり判定の処理
		DamageCollision(m_charaCon);
	}
	
	//スーパーアーマーの回復
	RecoverySuperArmor();

	//状態管理
	ManageState();
	//アニメーション
	PlayAnimation();
	//モデルのTRSと設定と更新
	SetTransFormModel(m_modelRender);
	m_modelRender.Update();
}

void Summoner::PlayAnimation()
{
	m_nowBossState->PlayAnimation();
}

void Summoner::ManageState()
{
	m_nowBossState->ManageState();
}

bool Summoner::IsStopProcessing()
{
	//ゲームステート以外なら
	if (GameManager::GetInstance()->GetGameSeenState() !=
		GameManager::enGameSeenState_Game)
	{
		return true;
	}
	//勝敗決定したら
	switch (GameManager::GetInstance()->GetOutComeState())
	{
		//負けた
	case GameManager::enOutComeState_PlayerWin:
		return true;
		break;

		//勝った
	case GameManager::enOutComeState_PlayerLose:
		//勝利アニメーション再生
		SetNextAnimationState(enAnimationState_Victory);
		return true;
		break;
	default:
		break;
	}

	return false;
}

void Summoner::Damage(int attack)
{
	//ダメージを受ける処理
	ProcessHit(attack);
	

	//やられたとき
	if (m_status.GetHp() <= 0)
	{
		//やられた時の処理
		ProcessDead();
	}

}

void Summoner::HitNormalAttack()
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

void Summoner::HitSkillAttack()
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

void Summoner::ProcessDead(bool seFlag)
{
	//やられるところをゆっくりにする
	//フレームレートを落とす
	g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Variable, 30);
	//ゲームマネージャーのプレイヤーの勝ちフラグを設定
	GameManager::GetInstance()->SetPlayerWinFlag(true);
	m_status.SetHp(0);
	//技の途中でやられたかもしれない

	//効果音の再生
	if (seFlag == true)
	{

	}

	//やられステート
	SetNextAnimationState(enAnimationState_Die);
	m_modelRender.SetAnimationSpeed(0.7f);
}

void Summoner::ProcessMove()
{

}

void Summoner::ProcessRotation()
{
	m_targetPosition = m_player->GetPosition();

	m_moveSpeed= CalcVelocity(m_status, m_targetPosition);

	//回転可能でないなら
	if (isRotationEnable() == false)
	{
		//回転しない
		return;
	}

	//xかzの移動速度があったら
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//緩やかに回転させる
		m_rotMove = Math::Lerp(
			g_gameTime->GetFrameDeltaTime() * 5.0f, m_rotMove, m_moveSpeed);
		m_rotation.SetRotationYFromDirectionXZ(m_rotMove);
		//todo
	}
	//前方向を設定
	m_forward = m_rotMove;
	m_forward.Normalize();
	m_moveSpeed= g_vec3Zero;
}

void Summoner::ProcessHit(int hitDamage)
{
	//HPを減らす
	m_status.CalcHp(hitDamage, false);
	//被ダメージカウント加算
	m_damageCount++;
	//スーパーアーマーを減らす
	CalcSuperArmor(false, hitDamage);

	//スーパーアーマーがブレイクしているなら
	if (GetBreakSuperArmorFlag() == true)
	{
		//一定確率で怯む
		if (IsFlinch() == true)
		{
			SetNextAnimationState(enAnimationState_CriticalHit);
		}
	}
}

void Summoner::RecoverySuperArmor()
{
	//スーパーアーマーがブレイクしていないなら処理しない
	if (GetBreakSuperArmorFlag() != true)
	{
		return;
	}
	//スーパーアーマーの回復
	CalcSuperArmor(true, g_gameTime->GetFrameDeltaTime());
}

void Summoner::SetNextAnimationState(EnAnimationState nextState)
{
	//古いステートを削除する
	if (m_nowBossState != nullptr)
	{
		delete m_nowBossState;
	}
	//新しいステートを作成
	m_enAnimationState = nextState;

	switch (nextState)
	{
	case Summoner::enAninationState_Idle://待機状態
		m_nowBossState = new SummonerState_Idle(this);
		break;
	case Summoner::enAninationState_Walk://歩き状態
		m_nowBossState = new SummonerState_Idle(this);
		break;
	case Summoner::enAnimationState_DarkBall://ダークボール
		m_nowBossState = new SummonerState_DarkBall(this);
		break;
	case Summoner::enAnimationState_DarkWall://ダークウォール
		m_nowBossState = new SummonerState_DarkWall(this);
		break;
	case Summoner::enAnimationState_DarkSpear_Start://ダークスピアメイン
		m_nowBossState = new SummonerState_DarkSpearStart(this);
		break;
	case Summoner::enAnimationState_DarkSpear_Main://ダークスピアメイン
		m_nowBossState = new SummonerState_DarkSpearMain(this);
		break;
	case Summoner::enAnimationState_DarkSpear_End://ダークスピアエンド
		m_nowBossState = new SummonerState_DarkSpearEnd(this);
		break;
	case Summoner::enAnimationState_KnockBack://ノックバック
		m_nowBossState = new SummonerState_KnockBack(this);
		break;
	case Summoner::enAnimationState_NormalAttack_1://通常攻撃１
		m_nowBossState = new SummonerState_NAttack_1(this);
		break;
	case Summoner::enAnimationState_NormalAttack_2://通常攻撃２
		m_nowBossState = new SummonerState_NAttack_2(this);
		break;
	case Summoner::enAnimationState_NormalAttack_3://通常攻撃３
		m_nowBossState = new SummonerState_NAttack_3(this);
		break;
	case Summoner::enAnimationState_Attack_DarkMeteorite_start://ダークメテオスタート
		m_nowBossState = new SummonerState_DMeteo_Start(this);//ダークメテオメイン
		break;
	case Summoner::enAnimationState_Attack_DarkMeteorite_main:
		m_nowBossState = new SummonerState_DMeteo_Main(this);
		break;
	case Summoner::enAnimationState_Angry://怒りモード
		m_nowBossState = new SummonerState_Angry(this);
		break;
	case Summoner::enAninationState_Summon://召喚
		m_nowBossState = new SummonerState_Summon(this);
		break;
	case Summoner::enAnimationState_Command://モブに号令
		m_nowBossState = new SummonerState_Command(this);
		break;
	case Summoner::enAnimationState_CriticalHit://スーパーアーマー割られた時
		m_nowBossState = new SummonerState_Hit(this);
		break;
	case Summoner::enAnimationState_Die://やられた
		m_nowBossState = new SummonerState_Die(this);
		break;
	case Summoner::enAnimationState_Victory://勝利
		m_nowBossState = new SummonerState_Victory(this);
		break;
	default:
		// ここに来たらステートのインスタンス作成処理の追加忘れ。
		std::abort();
		break;
	}
	
}

void Summoner::ProcessCommonStateTransition()
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

void Summoner::InitModel()
{
	//各種アニメーションクリップのロード
	m_animationClip[enAnimClip_Idle].Load(
		"Assets/animData/character/Lich/Idle.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Walk].Load(
		"Assets/animData/character/Lich/Walk.tka");
	m_animationClip[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClip[enAnimClip_DarkBall].Load(
		"Assets/animData/character/Lich/Attack1.tka");
	m_animationClip[enAnimClip_DarkBall].SetLoopFlag(false);
	m_animationClip[enAnimClip_DarkWall].Load(
		"Assets/animData/character/Lich/Attack2.tka");
	m_animationClip[enAnimClip_DarkWall].SetLoopFlag(false);
	m_animationClip[enAnimClip_DarkSpear_Start].Load(
		"Assets/animData/character/Lich/DarkSpear_Start.tka");
	m_animationClip[enAnimClip_DarkSpear_Start].SetLoopFlag(false);
	m_animationClip[enAnimClip_DarkSpear_Main].Load(
		"Assets/animData/character/Lich/DarkSpear_Main.tka");
	m_animationClip[enAnimClip_DarkSpear_Main].SetLoopFlag(false);
	m_animationClip[enAnimClip_DarkSpear_End].Load(
		"Assets/animData/character/Lich/DarkSpear_End.tka");
	m_animationClip[enAnimClip_DarkSpear_End].SetLoopFlag(false);
	m_animationClip[enAnimClip_KnockBack].Load(
		"Assets/animData/character/Lich/KnockBack.tka");
	m_animationClip[enAnimClip_KnockBack].SetLoopFlag(false);
	m_animationClip[enAnimClip_NormalAttack_1].Load(
		"Assets/animData/character/Lich/NormalAttack_1.tka");
	m_animationClip[enAnimClip_NormalAttack_1].SetLoopFlag(false);
	m_animationClip[enAnimClip_NormalAttack_2].Load(
		"Assets/animData/character/Lich/NormalAttack_2.tka");
	m_animationClip[enAnimClip_NormalAttack_2].SetLoopFlag(false);
	m_animationClip[enAnimClip_NormalAttack_3].Load(
		"Assets/animData/character/Lich/NormalAttack_3.tka");
	m_animationClip[enAnimClip_NormalAttack_3].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_DarkMeteorite_start].Load(
		"Assets/animData/character/Lich/DarkMeteorite_Start.tka");
	m_animationClip[enAnimClip_Attack_DarkMeteorite_start].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_DarkMeteorite_main].Load(
		"Assets/animData/character/Lich/DarkMeteorite_Main.tka");
	m_animationClip[enAnimClip_Attack_DarkMeteorite_main].SetLoopFlag(false);
	m_animationClip[enAnimClip_Command].Load(
		"Assets/animData/character/Lich/Command.tka");
	m_animationClip[enAnimClip_Command].SetLoopFlag(false);
	m_animationClip[enAnimClip_Summon].Load(
		"Assets/animData/character/Lich/Summon.tka");
	m_animationClip[enAnimClip_Summon].SetLoopFlag(false);
	m_animationClip[enAnimClip_Victory].Load(
		"Assets/animData/character/Lich/Victory.tka");
	m_animationClip[enAnimClip_Victory].SetLoopFlag(true);
	m_animationClip[enAnimClip_Angry].Load(
		"Assets/animData/character/Lich/Angry.tka");
	m_animationClip[enAnimClip_Angry].SetLoopFlag(false);
	m_animationClip[enAnimClip_Die].Load(
		"Assets/animData/character/Lich/Die.tka");
	m_animationClip[enAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enAnimClip_CriticalHit].Load(
		"Assets/animData/character/Lich/Damage.tka");
	m_animationClip[enAnimClip_CriticalHit].SetLoopFlag(false);
	
	m_modelRender.Init("Assets/modelData/character/Lich/Lich_real_Selllook.tkm",
		L"Assets/shader/ToonTextrue/lamp_Lich.DDS",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);

	//モデルのTRSの設定
	m_modelRender.SetTransform(
		m_position,
		m_rotation,
		m_scale *= SCALE_UP
	);
	m_modelRender.Update();

	//キャラコンの設定
	m_charaCon.Init(
		70.0f,
		50.0f,
		m_position
	);
	//アニメーションイベント用の関数を設定する。
	//m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
	//	OnAnimationEvent(clipName, eventName);
	//	});

}


void Summoner::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
}

void Summoner::DeleteMobMonsters()
{
	//モブモンスターが0体でないならリスト内のモブモンスターを死亡
	int mobMonsterNum = CharactersInfoManager::GetInstance()->GetMobMonsters().size();
	if (mobMonsterNum == 0)
		return;

	for (auto monster : CharactersInfoManager::GetInstance()->GetMobMonsters())
	{
		monster->ProcessDead(false);
		DeleteGO(monster);
	}
	//リストをクリア
	CharactersInfoManager::GetInstance()->GetMobMonsters().clear();
}

void Summoner::Render(RenderContext& rc)
{
	if (GetIsDrawModelFlag() != true)
	{
		return;
	}

	m_modelRender.Draw(rc);

}