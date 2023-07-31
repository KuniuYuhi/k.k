#include "stdafx.h"
#include "Lich.h"
#include "LichStateIdle.h"
#include "LichStateWalk.h"
#include "LichStateAttack_1.h"
#include "LichStateAttack_2.h"
#include "LichStateDie.h"
#include "Game.h"
#include "FireBall.h"
#include "FlamePillar.h"
#include "DarkWall.h"
#include "LichStateDamage.h"
#include "LichStateDarkMeteorite_Start.h"
#include "LichStateDarkMeteorite_Main.h"

#include "LichAction.h"



namespace {
	const float SCALE_UP = 3.0f;									//キャラクターのサイズ
	const Vector3 FIRST_POSITION = Vector3(0.0f, 0.0f, -250.0f);	//最初の座標
	const float DISTANCE = 1500.0f;									//プレイヤーを発見できる距離


	//ステータス
	int MAXHP = 200;
	int MAXMP = 500;
	int ATK = 20;
	float SPEED = 80.0f;
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
}

Lich::~Lich()
{
	delete m_lichAction;
}

bool Lich::Start()
{
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
	//
	SetSpecialActionState(enSpecialActionState_Normal);
	
	m_lichAction = new LichAction(this);

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
	m_animationClip[enAnimClip_Attack_DarkMeteorite_main].Load("Assets/animData/character/Lich/DarkMeteorite_Main.tka");
	m_animationClip[enAnimClip_Attack_DarkMeteorite_main].SetLoopFlag(false);


	m_modelRender.Init("Assets/modelData/character/Lich/Lich.tkm",
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

	m_hpFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_hpFont.SetScale(2.0f);
	m_hpFont.SetPosition(-800.0f, 500.0f);

	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//ダークウォールに使うボーンID取得
	m_darkWallBoonId = m_modelRender.FindBoneID(L"Index_Proximal_L");

}

void Lich::Update()
{
	//MPの表示
	int NowActorMP = m_status.hp;
	int NowActorMaxMP = m_status.maxHp;
	wchar_t MP[255];
	swprintf_s(MP, 255, L"HP %3d/%d", NowActorMP, NowActorMaxMP);
	m_hpFont.SetText(MP);

	if (m_CreateDarkWallFlag == true)
	{
		CreateDarkWall();
	}

	//被ダメージの当たり判定
	DamageCollision(m_charaCon);

	//倒されたら他の処理を実行しないようにする
	if (m_dieFlag==true)
	{
		ManageState();
		PlayAnimation();
		m_modelRender.Update();
		return;
	}

	//インターバルの計算
	AttackInterval(m_attackIntervalTime);
	DamageInterval(m_damageIntervalTime);

	Move();
	Rotation();

	DecideNextAction();

	ManageState();
	PlayAnimation();

	

	SetTransFormModel(m_modelRender);
	m_modelRender.Update();

	m_oldMoveSpeed = m_moveSpeed;
}

void Lich::Move()
{
	//プレイヤーの座標を取得
	SetTargetPosition();

	//移動処理
	m_moveSpeed = calcVelocity(m_status);

	//被ダメージ時は処理をしない
	if (isAnimationEntable() != true)
	{
		return;
	}

	//一定の距離になったらそれ以上動かない
	if (IsDistanceToPlayer() == true)
	{
		//移動しないようにする
		m_moveSpeed = Vector3::Zero;
	}
	//攻撃中なら移動しない
	else if (IsAttackEntable() != true)
	{
		//移動しないようにする
		m_moveSpeed = Vector3::Zero;
	}
	//プレイヤーを見つけたら
	else if (IsFindPlayer(m_distance) == true)
	{
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
	if (m_status.hp > 0)
	{
		//一定確率で怯む
		if (Isflinch() == true)
		{
			//技の途中かもしれない
			m_CreateDarkWallFlag = false;
			SetNextAnimationState(enAnimationState_Damage);
		}

		
		m_status.hp -= attack;

		//HPが半分になったらワープする
		if (m_status.hp <= m_status.maxHp / 2 && m_halfHpFlag == false)
		{
			SetSpecialActionState(enSpecialActionState_Warp);
			m_halfHpFlag = true;
		}
	}
	
	if(m_status.hp <= 0)
	{
		//Dieフラグをtrueにする
		m_dieFlag = true;
		m_status.hp = 0;
		//技の途中でやられたかもしれない
		m_CreateDarkWallFlag = false;
		SetNextAnimationState(enAnimationState_Die);
	}

}

bool Lich::Isflinch()
{
	//乱数を初期化。
	srand((unsigned)time(NULL));

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
	
	switch (m_enSpecialActionState)
	{
		case Lich::enSpecialActionState_Normal:
			//攻撃可能なら
			if (m_attackFlag == false)
			{
				//m_lichAction = new LichAction(this);
				//次の行動を選ぶ
				m_lichAction->NextAction();
				//delete m_lichAction;

				m_attackFlag = true;
			}
			break;

		case Lich::enSpecialActionState_Warp:
			//ターゲットから一番遠いところに移動する
			Warp();
			//
			//m_lichAction = new LichAction(this);
			m_lichAction->NextAction();
			//delete m_lichAction;
			//状態を元に戻す
			SetSpecialActionState(enSpecialActionState_Normal);
			break;

		case Lich::SpecialActionState:
			break;


		default:
			break;
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
				m_WarpPosition.emplace_back(warpPos);
				return true;
			}
			return false;
		});
}

