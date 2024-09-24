#pragma once
#include "KnockBackInfo.h"

//このステータスを使ってカーブデータを作成する。
//カーブデータを持っているクラスはどこからでもアクセスできるようにする
//

using namespace KnockBackInfo;

/// <summary>
/// キャラクターがノックバックするときにステータス。
/// </summary>
class KnockBackStatus
{
public:

	/// <summary>
	/// ノックバックに関するステータス
	/// </summary>
	struct Status
	{
		int knockBackLevel = -1;				//ノックバックレベル。lv0～
		float initialVelocity = 0.0f;			//初速度
		float angle = 0;						//角度
		float duration = 0.0f;					//持続時間
		float gravity = 0.0f;					//重力

		Vector2 knockBackTimeScale = g_vec2Zero;	//ノックバックのスピード。X＝横移動、Y＝縦移動
	};

private:

	//ノックバックパターンの数の配列
	Status m_status[enKnockBackPattern_Num];


	/// <summary>
	/// ノックバックステータスcsvファイルの読み込み
	/// </summary>
	void LoadKnockBackStatusCSV();

public:

	/// <summary>
	/// ノックバックパターンのステータスを取得
	/// </summary>
	/// <param name="getKnockBackPattern"></param>
	/// <returns></returns>
	Status& GetGetKnockBackStatus(EnKnockBackPattern getKnockBackPattern)
	{
		return m_status[getKnockBackPattern];
	}

	/// <summary>
	/// ノックバックのステータスを初期化
	/// </summary>
	void InitKnockBackStatus();



	Vector2 GetKnockBackSpeed(EnKnockBackPattern getKnockBackPattern)
	{
		return m_status[getKnockBackPattern].knockBackTimeScale;
	}


	
	//CSVの情報を保存したインスタンス
	static KnockBackStatus StatusDataMAP;
	//プレイヤーのステータスの初期化で使う静的な型
	static const std::string knockBackStatusCsvFilePath;
	static bool IsLoadedKnockBackStatusCSVFile;


};

