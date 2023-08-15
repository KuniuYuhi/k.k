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
#include "LichStateDarkMeteorite_End.h"
#include "DarkMeteorite.h"
#include "LichStateSummon.h"
#include "LichStateVictory.h"

#include "LichAction.h"
#include "Summon.h"

//#include "DamageFont.h"

//todo ターゲットがしばらく近くにいたら逃げる

namespace {
	const float SCALE_UP = 3.0f;									//キャラクターのサイズ
	const Vector3 FIRST_POSITION = Vector3(0.0f, 0.0f, -250.0f);	//最初の座標
	const float DISTANCE = 4000.0f;									//プレイヤーを発見できる距離
	const float NON_WARP_DISTANCE = 400.0f;							//ワープ不要な距離


	//ステータス
	int MAXHP = 1000;
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
	//生き残っているモンスターを死亡させる
	for (auto monster : m_monsters)
	{
		monster->Dead();
	}

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
	m_animationClip[enAnimClip_Attack_DarkMeteorite_main].Load("Assets/animData/character/Lich/DarkMeteorite.tka");
	m_animationClip[enAnimClip_Attack_DarkMeteorite_main].SetLoopFlag(false);
	m_animationClip[enAnimClip_Attack_DarkMeteorite_end].Load("Assets/animData/character/Lich/DarkMeteorite_End.tka");
	m_animationClip[enAnimClip_Attack_DarkMeteorite_end].SetLoopFlag(false);
	m_animationClip[enAnimClip_Summon].Load("Assets/animData/character/Lich/Summon.tka");
	m_animationClip[enAnimClip_Summon].SetLoopFlag(false);
	m_animationClip[enAnimClip_Victory].Load("Assets/animData/character/Lich/Victory.tka");
	m_animationClip[enAnimClip_Victory].SetLoopFlag(true);


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

	/*m_hpFont.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_hpFont.SetScale(1.5f);
	m_hpFont.SetPosition(-800.0f, 500.0f);*/

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
	/*int NowActorMP = m_status.hp;
	int NowActorMaxMP = m_status.maxHp;
	wchar_t MP[255];
	swprintf_s(MP, 255, L"ボス HP %3d/%d", NowActorMP, NowActorMaxMP);
	m_hpFont.SetText(MP);*/

	//被ダメージの当たり判定
	DamageCollision(m_charaCon);

	//攻撃中じゃないなら
	/*if (IsAttackEntable() != true)
	{*/
		//プレイヤーが全滅したら勝利アニメーション設定
		if (m_player->IsAnnihilation() == true && m_enAnimationState!= enAnimationState_Victory)
		{
			SetWinFlag(true);
			//攻撃中じゃないなら
			if (IsAttackEntable() == true)
			{
				SetNextAnimationState(enAnimationState_Victory);
			}
		}
	//}
	

	//倒されたら他の処理を実行しないようにする
	if (m_dieFlag==true|| GetWinFlag() == true)
	{
		ManageState();
		PlayAnimation();
		m_modelRender.Update();
		return;
	}

	//ダークウォールの生成
	/*if (m_CreateDarkWallFlag == true)
	{
		CreateDarkWall();
	}*/

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
	m_moveSpeed = CalcVelocity(m_status, m_targetPosition);

