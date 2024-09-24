#include "stdafx.h"
#include "KnockBackInfoManager.h"



KnockBackInfoManager* KnockBackInfoManager::m_instance = nullptr;

KnockBackInfoManager::KnockBackInfoManager()
{
	if (m_instance != nullptr)
	{
		std::abort();
	}
	m_instance = this;
}

KnockBackInfoManager::~KnockBackInfoManager()
{
	m_instance = nullptr;
}

void KnockBackInfoManager::CreateInstance()
{
	m_instance = new KnockBackInfoManager();
}

void KnockBackInfoManager::DeleteInstance()
{
	delete m_instance;
	m_instance = nullptr;
}

void KnockBackInfoManager::Init()
{
	//ステータスを初期化
	m_status.InitKnockBackStatus();

	//カーブデータ作成
	CreateKnockBackCurveData();


}

int KnockBackInfoManager::GetAddAttackId()
{
	//IDを加算
	m_attackId++;

	if (m_attackId > 1000)
	{
		m_attackId = 0;
	}

	return m_attackId;
}

void KnockBackInfoManager::CreateKnockBackCurveData()
{
	//マップをクリア
	m_curveDataMap.clear();

	//ノックバックの最初のパターン
	EnKnockBackPattern knockBackPattern = enKBPattern_ShortGroundedRetreat;

	//ノックバックパターンの数繰り返す
	while (knockBackPattern != enKnockBackPattern_Num)
	{
		//カーブデータを保存するリスト
		std::vector<CurvePoint> curvePointList;

		//リストにカーブデータを入れていく
		GenerateKnockBackCurve(knockBackPattern,curvePointList);

		//マップにペアを追加
		m_curveDataMap.insert(std::make_pair(knockBackPattern, curvePointList));

		//次のパターンにする
		knockBackPattern = static_cast<EnKnockBackPattern>(knockBackPattern + 1);
	}
}

void KnockBackInfoManager::GenerateKnockBackCurve(
	EnKnockBackPattern knockBackPattern, std::vector<CurvePoint>& curvePointList)
{
	float deltaTime = 0.1f; // サンプリングの間隔
	//初速度
	float initialVelocity = m_status.GetGetKnockBackStatus(knockBackPattern).initialVelocity;
	//角度
	float angle = m_status.GetGetKnockBackStatus(knockBackPattern).angle;
	//重力
	float gravity = m_status.GetGetKnockBackStatus(knockBackPattern).gravity;

	angle = Math::DegToRad(angle);

	float v_xz = initialVelocity * cos(angle);  // X,Z軸成分
	float v_y = initialVelocity * sin(angle);  // Y軸成分


	//カーブの座標をサンプリング
	for (float t = 0; 
		t <= m_status.GetGetKnockBackStatus(knockBackPattern).duration; 
		t += deltaTime
		)
	{
		CurvePoint point;

		
		//時間を保存
		point.time = t;
		//座標を計算
		point.curvePosition.x = v_xz * t;

		if (angle != 0.0f)
		{
			point.curvePosition.y = v_y * t - 0.5f * gravity * t * t;
		}
		
		
		point.curvePosition.z = v_xz * t;

		//座標を保存
		curvePointList.emplace_back(point);
	}

}
