#include "stdafx.h"
#include "BeholderEye.h"

#include "MobEnemyMovement.h"

#include "Brave.h"

#include "EnemyObjectPool.h"

#include "EnemyManager.h"

#include "KnockBackInfoManager.h"

#include "EyeBall.h"



using namespace KnockBackInfo;


namespace {
	const float ATTACK_COLLISION_RADIUS = 18.0f;
}


BeholderEye::~BeholderEye()
{
}

bool BeholderEye::Start()
{
	m_player = FindGO<Brave>("Brave");

	//モデルの読み込み
	InitModel();

	//キャラコン初期化
	if (m_charaCon == nullptr)
	{
		CreateCharacterController();
		m_charaCon.get()->Init(20.0f, 40.0f, m_position);
	}
	else
	{
		m_charaCon.get()->SetPosition(m_position);
	}

	//ステータスを初期化
	m_status.InitCommonEnemyStatus("BeholderEye");
	m_commonStatus.InitMobEnemyCommonStatus("BeholderEye");

	//コンポーネントを初期化していないなら
	if (!m_isSettingComponents)
	{
		InitComponents();
	}

	//エネミーマネージャーのリストに自身を入れる
	EnemyManager::GetInstance()->AddMobEnemyToList(this);

	//コンテキストがなかったら作る
	if (!m_beholderEyeContext)
	{
		//ステートコンテキストの初期化
		m_beholderEyeContext = std::make_unique<BeholderEyeStateContext>();
	}

	//最初のステートは出現ステート
	m_beholderEyeContext.get()->Init(this, enBeholderEyeState_Appear);

	


	return true;
}

void BeholderEye::LoadAnimationClip()
{
	m_animationClip[enBeholderEyeAnimClip_Idle].Load("Assets/animData/character/BeholderEye/Idle.tka");
	m_animationClip[enBeholderEyeAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enBeholderEyeAnimClip_Run].Load("Assets/animData/character/BeholderEye/Run.tka");
	m_animationClip[enBeholderEyeAnimClip_Run].SetLoopFlag(true);
	m_animationClip[enBeholderEyeAnimClip_Attack].Load("Assets/animData/character/BeholderEye/Attack.tka");
	m_animationClip[enBeholderEyeAnimClip_Attack].SetLoopFlag(false);
	m_animationClip[enBeholderEyeAnimClip_Hit].Load("Assets/animData/character/BeholderEye/Hit.tka");
	m_animationClip[enBeholderEyeAnimClip_Hit].SetLoopFlag(false);
	m_animationClip[enBeholderEyeAnimClip_Die].Load("Assets/animData/character/BeholderEye/Die.tka");
	m_animationClip[enBeholderEyeAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enBeholderEyeAnimClip_Victory].Load("Assets/animData/character/BeholderEye/Victory.tka");
	m_animationClip[enBeholderEyeAnimClip_Victory].SetLoopFlag(true);
	m_animationClip[enBeholderEyeAnimClip_Appear].Load("Assets/animData/character/BeholderEye/Appear.tka");
	m_animationClip[enBeholderEyeAnimClip_Appear].SetLoopFlag(false);
}

void BeholderEye::InitComponents()
{
	//基本的なコンポーネントをセッティング
	SettingDefaultComponent();
	//このクラスで追加したいコンポーネントをセッティング
	AddMoreComponent();

	m_isSettingComponents = true;
}

void BeholderEye::ReleaseThis()
{
	//エネミー管理マネージャーのリストから自身を削除
	EnemyManager::GetInstance()->RemoveMobEnemyToList(this);
	//キャラコンリセット
	m_charaCon.reset();
	//オブジェクトプールに自身のオブジェクトを返す
	EnemyObjectPool::GetInstance()->OnRelease("BeholderEye", this);
}

void BeholderEye::AddMoreComponent()
{

}

void BeholderEye::CreateCollisionObject()
{
	m_attackCollision = NewGO<CollisionObject>(0, g_collisionObjectManager->m_enemyAttackCollisionName);
	m_attackCollision->SetCreatorName(GetName());
	m_attackCollision->CreateSphere(
		m_position,
		m_rotation,
		ATTACK_COLLISION_RADIUS
	);
	//ワールド座標取得
	Matrix HeadMatrix = m_modelRender.GetBone(m_headBoonId)->GetWorldMatrix();
	//当たり判定の場所を頭のボーンの位置に変更
	m_attackCollision->SetWorldMatrix(HeadMatrix);
}

void BeholderEye::ProcessHit(DamageInfo damageInfo)
{
	//ノックバックパターンを取得
	//todo レベルによって変更
	m_hitKnockBackPattern = damageInfo.knockBackPattern;

	//ノックバックの時間間隔を取得
	m_knockBackTimeScale = damageInfo.knockBackTimeScale;

	//ダメージを受ける
	TakeDamage(damageInfo.attackPower);

	//ヒットステートに切り替える
	m_beholderEyeContext.get()->ChangeBeholderEyeState(this, enBeholderEyeState_Hit);
}


void BeholderEye::Attack()
{
	if (IsAction()) return;

	//攻撃可能でないなら処理しない
	if (!IsAttackable() || m_isWaitingFlag) return;

	//一定の範囲内にプレイヤーがいたら攻撃できる
	float toPlayerDistance = CalcDistanceToTargetPosition(m_player->GetPosition());

	//自身からプレイヤーに向かう距離の長さが一定の距離でないなら処理しない
	if (toPlayerDistance > m_status.GetApproachDistance()) return;

	//攻撃する！！
	//ステートを切り替える
	m_beholderEyeContext.get()->ChangeBeholderEyeState(this, enBeholderEyeState_Attack);

}