	//被ダメージ時は処理をしない
	if (isAnimationEntable() != true)
	{
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

		//HPが半分になったらワープする
		if (m_status.hp <= m_status.maxHp / 2 && m_halfHpFlag == false)
		{
			//ワープするステートにする
			SetSpecialActionState(enSpecialActionState_Warp);
			m_halfHpFlag = true;
			//ダークメテオを使い終わるまでダメージを受けないようにする
			SetInvincibleFlag(true);
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

	//HPが半分ならすぐに行動
	if (m_enSpecialActionState == enSpecialActionState_Warp)
	{
		//ダークメテオ確定
		m_lichAction->NextAction();
		return;
	}
	
	//攻撃可能なら
	if (m_attackFlag == false)
	{
		//m_lichAction = new LichAction(this);
		//次の行動を選ぶ
		m_lichAction->NextAction();
		//delete m_lichAction;

		m_attackFlag = true;
	}

	//switch (m_enSpecialActionState)
	//{
	//	case Lich::enSpecialActionState_Normal:
	//		//攻撃可能なら
	//		if (m_attackFlag == false)
	//		{
	//			//m_lichAction = new LichAction(this);
	//			//次の行動を選ぶ
	//			m_lichAction->NextAction();
	//			//delete m_lichAction;

	//			m_attackFlag = true;
	//		}
	//		break;

	//	case Lich::enSpecialActionState_Warp:
	//		//ターゲットから一番遠いところに移動する
	//		Warp(enSpecialActionState_Warp);
	//		//
	//		//m_lichAction = new LichAction(this);
	//		m_lichAction->NextAction();
	//		//delete m_lichAction;
	//		//状態を元に戻す
	//		SetSpecialActionState(enSpecialActionState_Normal);
	//		break;

	//	/*case Lich::enSpecialActionState_CenterWarp:

	//		break;*/


	//	default:
	//		break;
	//}
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

void Lich::Warp(EnSpecialActionState SpecialActionState)
{
	if (SpecialActionState == enSpecialActionState_Warp)
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
	}
	else if (SpecialActionState == enSpecialActionState_CenterWarp)
	{
		Vector3 CenterPos = Vector3::Zero;
		Vector3 diff = CenterPos - m_position;

		if (diff.Length() > NON_WARP_DISTANCE)
		{
			m_position = CenterPos;
		}
	}


	//switch (m_enSpecialActionState)
	//{
	//	//一番遠いところにワープする
	//case Lich::enSpecialActionState_Warp:
	//	float MaxLength = 0.0f;
	//	//ターゲットから一番遠いところ座標を調べる
	//	for (int amount = 0; amount < m_WarpPosition.size(); amount++)
	//	{
	//		//
	//		Vector3 diff = m_WarpPosition[amount] - m_targetPosition;
	//		//
	//		if (MaxLength < diff.Length())
	//		{
	//			MaxLength = diff.Length();
	//			m_position = m_WarpPosition[amount];
	//		}
	//	}
	//	break;
	//	//真ん中か真ん中近くにワープする
	//case Lich::enSpecialActionState_CenterWarp:
	//	Vector3 CenterPos = Vector3::Zero;
	//	Vector3 diff = CenterPos - m_position;

	//	if (diff.Length() > NON_WARP_DISTANCE)
	//	{
	//		m_position = CenterPos;
	//	}

	//	break;

	//default:
	//	break;
	//}
	//float MaxLength = 0.0f;
	////ターゲットから一番遠いところ座標を調べる
	//for (int amount = 0; amount < m_WarpPosition.size(); amount++)
	//{
	//	//
	//	Vector3 diff = m_WarpPosition[amount] - m_targetPosition;
	//	//
	//	if (MaxLength < diff.Length())
	//	{
	//		MaxLength = diff.Length();
	//		m_position = m_WarpPosition[amount];
	//	}
	//}

	Vector3 o = Vector3::Zero;
	//ワープする
	m_charaCon.SetPosition(m_position);
	m_charaCon.Execute(o, 1.0f / 60.0f);
}

void Lich::ProcessCommonStateTransition()
{
	//無敵時間ではない
	SetInvincibleFlag(false);
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
	

	//上昇していないなら処理をしない
	if (IsRisingDarkMeteorite() != true)
	{
		return;
	}
	//一度だけダークメテオを生成
	if (m_createDarkMeteoriteFlag == false)
	{
		CreateDarkMeteorite();
		m_createDarkMeteoriteFlag = true;
	}

	//サイズが最大まで大きくなったら
	//自分の変数にしてもいいかも
	if (m_darkMeteorite->GetSizeUpFlag() == true)
	{
		//メインに移る
		SetNextAnimationState(enAnimationState_Attack_DarkMeteorite_main);
		return;
	}
}

void Lich::OnProcessDarkMeteorite_MainStateTransition()
{
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
	//地面に降りる
	m_moveSpeed = Vector3::AxisY;
	m_moveSpeed.y *= -m_status.defaultSpeed;

	m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	
	//地面に着いたら
	if (m_charaCon.IsOnGround() == true)
	{
		m_moveSpeed = Vector3::Zero;
		m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
		//共通の状態遷移処理に移行
		ProcessCommonStateTransition();
	}
}

void Lich::OnProcessSummonStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
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
}

void Lich::CreateDarkWall()
{
	DarkWall* darkball = NewGO<DarkWall>(0, "darkwall");
	darkball->SetLich(this);
}

void Lich::CreateDarkMeteorite()
{
	//玉を生成
	m_darkMeteorite = NewGO<DarkMeteorite>(0, "darkmeteorite");
	Vector3 pos = m_position;
	pos.y += 370.0f;
	m_darkMeteorite->SetPosition(pos);
	m_darkMeteorite->SetRotation(m_rotation);
}

bool Lich::IsRisingDarkMeteorite()
{
	//Y座標が上限に到達したら
	if (m_RisingLimit <= m_position.y)
	{
		return true;
	}
	else
	{
		Vector3 moveSpeed = Vector3::Zero;
		moveSpeed.y += g_gameTime->GetFrameDeltaTime() * 250.0f;
		m_moveSpeed = moveSpeed;

		m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 2.0f);

		return false;
	}

}

void Lich::DamageCollision(CharacterController& characon)
{
	//抜け出す処理
	//死んだら処理をしない todo
	//無敵時間の間は処理をしない
	if (GetInvincibleFlag() == true)
	{
		return;
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
				CreateDamageFont(m_player->GetAtk());
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
				CreateDamageFont(m_player->GetSkillAtk());
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
			CreateDamageFont(fireball->GetAtk());
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
				CreateDamageFont(flamepillar->GetAtk());
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

	//メテオ生成タイミング
	if (wcscmp(eventName, L"createMeteo") == 0)
	{
		m_darkMeteorite->SetShotStartFlag(true);
		SetTargetPosition();
		m_darkMeteorite->SetTargetPosition(m_targetPosition);
	}

	//
	//if (wcscmp(eventName, L"Stop") == 0)
	//{
	//	m_modelRender.
	//}
	//
	if (wcscmp(eventName, L"Summon") == 0)
	{
		//モンスターを召喚する
		Summon* summon = NewGO<Summon>(0, "summon");
		summon->SetLich(this);
	}

}

void Lich::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	//m_hpFont.Draw(rc);
}


