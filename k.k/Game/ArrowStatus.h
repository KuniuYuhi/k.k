#pragma once

/// <summary>
/// 矢のステータス
/// </summary>
class ArrowStatus
{
private:

	struct Status
	{
		float arrowSpeed = 0.0f;			//矢のスピード
		Vector3 collisionSize = g_vec3Zero;	//コリジョンのサイズ
	};

	Status m_status;

public:

	Status& GetStatus()
	{
		return m_status;
	}

	/// <summary>
	/// 矢のステータスを初期化
	/// </summary>
	/// <param name="name">パターンの名前</param>
	void InitArrowStatus(const std::string& name);


	const float& GetArrowSpeed()
	{
		return m_status.arrowSpeed;
	}

	const Vector3& GetCollisionSize()
	{
		return m_status.collisionSize;
	}


	std::string name;          //名前

	//CSVの情報を保存したマップ
	static std::unordered_map<std::string, ArrowStatus> StatusDataMAP;
	//矢のステータスの初期化で使う静的な型
	static const std::string arrowStatusCsvFilePath;
	static bool IsLoadedArrowStatusCSVFile;

private:


	/// <summary>
	/// 矢のステータスcsvファイルの読み込み
	/// </summary>
	/// <param name="name">読み込みたいパターンの名前</param>
	void LoadArrowStatusCSV(const std::string& name);



};

