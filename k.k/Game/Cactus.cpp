#include "stdafx.h"
#include "Cactus.h"

#include "EnemyObjectPool.h"

#include "MobEnemyMovement.h"

#include "Brave.h"

#include "EnemyManager.h"

#include "KnockBackInfoManager.h"

#include "UseEffect.h"


using namespace KnockBackInfo;


namespace {
	const float ATTACK_COLLISION_RADIUS = 18.0f;
}

Cactus::~Cactus()
{
}

bool Cactus::Start()
{
	//やられてオブジェクトプールに格納された時のことも考えて死亡フラグをリセット
	SetDieFlag(false);

	m_player = FindGO<Brave>("Brave");


	//モデルの読み込み
	InitModel();
	
	//キャラコンを初期化していないなら
	if (m_charaCon == nullptr)
	{
		CreateCharacterController();
		m_charaCon.get()->Init(24.0f, 10.0f, m_position);
	}
	else
	{
		m_charaCon.get()->SetPosition(m_position);
	}

	//ステータスを初期化
	m_status.InitCommonEnemyStatus("Cactus");
	m_commonStatus.InitMobEnemyCommonStatus("Cactus");

	//コンポーネントを初期化していないなら
	if (!m_isSettingComponents)
	{
		InitComponents();
	}

	//エネミーマネージャーのリストに自身を入れる
	EnemyManager::GetInstance()->AddMobEnemyToList(this);
	
	//コンテキストがなかったら作る
	if (!m_cactusContext)
	{
		//ステートコンテキストの初期化
		m_cactusContext = std::make_unique<CactusStateContext>();
	}

	//最初のステートは出現ステート
	m_cactusContext.get()->Init(this, enCactusState_Appear);

	return true;
}

void Cactus::LoadAnimationClip()
{
	m_animationClip[enCactusAnimClip_Idle].Load("Assets/animData/character/Cactus/Idle.tka");
	m_animationClip[enCactusAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enCactusAnimClip_Run].Load("Assets/animData/character/Cactus/Run.tka");
	m_animationClip[enCactusAnimClip_Run].SetLoopFlag(true);
	m_animationClip[enCactusAnimClip_Attack].Load("Assets/animData/character/Cactus/Attack1.tka");
	m_animationClip[enCactusAnimClip_Attack].SetLoopFlag(false);
	m_animationClip[enCactusAnimClip_Hit].Load("Assets/animData/character/Cactus/Damage.tka");
	m_animationClip[enCactusAnimClip_Hit].SetLoopFlag(false);
	m_animationClip[enCactusAnimClip_Die].Load("Assets/animData/character/Cactus/Die.tka");
	m_animationClip[enCactusAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enCactusAnimClip_Victory].Load("Assets/animData/character/Cactus/Victory.tka");
	m_animationClip[enCactusAnimClip_Victory].SetLoopFlag(true);
	m_animationClip[enCactusAnimClip_Appear].Load("Assets/animData/character/Cactus/Appear.tka");
	m_animationClip[enCactusAnimClip_Appear].SetLoopFlag(false);
}

void Cactus::InitComponents()
{
	//基本的なコンポーネントをセッティング
	SettingDefaultComponent();
	//このクラスで追加したいコンポーネントをセッティング
	AddMoreComponent();

	m_isSettingComponents = true;
}

void Cactus::ReleaseThis()
{
	//エネミー管理マネージャーのリストから自身を削除
	EnemyManager::GetInstance()->RemoveMobEnemyToList(this);
	//キャラコンリセット
	m_charaCon.reset();
	//オブジェクトプールに自身のオブジェクトを返す
	EnemyObjectPool::GetInstance()->OnRelease("Cactus", this);
}

void Cactus::AddMoreComponent()
{
}

void Cactus::CreateCollisionObject()
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

void Cactus::ProcessHit(DamageInfo damageInfo)
{
	//ノックバックパターンを取得
	//todo レベルによって変更
	m_hitKnockBackPattern = damageInfo.knockBackPattern;

	//ノックバックの時間間隔を取得
	m_knockBackTimeScale = damageInfo.knockBackTimeScale;

	//ダメージを受ける
	TakeDamage(damageInfo.attackPower);

	//ヒットステートに切り替える
	m_cactusContext.get()->ChangeCactusState(this, enCactusState_Hit);
}

