#include "stdafx.h"
#include "DarkMeteo.h"
#include "InitEffect.h"
#include "Meteo.h"
#include "CharactersInfoManager.h"
#include "GameManager.h"

#include "DecisionMeteoEndpoint.h"

namespace {
	const float DARK_METEO_MAX_SIZE = 20.0f;

	const Vector3 DARK_METEO_POSITION = { 0.0f,550.0f,0.0f };

	const int CREATE_METEO_COUNT = 5;	//メテオを生成する数


	const float METEO_START_UP_POS = 20.0f;
	const float METEO_END_UP_POS = -20.0f;

	const float SHOT_METEO_TIMER_LIMMIT = 0.7f;

}

//衝突したときに呼ばれる関数オブジェクト(壁用)
struct IsWallResult :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//衝突フラグ。
	virtual	btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//地面とぶつかってなかったら。
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall) {
			//衝突したのは壁ではない。
			isHit = false;
			return 0.0f;
		}
		else
		{
			//地面とぶつかったら。
		//フラグをtrueに。
			isHit = true;
			return 0.0f;
		}

	}
};

DarkMeteo::DarkMeteo()
{
}

DarkMeteo::~DarkMeteo()
{
	if (m_darkMeteoriteEffect != nullptr)
	{
		//エフェクトを止める
		m_darkMeteoriteEffect->Stop();
	}

	//技を撃ち終わってからの削除なのでメテオは削除しない
	if (m_endFlag == true)
	{
		return;
	}

	//メテオを削除
	if (m_meteoList.size() != 0)
	{
		QueryGOs<Meteo>("meteo", [&](Meteo* meteo_) {
			DeleteGO(meteo_);
			return true;
			});
	}

}

bool DarkMeteo::Start()
{
	//ダークメテオの座標に高さを足す
	m_darkMeteoPositon += DARK_METEO_POSITION;

	//ダークメテオの初期化
	InitDarkMeteoEffect();

	return true;
}

void DarkMeteo::Update()
{
	//撃っている間にゲームが終わったら
	if (GameManager::GetInstance()->GetOutComeState() != 
		GameManager::enOutComeState_None)
	{
		DeleteGO(this);
		return;
	}


	ManageState();
}

void DarkMeteo::ManageState()
{
	switch (m_enDarkMeteoState)
	{
	case DarkMeteo::enDarkMeteoState_Charge:
		OnProcessDarkMeteoChargeTranstion();
		break;
	case DarkMeteo::enDarkMeteoState_ShotMeteo:
		OnProcessShotMeteoTranstion();
		break;
	case DarkMeteo::enDarkMeteoState_ShotDarkMeteo:
		OnProcessShotDarkMeteoTranstion();
		break;
	case DarkMeteo::enDarkMeteoState_End:
		OnProcessEndTranstion();
		break;
	default:
		break;
	}
}

void DarkMeteo::OnProcessDarkMeteoChargeTranstion()
{
	//最大までダークメテオが大きくなったら
	if (m_mulScaleDarkMeteo > DARK_METEO_MAX_SIZE)
	{
		//チャージ完了フラグをセット
		m_chargeCompleteFlag = true;
	}
	else
	{
		m_mulScaleDarkMeteo += g_gameTime->GetFrameDeltaTime() * 7.5f;
		//スケールアップ
		m_darkMeteoScale = g_vec3One;
		m_darkMeteoScale *= m_mulScaleDarkMeteo;

	}
	//拡大率を設定
	m_darkMeteoriteEffect->SetScale(m_darkMeteoScale);
	m_darkMeteoriteEffect->Update();

}

void DarkMeteo::OnProcessShotMeteoTranstion()
{
	//一定間隔で撃つ

	if (IsShotMeteo() != true)
	{
		return;
	}
	//メテオの最大生成数に達していないなら生成処理
	if (m_shotMeteoCounter < CREATE_METEO_COUNT)
	{
		while (true)
		{
			//座標の設定
			Vector3 meteoTargetPos = GetMeteoTargetPosition(
				CharactersInfoManager::GetInstance()->GetPlayerInstance()->GetPosition());
			//壁に落ちないならメテオを生成する
			if (IsMeteoHitWall(meteoTargetPos) == false)
			{
				//メテオの生成とカウントを加算
				CreateMeteo(meteoTargetPos);
				//カウントを加算
				m_shotMeteoCounter++;
				break;
			}
			else
			{
				//やり直し
				continue;
			}
		}
		return;
	}

	//ダークメテオを撃つなら、ダークメテオを撃つ処理のステートに遷移
	if (m_shotDarkMeteoFlag == true)
	{
		SetNextDarkMeteoState(enDarkMeteoState_ShotDarkMeteo);
	}
	//撃たないなら終わる処理のステートに遷移
	else
	{
		SetNextDarkMeteoState(enDarkMeteoState_End);
	}
	return;
}

void DarkMeteo::OnProcessShotDarkMeteoTranstion()
{
}

void DarkMeteo::OnProcessEndTranstion()
{
	m_endFlag = true;

}

bool DarkMeteo::IsMeteoHitWall(Vector3 endPosition)
{
	CCapsuleCollider m_capsuleCollider;
	m_capsuleCollider.Init(2.0f, 6.0f);
	//始点と終点を設定
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//始点はエネミーの座標。
	start.setOrigin(
		btVector3(endPosition.x, endPosition.y + METEO_START_UP_POS, endPosition.z));
	//終点はプレイヤーの座標。
	end.setOrigin(
		btVector3(endPosition.x, endPosition.y + METEO_END_UP_POS, endPosition.z));
	//壁の判定を返す
	IsWallResult isWallResult;
	//コライダーを始点から終点まで動かして。
	//壁と衝突するかどうかを調べる。
	PhysicsWorld::GetInstance()->ConvexSweepTest(
		(const btConvexShape*)m_capsuleCollider.GetBody(),
		start, end, isWallResult);
	//壁にヒットしたか
	if (isWallResult.isHit == true)
	{
		//ヒットした
		return true;
	}
	else
	{
		//ヒットしなかった
		return false;
	}
	
}

bool DarkMeteo::IsShotMeteo()
{
	if (m_shotMeteoTimer > SHOT_METEO_TIMER_LIMMIT)
	{
		m_shotMeteoTimer = 0.0f;
		return true;
	}
	else
	{
		m_shotMeteoTimer += g_gameTime->GetFrameDeltaTime();
	}

	return false;
}

void DarkMeteo::InitDarkMeteoEffect()
{
	m_darkMeteoriteEffect = NewGO<EffectEmitter>(0);
	m_darkMeteoriteEffect->Init(InitEffect::enEffect_Meteo);
	m_darkMeteoriteEffect->Play();
	m_darkMeteoriteEffect->SetPosition(m_darkMeteoPositon);
	m_darkMeteoriteEffect->Update();
}

void DarkMeteo::CreateMeteo(Vector3 endPosition)
{
	//メテオを生成。座標と終点を設定
	Meteo* meteo = NewGO<Meteo>(0, "meteo");
	meteo->SetPosition(m_darkMeteoPositon);
	meteo->SetTargetPosition(endPosition);
	//リストに追加
	m_meteoList.emplace_back(meteo);
	//メテオを撃つ音再生
	g_soundManager->InitAndPlaySoundSource(
		enSoundName_Boss_Meteo_Shot, g_soundManager->GetSEVolume());
}