#pragma once

/// <summary>
/// 何らかの効果付与アイテム
/// </summary>
class EffectItemStatus
{
private:

	struct Status
	{
		int effectValue = 0;			//効果量
		float duration = 0.0f;			//持続時間
	};

	Status m_status;

public:

	Status GetStatus()
	{
		return m_status;
	}


	/// <summary>
	/// エフェクトアイテムのステータスを初期化
	/// </summary>
	/// <param name="name">エフェクトアイテムを初期化したいアイテムの名前</param>
	void InitEffectItemStatus(const std::string& name);



	const int& GetEffectValue()
	{
		return m_status.effectValue;
	}

	const float& GetDuration()
	{
		return m_status.duration;
	}



	//csvは使わない
	std::string name;          //名前

	//CSVの情報を保存したマップ
	static std::unordered_map<std::string, EffectItemStatus> StatusDataMAP;
	//エフェクトアイテムのステータスの初期化で使う静的な型
	static const std::string effectItemStatusCsvFilePath;
	static bool IsLoadedEffectItemStatusCSVFile;

public:

	/// <summary>
	/// エフェクトアイテムステータスcsvファイルの読み込み
	/// </summary>
	/// <param name="name">読み込みたいアイテムの名前</param>
	void LoadEffectItemStatusCSV(const std::string& name);

};

