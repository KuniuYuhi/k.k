#include "stdafx.h"
#include "SummonerAIController.h"
#include "Level3DRender.h"
#include "Summoner.h"
#include "Brave.h"


#include "SummonerInfo.h"

#include "DarkBall.h"
#include "DarkMeteorite.h"


using namespace SummonerSkillType;


namespace {
	const Vector3 DARKMETEORITE_CREATE_POSITION = { 0.0f,1400.0f,0.0f };
}

SummonerAIController::SummonerAIController()
{
	Start();
}

SummonerAIController::~SummonerAIController()
{
}

void SummonerAIController::Start()
{
	//サモナーのインスタンスを探す
	m_summoner = FindGO<Summoner>("Summoner");
	//プレイヤーのインスタンスを探す
	m_player = FindGO<Brave>("Brave");



	InitWarpPointsFromStageLevel();
}

void SummonerAIController::UpdateComponent()
{
}

void SummonerAIController::InitWarpPointsFromStageLevel()
{
	Level3DRender levelbg;

	//レベルからワープ先座標を読み込んでいく
	levelbg.Init(
		"Assets/level3D/WarpPointsLevel.tkl",
		[&](LevelObjectData& objData)
		{
			//ワープ先座標
			if (objData.ForwardMatchName(L"Pos") == true)
			{
				//座標を一つずつ格納していく
				m_warpPointsList.emplace_back(objData.position);
				return true;
			}
			return false;
		});

}

void SummonerAIController::DecisionWarpPoint()
{
	Vector3 bossPos = m_summoner->GetPosition();
	Vector3 playerPos = m_player->GetPosition();


	Vector3 warpPos = playerPos;
	float maxLength = 0;

	for (auto pos : m_warpPointsList)
	{
		//プレイヤーからワープ先座標に向かうベクトルを求める
		Vector3 diff = pos - playerPos;
		//長さを比べる
		if (maxLength < diff.Length())
		{
			maxLength = diff.Length();
			warpPos = pos;
		}
	}

	m_warpPosition = warpPos;

}

void SummonerAIController::WarpProcess()
{
	Vector3 m = g_vec3Zero;
	m_summoner->SetPosition(m_warpPosition);
	m_summoner->GetCharaCon()->SetPosition(m_warpPosition);
	m_summoner->GetCharaCon()->Execute(m, 1.0f);
}

void SummonerAIController::CreateShockWaveCollision()
{
	//ダメージプロバイダーの設定
	m_summoner->SettingDamageInfo(enSkillType_ShockWave);

	m_shockWaveCollision = NewGO<CollisionObject>(0,g_collisionObjectManager->m_enemyAttackCollisionName);
	//
	m_shockWaveCollision->SetCreatorName(m_summoner->GetName());
	//
	m_shockWaveCollision->CreateSphere(
		m_summoner->GetPosition(),
		Quaternion::Identity,
		240.0f
	);
}

void SummonerAIController::CreateComboAttackCollision()
{
	
	m_comboAttackCollision = NewGO<CollisionObject>(0, g_collisionObjectManager->m_enemyAttackCollisionName);
	//製作者の名前
	m_comboAttackCollision->SetCreatorName(m_summoner->GetName());
	//当たり判定はボックス
	m_comboAttackCollision->CreateBox(
		m_summoner->GetPosition(),
		Quaternion::Identity,
		{ 90.0f,360.0f,160.0f }
	);
	//ワールド座標を設定する
	Matrix staffMatrix = m_summoner->GetModelRender().GetBone(m_summoner->GetStaffBoonId())->GetWorldMatrix();
	m_comboAttackCollision->SetWorldMatrix(staffMatrix);

}

void SummonerAIController::CreateThirdComboCollision()
{
	m_comboAttackCollision = NewGO<CollisionObject>(0, g_collisionObjectManager->m_enemyAttackCollisionName);
	//製作者の名前
	m_comboAttackCollision->SetCreatorName(m_summoner->GetName());
	//当たり判定はボックス
	m_comboAttackCollision->CreateSphere(
		m_summoner->GetPosition(),
		Quaternion::Identity,
		100.0f
	);
	//ワールド座標を設定する
	Matrix rightHandMatrix = m_summoner->GetModelRender().GetBone(m_summoner->GetRightHandBoonId())->GetWorldMatrix();
	m_comboAttackCollision->SetWorldMatrix(rightHandMatrix);


}

void SummonerAIController::CreateDarkBall()
{
	//ボールの名前を決める
	std::string result = std::string("DarkBall") + std::to_string(m_darkBallCreateCount);
	const char* finalStr = result.c_str();
	//ダークボール生成
	DarkBall* darkBall = NewGO<DarkBall>(0, finalStr);
	//開始座標を計算
	Vector3 createPos = g_vec3Zero;
	Matrix m = m_summoner->GetModelRender().GetBone(m_summoner->GetLeftHandBoonId())->GetWorldMatrix();
	m.Apply(createPos);


	//直進か追尾型か決める
	//プレイヤーとの距離が遠いほど確率が上がる

	//確率
	int probability = rand() % 10;

	//ボスからプレイヤーに向かうベクトルを計算
	float length = m_summoner->GetDistanceToPlayerPositionValue();

	//加算する確率は最大3。距離の補間は0～1000
	int add = Math::Lerp((length - 0.0f) / (1000.0f - 0.0f), 0.0f, 3.0f);

	//確率が小さいと追いかけるようになる
	if (probability < 4 + add)
	{
		//追いかける
		darkBall->SetChasePatternState();
	}
	else
	{
		//直進
		darkBall->SetStraightPatternState();
	}


	//撃つときのパラメータの設定
	darkBall->SetShotMagicBallParameters(
		createPos,
		m_summoner->GetForwardYZero()
	);

	//生成カウント加算
	m_darkBallCreateCount++;

}

void SummonerAIController::DecisionDarkMeteoriteFallPoint()
{
	//ダークメテオ落下地点を設定
	m_darkMeteoriteFallPoint = m_player->GetPosition();
	m_darkMeteoriteFallPoint.y = 0.0f;

	//ダークメテオを生成
	m_darkMeteorite = NewGO<DarkMeteorite>(0, "DarkMeteorite");
	//範囲エフェクトを再生
	m_darkMeteorite->PlayRangeEffect(m_darkMeteoriteFallPoint);

}

void SummonerAIController::CreateDarkMeteorite()
{
	
	Vector3 createPos = m_darkMeteoriteFallPoint;
	createPos += DARKMETEORITE_CREATE_POSITION;

	//撃つときのパラメータの設定
	m_darkMeteorite->SetShotMagicBallParameters(
		createPos,
		m_summoner->GetForwardYZero()
	);
	//メテオを撃ち始める
	m_darkMeteorite->ShotStartDarkMeteorite();

}
