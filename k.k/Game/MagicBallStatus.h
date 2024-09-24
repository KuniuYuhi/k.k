#pragma once

#include "KnockBackInfo.h"

using namespace KnockBackInfo;

/// <summary>
/// 魔法球に関するステータス
/// </summary>
class MagicBallStatus
{
private:

	struct Status
	{
		int power = 0;					//攻撃力
		float speed = 0.0f;				//スピード
		float deleteTimeLimit = 0.0f;		//消滅する時間

		EnKnockBackPattern knockBackPattern;		//ノックバックのパターン

		float knockBackTimeScale = 0.0f;			//ノックバックの進行速度

	};

	Status m_Status;

public:


	Status GetStatus()
	{
		return m_Status;
	}

	/// <summary>
	/// 魔法球ステータスを初期化
	/// </summary>
	/// <param name="name">ステータスを初期化したい魔法球の名前</param>
	void InitMagicBallStatus(const std::string& name);


	const int& GetPower()
	{
		return m_Status.power;
	}

	const float& GetSpeed()
	{
		return m_Status.speed;
	}

	void SetDeleteTimeLimit(float timeLimit)
	{
		m_Status.deleteTimeLimit = timeLimit;
	}

	const float& GetDeleteTimeLimit()
	{
		return m_Status.deleteTimeLimit;
	}

	EnKnockBackPattern GetKnockBackPattern()
	{
		return m_Status.knockBackPattern;
	}

	const float& GetKnockBackTimeScale()
	{
		return m_Status.knockBackTimeScale;
	}

	
	std::string name;          //名前

	//CSVの情報を保存したマップ
	static std::unordered_map<std::string, MagicBallStatus> StatusDataMAP;
	//魔法球のステータスの初期化で使う静的な型
	static const std::string magicBallStatusCsvFilePath;
	static bool IsLoadedMagicBallStatusCSVFile;

private:

	/// <summary>
	/// 魔法球のステータスcsvファイルの読み込み
	/// </summary>
	/// <param name="name">読み込みたい魔法球の名前</param>
	void LoadMagicBallStatusCSV(const std::string& name);


	EnKnockBackPattern StringToKnockBackPattern(std::string& knockBackPattern);
};

