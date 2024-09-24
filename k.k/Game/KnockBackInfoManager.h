#pragma once

#include "KnockBackStatus.h"
#include "KnockBackInfo.h"


using namespace KnockBackInfo;

class KnockBackStatus;

/// <summary>
/// ノックバックの情報を持つマネージャー
/// </summary>
class KnockBackInfoManager
{
private:
	KnockBackInfoManager();
	~KnockBackInfoManager();

	


public:

	static KnockBackInfoManager* m_instance; //唯一のインスタンスのアドレスを記録する変数。

	/// <summary>
	/// インスタンスを生成
	/// </summary>
	static void CreateInstance();

	/// <summary>
	/// インスタンスを削除
	/// </summary>
	static void DeleteInstance();


	/// <summary>
	/// インスタンスを取得。
	/// </summary>
	/// <returns></returns>
	static KnockBackInfoManager* GetInstance()
	{
		return m_instance;
	}

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	
	/// <summary>
	/// カーブデータのリストを取得
	/// </summary>
	/// <param name="knockBackPattern">取得したいカーブデータのパターン</param>
	/// <returns></returns>
	std::vector<CurvePoint> GetCurvePoint(EnKnockBackPattern knockBackPattern)
	{
		return m_curveDataMap.at(knockBackPattern);
	}


	KnockBackStatus GetStatus()
	{
		return m_status;
	}


	/// <summary>
	/// 加算した攻撃IDを取得
	/// </summary>
	/// <returns>加算された後の攻撃ID</returns>
	int GetAddAttackId();



private:

	/// <summary>
	/// ノックバックカーブデータの生成
	/// </summary>
	void CreateKnockBackCurveData();

	/// <summary>
	/// カーブの計算
	/// </summary>
	void GenerateKnockBackCurve(
		EnKnockBackPattern knockBackPattern, 
		std::vector<CurvePoint>& curvePointList
	);



private:

	KnockBackStatus m_status;		//ノックバックに関するステータス

	std::map<EnKnockBackPattern, std::vector<CurvePoint>> m_curveDataMap;	//カーブデータのマップ


	int m_attackId = 0;

};

