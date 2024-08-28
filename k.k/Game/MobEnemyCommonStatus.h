#pragma once

#include "KnockBackInfo.h"

using namespace KnockBackInfo;

/// <summary>
/// モブエネミーの基礎ステータス
/// </summary>
class MobEnemyCommonStatus
{
private:

	struct MobStatus
	{
		//通常攻撃のノックバックパターン
		EnKnockBackPattern normalAttackKnockBackPattern;

		float normalAttackTimeScale = 1.0f;		//通常攻撃のノックバックの進行速度 

	};


	MobStatus m_mobStatus;

public:

	const MobStatus& GetMobCommonStatus()
	{
		return m_mobStatus;
	}

	/// <summary>
	/// モブエネミーの基本ステータスを初期化
	/// </summary>
	/// <param name="name">ステータスを初期化したいモブエネミーの名前</param>
	void InitMobEnemyCommonStatus(const std::string& name);




	EnKnockBackPattern GetNormalAttackKnockBackPattern()
	{
		return m_mobStatus.normalAttackKnockBackPattern;
	}
	

	const float& GetNormalAttackTimeScale()
	{
		return m_mobStatus.normalAttackTimeScale;
	}



	//csvは使わない
	std::string name;          //名前

	//CSVの情報を保存したマップ
	static std::unordered_map<std::string, MobEnemyCommonStatus> StatusDataMAP;
	//エネミーのステータスの初期化で使う静的な型
	static const std::string mobEnemyCommonStatusCsvFilePath;
	static bool IsLoadedmobEnemyCommonStatusCSVFile;


public:
	/// <summary>
	/// モブエネミーの基本ファイルの読み込み
	/// </summary>
	/// <param name="name">読み込みたいモブエネミーの名前</param>
	void LoadEnemyCommonStatusCSV(const std::string& name);



	EnKnockBackPattern StringToKnockBackPattern(std::string& knockBackPattern);

};

