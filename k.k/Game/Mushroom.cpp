#include "stdafx.h"
#include "Mushroom.h"
#include "MobEnemyMovement.h"

#include "Brave.h"

#include "EnemyObjectPool.h"

#include "EnemyManager.h"

#include "KnockBackInfoManager.h"

#include "UseEffect.h"


using namespace KnockBackInfo;


namespace {
	const float ATTACK_COLLISION_RADIUS = 18.0f;
}

Mushroom::~Mushroom()
{
}

bool Mushroom::Start()
{
	//やられてオブジェクトプールに格納された時のことも考えて死亡フラグをリセット
	SetDieFlag(false);

	m_player = FindGO<Brave>("Brave");

	//モデルの読み込み
	InitModel();

	//キャラコン初期化
	if (m_charaCon == nullptr)
	{
		CreateCharacterController();
		m_charaCon.get()->Init(22.0f, 10.0f, m_position);
	}
	else
	{
		m_charaCon.get()->SetPosition(m_position);
	}

	//ステータスを初期化
	m_status.InitCommonEnemyStatus("Mushroom");
	m_commonStatus.InitMobEnemyCommonStatus("Mushroom");

	//コンポーネントを初期化していないなら
	if (!m_isSettingComponents)
	{
		InitComponents();
	}

	//エネミーマネージャーのリストに自身を入れる
	EnemyManager::GetInstance()->AddMobEnemyToList(this);


	//コンテキストがなかったら作る
	if (!m_mushroomContext)
	{
		//ステートコンテキストの初期化
		m_mushroomContext = std::make_unique<MushroomStateContext>();
	}

	//最初のステートは出現ステート
	m_mushroomContext.get()->Init(this, enMushroomState_Appear);



	return true;
}

void Mushroom::LoadAnimationClip()
{
	m_animationClip[enMushroomAnimClip_Idle].Load("Assets/animData/character/Mushroom/Idle_Normal.tka");
	m_animationClip[enMushroomAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enMushroomAnimClip_Run].Load("Assets/animData/character/Mushroom/Run.tka");
	m_animationClip[enMushroomAnimClip_Run].SetLoopFlag(true);
	m_animationClip[enMushroomAnimClip_Attack].Load("Assets/animData/character/Mushroom/Attack1.tka");
	m_animationClip[enMushroomAnimClip_Attack].SetLoopFlag(false);
	m_animationClip[enMushroomAnimClip_Hit].Load("Assets/animData/character/Mushroom/Damage.tka");
	m_animationClip[enMushroomAnimClip_Hit].SetLoopFlag(false);
	m_animationClip[enMushroomAnimClip_Die].Load("Assets/animData/character/Mushroom/Die.tka");
	m_animationClip[enMushroomAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enMushroomAnimClip_Victory].Load("Assets/animData/character/Mushroom/Victory.tka");
	m_animationClip[enMushroomAnimClip_Victory].SetLoopFlag(true);
	m_animationClip[enMushroomAnimClip_Appear].Load("Assets/animData/character/Mushroom/Appear.tka");
	m_animationClip[enMushroomAnimClip_Appear].SetLoopFlag(false);
}

void Mushroom::InitComponents()
{
	//基本的なコンポーネントをセッティング
	SettingDefaultComponent();
	//このクラスで追加したいコンポーネントをセッティング
	AddMoreComponent();

	m_isSettingComponents = true;
}

void Mushroom::ReleaseThis()
{
	//エネミー管理マネージャーのリストから自身を削除
	EnemyManager::GetInstance()->RemoveMobEnemyToList(this);
	//キャラコンリセット
	m_charaCon.reset();
	//オブジェクトプールに自身のオブジェクトを返す
	EnemyObjectPool::GetInstance()->OnRelease("Mushroom", this);
}

void Mushroom::AddMoreComponent()
{

}

void Mushroom::CreateCollisionObject()
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