void Cactus::Attack()
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
	m_cactusContext.get()->ChangeCactusState(this, enCactusState_Attack);
}

void Cactus::ProcessCommonTranstion()
{
	if (fabsf(GetMoveSpeed().x) >= 0.001f ||
		fabsf(GetMoveSpeed().z) >= 0.001f)
	{
		m_cactusContext.get()->ChangeCactusState(this, enCactusState_Run);
	}
	else
	{
		m_cactusContext.get()->ChangeCactusState(this, enCactusState_Idle);
	}
}

void Cactus::TurnToPlayer()
{
	TurnToTarget();
}

void Cactus::EntryAttackActionProcess()
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

void Cactus::UpdateAttackActionProcess()
{
	//攻撃用コリジョンを生成できるなら
	if (m_isCreateAttackCollision)
	{
		//コリジョンを生成
		CreateCollisionObject();
	}
}

void Cactus::ExitAttackActionProcess()
{
	//アクションを終わる
	ActionDeactive();
	//次の攻撃までのタイマーをリセット
	m_attackIntarvalTimer = 0.0f;
}

void Cactus::EntryHitActionProcess()
{
	//ノックバックする前の準備
	SettingKnockBackProcess();
	//ノックバックカウントリセット
	count = 0.0f;
	//硬直タイマーをリセット
	m_starkTimer = 0.0f;
	//攻撃中かもしれないのでコリジョン生成フラグをリセットしておく
	m_isCreateAttackCollision = false;

	PlayHitSound();
}

void Cactus::UpdateHitActionProcess()
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
				m_cactusContext.get()->ChangeCactusState(this, enCactusState_Die);
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

void Cactus::ExitHitActionProcess()
{
	if (IsDie()) return;


	//アクションを終わる
	ActionDeactive();
}

void Cactus::DieProcess()
{
	//ダメージによってやられた時の処理
	DieFromDamage();
}

void Cactus::WinProcess()
{
	m_cactusContext.get()->ChangeCactusState(this, enCactusState_Victory);
}

void Cactus::DieFlomOutside(bool isPlayEffect)
{
	//キャラコンリセット
	m_charaCon.reset();
	//オブジェクトプールに自身のオブジェクトを返す
	EnemyObjectPool::GetInstance()->OnRelease("Cactus", this);

	//エフェクトを再生しないなら
	if (!isPlayEffect) return;

	//死亡エフェクト生成
	UseEffect* effect = NewGO<UseEffect>(0, "DieEffect");
	effect->PlayEffect(enEffect_Mob_Dead,
		m_position, g_vec3One * 5.0f, Quaternion::Identity, false);

}

void Cactus::Update()
{
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		/*ReleaseThis();
		return;*/
	}

	//処理を止める要求があるなら
	if (IsStopRequested())return;

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
	m_cactusContext.get()->UpdateCurrentState();
	m_cactusContext.get()->PlayAnimationCurrentState();

	//モデルのトランスフォームの設定
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	//モデル情報を更新
	m_modelRender.Update();
}

void Cactus::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

void Cactus::InitModel()
{
	//モデルを初期化していないなら
	if (!m_modelRender.IsInit())
	{
		//アニメーションの読み込み
		LoadAnimationClip();

		m_modelRender.Init(
			"Assets/modelData/character/Cactus/Cactus.tkm",
			L"Assets/shader/ToonTextrue/lamp_Cactus.DDS",
			m_animationClip,
			enCactusAnimClip_num,
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
	m_headBoonId = m_modelRender.FindBoneID(L"cactus_leftHandTip");
	
}

void Cactus::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//当たり判定生成タイミング
	if (wcscmp(eventName, L"Collision_Start") == 0)
	{
		m_isCreateAttackCollision = true;
	}
	//当たり判定生成終わり
	if (wcscmp(eventName, L"Collision_End") == 0)
	{
		m_isCreateAttackCollision = false;
	}
}


