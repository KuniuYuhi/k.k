#include "stdafx.h"
#include "Summoner.h"
//マネージャー
#include "CharactersInfoManager.h"
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


namespace {
	const float SCALE_UP = 4.0f;		//キャラクターのサイズ

	//ステータス
	int MAXHP = 100;
	int MAXMP = 500;
	int ATK = 20;
	float SPEED = 160.0f;
	const char* NAME = "Summoner";
}

Summoner::Summoner()
{
}

Summoner::~Summoner()
{
	//ステートマシンの削除
	IBossStateMachine::DeleteInstance();
}

bool Summoner::Start()
{
	//ステータスの初期化
	m_status.InitStatus(
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


	return true;
}

void Summoner::Update()
{

	
	//回転処理
	ProcessRotation();
	//移動処理。怒りモードの時のみ
	ProcessMove();

	//ステートマシンの毎フレームの処理
	IBossStateMachine::GetInstance()->Execute();

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

void Summoner::ProcessMove()
{

}

void Summoner::ProcessRotation()
{
	m_targetPosition = 
		CharactersInfoManager::GetInstance()->GetBraveInstance()->GetPosition();

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
		m_rotMove = Math::Lerp(g_gameTime->GetFrameDeltaTime() * 5.0f, m_rotMove, m_moveSpeed);
		m_rotation.SetRotationYFromDirectionXZ(m_rotMove);
	}
	//前方向を設定
	m_forward = m_rotMove;
	m_forward.Normalize();
	m_moveSpeed= g_vec3Zero;
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
	case Summoner::enAnimationState_DarkBall://ダークボール
		m_nowBossState = new SummonerState_DarkBall(this);
		break;
	case Summoner::enAnimationState_DarkWall://ダークウォール
		m_nowBossState = new SummonerState_DarkWall(this);
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
		m_nowBossState = new SumonerState_NAttack_3(this);
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
		//SetNextAnimationState(enAninationState_Walk);
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
	m_animationClip[enAnimClip_DarkBall].Load(
		"Assets/animData/character/Lich/Attack1.tka");
	m_animationClip[enAnimClip_DarkBall].SetLoopFlag(false);
	m_animationClip[enAnimClip_DarkWall].Load(
		"Assets/animData/character/Lich/Attack2.tka");
	m_animationClip[enAnimClip_DarkWall].SetLoopFlag(false);
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
	m_animationClip[enAnimClip_Command].Load(
		"Assets/animData/character/Lich/Command.tka");
	m_animationClip[enAnimClip_Command].SetLoopFlag(false);



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
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});


	m_modelRender.PlayAnimation(enAnimClip_Idle, 0.2f);
}


void Summoner::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
}


void Summoner::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}