void Lich::Warp()
{
	float MaxLength = 0.0f;
	//ターゲットから一番遠いところ座標を調べる
	for (int amount = 0; amount < m_WarpPosition.size(); amount++)
	{
		//
		Vector3 diff = m_WarpPosition[amount] - m_targetPosition;
		//
		if (MaxLength < diff.Length())
		{
			MaxLength = diff.Length();
			m_position = m_WarpPosition[amount];
		}
	}
	Vector3 o = Vector3::Zero;
	//ワープする
	m_charaCon.SetPosition(m_position);
	m_charaCon.Execute(o, 1.0f / 60.0f);
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
		//自身が倒されたらことをゲームに伝える
		Game* game = FindGO<Game>("game");
		game->SetDeathBossFlag(true);

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
	//空中に移動する
	if (IsRisingDarkMeteorite() == true)
	{
		//玉を大きくする



		//メインに移る
		SetNextAnimationState(enAnimationState_Attack_DarkMeteorite_main);
	}
}

void Lich::OnProcessDarkMeteorite_MainStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{

		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Lich::CreateDarkWall()
{
	//ボーン取得
	DarkWall* darkball = NewGO<DarkWall>(0, "darkwall");
	darkball->SetLich(this);
}

bool Lich::IsRisingDarkMeteorite()
{
	Vector3 moveSpeed = Vector3::Zero;
	moveSpeed.y += g_gameTime->GetFrameDeltaTime() * 250.0f;
	m_moveSpeed = moveSpeed;

	m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 10.0f);

	//Y座標が上限に到達したら
	if (m_RisingLimit <= m_position.y)
	{
		return true;
	}
	else
	{
		return false;
	}

}

void Lich::DamageCollision(CharacterController& characon)
{
	//抜け出す処理
	//死んだら処理をしないtodo

	//通常攻撃の当たり判定
	const auto& Attack_1Collisions = g_collisionObjectManager->FindCollisionObjects("Attack");
	//コリジョンの配列をfor文で回す
	for (auto collision : Attack_1Collisions)
	{
		//自身のキャラコンと衝突したら
		if (collision->IsHit(characon) == true)
		{
			//１コンボの間に1回だけ判定
			//ダメージを受けた時のコンボステートと現在のコンボステートが違うなら
			if (m_player->IsComboStateSame()==true)
			{
				Damage(m_player->GetAtk());

				//ダメージを受けた時のコンボステートに現在のコンボステートを代入する
				m_player->SetDamagedComboState(m_player->GetNowComboState());
			}
		}
	}
	
	//ヒーローのスキルの当たり判定
	const auto& SkillCollisions = g_collisionObjectManager->FindCollisionObjects("SkillAttack");
	//コリジョンの配列をfor文で回す
	for (auto collision : SkillCollisions)
	{
		//自身のキャラコンと衝突したら
		if (collision->IsHit(characon) == true)
		{
			//一定間隔でダメージを受ける
			if (m_damageFlag == false)
			{
				m_damageFlag = true;
				Damage(m_player->GetSkillAtk());
			}
		}
	}

	//ウィザードのファイヤーボールの当たり判定
	const auto& FireBallCollisions = g_collisionObjectManager->FindCollisionObjects("fireball");
	//コリジョンの配列をfor文で回す
	for (auto collision : FireBallCollisions)
	{
		//自身のキャラコンと衝突したら
		if (collision->IsHit(characon) == true)
		{
			auto fireball = FindGO<FireBall>(collision->GetName());
			Damage(fireball->GetAtk());
			//ぶつかったのでファイヤーボールを消すフラグを立てる
			fireball->SetHitEnemeyFlag(true);
		}
	}

	//ウィザードのフレイムピラーの当たり判定
	const auto& FlamePillarCollisions = g_collisionObjectManager->FindCollisionObjects("flamepillar");
	//コリジョンの配列をfor文で回す
	for (auto collision : FlamePillarCollisions)
	{
		//自身のキャラコンと衝突したら
		if (collision->IsHit(characon) == true)
		{
			//一定間隔でダメージを受ける
			if (m_damageFlag == false)
			{
				auto flamepillar = FindGO<FlamePillar>(collision->GetName());

				m_damageFlag = true;
				Damage(flamepillar->GetAtk());
			}
		}
	}

}

void Lich::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//ボール生成タイミング
	if (wcscmp(eventName, L"Create_Ball") == 0)
	{
		//ボール生成
		FireBall* fireball = NewGO<FireBall>(0, "darkball");
		fireball->SetLich(this);
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
}

void Lich::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	m_hpFont.Draw(rc);
}