void Mushroom::ProcessHit(DamageInfo damageInfo)
{
	//ノックバックパターンを取得
	//todo レベルによって変更
	m_hitKnockBackPattern = damageInfo.knockBackPattern;

	//ノックバックの時間間隔を取得
	m_knockBackTimeScale = damageInfo.knockBackTimeScale;

	//ダメージを受ける
	TakeDamage(damageInfo.attackPower);

	//ヒットステートに切り替える
	m_mushroomContext.get()->ChangeMushroomState(this, enMushroomState_Hit);
}


void Mushroom::Attack()
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
	m_mushroomContext.get()->ChangeMushroomState(this, enMushroomState_Attack);

}

void Mushroom::EntryAttackActionProcess()
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

void Mushroom::UpdateAttackActionProcess()
{
	//攻撃用コリジョンを生成できるなら
	if (m_isCreateAttackCollision)
	{
		//コリジョンを生成
		CreateCollisionObject();
	}
}

void Mushroom::ExitAttackActionProcess()
{
	//アクションを終わる
	ActionDeactive();
	//次の攻撃までのタイマーをリセット
	m_attackIntarvalTimer = 0.0f;

}

void Mushroom::EntryHitActionProcess()
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

void Mushroom::UpdateHitActionProcess()
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
				m_mushroomContext.get()->ChangeMushroomState(this, enMushroomState_Die);
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

void Mushroom::ExitHitActionProcess()
{
	if (IsDie()) return;


	//アクションを終わる
	ActionDeactive();
}

void Mushroom::DieProcess()
{
	//アイテムを落とすか決める
	if (IsDropBuffItem())
	{
		DropBuffItem();
	}

	//ダメージによってやられた時の処理
	DieFromDamage();
}

void Mushroom::WinProcess()
{
	m_mushroomContext.get()->ChangeMushroomState(this, enMushroomState_Victory);
}

void Mushroom::ForceChangeStateIdle()
{
	m_mushroomContext.get()->ChangeMushroomState(this, enMushroomState_Idle);
	m_moveSpeed = g_vec3Zero;
}

void Mushroom::DieFlomOutside(bool isPlayEffect)
{
	//キャラコンリセット
	m_charaCon.reset();
	//オブジェクトプールに自身のオブジェクトを返す
	EnemyObjectPool::GetInstance()->OnRelease("Mushroom", this);

	//エフェクトを再生しないなら
	if (!isPlayEffect) return;

	//死亡エフェクト生成
	UseEffect* effect = NewGO<UseEffect>(0, "DieEffect");
	effect->PlayEffect(enEffect_Mob_Dead,
		m_position, g_vec3One * 5.0f, Quaternion::Identity, false);

}

void Mushroom::ProcessCommonTranstion()
{
	if (fabsf(GetMoveSpeed().x) >= 0.001f ||
		fabsf(GetMoveSpeed().z) >= 0.001f)
	{
		m_mushroomContext.get()->ChangeMushroomState(this, enMushroomState_Run);
	}
	else
	{
		m_mushroomContext.get()->ChangeMushroomState(this, enMushroomState_Idle);
	}

}

void Mushroom::TurnToPlayer()
{
	TurnToTarget();
}

void Mushroom::Update()
{

	//処理を止める要求がない限り処理をする
	if (!IsStopRequested())
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
	m_mushroomContext.get()->UpdateCurrentState();
	m_mushroomContext.get()->PlayAnimationCurrentState();

	//モデルのトランスフォームの設定
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	//モデル情報を更新
	m_modelRender.Update();
}




void Mushroom::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

void Mushroom::InitModel()
{
	//モデルを初期化していないなら
	if (!m_modelRender.IsInit())
	{
		//アニメーションの読み込み
		LoadAnimationClip();

		//モデルを初期化
		m_modelRender.Init(
			"Assets/modelData/character/Mushroom/Mushroom.tkm",
			L"Assets/shader/ToonTextrue/lamp_Mushroom.DDS",
			m_animationClip,
			enMushroomAnimClip_num,
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
	m_headBoonId = m_modelRender.FindBoneID(L"mushroom_spine03");

}

void Mushroom::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
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