void BeholderEye::EntryAttackActionProcess()
{
	//アクション中にする
	ActionActive();
	//攻撃するのでタイマーをリセット
	m_attackIntarvalTimer = 0.0f;


	//攻撃情報を設定
	m_damageProvider->SetDamageInfo(
		KnockBackInfoManager::GetInstance()->GetAddAttackId(), m_status.GetCurrentAtk(),
		m_commonStatus.GetNormalAttackTimeScale(),
		m_commonStatus.GetNormalAttackKnockBackPattern()
	);

}

void BeholderEye::UpdateAttackActionProcess()
{
	//攻撃用コリジョンを生成できるなら
	if (m_isCreateAttackCollision)
	{
		//コリジョンを生成
		CreateCollisionObject();
	}
}

void BeholderEye::ExitAttackActionProcess()
{
	//アクションを終わる
	ActionDeactive();
	//次の攻撃までのタイマーをリセット
	m_attackIntarvalTimer = 0.0f;

}

void BeholderEye::EntryHitActionProcess()
{
	//m_hitKnockBackPattern = enKBPattern_SlightAirborneRetreat;
	//ノックバックする前の準備
	SettingKnockBackProcess();
	//ノックバックカウントリセット
	count = 0.0f;
	//硬直タイマーをリセット
	m_starkTimer = 0.0f;
	//攻撃中かもしれないのでコリジョン生成フラグをリセットしておく
	m_isCreateAttackCollision = false;
}

void BeholderEye::UpdateHitActionProcess()
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
			//もし死んでいるなら
			if (IsDie())
			{
				//死亡ステートに遷移
				m_beholderEyeContext.get()->ChangeBeholderEyeState(this, enBeholderEyeState_Die);
				return;
			}

			//少し硬直して共通ステート処理に移行
			if (m_starkTimer >= 0.1f)
			{
				//共通ステートに移行
				ProcessCommonTranstion();
			}
			m_starkTimer += g_gameTime->GetFrameDeltaTime();
		}
	}
}

void BeholderEye::ExitHitActionProcess()
{
	if (IsDie()) return;


	//アクションを終わる
	ActionDeactive();
}

void BeholderEye::DieProcess()
{
	//ダメージによってやられた時の処理
	DieFromDamage();
}

void BeholderEye::ProcessCommonTranstion()
{
	if (fabsf(GetMoveSpeed().x) >= 0.001f ||
		fabsf(GetMoveSpeed().z) >= 0.001f)
	{
		m_beholderEyeContext.get()->ChangeBeholderEyeState(this, enBeholderEyeState_Run);
	}
	else
	{
		m_beholderEyeContext.get()->ChangeBeholderEyeState(this, enBeholderEyeState_Idle);
	}

}

void BeholderEye::TurnToPlayer()
{
	TurnToTarget();
}

void BeholderEye::Update()
{
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		//ReleaseThis();
		//return;
	}

	//死んでいないなら処理する
	if (!IsDie())
	{
		//攻撃処理
		Attack();
		//キャラクターの移動
		ChaseMovement(m_player->GetPosition());
		//回転
		Rotation();

		//当たり判定
		CheckSelfCollision();
	}

	//コンテキストの処理
	m_beholderEyeContext.get()->UpdateCurrentState();
	m_beholderEyeContext.get()->PlayAnimationCurrentState();

	//モデルのトランスフォームの設定
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	//モデル情報を更新
	m_modelRender.Update();
}




void BeholderEye::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

void BeholderEye::InitModel()
{
	//モデルを初期化していないなら
	if (!m_modelRender.IsInit())
	{
		//アニメーションの読み込み
		LoadAnimationClip();

		//モデルを初期化
		m_modelRender.Init(
			"Assets/modelData/character/BeholderEye/BeholderEye.tkm",
			L"Assets/shader/ToonTextrue/lamp_Slime.DDS",
			m_animationClip,
			enBeholderEyeAnimClip_num,
			enModelUpAxisZ
		);

		//アニメーションイベント用の関数を設定する。
		m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
			OnAnimationEvent(clipName, eventName);
		});

	}
	//トランスフォームの設定
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);

	//頭のボーンIdを取得
	m_headBoonId = m_modelRender.FindBoneID(L"EyeBallCTRL");


}

void BeholderEye::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//通常攻撃のボール生成タイミング
	if (wcscmp(eventName, L"ShotNormalBall") == 0)
	{
		//ボールを放つ
		ShotNormalBall();
	}
	

}

void BeholderEye::ShotNormalBall()
{
	//ボールの名前を決める
	std::string result = std::string("EyeBall") + std::to_string(m_eyeBallCount);
	const char* finalStr = result.c_str();

	//ボールを生成
	EyeBall* m_eyeBall = NewGO<EyeBall>(0, finalStr);

	//ボールを撃つ座標を計算
	Vector3 createPos = g_vec3Zero;
	Matrix m = m_modelRender.GetBone(m_headBoonId)->GetWorldMatrix();
	m.Apply(createPos);

	//撃つときのパラメータの設定
	m_eyeBall->SetShotMagicBallParameters(
		createPos,
		m_forward
	);

	//ボールを撃ったのでカウント加算
	m_eyeBallCount++;
}

