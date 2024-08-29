#include "stdafx.h"
#include "Slime.h"

#include "MobEnemyMovement.h"

#include "Brave.h"

#include "EnemyObjectPool.h"

#include "EnemyManager.h"

#include "KnockBackInfoManager.h"


using namespace KnockBackInfo;


namespace {
	const float ATTACK_COLLISION_RADIUS = 18.0f;
}

Slime::~Slime()
{
}

bool Slime::Start()
{
	
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
	m_status.InitCommonEnemyStatus("Slime");
	m_commonStatus.InitMobEnemyCommonStatus("Slime");

	//コンポーネントを初期化していないなら
	if (!m_isSettingComponents)
	{
		InitComponents();
	}

	//エネミーマネージャーのリストに自身を入れる
	EnemyManager::GetInstance()->AddMobEnemyToList(this);


	//コンテキストがなかったら作る
	if (!m_slimeContext)
	{
		//ステートコンテキストの初期化
		m_slimeContext = std::make_unique<SlimeStateContext>();
	}

	//最初のステートは出現ステート
	m_slimeContext.get()->Init(this, enSlimeState_Appear);

	

	return true;
}

void Slime::LoadAnimationClip()
{
	m_animationClip[enSlimeAnimClip_Idle].Load("Assets/animData/character/Slime/Idle_Normal.tka");
	m_animationClip[enSlimeAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enSlimeAnimClip_Run].Load("Assets/animData/character/Slime/Run.tka");
	m_animationClip[enSlimeAnimClip_Run].SetLoopFlag(true);
	m_animationClip[enSlimeAnimClip_Attack].Load("Assets/animData/character/Slime/Attack1.tka");
	m_animationClip[enSlimeAnimClip_Attack].SetLoopFlag(false);
	m_animationClip[enSlimeAnimClip_Hit].Load("Assets/animData/character/Slime/Damege.tka");
	m_animationClip[enSlimeAnimClip_Hit].SetLoopFlag(false);
	m_animationClip[enSlimeAnimClip_Die].Load("Assets/animData/character/Slime/Die.tka");
	m_animationClip[enSlimeAnimClip_Die].SetLoopFlag(false);
	m_animationClip[enSlimeAnimClip_Victory].Load("Assets/animData/character/Slime/Victory.tka");
	m_animationClip[enSlimeAnimClip_Victory].SetLoopFlag(true);
	m_animationClip[enSlimeAnimClip_Appear].Load("Assets/animData/character/Slime/Appear.tka");
	m_animationClip[enSlimeAnimClip_Appear].SetLoopFlag(false);
}

void Slime::InitComponents()
{
	//基本的なコンポーネントをセッティング
	SettingDefaultComponent();
	//このクラスで追加したいコンポーネントをセッティング
	AddMoreComponent();

	m_isSettingComponents = true;
}

void Slime::ReleaseThis()
{
	//エネミー管理マネージャーのリストから自身を削除
	EnemyManager::GetInstance()->RemoveMobEnemyToList(this);
	//キャラコンリセット
	m_charaCon.reset();
	//オブジェクトプールに自身のオブジェクトを返す
	EnemyObjectPool::GetInstance()->OnRelease("Slime", this);
}

void Slime::AddMoreComponent()
{

}

void Slime::CreateCollisionObject()
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

void Slime::ProcessHit(DamageInfo damageInfo)
{
	//ノックバックパターンを取得
	//todo レベルによって変更
	m_hitKnockBackPattern = damageInfo.knockBackPattern;

	//ノックバックの時間間隔を取得
	m_knockBackTimeScale = damageInfo.knockBackTimeScale;

	//ヒットステートに切り替える
	m_slimeContext.get()->ChangeSlimeState(this, enSlimeState_Hit);
}


void Slime::Attack()
{
	if (IsAction()) return;

	//攻撃可能でないなら処理しない
	if (!IsAttackable()||m_isWaitingFlag) return;

	//一定の範囲内にプレイヤーがいたら攻撃できる
	float toPlayerDistance = CalcDistanceToTargetPosition(m_player->GetPosition());

	//自身からプレイヤーに向かう距離の長さが一定の距離でないなら処理しない
	if (toPlayerDistance > m_status.GetApproachDistance()) return;

	//攻撃する！！
	//ステートを切り替える
	m_slimeContext.get()->ChangeSlimeState(this, enSlimeState_Attack);

}

void Slime::EntryAttackActionProcess()
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

void Slime::UpdateAttackActionProcess()
{
	//攻撃用コリジョンを生成できるなら
	if (m_isCreateAttackCollision)
	{
		//コリジョンを生成
		CreateCollisionObject();
	}
}

void Slime::ExitAttackActionProcess()
{
	//アクションを終わる
	ActionDeactive();
	//次の攻撃までのタイマーをリセット
	m_attackIntarvalTimer = 0.0f;

}

void Slime::EntryHitActionProcess()
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

void Slime::UpdateHitActionProcess()
{
	//まずカーブデータを元に移動
	if (count < m_curvePointList.size())
	{
		//ノックバックによる移動
		KnockBackMove(count);
		//タイムスケールを加算
		count+= m_knockBackTimeScale;
	}
	//次に空中に浮いていたら地面に降りる
	else if(m_charaCon.get()->IsJump())
	{
		KnockBackGravityFall();

	}
	//最後に少し硬直させて共通ステート処理に移行
	else
	{
		//アニメーションが終わったら
		if (GetModelRender().IsPlayingAnimation() == false)
		{
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

void Slime::ProcessCommonTranstion()
{
	if (fabsf(GetMoveSpeed().x) >= 0.001f ||
		fabsf(GetMoveSpeed().z) >= 0.001f)
	{
		m_slimeContext.get()->ChangeSlimeState(this,enSlimeState_Run);
	}
	else
	{
		m_slimeContext.get()->ChangeSlimeState(this,enSlimeState_Idle);
	}

}

void Slime::TurnToPlayer()
{
	TurnToTarget();
}

void Slime::Update()
{
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		//ReleaseThis();
		//return;
	}



	//攻撃処理
	Attack();
	//キャラクターの移動
	ChaseMovement(m_player->GetPosition());
	//回転
	Rotation();

	//当たり判定
	CheckSelfCollision();

	//コンテキストの処理
	m_slimeContext.get()->UpdateCurrentState();
	m_slimeContext.get()->PlayAnimationCurrentState();

	//モデルのトランスフォームの設定
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	//モデル情報を更新
	m_modelRender.Update();
}




void Slime::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

void Slime::InitModel()
{
	//モデルを初期化していないなら
	if (!m_modelRender.IsInit())
	{
		//アニメーションの読み込み
		LoadAnimationClip();

		//モデルを初期化
		m_modelRender.Init(
			"Assets/modelData/character/Slime/slime.tkm",
			L"Assets/shader/ToonTextrue/lamp_Slime.DDS",
			m_animationClip,
			enSlimeAnimClip_num,
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
	m_headBoonId= m_modelRender.FindBoneID(L"Head");

}

void Slime::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
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
